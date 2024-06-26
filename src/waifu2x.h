// waifu2x implemented with ncnn library

#ifndef WAIFU2X_H
#define WAIFU2X_H

#include <string>

// ncnn
#include "net.h"
#include "gpu.h"
#include "layer.h"
#include "layer_type.h"

class Waifu2x
{
public:
    Waifu2x(int gpuid, bool tta_mode = false, int num_threads = 0);
    ~Waifu2x();

    int load(const unsigned char* param, const unsigned char* model);

    int process(const ncnn::Mat& inimage, ncnn::Mat& outimage) const;

    int process_cpu(const ncnn::Mat& inimage, ncnn::Mat& outimage) const;

public:
    // waifu2x parameters
    int noise;
    int scale;
    int tilesize;

private:
    ncnn::VulkanDevice* vkdev;
    ncnn::Net net;
    ncnn::Pipeline* waifu2x_preproc;
    ncnn::Pipeline* waifu2x_postproc;
    ncnn::Layer* bicubic_2x;
#ifdef TTA_MODE
    bool tta_mode;
#endif
    int prepadding;
};

#endif // WAIFU2X_H
