// Behaviour test for libvorbis vorbis_dBquant (0x00CA1E50): x*7.3142857+1023.5 truncated, clamped to 0..1023.
#include <cstdio>
static int vorbis_dBquant(const float* x)
{
    int i = *x * 7.3142857f + 1023.5f;
    if (i > 1023) return (1023);
    if (i < 0) return (0);
    return i;
}
int main()
{
    float a = 0.0f, b = -140.0f, c = -200.0f, d = 10.0f, e = -1.0f;
    if (vorbis_dBquant(&a) != 1023 || vorbis_dBquant(&c) != 0 || vorbis_dBquant(&d) != 1023) { std::printf("00ca1e50_TEST FAIL\n"); return 1; }
    if (vorbis_dBquant(&b) != 0 && vorbis_dBquant(&b) != 1) { std::printf("00ca1e50_TEST FAIL\n"); return 1; }
    if (vorbis_dBquant(&e) != 1016) { std::printf("00ca1e50_TEST FAIL\n"); return 1; }
    std::printf("00ca1e50_TEST PASS\n");
    return 0;
}