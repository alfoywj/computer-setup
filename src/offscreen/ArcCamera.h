//
// Created by redbean on 22. 7. 31.
//

#ifndef VULKAN_TOOLS_ARCCAMERA_H
#define VULKAN_TOOLS_ARCCAMERA_H


struct ArcCamera {
public:
    static void getCamera(float* look, float* up, float* newVector);
};


#endif //VULKAN_TOOLS_ARCCAMERA_H
