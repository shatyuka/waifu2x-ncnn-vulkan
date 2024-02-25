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

#if _WIN32
int waifu2x_load(waifu2x_t waifu2x, const wchar_t* param_path, const wchar_t* model_path)
#else
int waifu2x_load(waifu2x_t waifu2x, const char* param_path, const char* model_path)
#endif
{
    Waifu2x* instance = (Waifu2x*)waifu2x;
    return instance->load(param_path, model_path);
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

#define SET_VALUE(old, new) \
    ({ \
    const auto __old_value = old; \
    old = new; \
    __old_value; \
    })

int waifu2x_set(waifu2x_t waifu2x, int param, int value)
{
    Waifu2x* instance = (Waifu2x*)waifu2x;
    switch (param)
    {
    case WAIFU2X_PARAM_NOISE:
        return SET_VALUE(instance->noise, value);
    case WAIFU2X_PARAM_SCALE:
        return SET_VALUE(instance->scale, value);
    case WAIFU2X_PARAM_TILE_SIZE:
        return SET_VALUE(instance->tilesize, value);
    case WAIFU2X_PARAM_PRE_PADDING:
        return SET_VALUE(instance->prepadding, value);
    default:
        return 0;
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
    case WAIFU2X_PARAM_PRE_PADDING:
        return instance->prepadding;
    default:
        return 0;
    }
}
