// HeatDistributionApplication.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <fstream>
//#include <chrono>
//#include <ctime>
#include <omp.h>
#include <math.h>


int main()
{
	//Number of rows.
	const int m = 100;
	//Number of columns.
	const int n = 100;
	//Starting temperature.
	double startTemp = 25;
	//Tolerance.
	double tolerance = 0.00001;
	//Number of threads.
	int numberOfThreads = 4;

	//Create a grid of m by n nodes for single thread run.
	double grid[m][n];
	////Create a grid of m by n nodes for four thread run.
	double grid4[m][n];

	//Create arrays to hold r g b values.
	int r[m][n], g[m][n], b[m][n];

	//Set elements to starting temperature.
	for (int x = 0; x < m; x++) {
		for (int y = 0; y < n; y++) {
			grid[x][y] = startTemp;
			grid4[x][y] = startTemp;
		}
	}

	//Variable to hold the average difference between values in sequential calculations.
	double averageDifference;
	//Variable to hold the sum of all the differences, used to calculate average difference.
	double differenceTotal = 0;
	//Variable to hold the average difference between values in sequential calculations.
	double averageDifference4;
	//Variable to hold the sum of all the differences, used to calculate average difference.
	double differenceTotal4 = 0;
	//Variable to hold difference total for each parallel region.
	double regionDifferenceTotal4 = 0;

	/*

		Single threaded calculation.
	
	*/

	//Iterate through each element in grid and calculate new value from average of adjacent elements.
	//The elements at the top edge of the grid use zero as the upper adjacent value, while
	//elements on the right, left and bottom edge use 100 as the right, left, and lower adjacent values,
	//respectively.

	do {
		for (int x = 0; x < m; x++) {
			for (int y = 0; y < n; y++) {
				//Variable to hold the current elements value before reapproximation.
				double tempValue = grid[x][y];

				if (x == m - 1) {
					if (y == 0) {
						grid[x][y] = (100 + grid[x - 1][y] + grid[x][y + 1] + 100) / 4;
					}
					else if (y == n - 1) {
						grid[x][y] = (100 + grid[x - 1][y] + 100 + grid[x][y - 1]) / 4;
					}
					else {
						grid[x][y] = (100 + grid[x - 1][y] + grid[x][y + 1] + grid[x][y - 1]) / 4;
					}
				}
				else if (x == 0) {
					if (y == 0) {
						grid[x][y] = (grid[x + 1][y] + 0 + grid[x][y + 1] + 100) / 4;
					}
					else if (y == n - 1) {
						grid[x][y] = (grid[x + 1][y] + 0 + 100 + grid[x][y - 1]) / 4;
					}
					else {
						grid[x][y] = (grid[x + 1][y] + 0 + grid[x][y + 1] + grid[x][y - 1]) / 4;
					}
				}
				else if (y == n - 1) {
					grid[x][y] = (grid[x + 1][y] + grid[x - 1][y] + 100 + grid[x][y - 1]) / 4;
				}
				else if (y == 0) {
					grid[x][y] = (grid[x + 1][y] + grid[x - 1][y] + grid[x][y + 1] + 100) / 4;
				}
				else {
					grid[x][y] = (grid[x + 1][y] + grid[x - 1][y] + grid[x][y + 1] + grid[x][y - 1]) / 4;
				}
				//Difference is added to difference total.
				differenceTotal += grid[x][y] - tempValue;
			}
		}
		//Calculate average differnce from difference total.
		averageDifference = differenceTotal / (m*n);

		differenceTotal = 0;

	} while (averageDifference > tolerance || averageDifference < -tolerance);

	/*
	
		Four threaded calculation.
	
	*/


	//Set the number of threads.
	omp_set_num_threads(numberOfThreads);
/*
	//Get the current time in milliseconds at start.
	using namespace std::chrono;
	milliseconds start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
*/
	//Iterate through each element in grid and calculate new value from average of adjacent elements.
	//The elements at the top edge of the grid use zero as the upper adjacent value, while
	//elements on the right, left and bottom edge use 100 as the right, left, and lower adjacent values,
	//respectively.
	do {
# pragma omp parallel firstprivate (regionDifferenceTotal4)
		{//Start of parallel block.
		
			
# pragma omp for
			for (int x = 0; x < m; x++) {
				for (int y = 0; y < n; y++) {
					//Variable to hold the current elements value before reapproximation.
					double tempValue = grid4[x][y];

					if (x == m - 1) {
						if (y == 0) {
							grid4[x][y] = (100 + grid4[x - 1][y] + grid4[x][y + 1] + 100) / 4;
						}
						else if (y == n - 1) {
							grid4[x][y] = (100 + grid4[x - 1][y] + 100 + grid4[x][y - 1]) / 4;
						}
						else {
							grid4[x][y] = (100 + grid4[x - 1][y] + grid4[x][y + 1] + grid4[x][y - 1]) / 4;
						}
					}
					else if (x == 0) {
						if (y == 0) {
							grid4[x][y] = (grid4[x + 1][y] + 0 + grid4[x][y + 1] + 100) / 4;
						}
						else if (y == n - 1) {
							grid4[x][y] = (grid4[x + 1][y] + 0 + 100 + grid4[x][y - 1]) / 4;
						}
						else {
							grid4[x][y] = (grid4[x + 1][y] + 0 + grid4[x][y + 1] + grid4[x][y - 1]) / 4;
						}
					}
					else if (y == n - 1) {
						grid4[x][y] = (grid4[x + 1][y] + grid4[x - 1][y] + 100 + grid4[x][y - 1]) / 4;
					}
					else if (y == 0) {
						grid4[x][y] = (grid4[x + 1][y] + grid4[x - 1][y] + grid4[x][y + 1] + 100) / 4;
					}
					else {
						grid4[x][y] = (grid4[x + 1][y] + grid4[x - 1][y] + grid4[x][y + 1] + grid4[x][y - 1]) / 4;
					}
					//Difference is added to difference total.
					regionDifferenceTotal4 += grid4[x][y] - tempValue;
				}
			}

#pragma omp critical
			//Reduction of regionalDifferenceTotals into a single differenceTotal.
			differenceTotal4 = differenceTotal4 + regionDifferenceTotal4;
			//std::cout << "region difference" << regionDifferenceTotal << "\n";

		} //End of parallel block.


		//Average difference is calculated.
		averageDifference4 = differenceTotal4 / (m*n);

		
		//Ongoing difference totals are reset.
		differenceTotal4 = 0;
		regionDifferenceTotal4 = 0;

		//loop continues while average difference is greater than tolerance.
	} while (averageDifference4 > tolerance || averageDifference4 < -tolerance);

/*	//Get the current time in milliseconds at end.
	milliseconds end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	//Print the run time calculated from the difference in start and end times.
	std::cout << "Number of processors = " << numberOfThreads
		<< ", run time = " << (end_time.count() - start_time.count()) << "ms\n";
*/

	/*
	
		Generating heat map image for single threaded calculation.
	
	*/


	//R G B values are calculated from temperature with single thread, with equal values creating grey scale.
	for (int x = 0; x < m; x++) {
		for (int y = 0; y < n; y++) {
			r[x][y] = static_cast<int>((grid[x][y] / 100) * 255);
			g[x][y] = static_cast<int>((grid[x][y] / 100) * 255);
			b[x][y] = static_cast<int>((grid[x][y] / 100) * 255);
		}
	}

	using namespace std;
	//Open file, or create one if it doesn't already exist.
	ofstream outf("heat_map_1_thread.ppm");

	//Write R G B values in ppm format to file, to create a grey scale heat image.
	outf << "P3" << endl;
	outf << n << " " << m << endl;
	outf << "255" << endl;
	for (int x = 0; x < m; x++) {
		for (int y = 0; y < n; y++) {
			outf << r[x][y] << " " << g[x][y] << " " << b[x][y] << " ";
		}
		outf << endl;
	}

	/*
	
		Generating heat map image for four theaded calculation.
	
	*/

	//R G B values are calculated from temperature with single thread, with equal values creating grey scale.
	for (int x = 0; x < m; x++) {
		for (int y = 0; y < n; y++) {
			r[x][y] = static_cast<int>((grid4[x][y] / 100) * 255);
			g[x][y] = static_cast<int>((grid4[x][y] / 100) * 255);
			b[x][y] = static_cast<int>((grid4[x][y] / 100) * 255);
		}
	}

	using namespace std;
	//Open file, or create one if it doesn't already exist.
	ofstream outf4("heat_map_4_threads.ppm");

	//Write R G B values in ppm format to file, to create a grey scale heat image.
	outf4 << "P3" << endl;
	outf4 << n << " " << m << endl;
	outf4 << "255" << endl;
	for (int x = 0; x < m; x++) {
		for (int y = 0; y < n; y++) {
			outf4 << r[x][y] << " " << g[x][y] << " " << b[x][y] << " ";
		}
		outf4 << endl;
	}

	/*
	
		Calculating the standard deviation of the two solutions.
	
	*/

	double differenceSquaredTotal = 0;

	double varianceOfDifferences;

	double sdevOfDifferences;

	for (int x = 0; x < m; x++) {
		for (int y = 0; y < n; y++) {
			double diff = grid[x][y] - grid4[x][y];
			differenceSquaredTotal += diff * diff;
		}
	}

	varianceOfDifferences = differenceSquaredTotal / (m*n);

	sdevOfDifferences = sqrt(varianceOfDifferences);

	printf("\nStandard deviation of the differences between 1 and 4 thread solutions is: %4.8f\n", sdevOfDifferences);


    return 0;
}

