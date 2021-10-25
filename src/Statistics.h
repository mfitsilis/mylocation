#pragma once

#include <vector>
#include <string>

#include "GeoPoint.h"

typedef struct Stats {
	float minAlt;
	float maxAlt;
	float avgAlt;
	float numOfCities;
} Stats;

class Statistics
{
private:
	float maxAltitude;
	float minAltitude;
	std::vector<std::string> cities;
	std::vector<std::string> countries;
public:
void addStatistic(GeoPoint* gp) {

}
float getMaxAltitude() {
	return maxAltitude;
}
float getMinAltitude() {
	return minAltitude;
}
float getNumOfCities() {
	
}

};

