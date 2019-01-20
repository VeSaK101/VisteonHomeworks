#ifndef CIRCLE_HPP_
#define CIRCLE_HPP_
#include "Shape.hpp"

class Circle : public Shape {

public:
	Circle(const Coordinates & center, double radius);
	void draw() override;
	bool containsCoordinates(const Coordinates& point) override; 	

private:
	double Radius;
};

#endif 
