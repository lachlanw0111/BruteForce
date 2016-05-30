// 301-assignment-two.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <cstdio>

// CONSTANTS //
const int MIN_INPUT_SIZE = 50000;  // Minimum input size for a run through
const int MAX_INPUT_SIZE = 10000000;  // Maximum input size '' ''
const int INPUT_SIZE_INCREMENT = 10000; // Size of increments in input size between runs
const int MAX_SAMPLES = 1000; // Number of samples for each level of input size
std::ofstream resultsFile("OVERNIGHTRESULTS.csv");    // File for results to be written to

int *values; // point for generated arrays
int *valuesCopy;
int MAX_RAND_N;
unsigned long long emExecutionTime = 0;
unsigned long long bfExecutionTime = 0;
unsigned long long emBasicOperations = 0;
unsigned long long bfBasicOperations = 0;


/*****************************************************************************
 *                      Efficient Median algorithm                           *
 *****************************************************************************/

int partition(int numbersArray[], int l, int h) {
    int pivotVal = numbersArray[l];
    int pivotLoc = l;

    for (int j = l + 1; j <= h; ++j) {
        if (numbersArray[j] < pivotVal && ++emBasicOperations) {
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
 *****************************************************************************/

unsigned long long bruteForceMedian(int A[], int n) {   // INPUT_SIZE is passed as n, for consistency with the psuedocode
    int k = ceil(static_cast<float>(n) / 2); // Position of median when array is sorted

    for (int i = 0; i <= (n - 1); i++) {
        int numberSmaller = 0;
        int numberEqual = 0;
        for (int j = 0; j <= (n - 1); j++) {
            bfBasicOperations++;
            if (values[j] < values[i]) {
                numberSmaller++;
            } else {
                if (values[j] == values[i]) {
                    numberEqual++;
                }
            }
        }
        if ((numberSmaller < k) && (k <= (numberSmaller + numberEqual))) {
            return bfBasicOperations;
        }
    }
    return -1;
}

/*****************************************************************************/

// Times a given algorithm
void timeAlgorithm(std::string algorithm, int values[], int count) {

    auto start = std::chrono::high_resolution_clock::now(); // Start clock

    if (algorithm == "bruteForceMedian") {
        bruteForceMedian(values, count); // Do bruteForce algorithm
        auto elapsed = std::chrono::high_resolution_clock::now() - start;   // Measure elapsed time
        bfExecutionTime = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  // Record measurement
    } else { // (algorithm == "efficientMedian")
        efficientMedian(values, count); // Do efficientMedian algorithm
        auto elapsed = std::chrono::high_resolution_clock::now() - start;    // Measure elapsed time
        emExecutionTime = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  // Record measurement
    }
}

// Returns an array of size inputSize filled with
// random integers between 0 and MAX_RAND_N
void generateArray(int inputSize) {
    values = new int[inputSize];
    valuesCopy = new int[inputSize];

    for (int i = 0; i < inputSize; ++i) {
        values[i] = rand() % MAX_RAND_N;
    }

    std::copy_n(values, inputSize, valuesCopy); // Copies values to valuesCopy
}

// Outputs results to console, and records to CSV
void recordResults(int inputSize) {
    std::cout << "bruteForceMedian | Execution Time: " << bfExecutionTime << " microseconds" << std::endl; // Run Brute Force Median algorithm with timer, output Execution Time
    std::cout << "bruteForceMedian | Basic Operations: " << bfBasicOperations << std::endl; // output Basic Operations
    std::cout << "efficientMedian  | Execution Time: " << emExecutionTime << " microseconds" << std::endl; // Run Efficient Median algorithm with timer, output Execution Time
    std::cout << "efficientMedian  | Basic Operations: " << emBasicOperations << std::endl << std::endl; // output Basic Operations
    resultsFile << inputSize << ", " << bfExecutionTime << ", " << bfBasicOperations << ", " << emExecutionTime << ", " << emBasicOperations << std::endl;
}

// Resets the counting varaibles for the next run
void resetVariables() {
    delete[] values;
    delete[] valuesCopy;
    values, valuesCopy = nullptr;

    emBasicOperations, bfBasicOperations = 0;
    emExecutionTime, bfExecutionTime = 0;
}

int main() {
    resultsFile << "Input Size, BF Execution Time, BF Basic Operations, EM Execution Time, EM Basic Operations" << std::endl;

    for (int inputSize = MIN_INPUT_SIZE; inputSize <= MAX_INPUT_SIZE; inputSize += INPUT_SIZE_INCREMENT) {
        MAX_RAND_N = (inputSize / 100);
        // output headings to CSV and console
        std::cout << "-----------" << MAX_SAMPLES << " SAMPLES OF N = " << inputSize << "-----------" << std::endl << std::endl;

        // FOR EVERY SAMPLE:
        for (int sample = 0; sample < MAX_SAMPLES; ++sample) {
            generateArray(inputSize);   // Generate array of random numbers
            std::cout << "Sample #" << (sample + 1) << std::endl;
            timeAlgorithm("bruteForceMedian", values, inputSize);   // Run the brute force algorithm and time it
            timeAlgorithm("efficientMedian", valuesCopy, inputSize);    // Run the efficient algorithm and time it
            recordResults(inputSize);    // Output results to console and record in CSV
            resetVariables();   // Reset variables for next sample
        }

        std::cout << std::endl << std::endl;
    }

    resultsFile.close();
    return 0;
}
