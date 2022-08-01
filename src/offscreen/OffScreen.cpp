//
// Created by redbean on 22. 7. 30.
//

#include "OffScreen.h"
#include <array>

OffScreen::OffScreen(VkFormat colorFormat, VkFormat depthFormat, VkBundle* vkBundle)
{
    mWidth = 1024;
    mHeight = 1024;
    mSize = {mWidth, mHeight};
    mVkBundle = vkBundle;
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    VkAttachmentReference colorRef = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    VkAttachmentReference depthRef = {1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
    std::array<VkAttachmentDescription, 2> at = {};
    VkRenderPassCreateInfo rpInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    rpInfo.attachmentCount = 0;
    rpInfo.pAttachments = at.data();
    rpInfo.subpassCount = 1;
    rpInfo.pSubpasses = &subpass;

    {
        colorRef.attachment = rpInfo.attachmentCount++;

        at[colorRef.attachment].format = mColorBuffer.mVkFormat;
        at[colorRef.attachment].samples = VK_SAMPLE_COUNT_1_BIT;
        at[colorRef.attachment].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        at[colorRef.attachment].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        at[colorRef.attachment].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        at[colorRef.attachment].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        at[colorRef.attachment].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        at[colorRef.attachment].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorRef;
    }

    {
        depthRef.attachment = rpInfo.attachmentCount++;

        at[depthRef.attachment].format = mDepthBuffer.mVkFormat;
        at[depthRef.attachment].samples = VK_SAMPLE_COUNT_1_BIT;
        at[depthRef.attachment].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        at[depthRef.attachment].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        at[depthRef.attachment].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        at[depthRef.attachment].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        at[depthRef.attachment].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        at[depthRef.attachment].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        subpass.pDepthStencilAttachment = &depthRef;
    }
    VK_CHECK(vkCreateRenderPass(mVkBundle->mVkDevice, &rpInfo, nullptr, &mVkRenderPass));

    //color image
    {
        VkImageCreateInfo vkImageCreateInfo{
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .pNext = VK_NULL_HANDLE,
                .flags = 0,
                .imageType = VK_IMAGE_TYPE_2D,
                .format = colorFormat,
                .extent = {static_cast<uint32_t>(mWidth),
                           static_cast<uint32_t>(mHeight), 1},
                .mipLevels = 1,
                .arrayLayers = 1,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount = 1,
                .pQueueFamilyIndices = &mVkBundle->mQueueIndex,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };
        mColorBuffer.CreateAllocBind(mVkBundle, &vkImageCreateInfo);


        VkImageViewCreateInfo viewCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = VK_NULL_HANDLE,
                .flags = 0,
                .image = mColorBuffer.mVkImage,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = mColorBuffer.mVkFormat,
                .components =
                        {
                                .r = VK_COMPONENT_SWIZZLE_R,
                                .g = VK_COMPONENT_SWIZZLE_G,
                                .b = VK_COMPONENT_SWIZZLE_B,
                                .a = VK_COMPONENT_SWIZZLE_A,
                        },
                .subresourceRange =
                        {
                                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        },
        };
        VK_CHECK(vkCreateImageView(mVkBundle->mVkDevice, &viewCreateInfo, nullptr, &mColorBuffer.mVkImageView));
    }
    //depth image
    {

        VkImageFormatProperties imageFormatProperties;
        vkGetPhysicalDeviceImageFormatProperties(mVkBundle->mVkPhysicalDevice, VK_FORMAT_D32_SFLOAT, VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 0, &imageFormatProperties);
        VkImageCreateInfo vkImageCreateInfo{
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .pNext = VK_NULL_HANDLE,
                .flags = 0,
                .imageType = VK_IMAGE_TYPE_2D,
                .format = depthFormat,
                .extent = {static_cast<uint32_t>(mWidth),
                           static_cast<uint32_t>(mHeight), 1},
                .mipLevels = 1,
                .arrayLayers = 1,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount = 1,
                .pQueueFamilyIndices = &mVkBundle->mQueueIndex,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };
        mDepthBuffer.CreateAllocBind(mVkBundle, &vkImageCreateInfo);

        VkImageViewCreateInfo viewCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .image = mDepthBuffer.mVkImage,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = mDepthBuffer.mVkFormat,
                .components =
                        {
                                .r = VK_COMPONENT_SWIZZLE_R,
                                .g = VK_COMPONENT_SWIZZLE_G,
                                .b = VK_COMPONENT_SWIZZLE_B,
                                .a = VK_COMPONENT_SWIZZLE_A,
                        },
                .subresourceRange =
                        {
                                .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        },
        };
        VK_CHECK(vkCreateImageView(mVkBundle->mVkDevice, &viewCreateInfo, nullptr, &mDepthBuffer.mVkImageView));
        {
            std::array<VkImageView, 2> attachments{};
            attachments[0] = mColorBuffer.mVkImageView;
            attachments[1] = mDepthBuffer.mVkImageView;

            VkFramebufferCreateInfo fbInfo{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
            fbInfo.renderPass = mVkRenderPass;
            fbInfo.attachmentCount = attachments.size();
            fbInfo.pAttachments = attachments.data();
            fbInfo.width = mWidth;
            fbInfo.height = mHeight;
            fbInfo.layers = 1;
            VK_CHECK(vkCreateFramebuffer(mVkBundle->mVkDevice, &fbInfo, nullptr, &mVkFramebuffer));
        }
    }
    {
        // Create sampler to sample from the attachment in the fragment shader
        VkSamplerCreateInfo samplerInfo{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV = samplerInfo.addressModeU;
        samplerInfo.addressModeW = samplerInfo.addressModeU;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        VK_CHECK(vkCreateSampler(mVkBundle->mVkDevice, &samplerInfo, nullptr, &sampler));
    }
    {
        descriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        descriptorInfo.imageView = mColorBuffer.mVkImageView;
        descriptorInfo.sampler = sampler;
    }
    VkCommandPoolCreateInfo cmdPoolCreateInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = 0,
    };
    VK_CHECK(vkCreateCommandPool(mVkBundle->mVkDevice, &cmdPoolCreateInfo, nullptr, &mVkCommandPool));


    const VkCommandBufferAllocateInfo cmd = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = mVkCommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
    };

    VK_CHECK(vkAllocateCommandBuffers(mVkBundle->mVkDevice, &cmd, &mVkCommandBuffer));

    VkFenceCreateInfo fenceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
    };
    VK_CHECK(vkCreateFence(mVkBundle->mVkDevice, &fenceCreateInfo, nullptr, &mVkFence));
}


void OffScreen::buildCommandBuffers()
{

    VkCommandBufferBeginInfo cmd_buf_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr};
    VK_CHECK(vkBeginCommandBuffer(mVkCommandBuffer, &cmd_buf_info));
    {
        static float darkSlateGrey[4] = {0.0, 0.0, 0.0, 0.0f};
        static std::array<VkClearValue, 2> clearValues;
        clearValues[0].color.float32[0] = darkSlateGrey[0];
        clearValues[0].color.float32[1] = darkSlateGrey[1];
        clearValues[0].color.float32[2] = darkSlateGrey[2];
        clearValues[0].color.float32[3] = darkSlateGrey[3];

        clearValues[1].depthStencil.depth = 1.0f;
        clearValues[1].depthStencil.stencil = 0;

        VkRenderPassBeginInfo renderPassBeginInfo{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};

        renderPassBeginInfo.clearValueCount = (uint32_t)clearValues.size();
        renderPassBeginInfo.pClearValues = clearValues.data();
        renderPassBeginInfo.renderPass = mVkRenderPass;
        renderPassBeginInfo.framebuffer = mVkFramebuffer;
        renderPassBeginInfo.renderArea.offset = {0, 0};
        renderPassBeginInfo.renderArea.extent = mSize;

        vkCmdBeginRenderPass(mVkCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdEndRenderPass(mVkCommandBuffer);

        VkImageMemoryBarrier imageMemoryBarrier = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                .pNext = NULL,
                .srcAccessMask = VK_IMAGE_LAYOUT_UNDEFINED,
                .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
                .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image = mColorBuffer.mVkImage,
                .subresourceRange =
                        {
                                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        },
        };
        vkCmdPipelineBarrier(mVkCommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, NULL, 0, NULL, 1,
                             &imageMemoryBarrier);
    }
    vkEndCommandBuffer(mVkCommandBuffer);

    VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &mVkCommandBuffer,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
    };
    VK_CHECK(vkQueueSubmit(mVkBundle->mVkQueue, 1, &submitInfo, mVkFence));
    VK_CHECK(vkWaitForFences(mVkBundle->mVkDevice, 1, &mVkFence, VK_TRUE, 100000000));
}

/**
 * typedef struct VkBufferCreateInfo {
    VkStructureType        sType;
    const void*            pNext;
    VkBufferCreateFlags    flags;
    VkDeviceSize           size;
    VkBufferUsageFlags     usage;
    VkSharingMode          sharingMode;
    uint32_t               queueFamilyIndexCount;
    const uint32_t*        pQueueFamilyIndices;
} VkBufferCreateInfo;
 */
void OffScreen::dump() {
    VkBuffer vkBuffer = nullptr;
    VkBufferCreateInfo tVkBufferCreateInfo {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .size = mColorBuffer.mAllocationSize,
        .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices = &mVkBundle->mQueueIndex,
    };

    vkCreateBuffer(mVkBundle->mVkDevice, &tVkBufferCreateInfo, NULL, &vkBuffer);
    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(mVkBundle->mVkDevice, vkBuffer, &mem_reqs);

    VkMemoryAllocateInfo vkMemoryAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = VK_NULL_HANDLE,
            .allocationSize = mem_reqs.size,
            .memoryTypeIndex = 0,
    };
    VkDeviceMemory deviceMemory;
    mVkBundle->AllocateMemoryTypeFromProperties(mem_reqs.memoryTypeBits,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &vkMemoryAllocateInfo.memoryTypeIndex);


    vkAllocateMemory(mVkBundle->mVkDevice, &vkMemoryAllocateInfo, NULL, &deviceMemory);

    /* bind memory */
    vkBindBufferMemory(mVkBundle->mVkDevice, vkBuffer, deviceMemory, 0);

    VkBufferImageCopy vkBufferCopy{
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .mipLevel=0,.baseArrayLayer=0,.layerCount=1},
        .imageOffset = {.x=0, .y=0, .z=0},
        .imageExtent = {.width = 1024, .height=1024, .depth = 1},
    };

    vkResetFences(mVkBundle->mVkDevice, 1, &mVkFence);
    VkCommandBufferBeginInfo cmd_buf_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr};
    VK_CHECK(vkBeginCommandBuffer(mVkCommandBuffer, &cmd_buf_info));

    vkCmdCopyImageToBuffer(mVkCommandBuffer, mColorBuffer.mVkImage, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, vkBuffer, 1, &vkBufferCopy);

    vkEndCommandBuffer(mVkCommandBuffer);
    VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &mVkCommandBuffer,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
    };
    VK_CHECK(vkQueueSubmit(mVkBundle->mVkQueue, 1, &submitInfo, mVkFence));
    VK_CHECK(vkWaitForFences(mVkBundle->mVkDevice, 1, &mVkFence, VK_TRUE, 100000000));

    void* data;
    vkMapMemory(mVkBundle->mVkDevice,deviceMemory,0, mem_reqs.size, 0,&data);
    float * fData = (float*)data;
    uint32_t  sd = mColorBuffer.mExtent.width * mColorBuffer.mExtent.height;
    for(uint32_t i = 0; i < sd ; i = i + 2) {
        printf("%.3f, %.3f\n", fData[i], fData[i +1]);
    }
}
