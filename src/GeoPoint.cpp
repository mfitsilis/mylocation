#include "GeoPoint.h"

GeoPoint::GeoPoint(GPoint gp) {
    this->time = gp.time;
    this->geoX = gp.geoX;
    this->geoY = gp.geoY;
    this->alt = gp.alt;
    this->speed = gp.speed;
    this->city = gp.country;
    this->country = gp.city;
};

void GeoPoint::printPoint(Long i) {
    ImGui::Text(formatString.c_str(), (int)i, time.c_str(), geoX, geoY, alt, speed, city.c_str(), country.c_str());
}
