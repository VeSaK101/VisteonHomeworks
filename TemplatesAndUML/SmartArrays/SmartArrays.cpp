#include <iostream>
#include <stdexcept>

template <typename T>
class MyArray {

public:
	
	const size_t Size;

	MyArray(size_t size) : Size(size) {
		Array = new T[Size];
	}

	
	MyArray(const MyArray<T>& arr): Array(new T[arr.Size]),Size(arr.Size) {

		std::copy(arr.Array, arr.Array + arr.Size, Array);
		
	};

	~MyArray() {
		delete[] Array;
	}

	T& operator[](size_t index) {
		if (index < 0 || index >= Size)
		{
			throw std::out_of_range("Index is out of range!");
		}

		return Array[index];
	}

	MyArray<T>& operator= (const MyArray<T>& arr)
	{
		if (Size != arr.Size)
		{
			delete[] Array;
			Array = MyArray(arr.Size).Array;
		}

		for (size_t i = 0; i < Size; i++)
		{
			Array[i] = arr.Array[i];
		}
		return *this;
	}


private:
	T* Array;
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
	std::cout << arr.Size;


	return 0;
}
