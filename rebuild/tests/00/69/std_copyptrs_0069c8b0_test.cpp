// Behaviour test for std::__copy_ptrs<CQuake*> (0x0069C8B0): element-wise forward copy, returns result + n.
#include <cstdio>
struct V { float X, Y, Z; };
struct CQuake { V Position; V Axis; int Frame; float Amplitude, Duration, Frequency, Phase; bool Global; };
static CQuake* copy_ptrs(CQuake* first, CQuake* last, CQuake* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    CQuake src[3], dst[4];
    for (int i = 0; i < 3; ++i) { src[i].Frame = i + 10; src[i].Amplitude = i * 0.5f; src[i].Global = (i & 1) != 0; src[i].Axis.Y = (float)i; }
    for (int i = 0; i < 4; ++i) dst[i].Frame = -1;
    CQuake* r = copy_ptrs(src, src + 3, dst);
    if (r != dst + 3 || dst[3].Frame != -1) { std::printf("0069c8b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (dst[i].Frame != i + 10 || dst[i].Amplitude != i * 0.5f || dst[i].Global != ((i & 1) != 0) || dst[i].Axis.Y != (float)i) { std::printf("0069c8b0_TEST FAIL\n"); return 1; }
    if (copy_ptrs(src, src, dst) != dst) { std::printf("0069c8b0_TEST FAIL\n"); return 1; }
    std::printf("0069c8b0_TEST PASS\n");
    return 0;
}