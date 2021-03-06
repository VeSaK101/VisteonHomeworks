// verry broken, probabbly in multiple locations.
// !!! For a large number of thread launches it will freeze the PC.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

typedef struct MyData {
	int* numbers;
	int i;
} MYDATA, *PMYDATA;

DWORD WINAPI bubbleSort(LPVOID lpParam) {
	int i;
	int *temp = (int *)malloc(sizeof(int *));

	PMYDATA data;
	data = (PMYDATA)lpParam;
	i = data->i;
	int* numbers = data->numbers;

	if (numbers[i] > numbers[i + 1]) {
		*temp = numbers[i];
		numbers[i] = numbers[i + 1];
		numbers[i + 1] = *temp;

	}

	free(temp);
	temp = nullptr;

	return 0;
}

int main(int argc, char **argv) {

	int size = 20;
	int *numbers = (int *)calloc(size, sizeof(int));
	const char* x[] = { "73745","18265","21426","56321","54013","1392","19938","52987","69633","45532","5536","545","60162","51016","15763","30271","74625","59010","29120","51631","52536" };


	int i;
	printf("Original array:");
	for (i = 0; i < size; ++i) {
		printf(" %s", x[i]);
		numbers[i] = atoi(x[i]);
	}
	printf("\n");


	DWORD   *dwThreadIdArray = (DWORD *)calloc(size, sizeof(DWORD));
	HANDLE  *hThreadArray = (HANDLE *)calloc(size, sizeof(HANDLE));



	for (int j = size - 1; j > 0; j--)
	{

		for (i = 0; i < j; i += 2)
		{
			MYDATA data;
			data.numbers = numbers;
			data.i = i;
			hThreadArray[i] = CreateThread(NULL, 0, bubbleSort, &data, 0, &dwThreadIdArray[i]);
		}
		WaitForMultipleObjects(j, hThreadArray, TRUE, INFINITE);

		for (i = 1; i < j; i += 2)
		{
			MYDATA data;
			data.numbers = numbers;
			data.i = i;
			hThreadArray[i] = CreateThread(NULL, 0, bubbleSort, &data, 0, &dwThreadIdArray[i]);
		}
		WaitForMultipleObjects(j, hThreadArray, TRUE, INFINITE);


		printf("Sorted array:");
		for (i = 0; i < size; ++i)
			printf(" %d", numbers[i]);
		printf("\n");
	}

	for (int i = 0; i < size; i++)
	{
		CloseHandle(hThreadArray[i]);
	}


	printf("Sorted array:");
	for (i = 0; i < size; ++i)
		printf(" %d", numbers[i]);
	printf("\n");
	//printf("%f",time_taken);

	free(numbers);
	free(dwThreadIdArray);
	free(hThreadArray);

	return 0;
}