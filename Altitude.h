#pragma once

#include <vector>

class Altitude
{
private:
	std::vector<float> heights;
	float minHeight;
	float maxHeight;

public:
	void addHeight(float);

	int getSize();

	float getMinHeight() {
		return minHeight;
	}

	float getMaxHeight() {
		return maxHeight;
	}

	std::vector<float> getHeights() {
		return heights;
	}
};

