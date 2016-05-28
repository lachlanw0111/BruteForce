// 301-assignment-two.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <tgmath.h>
#include <math.h>
#include <stdlib.h>
#include <cstdio>

// CONSTANTS //
const int MAX_RAND_N = 10000;   // Maximum number a random number can be in the aray
const int MIN_INPUT_SIZE = 100000;  // Minimum input size for a run through
const int MAX_INPUT_SIZE = 600000;  // Maximum input size '' ''
const int INPUT_SIZE_INCREMENT = 20000; // Size of increments in input size between runs
const int MAX_SAMPLES = 10; // Number of samples for each level of input size
std::ofstream Results_File ("TEST.csv");    // File for results to be written to

int *values; // point for generated arrays
double EM_executionTime = 0;
double BF_executionTime = 0;
unsigned long long EM_basicOperations = 0;
unsigned long long BF_basicOperations = 0;


/*****************************************************************************
 *                      Efficient Median algorithm                           *
 ****************************************************************************/

int partition(int numbersArray[], int l, int h) {
    int pivotVal = numbersArray[l];
    int pivotLoc = l;

    for (int j = l + 1; j <= h; ++j) {
        if (numbersArray[j] < pivotVal && ++EM_basicOperations) {
            pivotLoc = pivotLoc + 1;
            std::swap(numbersArray[pivotLoc], numbersArray[j]);
        }
    }

    std::swap(numbersArray[l], numbersArray[pivotLoc]);

    return pivotLoc;
}

int select(int numbersArray[], int l, int m, int h) {
    int position = partition(numbersArray, l, h);

    if (position == m) {
        return numbersArray[position];
    } else if (position > m) {
        return select(numbersArray, l, m, position - 1);
    } else if (position < m) {
        return select(numbersArray, position + 1, m, h);
    }
}

int efficientMedian(int numbersArray[], int inputSize) {
    if (inputSize == 1) {
        return numbersArray[0];
    } else {
        select(numbersArray, 0, floor(inputSize / 2), inputSize - 1);
    }
}

/*****************************************************************************/

/*****************************************************************************
 *                       Brute Force Algorithm                               *
 ****************************************************************************/

unsigned long long bruteForceMedian(int A[], int n) {   // INPUT_SIZE is passed as n, for consistency with the psuedocode
    int k = ceil((float)n / 2); // Position of median when array is sorted

    for (int i = 0; i <= (n - 1); i++) {
        int numsmaller = 0;
        int numequal = 0;
        for (int j = 0; j <= (n - 1); j++) {
            BF_basicOperations++;
            if (values[j] < values[i]) {
                numsmaller++;
            }
            else {
                if (values[j] == values[i]) {
                    numequal++;
                }
            }
        }
        if ((numsmaller < k) && (k <= (numsmaller + numequal))) {
            return BF_basicOperations;
        }
    }
}

/*****************************************************************************/

// Times a given algorithm
void timeAlgorithm(std::string algorithm, int values[], int count) {
    std::clock_t start;
    start = std::clock(); // Start clock

    if (algorithm == "bruteForceMedian") {
        bruteForceMedian(values, count); // Do bruteForce algorithm
        BF_executionTime = (( std::clock() - start ) / (double) CLOCKS_PER_SEC); // Stop clock, record time
    }
    else { // (algorithm == "efficientMedian")
        efficientMedian(values, count); // Do efficientMedian algorithm
        EM_executionTime = (( std::clock() - start ) / (double) CLOCKS_PER_SEC); // Stop clock, record time
    }
}

// Returns an array of size inputSize filled with
// random integers between 0 and MAX_RAND_N
void generateArray(int inputSize) {
    values = new int[inputSize];

    for (int i = 0; i < inputSize; ++i) {
        values[i] = rand() % MAX_RAND_N;
    }
}

// Outputs results to console, and records to CSV
void recordResults() {
    std::cout << "bruteForceMedian | Execution Time: " << BF_executionTime << "s" << std::endl; // Run Brute Force Median algorithm with timer, output Execution Time
    std::cout << "bruteForceMedian | Basic Operations: " << BF_basicOperations << std::endl; // output Basic Operations
    std::cout << "efficientMedian  | Execution Time: " << EM_executionTime << "s" << std::endl; // Run Efficient Median algorithm with timer, output Execution Time
    std::cout << "efficientMedian  | Basic Operations: " << EM_basicOperations << std::endl << std::endl; // output Basic Operations
    Results_File << BF_executionTime << ", " << BF_basicOperations << ", " << EM_executionTime << ", " << EM_basicOperations << std::endl;
}

// Resets the counting varaibles for the next run
void resetVariables() {
    delete[] values;
    values = nullptr;
    EM_basicOperations = 0;
    BF_basicOperations = 0;
    EM_executionTime = 0;
    BF_executionTime = 0;
}

int main() {
    for (int inputSize = MIN_INPUT_SIZE; inputSize <= MAX_INPUT_SIZE; inputSize += INPUT_SIZE_INCREMENT) {
        // output headings to CSV and console
        std::cout << "-----------" << MAX_SAMPLES << " SAMPLES OF N = " << inputSize << "-----------" << std::endl << std::endl;
        Results_File << MAX_SAMPLES << " SAMPLES OF N = " << inputSize << std::endl;
        Results_File << "BF Execution Time, BF Basic Operations, EM Execution Time, EM Basic Operations" << std::endl;

        // FOR EVERY SAMPLE:
        for (int sample = 0; sample < MAX_SAMPLES; ++sample) {
            generateArray(inputSize);   // Generate array of random numbers
            std::cout << "Sample #" << (sample + 1) << std::endl;
            timeAlgorithm("bruteForceMedian", values, inputSize);   // Run the brute force algorithm and time it
            timeAlgorithm("efficientMedian", values, inputSize);    // Run the efficient algorithm and time it
            recordResults();    // Output results to console and record in CSV
            resetVariables();   // Reset variables for next sample
        }

        std::cout << std::endl << std::endl;
    }

    Results_File.close();
    return 0;
}
