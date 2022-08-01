//
// Created by redbean on 22. 7. 31.
//

#include "ArcCamera.h"
#include <Eigen/Dense>

void ArcCamera::getCamera(float *lookF, float *upF, float *newVectorF) {
    Eigen::Vector3f zAxis(0,0,1);
    Eigen::Vector3f newVector(newVectorF);
    float angle = zAxis.dot(newVector);
    auto sinA = std::sin(angle / 2);
    auto cosA = std::cos(angle / 2);

    Eigen::Quaternionf lookAt = {0, 0.0f, 0.0f, 10.0f};
    Eigen::Quaternionf up = {0, 0.0f, -1.0f, 0.0};

    Eigen::Vector3f v = newVector.cross(zAxis);
    Eigen::Quaternionf q;
    q.vec() = v * sinA;
    q.w() = cosA;
    q.normalize();

    Eigen::Quaternionf rotatedP = q * lookAt * q.inverse();
    lookF[0] = rotatedP.vec().x();
    lookF[1] = rotatedP.vec().y();
    lookF[2] = rotatedP.vec().z();
    rotatedP = q * up * q.inverse();
    upF[0] = rotatedP.vec().x();
    upF[1] = rotatedP.vec().y();
    upF[2] = rotatedP.vec().z();
}
