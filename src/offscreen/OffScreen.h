//
// Created by redbean on 22. 7. 30.
//

#ifndef VULKAN_TOOLS_OFFSCREEN_H
#define VULKAN_TOOLS_OFFSCREEN_H

#include "ImageBuffer.h"

struct OffScreen {
    OffScreen(VkFormat colorFormat, VkFormat depthFormat, VkBundle* vkBundle);
    VkBundle* mVkBundle;
    ImageBuffer mColorBuffer;
    ImageBuffer mDepthBuffer;

    VkRenderPass mVkRenderPass{VK_NULL_HANDLE};
    VkFramebuffer mVkFramebuffer{VK_NULL_HANDLE};

    uint32_t mWidth;
    uint32_t mHeight;
    VkExtent2D mSize;
    VkCommandPool mVkCommandPool;
    VkCommandBuffer mVkCommandBuffer;
    VkFence mVkFence;
    VkDescriptorImageInfo descriptorInfo{};
    VkSampler sampler;
    void buildCommandBuffers();
    void dump();
};


#endif //VULKAN_TOOLS_OFFSCREEN_H

