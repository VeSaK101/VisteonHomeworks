#include "HeaderFiles\Shapes\Rectangle.hpp"
#include "HeaderFiles\Shapes\Circle.hpp"
#include "HeaderFiles\Canvas\Canvas.hpp"
#include <iostream>

int main()
{
	Canvas * canvas = new Canvas();
	Shape * rectangle = new Rectangle(new Coordinates(1, 9), new Coordinates(2, 7));
	
	canvas->addShape(rectangle);
	canvas->addShape(new Circle(new Coordinates(1,3), 2.5));
	canvas->addShape(new Rectangle(new Coordinates(9,8), new Coordinates(7,6)));
	canvas->addShape(new Circle(new Coordinates(6,6), 1));
	

	try
	{
		canvas->update();
		std::cout << std::endl;
		
		canvas->removeShape(rectangle);

		canvas->update();
		std::cout << std::endl;

		Shape * shape1 = canvas->getShapeAt(new Coordinates(1,2)); // <-- getShapeAt() has undefined behaviour if point is in two shapes.
		shape1->draw();

		std::cout << std::endl;
		Shape * shape2 = canvas->getShapeAt(new Coordinates(100, 100));

		shape2->draw();
	}
	catch (const std::invalid_argument& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	

	
}
