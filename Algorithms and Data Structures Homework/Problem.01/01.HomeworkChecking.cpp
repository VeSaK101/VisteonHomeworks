#include <iostream>
#include <string>
#include <unordered_map>
#include <map>

struct Data {

};

struct Item {
	Data data;
	bool isChecked = false;
};

class Student
{
private:
	std::unordered_map<int, Item> homework;
	static int count;
	int studentId;

public:
	Student()
	{
		studentId = count;
		count++;
		this->importHomework();
	}

	void printHomeworkItemStatus(int id)
	{
		if (id < 0 || id > this->homework.size())
		{
			std::cout << "	Homework item No." << id << " is Not Valid!" << std::endl;
			return;
		}
		if (this->homework[id].isChecked)
		{
			std::cout << "	Homework item No." << id << " is checked!" << std::endl;
		}
		else
		{
			std::cout << "	Homework item No." << id << " is Not checked!" << std::endl;
		}
		std::cout << std::endl;
	}

	void importHomework()
	{
		for (int i = 0; i < 2000; i++)
		{
			this->homework[i] = Item();
			if (i % 3 == 0)
			{
				this->homework[i].isChecked = true;
			}
		}
	}
};
int Student::count = 0;

int main()
{
	std::map<std::string, Student> students;

	Student gosho;
	students.insert(std::pair<std::string, Student>("gosho", gosho));
	
	Student pesho;
	students.insert(std::pair<std::string, Student>("pesho", pesho));
	
	Student misho;
	students.insert(std::pair<std::string, Student>("misho", misho));

	std::cout << "enter 'q' to quit, or..." << std::endl;

	while (true)
	{
		std::cout << "Enter the name of one of your 3 students - (gosho),(pesho),(misho):" << std::endl;
		std::string studentName = "";
		std::cin >> studentName;

		if (studentName == "q")
		{
			std::cout << "Goodbye!" << std::endl;
			break;
		}

		std::map<std::string, Student>::iterator it = students.find(studentName);

		if (it != students.end())
		{
			std::cout << "Enter the problem/item id you wish to check - int in range (0-1999)" << std::endl;
			int homeworkId;
			std::cin >> homeworkId;

			std::pair<std::string, Student> kvp = *it;
			Student s = kvp.second;
			std::cout <<"Student:"<< studentName << std::endl;
			s.printHomeworkItemStatus(homeworkId);
		}
		else
		{
			std::cout << "Incorrect name" << std::endl;
		}
	}
}

