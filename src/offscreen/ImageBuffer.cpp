//
// Created by redbean on 22. 7. 30.
//

#include "ImageBuffer.h"

void ImageBuffer::CreateAllocBind(VkBundle *vkBundle, const VkImageCreateInfo *pCreateInfo) {
    mVkFormat = pCreateInfo->format;
    mExtent = pCreateInfo->extent;

    VK_CHECK(vkCreateImage(vkBundle->mVkDevice, pCreateInfo, nullptr, &mVkImage));

    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(vkBundle->mVkDevice, mVkImage, &memReqs);
    mAllocationSize = memReqs.size;

    VkMemoryAllocateInfo vkMemoryAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = VK_NULL_HANDLE,
            .allocationSize = mAllocationSize,
            .memoryTypeIndex = 0,
    };

    vkBundle->AllocateMemoryTypeFromProperties(memReqs.memoryTypeBits,
                                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                                &vkMemoryAllocateInfo.memoryTypeIndex);
    VK_CHECK(vkAllocateMemory(vkBundle->mVkDevice, &vkMemoryAllocateInfo, nullptr, &mVkDeviceMemory));
    VK_CHECK(vkBindImageMemory(vkBundle->mVkDevice, mVkImage, mVkDeviceMemory, 0));
}


VkBundle::VkBundle(VkDevice vkDevice, VkQueue vkQueue, uint32_t queueIndex,  VkPhysicalDevice vkPhysicalDevice) : mVkDevice(vkDevice), mVkQueue(vkQueue), mQueueIndex(queueIndex), mVkPhysicalDevice(vkPhysicalDevice){
    vkGetPhysicalDeviceMemoryProperties(mVkPhysicalDevice, &mVkPhysicalDeviceMemoryProperties);
}

VkBool32 VkBundle::AllocateMemoryTypeFromProperties(uint32_t typeBits, VkMemoryPropertyFlagBits requirements_mask, uint32_t *typeIndex) {
    // Search memtypes to find first index with those properties
    for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
        if ((typeBits & 1) == 1) {
            // Type is available, does it match user properties?
            if ((mVkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
                *typeIndex = i;
                return VK_TRUE;
            }
        }
        typeBits >>= 1;
    }
    return VK_FALSE;
}
