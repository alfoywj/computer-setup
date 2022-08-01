
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
    XrMatrix4x4f_CreateProjectionFov(&resultM, GRAPHICS_VULKAN, XrFovf{-45,45,-45,45},0.1, 12.0);
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
        Vector4f projectedV;
        for(int i = -20; i < 20; ++i) {
            Vector4f v(-1, 2, i, 1);
            projectedV = project * v;
            cout << "projectedV : " << i << endl;
            cout << projectedV << endl;
            cout << "perspective division : " << i << endl;
            cout << projectedV / projectedV.w() << endl;
        }




        Matrix4f projectInv = project.inverse();
        Vector4f restoredV = projectInv * projectedV;
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
    {
        cout << "Inverse matrices, column space and null space | Chapter 7, Essence of linear algebra" << endl;
        Matrix3f A;
        Vector3f ret;
        A << 2,4,1,5,0,3,3,8,0;
        ret << -3, 0, 2;
        cout << A.inverse() << endl << ret << endl;

        cout << A.inverse() * ret << endl;
        cout << "column space ,null space, kernel" << endl;
    }
    {
        cout << "Dot products and duality | Chapter 9, Essence of linear algebra" << endl;
        Vector2f V{4,1};
        Vector2f W{2,-1};
        float LofProjectedVMulLofW = V.dot(W);
        float LofProjectedV = LofProjectedVMulLofW / W.norm();
        cout << W.norm() << endl;
        cout << LofProjectedVMulLofW << endl;
        cout << LofProjectedV << endl;

    }
    {
        cout << "Chapter 12" << endl;
        Vector3f i{-4,-1, -4};
        Vector3f j{2,0, 6};
        Vector3f k{3,2,-9};
        Vector3f V{7,-8,3};
        Vector3f X;
        Matrix3f A, B;
        A << i, j ,k;
        B << V, j, k;
        float x = B.determinant() / A.determinant();
        B << i, V, k;
        float y = B.determinant() / A.determinant();
        B << i, j, V;
        float z = B.determinant() / A.determinant();
        X << x,y,z;
        cout << X.format(vectorFmt) << endl;
        cout << (A.inverse() * V).format(vectorFmt) << endl;
        cout << (A.colPivHouseholderQr().solve(V)).format(vectorFmt) << endl;
    }
    {
        cout << "Chapter 13" << endl;
        Vector2f b1{2,1};
        Vector2f b2{-1,1};
        Vector2f v1{3,2};
        Matrix2f jennifer;
        jennifer << b1, b2;
        cout << jennifer << endl;
        cout << jennifer.inverse() * v1 << endl;// 5/3, 1/3

        Vector2f vj{-1,2};
        cout << jennifer * vj << endl;

        Matrix2f rot;
        rot << 0,-1,
                1,0;
        cout <<  rot  << endl;
        cout << jennifer.inverse() * rot * (jennifer * vj) << endl;

        Matrix2f our;
        our.setIdentity();
        cout << (jennifer.inverse() * rot * jennifer) << endl; // Transform matrix in her language
        Vector2f vj2{1,2};
        cout << (jennifer.inverse() * rot * jennifer) *  vj2 << endl; //-1, 1
    }
    {
        cout << "Eigenvectors and eigenvalues  14" << endl;
        Matrix3f A; //matrix
        Vector3f eigenVector; //matrix
        float eigenValue = 1.0f;
        /**
         * A * eigenVector == eigenValue * eigenVector;
         * AV = λV,
         * AV = (λI)V;
         * AV -(λI)V = 0;
         * (A -(λI))V = 0;
         * Diagonal matrix
         */
        {
            Vector2f diagonalBasisX{-1,0};
            Vector2f diagonalBasisY{0,2};
            Matrix2f diagonalMat;
            diagonalMat << diagonalBasisX, diagonalBasisY;
        }
        {
            Vector2f ihat{3,0};
            Vector2f jhat{1,2};
            Matrix2f mat;
            mat << ihat, jhat;
            cout << mat * mat << endl;

            Vector2f eigenBasisX{1,0};
            Vector2f eigenBasisY{-1,1};
            Matrix2f eigenMat;
            eigenMat << eigenBasisX, eigenBasisY;
            //-1 and 2 are eigenValues,
            Matrix2f diagonalMat = eigenMat.inverse() * mat * eigenMat;
            cout << diagonalMat << endl;
            cout << eigenMat * diagonalMat * diagonalMat * eigenMat.inverse() << endl;
            cout << "mat.eigenvalues() : "<< mat.eigenvalues()  << endl;
        }
    }
    {
        cout << "Eigenvectors and eigenvalues  15" << endl;
        /*
         * product = λ1 * λ2 * λ3 = det(A)
         * a + e + i == λ1 + λ2 + λ2
         */
        Matrix3f eigenMat;
        eigenMat <<
        2, 0, 0,
        0, 3, 4,
        0, 4, 9;
        cout << eigenMat.eigenvalues() << endl;

        Vector3f q1(1,0,0), q2(0,1,0), q3(0,0,1);
        Vector4f r(1.0/3.0, 1.0/3.0, 1.0/3.0, 1);
        cout << (q2-q1).cross(q3-q1).norm() << endl;
    }
    {
        auto angle = 2.0 * M_PI / 360.0 * 90.0;
        auto sinA = std::sin(angle / 2);
        auto cosA = std::cos(angle / 2);

        Eigen::Vector3d v(0, 0, 1.0);
        Eigen::Quaterniond q;
        q.vec() = v * sinA;
        q.w() = cosA;
        q.normalize();
        Eigen::Quaterniond v1(0, 1, 1.0, 1);
        Eigen::Quaterniond rotatedP = q * v1 * q.inverse();

        cout << rotatedP.vec() << endl;
    }
}

