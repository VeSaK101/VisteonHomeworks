#ifndef RECTANGLE_HPP_
#define RECTANGLE_HPP_
#include "Shape.hpp"

class Rectangle : public Shape {

public:
	Rectangle(const Coordinates & pointA,const Coordinates & pointB);
	void draw() override;
	bool containsCoordinates(const Coordinates & point) override; 

private:
	Coordinates  PointB;

};

#endif
