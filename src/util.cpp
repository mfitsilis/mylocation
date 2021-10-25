#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <tgmath.h>

#include "util.h"
#include "mytypes.h"

#define KXVER 3
#include "k.h"

#define LOC_GMT (!pbflag?'l':'g')

int kdbtime2int(float f) { return (int)(86400 * (f + 10957)); }  // int from float kdb timestamp

int indexOf(std::vector<std::string> vec, std::string item) {
    int ret = (std::find(vec.begin(), vec.end(), item) - vec.begin());
    return ret == (int)vec.size() ? -1 : ret;
}

Long timestampNow() {
    return (std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1));
}

int getdayofweek(char typ1) {  //Sat,Sun,Mo,Tue,Wed,Thu,Fri <-> 0,1,2,3,4,5,6  // q)(1 + (til 7)) mod 7
    struct tm timeinfo;
    time_t rawtime;
    time(&rawtime);
    if (typ1 == 'l') localtime_s(&timeinfo, &rawtime); else gmtime_s(&timeinfo, &rawtime);
    return (1 + timeinfo.tm_wday) % 7;
}

std::string int2strtime(Long inttime, char typ1) {
    char strbuf1[9] = "";
    time_t rawtime = (time_t)inttime;
    struct tm timeinfo;
    if (typ1 == 'l') localtime_s(&timeinfo, &rawtime); else gmtime_s(&timeinfo, &rawtime);
    strftime(strbuf1, sizeof(strbuf1), "%H:%M:%S", &timeinfo);
    std::string str(strbuf1);
    return str;
}

std::string int2strdatetime(Long inttime, char typ1) {
    char strbuf1[20] = "";
    time_t rawtime = (time_t)inttime;
    struct tm timeinfo;
    if (typ1 == 'l') localtime_s(&timeinfo, &rawtime); else gmtime_s(&timeinfo, &rawtime);
    strftime(strbuf1, sizeof(strbuf1), "%Y.%m.%d %H:%M:%S", &timeinfo);
    std::string str(strbuf1);
    return str;
}


std::string int2strdate(Long inttime, char typ1) { //used in loadkdbdate	
    char strbuf1[11] = "";
    time_t rawtime = (time_t)inttime;
    struct tm timeinfo;
    //if ((timeinfo.tm_hour < 0) || (timeinfo.tm_hour>23)) exit(1);
    if (typ1 == 'l') localtime_s(&timeinfo, &rawtime); else gmtime_s(&timeinfo, &rawtime);
    strftime(strbuf1, sizeof(strbuf1), "%Y.%m.%d", &timeinfo);
    std::string str(strbuf1);
    return str;
}

std::string strf(double x) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << x;
    return ss.str();
}

std::string strfx(float x, int y) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(y) << x;
    return ss.str();
}

int connectToKdb(char* host, int kdbport) {

    int khandle = khp(S(host), kdbport);
    if (khandle < 0) {
        std::cout << "Cannot connect to kdb port " << kdbport << '\n';
    }
    else {
        std::cout << "Connected to kdb port " << kdbport << '\n';
    }
    return khandle;
}

int reconnectToKdb(char* host, int kdbport, int khandle) {
    if (khandle > 0)
        kclose(khandle);
    return connectToKdb(host,kdbport);
}

double distance(double lat1, double lon1, double lat2, double lon2, char unit = 'K') {
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    }
    else {
        double radlat1 = M_PI * lat1 / 180;
        double radlat2 = M_PI * lat2 / 180;
        double theta = lon1 - lon2;
        double radtheta = M_PI * theta / 180;
        double dist = sin(radlat1) * sin(radlat2) + cos(radlat1) * cos(radlat2) * cos(radtheta);
        if (dist > 1) {
            dist = 1;
        }
        dist = acos(dist);
        dist = dist * 180 / M_PI;
        dist = dist * 60 * 1.1515;
        if (unit == 'K') { dist = dist * 1.609344; }
        if (unit == 'N') { dist = dist * 0.8684; }
        return dist;
    }
}