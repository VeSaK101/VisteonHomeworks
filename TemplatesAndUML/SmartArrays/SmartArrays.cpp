#include <iostream>
#include <stdexcept>

template <typename T>
class MyArray {

public:

	MyArray(size_t size) : Size(size) {
		Array = new T[Size];
	}


	MyArray(const MyArray<T>& arr) : Array(new T[arr.Size]), Size(arr.Size) {

		for (size_t i = 0; i < Size; i++)
		{
			Array[i] = arr.Array[i];
		}

	};

	~MyArray() {
		delete[] Array;
		Array = nullptr;
	}

	T& operator[](size_t index) {
		if (index >= Size)
		{
			throw std::out_of_range("Index is out of range!");
		}

		return Array[index];
	}

	MyArray<T>& operator= (const MyArray<T>& arr)
	{
		if (this != &arr)
		{
			delete[] Array;
			Array = new T[arr.Size];

			for (size_t i = 0; i < Size; i++)
			{
				Array[i] = arr.Array[i];
			}
		}

		return *this;
	}


private:
	T* Array;
	size_t Size;
};


int main()
{
	MyArray<int> arr2(3);

	arr2[0] = 60;
	arr2[1] = 61;
	arr2[2] = 62;

	MyArray<int> arr = arr2;

	std::cout << arr[0];
	std::cout << arr[1];
	std::cout << arr[2];


	return 0;
}
