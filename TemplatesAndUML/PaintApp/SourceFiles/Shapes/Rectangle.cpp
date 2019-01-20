#include "../../HeaderFiles/Shapes/Rectangle.hpp"
#include <iostream>
Rectangle::Rectangle(const Coordinates & pointA,const Coordinates & pointB) : Shape(pointA), PointB(pointB) {

}

void Rectangle::draw() {
	std::cout << "I am Rectangle with point A: ("<< Point.x << ", "<< Point.y<<") and point B: ("<< PointB.x << ", " << PointB.y <<")"<< std::endl;
}

bool Rectangle::containsCoordinates(const Coordinates & point) {

	if (point.x >= Point.x && point.x <= PointB.x &&
		point.y >= Point.y && point.y <= PointB.y) {

		return true;
	}

	return false;
}