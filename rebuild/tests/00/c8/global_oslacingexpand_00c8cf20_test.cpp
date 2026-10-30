// Behaviour test for libogg framing.c _os_lacing_expand (0x00C8CF20): grow lacing/granule fifos by needed + 32 when full.
#include <cstdio>
#include <cstdlib>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
#include <stdint.h>
typedef int64_t ogg_int64_t;
#endif
#endif
struct os_t { int* lacing_vals; ogg_int64_t* granule_vals; long lacing_storage, lacing_fill; };
static void _os_lacing_expand(os_t* os, int needed)
{
    if (os->lacing_storage <= os->lacing_fill + needed) {
        os->lacing_storage += (needed + 32);
        os->lacing_vals = (int*)std::realloc(os->lacing_vals, os->lacing_storage * sizeof(*os->lacing_vals));
        os->granule_vals = (ogg_int64_t*)std::realloc(os->granule_vals, os->lacing_storage * sizeof(*os->granule_vals));
    }
}
int main()
{
    os_t os = { (int*)std::malloc(8 * sizeof(int)), (ogg_int64_t*)std::malloc(8 * sizeof(ogg_int64_t)), 8, 4 };
    _os_lacing_expand(&os, 3);
    if (os.lacing_storage != 8) { std::printf("00c8cf20_TEST FAIL\n"); return 1; }
    _os_lacing_expand(&os, 4);
    if (os.lacing_storage != 44 || !os.lacing_vals || !os.granule_vals) { std::printf("00c8cf20_TEST FAIL\n"); return 1; }
    os.lacing_vals[43] = 1; os.granule_vals[43] = 2; std::free(os.lacing_vals); std::free(os.granule_vals);
    std::printf("00c8cf20_TEST PASS\n"); return 0;
}