#include <iostream>
#include <chrono>

using namespace std::chrono;
using std::cout;
using std::endl;

int main(void) {

    high_resolution_clock::time_point startPoint = high_resolution_clock::now();
    high_resolution_clock::time_point endPoint;
    for (int i = 0 ; i < 100000000L; ++i) {
        endPoint = high_resolution_clock::now();
    }
    cout << (duration_cast<nanoseconds>(endPoint - startPoint).count() ) / 100000000L
    << endl;
}