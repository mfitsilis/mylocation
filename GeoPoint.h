#pragma once

#include <string>

#include "imgui.h"
#include "mytypes.h"

typedef struct GPoint {
    std::string time;
    float geoX;
    float geoY;
    float alt;
    float speed;
    std::string city;
    std::string country;
} GPoint;

class GeoPoint {
private:
    const std::string formatString = "%5d %10s %7.5f %7.5f      %5.1f    %3.0f    %10s %10s";
    std::string time;
    float geoX;
    float geoY;
    float alt;
    float speed;
    std::string city;
    std::string country;

public:
    GeoPoint(GPoint gp);
    void printPoint(Long i);
};
