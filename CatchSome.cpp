#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
#define TESTSETFILE "template.txt"

const bool debug = true;
vector<int> bucketColors(vector<int> dColors) {
    int largestColor = 0;
    vector<int> buckets(1000, 0); //There can only be 1000 colors: 1 <= A(i) <= 1000
    for(int i = 0; i < dColors.size(); ++i) {
        buckets[dColors[i]]++;
        if (dColors[i] > largestColor) {
            largestColor = dColors[i];
        }
    }
    buckets.resize(largestColor + 1);
    return buckets;
}

int runTestCase(ifstream& inFile) {
    int numberOfDogs, dogsToObserve;
    inFile >> numberOfDogs >> dogsToObserve;
    vector<int> dogPositions, dogColors;
    dogPositions.reserve(numberOfDogs);
    dogColors.reserve(numberOfDogs);
    for(int i = 0; i < numberOfDogs; ++i) {
        int buffer;
        inFile >> buffer;
        dogPositions.push_back(buffer);
    }
    for(int j = 0; j < numberOfDogs; ++j) {
        int buffer;
        inFile >> buffer;
        dogColors.push_back(buffer);
    }
    bucketColors(dogColors);
    return;
}

void main() {
    int totalTestCases;
    int testCase = 0;
    ifstream testSetInput;
    testSetInput.open(TESTSETFILE);
    if (testSetInput.is_open()) {
        testSetInput >> totalTestCases;
    }
    else {
        cout << "Unable to open file " << TESTSETFILE << endl;
        return;
    }
    while(testCase <= totalTestCases) {
        cout << "Case #" << testCase << ": " << runTestCase(testSetInput) << endl;
        ++testCase;
    }
    testSetInput.close();
    return;
}