#pragma once

#include <variant>
#include <string>

#define KXVER 3
#include "k.h"

#include "GeoPoint.h"
#include "Altitude.h"
#include "mytypes.h"

/* execute query and fill kdb table with data
 */
class Table {
private:
    K colData, colNames;
    K kresult = nullptr; 
    K flip = nullptr;
    K table = nullptr;
    int khandle = 0;

    K kresultUpd = nullptr;

    int numOfCities = 0;
    float maxPointsPerCity = 0;
    std::vector<float> pointsPerCity;
    std::vector<int> columnTypes;
    std::vector<std::string> cities;
    std::vector<std::string> columnNames;
    Long nRows = 0;
    Long nCols = 0;

    std::vector<GeoPoint*> geopoints;
    Altitude altitude;

    void getColumnNamesTypes();

    /** return 0 if successfully retrieved table (nRows>=0) - else 1 */
    Long getKdbData(std::string qry);

    void getData(std::string qry);

    /** returns either int, long or string depending on kdb type */
    std::variant<int, Long, float, std::string> getItem(K columnData, int col, int row, int typ);

    void dataColumnsToRows(Long rows);

    void addPoint(GPoint gpoint);

    Long getKdbUpdData(K sub);

    void addPointsPerCity(std::string city);
    
    void pointsPerCityAddOne(int pos);

public:

    Long getNRows();
    
    float getMaxPointsPerCity();

    std::vector<float> getPointsPerCity();

    void printRow(Long i);

    Altitude* getAltitude();

    Table(int khandle,std::string qry);
    ~Table();

    void getDataUpd(K sub);

    I isRemoteErr(K x);

    K kdbquery(std::string qry);

    K Table::kdbquery(S khostname, I portnumber, std::string qry);
};
