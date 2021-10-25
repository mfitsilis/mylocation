
#include <variant>
#include <iostream>

#include "util.h"
#include "table.h"
#include "geopoint.h"
#include "mytypes.h"
#include "altitude.h"


void Table::getColumnNamesTypes() {
    for (int j = 0; j < nCols; j++) {
        K obj = kK(colData)[j];
        columnNames.push_back(kS(colNames)[j]);

        switch (obj->t) {
        case(6): { columnTypes.push_back(6); }break;   //int
        case(7): { columnTypes.push_back(7); }break;   //long
        case(9): { columnTypes.push_back(9); }break;   //float
        case(11): { columnTypes.push_back(11); }break; //symbol
        case(14): { columnTypes.push_back(14); }break; //date
        case(15): { columnTypes.push_back(15); }break; //datetime
        default: {printf("type %d not supported by this client", obj->t); exit(1); }break;
        }
    }
}

/** return 0 if successfully retrieved table (nRows>=0) - else 1
*/
Long Table::getKdbData(std::string qry) {
    Long nrows = 0;

    if (kresult != NULL) r0(kresult);    // decrement reference count, free memory

    kresult = k(khandle, (S)qry.c_str(), (K)0);

    if (!kresult) {
        printf("Network Error\n"), perror("Network");
        return -1;
    }
    if (kresult->t == -128) {
        printf("Server Error, closing connection %s\n", kresult->s), kclose(khandle);
        return -2;
    }
    if (kresult->t != 99 && kresult->t != 98) // accept table or dict
    {
        printf("type %d\n", kresult->t);
        r0(kresult);
        return -3;
    }
    else {
        flip = ktd(kresult); // if keyed table, unkey it. ktd decrements ref count of arg.
                            // table (flip) is column names!list of columns (data)
        colNames = kK(flip->k)[0];
        colData = kK(flip->k)[1];
        nCols = colNames->n;
        nrows = kK(colData)[0]->n;
    }

    return nrows;
}

/** kresultUpd will be called once kdb publishes a new point */
Long Table::getKdbUpdData(K res) {
    Long nrows = 0;
    if (kresultUpd == nullptr) kresultUpd = res;
        
    kresultUpd = k(khandle, (S)0);
    if (kresultUpd) {
        if (kresultUpd->t == 0) {

            table = kK(kresultUpd)[2]->k;

            colNames = kK(table)[0];
            colData = kK(table)[1];
            nCols = colNames->n;
            nrows = kK(colData)[0]->n;

            printf("\n%s update", kK(kresultUpd)[1]->s); // table name symbol atom
            printf(" rows: %lld cols: ", kK(colData)[0]->n);
        }
        r0(kresultUpd); // decrement reference count, free memory
    }

    return nrows;
}

void Table::getData(std::string qry) {
    columnNames.clear();
    columnTypes.clear();
    geopoints.clear();
    Long rows = getKdbData(qry);

    if (rows >= 0) {
        getColumnNamesTypes();

        dataColumnsToRows(rows);
    }

}

void Table::getDataUpd(K sub) {

    Long rows = getKdbUpdData(sub);

    if (rows > 0) {
        getColumnNamesTypes();

        dataColumnsToRows(rows);
    }
}

std::variant<int, Long, float, std::string> Table::getItem(K columnData, int col, int row, int typ) {
    K obj = kK(columnData)[col];
    switch (typ) {
    case 6:    return ((int)(kI(obj)[row]));
        break;
    case 7:    return ((Long)(kJ(obj)[row]));
        break;
    case 9:    return ((float)(kF(obj)[row]));
        break;
    case 11:   return std::string((kS(obj)[row]));
        break;
    case 14:   return std::string(int2strdate(kdbtime2int(kF(obj)[row]), 'g'));
        break;
    case 15:   return std::string(int2strdatetime(kdbtime2int(kF(obj)[row]), 'g'));
        break;
    default:   return std::string((kS(obj)[row]));
        break;
    }
}

void Table::dataColumnsToRows(Long rows) {
    GPoint gpoint;
    for (int i = 0; i < rows; i++) {
        gpoint.time = std::get<std::string>(getItem(colData, 1, i, columnTypes[1]));
        gpoint.geoX = std::get<float>(getItem(colData, 2, i, columnTypes[2]));
        gpoint.geoY = std::get<float>(getItem(colData, 3, i, columnTypes[3]));
        gpoint.alt = std::get<float>(getItem(colData, 4, i, columnTypes[4]));
        gpoint.speed = std::get<float>(getItem(colData, 5, i, columnTypes[5]));
        gpoint.country = std::get<std::string>(getItem(colData, 6, i, columnTypes[6]));
        gpoint.city = std::get<std::string>(getItem(colData, 7, i, columnTypes[7]));
        
        addPoint(gpoint);
    }
}

Table::Table(int khandle, std::string qry) {
    this->khandle = khandle;
    getData(qry);
}

Table::~Table() {
    std::cout << "table is deleted\n";
}

Long Table::getNRows() {
    return nRows;
}

std::vector<float> Table::getPointsPerCity() {
    return pointsPerCity;
}

void Table::printRow(Long i) {
    geopoints.at((int)i)->printPoint(i);
}

Altitude* Table::getAltitude() {
    return &altitude;
}

void Table::addPoint(GPoint gpoint) {
    geopoints.push_back(new GeoPoint(gpoint));
    altitude.addHeight(gpoint.alt);
    addPointsPerCity(gpoint.city);
    nRows = geopoints.size();
}

float Table::getMaxPointsPerCity() {
    return maxPointsPerCity;
}

void Table::pointsPerCityAddOne(int pos) {
    int ppc = pointsPerCity.at(pos);
    ppc++;
    pointsPerCity.at(pos) = (float)ppc;
    maxPointsPerCity = ppc > maxPointsPerCity ? ppc : maxPointsPerCity;

}

void Table::addPointsPerCity(std::string city) {
    int pos = indexOf(cities, city);
    if (pos > -1) {
        pointsPerCityAddOne(pos);
    }
    else {
        cities.push_back(city);
        pointsPerCity.push_back(1.0);
    }
}

I Table::isRemoteErr(K x) {
    if (!x) {
        fprintf(stderr, "Network error: %s\n", strerror(errno));
        return 1;
    }
    else if (-128 == xt) {
        fprintf(stderr, "Error message returned : %s\\n", x->s);
        r0(x);
        return 1;
    }
    return 0;
}

K Table::kdbquery(std::string qry) {
    if (kresult != NULL) r0(kresult);    // decrement reference count, free memory

    kresult = k(khandle, (S)qry.c_str(), (K)0);
    return kresult;
}

K Table::kdbquery(S khostname, I portnumber, std::string qry) {
        I handle;
        K result;

        handle = khp(khostname, portnumber);
        if (handle<0)
            return (K)-1;
        result = k(handle, (S)qry.c_str(), (K)0);
        if (isRemoteErr(result)) {
            kclose(handle);
            return (K)EXIT_FAILURE;
        }
        r0(result);
        kclose(handle);
        return result;
}