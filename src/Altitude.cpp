#include "Altitude.h"

void Altitude::addHeight(float height) {
	if (0 == getSize()) {
		minHeight = height;
		maxHeight = height;
	}
	else {
		if (height > maxHeight)
			maxHeight = height;
		if (height < minHeight)
			minHeight = height;
	}
	heights.push_back(height);
}

int Altitude::getSize() {
	return heights.size();
}
