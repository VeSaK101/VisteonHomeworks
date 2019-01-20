#include "../../HeaderFiles/Canvas/Canvas.hpp"
#include <excpt.h>

void Canvas::addShape(Shape * shape) {
	
	canvas.emplace(shape);

}
void Canvas::removeShape(Shape * shape) {
	
	if (canvas.find(shape) != canvas.end())
	{
		canvas.erase(shape);
		delete shape;
		shape = nullptr;
	}
	else {
		throw std::invalid_argument("No such shape on canvas!");

	}

}
Shape * Canvas::getShapeAt(const Coordinates & coord) {

	for (Shape * shape : canvas) {
		if (shape->containsCoordinates(coord))
		{
			return shape;
		}
	}

	throw std::invalid_argument("No such shape on canvas!");

}
void Canvas::update() {
	for (Shape * shape : canvas) {
		shape->draw();
	}
}