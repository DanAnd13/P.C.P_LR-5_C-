#include<iostream>
#include"omp.h"

using namespace std;

const int rows = 20000;
const int columns = 20000;
const int threadCount = 5;
int arr[rows][columns];
void init_arr();
long long sum(int, int, int, int);
long long part_min(int, int, int, int);

int main() {
	init_arr();
	omp_set_nested(1);
	double t1 = omp_get_wtime();
#pragma omp parallel sections
	{
#pragma omp section
		{
			for (int i = 0; i < threadCount; i++) {
				cout << "min " << i + 1 << " = " << part_min(0, rows, columns, i + 1) << endl;
			}
		}
#pragma omp section
		{
			for (int i = 0; i < threadCount; i++) {
				cout << "sum "<< i + 1 <<" = " << sum(0, rows, columns, i + 1) << endl;
			}
		}
	}
	double t2 = omp_get_wtime();
	cout << "Total time - " << t2 - t1 << " seconds" << endl;
	return 0;
}

void init_arr() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			arr[i][j] = columns - j;
		}
	}
	arr[rows / 2][columns / 2] = -10000;
}

long long sum(int start_index, int finish_index_x, int finish_index_y, int num_threads) {
	long long sum = 0;
	double t1 = omp_get_wtime();
#pragma omp parallel for reduction(+:sum) num_threads(num_threads)	
	for (int i = start_index; i < finish_index_x; i++) {
		for (int j = start_index; j < finish_index_y; j++) {
			sum += arr[i][j];
		}
	}
	double t2 = omp_get_wtime();
	cout << "sum " << num_threads << " threads worked - " << t2 - t1 << " seconds" << endl;
	return sum;
}

long long part_min(int start_index, int finish_index_x, int finish_index_y, int num_threads) {
	long long min = std::numeric_limits<long long>::max(); // Initialize min with the maximum possible value
	int index = start_index;
	double t1 = omp_get_wtime();
#pragma omp parallel for reduction(+:part_sum) num_threads(num_threads)
	for (int i = start_index; i < finish_index_x; i++) {
		long long part_sum = 0;
		for (int j = start_index; j < finish_index_y; j++) {
			part_sum += arr[i][j];
		}
#pragma omp critical
		if (min > part_sum) {
			min = part_sum;
			index = i;
		}
	}
	double t2 = omp_get_wtime();
	cout << "min " << num_threads << " threads worked - " << t2 - t1 << " seconds" << endl;
	cout << "min rows - " << index << endl;
	return min;
}