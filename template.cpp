#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
#define TESTSETFILE "template.txt"

const bool debug = true;

int runTestCase(ifstream inFile) {
    //Enter code in this function to process a test case
    inFile >> code;
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