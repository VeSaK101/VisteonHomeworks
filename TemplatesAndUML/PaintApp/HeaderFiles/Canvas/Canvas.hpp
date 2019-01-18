#ifndef CANVAS__HPP__
#define CANVAS__HPP__

#include <unordered_set>
#include "../Shapes/Shape.hpp"
class Canvas
{
public:
	void addShape(Shape * shape);
	void removeShape(Shape * shape);
	Shape * getShapeAt(Coordinates * coord);
	void update();

private:
	std::unordered_set<Shape *> canvas;
};
#endif