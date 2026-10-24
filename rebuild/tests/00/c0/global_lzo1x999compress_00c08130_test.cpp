#include <cstdio>
#ifdef _MSC_VER
#define CDECL __cdecl
#else
#define CDECL
#endif
typedef unsigned char lzo_byte; typedef unsigned int lzo_uint; typedef lzo_uint* lzo_uintp; typedef void* lzo_voidp;
typedef unsigned int lzo_uint32; typedef void (CDECL *lzo_progress_callback_t)(lzo_uint, lzo_uint);
static lzo_uint g_a[16]; static const void* g_p[8];
static int CDECL lzo1x_999_compress_internal(const lzo_byte* in, lzo_uint in_len, lzo_byte* out, lzo_uintp out_len,
    lzo_voidp wrkmem, const lzo_byte* dict, lzo_uint dict_len, lzo_progress_callback_t cb, int try_lazy,
    lzo_uint good_length, lzo_uint max_lazy, lzo_uint nice_length, lzo_uint max_chain, lzo_uint32 flags)
{
    g_p[0] = in; g_a[0] = in_len; g_p[1] = out; g_p[2] = out_len; g_p[3] = wrkmem; g_p[4] = dict; g_a[1] = dict_len;
    g_p[5] = (const void*)cb; g_a[2] = (lzo_uint)try_lazy; g_a[3] = good_length; g_a[4] = max_lazy; g_a[5] = nice_length;
    g_a[6] = max_chain; g_a[7] = flags; return 42;
}
static int CDECL lzo1x_999_compress(const lzo_byte* in, lzo_uint in_len, lzo_byte* out, lzo_uintp out_len, lzo_voidp wrkmem)
{
    return lzo1x_999_compress_internal(in, in_len, out, out_len, wrkmem, 0, 0, (lzo_progress_callback_t)0, 2, 32, 128, 0, 2048, 1);
}
int main()
{
    lzo_byte i[4], o[4]; lzo_uint ol; char w[4];
    if (lzo1x_999_compress(i, 4, o, &ol, w) != 42 || g_p[0] != i || g_a[0] != 4 || g_p[1] != o || g_p[2] != &ol ||
        g_p[3] != w || g_p[4] != 0 || g_a[1] != 0 || g_p[5] != 0 || g_a[2] != 2 || g_a[3] != 32 || g_a[4] != 128 ||
        g_a[5] != 0 || g_a[6] != 2048 || g_a[7] != 1) { std::printf("00c08130_TEST FAIL\n"); return 1; }
    std::printf("00c08130_TEST PASS\n");
    return 0;
}