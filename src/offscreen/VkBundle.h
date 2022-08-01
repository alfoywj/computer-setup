//
// Created by redbean on 22. 7. 30.
//

#ifndef VULKAN_TOOLS_VKBUNDLE_H
#define VULKAN_TOOLS_VKBUNDLE_H


#include <vulkan/vulkan.h>
#include <assert.h>

#define VK_CHECK(result) assert((result) == VK_SUCCESS)


struct VkBundle {
    VkBundle(VkDevice vkDevice, VkQueue vkQueue, uint32_t queueIndex, VkPhysicalDevice vkPhysicalDevice);

    VkDevice mVkDevice;
    VkQueue mVkQueue;
    uint32_t mQueueIndex;
    VkPhysicalDevice mVkPhysicalDevice;
    VkPhysicalDeviceMemoryProperties mVkPhysicalDeviceMemoryProperties;

    VkBool32 AllocateMemoryTypeFromProperties(uint32_t i, VkMemoryPropertyFlagBits bits, uint32_t *pInt);
};


#endif //VULKAN_TOOLS_VKBUNDLE_H
