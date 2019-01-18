#include "../../HeaderFiles/Shapes/Rectangle.hpp"
#include <iostream>
Rectangle::Rectangle(Coordinates * pointA, Coordinates * pointB) : PointA(*pointA), PointB(*pointB) {

}

void Rectangle::draw() {
	std::cout << "I am Rectangle with point A: ("<< PointA.x << ", "<<PointA.y<<") and point B: ("<< PointB.x << ", " << PointB.y <<")"<< std::endl;
}

bool Rectangle::containsCoordinates(Coordinates point) {

	if (point.x >= PointA.x && point.x <= PointB.x && 
		point.y >= PointA.y && point.y <= PointB.y) {

		return true;
	}

	return false;
}