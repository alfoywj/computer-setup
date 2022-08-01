//
// Created by redbean on 22. 7. 24.
//

#include <Eigen/Dense>
#include <cstdio>
#include <iostream>
using namespace Eigen;

IOFormat vectorFmt(StreamPrecision, DontAlignCols, ", ", ",");


using std::endl;
using namespace std;
int main(void) {
    {
        cout << "Eigenvectors and eigenvalues  15" << endl;
        Matrix3f eigenMat;
        eigenMat <<
                 2, 0, 0,
                0, 3, 4,
                0, 4, 9;
        cout << eigenMat.eigenvalues() << endl;
    }
}