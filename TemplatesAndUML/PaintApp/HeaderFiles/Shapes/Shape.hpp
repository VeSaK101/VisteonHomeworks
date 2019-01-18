#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include "../Common/Coordinates.hpp"

class Shape {

public:
	virtual void draw() = 0;
	virtual bool containsCoordinates(Coordinates point) = 0;
	virtual ~Shape() = default;

};
#endif 

