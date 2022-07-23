
#include <Eigen/Dense>
#include <vulkan/vulkan.h>
#include <cstdio>
#include <iostream>
#include "xr_linear.h"
using namespace Eigen;

IOFormat vectorFmt(StreamPrecision, DontAlignCols, ", ", ",");

using std::endl;
using namespace std;
int main(void) {
    {
        Eigen::Vector2f i(1, 2);
        Eigen::Vector2f j(3, -1);
        Eigen::Vector2f ret(i + j);
        printf("%f\n", ret.norm());
    }
    XrMatrix4x4f resultM;
    XrMatrix4x4f_CreateProjectionFov(&resultM, GRAPHICS_VULKAN, XrFovf{-45,45,-45,45},0.1, 100.0);
    {
        Eigen::Matrix3f basisTest;
        basisTest <<
           Eigen::Vector3f(1,2,3),
           Eigen::Vector3f(4,5,6),
           Eigen::Vector3f(7,8,9),
        cout << basisTest << endl;
    }

    {
        Eigen::Vector2f V(1,2);
        Eigen::Vector2f W(3, -1);
        float a = -0.8f;
        float b = 1.30f;
        Eigen::Vector2f R = a * V + b * W;
        cout << R << endl;
    }

    cout << "chapter 3 linear transformation" << endl;
    {
        Eigen::Vector2f iHat(1,-2);
        Eigen::Vector2f jHat(3, 0);
        Eigen::Vector2f v1(-1, 2);
        Matrix2f mat;
        mat << iHat, jHat;
        Vector2f v2 = mat * v1;
        cout << v2 << endl;
        cout << "chapter 3 application" << endl;
        Matrix2f matInv = mat.inverse();
        Vector2f v3 = matInv * v2;
        cout << v3 << endl;
    }
    cout << "openxr inverse test" << endl;
    {
        Matrix4f project(resultM.m);
        cout << "project det = " << project.determinant() << endl;
        Vector4f v(-1, 2, -10, 1);
        Vector4f projectedV = project * v;
        cout << "projectedV" << endl;
        cout << projectedV << endl;
        cout << "perspective division" << endl;
        cout << projectedV / projectedV.w() << endl;
        Matrix4f projectInv = project.inverse();
        Vector4f restoredV = projectInv * projectedV;
        cout << "v" << endl;
        cout << v << endl;
        cout << "restoredV" << endl;
        cout << restoredV << endl;
    }
    {
        cout << "chapter 4 linear composition" << endl;
        Matrix2f shear, rotation;
        Vector2f iHat(1,0);
        Vector2f jHat(1, 1);
        shear << iHat, jHat;
        Vector2f iHat2(0,1);
        Vector2f jHat2(-1, 0);
        rotation << iHat2, jHat2;
        cout << shear * rotation << endl;
    }

    {
        cout << "chapter 4 linear composition 2" << endl;
        Matrix2f m1;
        Vector2f m1ihat(1.0,1);
        Vector2f m1jhat(-2, 0);
        m1 << m1ihat, m1jhat;
        Matrix2f m2;
        Vector2f m2iHat(0,1);
        Vector2f m2jHat(2, 0);
        m2 << m2iHat, m2jHat;
        cout << (m2 * m1ihat).format(vectorFmt) << endl;
        cout << (m2 * m2jHat).format(vectorFmt) << endl;
        cout << "matrix test" << endl;
        cout << m2.row(0) << endl;
        cout << "matrix test2" << endl;
        cout << m2.row(0) * m1ihat << "," << m2.row(1) * m1ihat << endl;
        cout << m2.row(0) * m1jhat << "," << m2.row(1) * m1jhat << endl;
        cout << "matrix test2" << endl;
        Matrix2f m3;
        Vector2f m3iHat(0,1);
        Vector2f m3jHat(1, 0);
        m3 << m3iHat, m3jHat;

        cout << m1 * (m2 * m3) << endl << (m1 * m2) * m3 << endl;
    }
    {
        cout << "Three-dimensional linear transformations | Chapter 5, Essence of linear algebra" << endl;
    }
    {
        cout << "The determinant | Chapter 6, Essence of linear algebra" << endl;
        Matrix2f m1;
        Vector2f m1ihat(3,0);
        Vector2f m1jhat(0, 2);
        m1 << m1ihat, m1jhat;
        cout << m1.determinant() << endl;
    }
}
