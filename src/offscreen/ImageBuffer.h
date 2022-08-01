//
// Created by redbean on 22. 7. 30.
//

#ifndef VULKAN_TOOLS_IMAGEBUFFER_H
#define VULKAN_TOOLS_IMAGEBUFFER_H

#include "VkBundle.h"

struct ImageBuffer {
    VkImage mVkImage;
    VkFormat mVkFormat;
    VkDeviceMemory mVkDeviceMemory;
    VkImageView mVkImageView;
    VkDeviceSize       mAllocationSize;
    VkExtent3D mExtent;

    void CreateAllocBind(VkBundle* vkBundle,
                             const VkImageCreateInfo* pCreateInfo);
};


#endif //VULKAN_TOOLS_IMAGEBUFFER_H
