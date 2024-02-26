//
// Created by Shatyuka on 2024/2/25.
//

#ifndef API_H
#define API_H

#ifndef DLL_EXPORT
#ifdef _WIN32
#ifdef _WINDLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

typedef void* waifu2x_t;

#define WAIFU2X_PARAM_NOISE 0
#define WAIFU2X_PARAM_SCALE 1
#define WAIFU2X_PARAM_TILE_SIZE 2
#define WAIFU2X_PARAM_PRE_PADDING 3

EXTERN_C DLL_EXPORT waifu2x_t waifu2x_create(int gpuid, bool tta_mode = false, int num_threads = 1);
EXTERN_C DLL_EXPORT void waifu2x_destroy(waifu2x_t waifu2x);
EXTERN_C DLL_EXPORT int waifu2x_load(waifu2x_t waifu2x, const unsigned char* param, const unsigned char* model);
EXTERN_C DLL_EXPORT int waifu2x_process(waifu2x_t waifu2x, int w, int h, int c, const void* in_data, void* out_data);
EXTERN_C DLL_EXPORT int waifu2x_process_cpu(waifu2x_t waifu2x, int w, int h, int c, const void* in_data, void* out_data);
EXTERN_C DLL_EXPORT int waifu2x_set(waifu2x_t waifu2x, int param, int value);
EXTERN_C DLL_EXPORT int waifu2x_get(waifu2x_t waifu2x, int param);
EXTERN_C DLL_EXPORT bool waifu2x_support_gpu(waifu2x_t waifu2x);

#endif // API_H
