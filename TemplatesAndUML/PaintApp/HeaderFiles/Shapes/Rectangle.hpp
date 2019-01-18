#ifndef RECTANGLE_HPP_
#define RECTANGLE_HPP_
#include "Shape.hpp"

class Rectangle : public Shape {

public:
	Rectangle(Coordinates * pointA, Coordinates * pointB);
	void draw() override;
	bool containsCoordinates(Coordinates point) override; //<-- Shape::containsCoordinates for override ?	

private:
	Coordinates  PointA;
	Coordinates  PointB;

};

#endif
