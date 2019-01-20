#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include "../Common/Coordinates.hpp"

class Shape {

public:
	Shape(const Coordinates & point);
	virtual void draw() = 0;
	virtual bool containsCoordinates(const Coordinates & point) = 0;
	virtual ~Shape() = default;
protected:
	Coordinates Point;
};
#endif 

