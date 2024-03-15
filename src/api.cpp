//
// Created by Shatyuka on 2024/2/25.
//

#include "api.h"
#include "waifu2x.h"

#include <csignal>
#include <csetjmp>

int ncnn_get_default_gpu_index()
{
    return ncnn::get_default_gpu_index();
}

const char* ncnn_get_gpu_name(int gpuid)
{
    if (gpuid < 0) return "CPU";
    return ncnn::get_gpu_info(gpuid).device_name();
}

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

void waifu2x_set_param(waifu2x_t waifu2x, int param, int value)
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

int waifu2x_get_param(waifu2x_t waifu2x, int param)
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

thread_local jmp_buf waifu2x_self_test_jmpbuf;

void waifu2x_self_test_signal_handler(int signal)
{
    longjmp(waifu2x_self_test_jmpbuf, 1); // NOLINT(*-err52-cpp)
}

waifu2x_status waifu2x_self_test(waifu2x_t waifu2x)
{
    waifu2x_status result;
#ifdef _WIN32
    _crt_signal_t old_sigsegv_handler;
#elif
    sig_t old_sigsegv_handler;
#endif
    old_sigsegv_handler = signal(SIGSEGV, &waifu2x_self_test_signal_handler);
    if (setjmp(waifu2x_self_test_jmpbuf) == 0) // NOLINT(*-err52-cpp)
    {
        if (!waifu2x)
        {
            int gpu_id = ncnn_get_default_gpu_index();
            result = gpu_id == -1 ? CpuMode : Ok;
        }
        else
        {
            uint8_t in[]{0xFF, 0xFF, 0xFF};
            uint8_t out[2 * 2 * 3]{0};
            waifu2x_process(waifu2x, 1, 1, 3, in, out);
            result = out[0] == 0 ? TestNotPassed : Ok;
        }
    }
    else
    {
        // TODO: release resources
        result = NotAvailable;
    }
    signal(SIGSEGV, old_sigsegv_handler);
    return result;
}
