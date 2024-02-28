//
// Created by Shatyuka on 2024/2/25.
//

#include "api.h"
#include "waifu2x.h"

waifu2x_t waifu2x_create(int gpuid, bool tta_mode, int num_threads)
{
    return (waifu2x_t)new Waifu2x(gpuid, tta_mode, num_threads);
}

void waifu2x_destroy(waifu2x_t waifu2x)
{
    delete (Waifu2x*)waifu2x;
}

int waifu2x_load(waifu2x_t waifu2x, const unsigned char* param, const unsigned char* model)
{
    Waifu2x* instance = (Waifu2x*)waifu2x;
    return instance->load(param, model);
}

int waifu2x_process(waifu2x_t waifu2x, int w, int h, int c, const void* in_data, void* out_data)
{
    Waifu2x* instance = (Waifu2x*)waifu2x;
    ncnn::Mat in(w, h, (void*)in_data, c, c);
    ncnn::Mat out(w, w, out_data, c, c);
    return instance->process(in, out);
}

int waifu2x_process_cpu(waifu2x_t waifu2x, int w, int h, int c, const void* in_data, void* out_data)
{
    Waifu2x* instance = (Waifu2x*)waifu2x;
    ncnn::Mat in(w, h, (void*)in_data, c, c);
    ncnn::Mat out(w, w, out_data, c, c);
    return instance->process_cpu(in, out);
}

void waifu2x_set(waifu2x_t waifu2x, int param, int value)
{
    Waifu2x* instance = (Waifu2x*)waifu2x;
    switch (param)
    {
    case WAIFU2X_PARAM_NOISE:
        instance->noise = value;
        break;
    case WAIFU2X_PARAM_SCALE:
        instance->scale = value;
        break;
    case WAIFU2X_PARAM_TILE_SIZE:
        instance->tilesize = value;
        break;
    default:
        break;
    }
}

int waifu2x_get(waifu2x_t waifu2x, int param)
{
    Waifu2x* instance = (Waifu2x*)waifu2x;
    switch (param)
    {
    case WAIFU2X_PARAM_NOISE:
        return instance->noise;
    case WAIFU2X_PARAM_SCALE:
        return instance->scale;
    case WAIFU2X_PARAM_TILE_SIZE:
        return instance->tilesize;
    default:
        return 0;
    }
}

bool waifu2x_support_gpu(waifu2x_t waifu2x)
{
    Waifu2x* instance = (Waifu2x*)waifu2x;
    return instance->support_gpu();
}
