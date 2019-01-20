#include "../../HeaderFiles/Shapes/Circle.hpp"
#include <iostream>


Circle::Circle(const Coordinates & center, double radius) : Shape(center), Radius(radius) {

}

void Circle::draw() {
	std::cout << "I am Circle with center: ("<< Point.x<<", "<<Point.y << ") and radius: "<<Radius<<std::endl;
}

bool Circle::containsCoordinates(const Coordinates & point) {
	double distanceFromCenterSquared = ((point.x - Point.x)*(point.x - Point.x)) + ((point.y - Point.y)*(point.y - Point.y));
	double radiusSquared = Radius * Radius;

	if (radiusSquared >= distanceFromCenterSquared)
	{
		return true;
	}

	return false;
}