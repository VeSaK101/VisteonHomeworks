#include "../../HeaderFiles/Shapes/Circle.hpp"
#include <iostream>
#include <cfloat>
#include <math.h>

Circle::Circle(Coordinates * center, double radius) : Center(*center), Radius(radius) {

}

void Circle::draw() {
	std::cout << "I am Circle with center: ("<< Center.x<<", "<<Center.y << ") and radius: "<<Radius<<std::endl;
}

bool Circle::containsCoordinates(Coordinates point) {
	double distanceFromCenterSquared = ((point.x - Center.x)*(point.x - Center.x)) + ((point.y - Center.y)*(point.y - Center.y));
	double radiusSquared = Radius * Radius;

	if (radiusSquared >= distanceFromCenterSquared)
	{
		return true;
	}

	return false;
}