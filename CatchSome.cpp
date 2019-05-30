#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

using namespace std;
#define TESTSETFILE "template.txt"

/*
Problem
Bundle is an animal researcher and needs to go observe K dogs. She lives on a horizontal street marked at metre increments with consecutive numbers 0, 1, 2, 3 and so on. She begins in her home, which is at position 0. There are also N dogs on the street. The i-th dog is Pi metres to the right of her home on the street (multiple dogs can share the same position).

Dogs come in different colors, which are denoted by positive integers. The i-th animal is of color Ai.

If Bundle is at her home, she can change the current color of her shirt. This is important since the dogs are very shy! Bundle can only observe a dog if she is at the same position as that dog, and is wearing a shirt of the same color as the dog.

It takes Bundle one second to move one metre to the left or right on the street. It takes her no time to change shirts or observe a dog.

What is the least amount of time it will take Bundle to observe K dogs? Note that she does not have to return home after observing K dogs.
Input

The first line of the input gives the number of test cases, T. T test cases follow. Each testcase begins with a line containing the two integers N and K, the number of dogs on the number line and the number of dogs Bundle needs to observe, respectively. The second line contains N integers, the i-th of which is Pi, the position of the i-th dog. The third line contains N integers, the i-th of which is Ai, the color of the i-th dog.
Output

For each test case, output one line containing Case #x: y, where x is the test case number (starting from 1) and y is the least time Bundle needs to observe K dogs.
Limits

Time limit: 30 seconds per test set.
Memory limit: 1GB.
1 ≤ T ≤ 100.
1 ≤ K ≤ N <= 1000.
1 ≤ Ai ≤ 1000.
1 ≤ Pi ≤ 105.
*/

const bool debug = true;

class Dog {
    private:
    int color;
    int position;

    public:
    Dog(int, int);
    int getColor() const;
    int getPosition() const;
    int getObsCost(int, int) const;

};

Dog::Dog(int P, int A) {
    color = A;
    position = P;
}

int Dog::getColor() const {
    return color;
}

int Dog::getPosition() const {
    return position;
}

int Dog::getObsCost(int bundlePosition, int bundleShirtColor) const {
    if (bundleShirtColor == color) {
        return (abs(bundlePosition - position));
    }
    else if (bundlePosition == 0) {
        return position;
    }
    else {
        return (bundlePosition + position);
    }
}

bool operator<(const Dog& a, const Dog& b) {
    return a.getPosition() < b.getPosition();
}

ostream& operator<<(ostream& os, const Dog& d) {
    os << "pos: " << d.getPosition() << " color: " << d.getColor() << endl;
    return os;
}

ostream& operator<<(ostream& os, const vector<Dog>& a) {
    int i = 0;
    for (Dog d : a) {
        os << "Dog (" << i << "): " << d; 
        ++i;
    }
    return os;
}

ostream& operator<<(ostream& os, const vector<int>& i) {
    int n = 0;
    os << "[";
    for (int j : i) {
        os << n << ": " << j << ", ";
        ++n;
    }
    os << "]" << endl;
    return os;
}

class Observer {
    private:
    int position;
    int shirtColor;
    int dogsLeft;
    int timeTaken;

    public:
    Observer(int);
    int getPosition();
    int getShirtColor();
    int getRemainingDogCount();
    int getTimeSoFar();
    void observeDog(const Dog&);
    void changeShirt(int);
};

Observer::Observer(int dogsToObserve) {
    position = 0; //new Observers start at home
    shirtColor = 0; //new Observer has no shirt color
    dogsLeft = dogsToObserve;
    timeTaken = 0; //new Observe has not taken any seconds
}

int Observer::getPosition() {
    return position;
}

int Observer::getShirtColor() {
    return shirtColor;
}

int Observer::getRemainingDogCount() {
    return dogsLeft;
}

int Observer::getTimeSoFar() {
    return timeTaken;
}

void Observer::observeDog(const Dog& observationSubject) {
    if (shirtColor == observationSubject.getColor()) {
        timeTaken += abs(observationSubject.getPosition() - position);
        position = observationSubject.getPosition();
        --dogsLeft;
    }
    else {
        timeTaken += observationSubject.getPosition() + position; //Observer has to go home to change shirt, then go to dog
        shirtColor = observationSubject.getColor();
        position = observationSubject.getPosition();
        --dogsLeft;
    }
}

void findAllDogs(vector<Dog>& emptyList, ifstream& inFile) {
    vector<int> dogPosBuffer;
    int numberOfDogs = emptyList.capacity();
    dogPosBuffer.reserve(numberOfDogs);
    for(int i = 0; i < numberOfDogs; ++i) {
        inFile >> dogPosBuffer[i];
    }
    for(int j = 0; j < numberOfDogs; ++j) {
        int dogColorBuffer;
        inFile >> dogColorBuffer;
        Dog nextDog(dogPosBuffer[j], dogColorBuffer);
        emptyList.push_back(nextDog);
    }
    std::sort(emptyList.begin(), emptyList.end());
}

void observeNextDogs(vector<Dog>& remainingDogs, Observer& Bundle) {
    cout << remainingDogs;
    int leastTime = remainingDogs[0].getObsCost(Bundle.getPosition(), Bundle.getShirtColor());
    int dogsToObserve = 1;
    vector<int> nextDogs = {0};
    for ( int k = 1; k < remainingDogs.size(); ++k ) {
        int thisCost = remainingDogs[k].getObsCost(Bundle.getPosition(), Bundle.getShirtColor());
        int thisDogsSameColor = 1;
        vector<int> dogIndices;
        cout << "k=" << k << ", leastTime=" << leastTime << ", thisCost=" << thisCost << endl;
        for ( int n = k - 1; n >= 0; --n ) {
            if ( remainingDogs[k].getColor() == remainingDogs[n].getColor() ) {
                ++thisDogsSameColor;
                dogIndices.push_back(n);
            }
        }
        thisCost = thisCost / thisDogsSameColor;
        if ( thisCost < leastTime ) {
            leastTime = thisCost;
            dogsToObserve = thisDogsSameColor;
            dogIndices.push_back(k);
            nextDogs = dogIndices;
        }
        else if ( ( thisCost == leastTime ) && ( thisDogsSameColor > dogsToObserve) ) {
            dogsToObserve = thisDogsSameColor;
            dogIndices.push_back(k);
            nextDogs = dogIndices;
        }
        cout << nextDogs;
    }
    int offset = 0;
    for(int index : nextDogs) {
        cout << "removing " << remainingDogs[index - offset];
        Bundle.observeDog(remainingDogs[index - offset]);
        remainingDogs.erase(remainingDogs.begin() + index - offset);
        ++offset;
    }
}

int observeAllDogs(vector<Dog>& hiddenDogs, int dogsToObserve) {
    Observer Bundle(dogsToObserve);
    while ( Bundle.getRemainingDogCount() > 0 ) {
        observeNextDogs(hiddenDogs, Bundle);
    }
    return Bundle.getTimeSoFar();
}
int runTestCase(ifstream& inFile) {
    int numberOfDogs, dogsToObserve;
    inFile >> numberOfDogs >> dogsToObserve;
    vector<Dog> allDogs;
    allDogs.reserve(numberOfDogs);
    findAllDogs(allDogs, inFile);
    return observeAllDogs(allDogs, dogsToObserve);
}

int main() {
    int totalTestCases;
    int testCase = 1;
    ifstream testSetInput;
    testSetInput.open(TESTSETFILE);
    if (testSetInput.is_open()) {
        testSetInput >> totalTestCases;
    }
    else {
        cout << "Unable to open file " << TESTSETFILE << endl;
        return -1;
    }
    while(testCase <= totalTestCases) {
        cout << "Case #" << testCase << ": " << runTestCase(testSetInput) << endl;
        ++testCase;
    }
    testSetInput.close();
    return 1;
}