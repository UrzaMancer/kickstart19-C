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
    vector<Dog> orderObserved;

    public:
    Observer(int);
    Observer(int, int, int);
    int getPosition() const;
    int getShirtColor() const;
    int getRemainingDogCount() const;
    int getTimeSoFar() const;
    void observeDog(const Dog&);
};

Observer::Observer(int dogsToObserve) {
    if (dogsToObserve < 0) {
        cout << "Observer construct called with negative value: " << dogsToObserve << endl;
    }
    position = 0; //new Observers start at home
    shirtColor = 0; //new Observer has no shirt color
    dogsLeft = dogsToObserve;
    timeTaken = 0; //new Observe has not taken any seconds
    orderObserved.reserve(dogsToObserve);
}

Observer::Observer(int dogsToObserve, int startPosition, int startShirtColor) {
    position = startPosition;
    shirtColor = startShirtColor; 
    dogsLeft = dogsToObserve;
    timeTaken = 0;
    orderObserved.reserve(dogsToObserve);
}

int Observer::getPosition() const {
    return position;
}

int Observer::getShirtColor() const {
    return shirtColor;
}

int Observer::getRemainingDogCount() const {
    return dogsLeft;
}

int Observer::getTimeSoFar() const {
    return timeTaken;
}

void Observer::observeDog(const Dog& observationSubject) {
    if (shirtColor == observationSubject.getColor()) {
        timeTaken += abs(observationSubject.getPosition() - position);
    }
    else {
        timeTaken += observationSubject.getPosition() + position; //Observer has to go home to change shirt, then go to dog
        shirtColor = observationSubject.getColor();
    }
    position = observationSubject.getPosition();
    --dogsLeft;
    orderObserved.push_back(observationSubject);
}

vector<int> findAllDogs(vector<Dog>& emptyList, ifstream& inFile) {
    vector<int> dogPosBuffer;
    vector<int> colorBuckets(1001,0);
    int colorCount = 0;
    int numberOfDogs = emptyList.capacity();
    dogPosBuffer.reserve(numberOfDogs);
    for(int i = 0; i < numberOfDogs; ++i) {
        inFile >> dogPosBuffer[i];
    }
    int maxColor = 0;
    for(int j = 0; j < numberOfDogs; ++j) {
        int dogColorBuffer;
        inFile >> dogColorBuffer;
        Dog nextDog(dogPosBuffer[j], dogColorBuffer);
        if ( ++colorBuckets[nextDog.getColor()] == 1 ) {
            ++colorCount;
            if ( nextDog.getColor() > maxColor ) {
                maxColor = nextDog.getColor();
            }
        }
        emptyList.push_back(nextDog);
    }
    colorBuckets[0] = colorCount;
    colorBuckets.resize(maxColor + 1);
    std::sort(emptyList.begin(), emptyList.end());
    return colorBuckets;
}


int costToPath(const vector<Dog>& dogList, const Observer& parallelObserver) {
    if (dogList.empty()) {
        return 0;
    }
    vector<Dog> otherDogs = dogList;
    auto oDogsIt = otherDogs.begin();
    int minimumCost = 0;
    for (auto it = dogList.begin(); it != dogList.end(); ++it) {
        Observer pathProbe( parallelObserver.getRemainingDogCount(), parallelObserver.getPosition(), parallelObserver.getShirtColor() ); 
        if (pathProbe.getRemainingDogCount() > 0) {
            pathProbe.observeDog(*it);
            otherDogs.erase(oDogsIt);
        }
        int thisPathCost = pathProbe.getTimeSoFar() + costToPath(otherDogs, pathProbe);
        if (minimumCost == 0) {
            minimumCost = thisPathCost;
        }
        else if (minimumCost > thisPathCost) {
            minimumCost = thisPathCost;
            //bestPathProbe = pathProbe;
        }
    }
    return minimumCost;
    //return bestPathProbe //.orderObserved //vector<Dog>
}

/*
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
*/

int observeAllDogs(vector<Dog>& hiddenDogs, vector<int> colorBuckets, int dogsToObserve) {
    Observer Bundle(dogsToObserve);
    if (dogsToObserve == hiddenDogs.size()) { //corner case processing: if you have to observe all dogs (N=K) and all are different colors
        if ( colorBuckets[0] == hiddenDogs.size() ) {
            for ( Dog d : hiddenDogs ) {
                Bundle.observeDog(d);
            }
            return Bundle.getTimeSoFar();
        }
    }

    return costToPath(hiddenDogs, Bundle); 
/*
    while ( Bundle.getRemainingDogCount() > 0 ) {
        observeNextDogs(hiddenDogs, Bundle);
    }
    return Bundle.getTimeSoFar();
    */
}

int runTestCase(ifstream& inFile) {
    int numberOfDogs, dogsToObserve;
    inFile >> numberOfDogs >> dogsToObserve;
    vector<Dog> allDogs;
    allDogs.reserve(numberOfDogs);
    return observeAllDogs(allDogs, findAllDogs(allDogs, inFile), dogsToObserve);
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
        int result = runTestCase(testSetInput);
        cout << "Case #" << testCase << ": " << result << endl;
        ++testCase;
    }
    testSetInput.close();
    return 1;
}