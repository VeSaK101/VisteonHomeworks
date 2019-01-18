#ifndef CIRCLE_HPP_
#define CIRCLE_HPP_
#include "Shape.hpp"

class Circle : public Shape {

public:
	Circle(Coordinates * center, double radius);
	void draw() override;
	bool containsCoordinates(Coordinates point) override; //<-- Shape::containsCoordinates for override ?	

private:
	Coordinates Center;
	double Radius;


};

#endif 
