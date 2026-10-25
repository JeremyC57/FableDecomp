// Behaviour test for 0x00C92640 libvorbis unitnorm (copy of the logic).
#include <cstdio>
#include <cstring>
typedef unsigned int u32;
static float unitnorm(float x)
{
    u32 ix; std::memcpy(&ix, &x, 4);
    ix = (ix & 0x80000000u) | 0x3f800000u;
    std::memcpy(&x, &ix, 4);
    return x;
}
static float from_bits(u32 i) { float f; std::memcpy(&f, &i, 4); return f; }
int main()
{
    float negzero = from_bits(0x80000000u);                  /* VC7.1 folds a -0.0f literal to +0.0 */
    int ok = unitnorm(3.5f) == 1.0f && unitnorm(-0.25f) == -1.0f && unitnorm(0.0f) == 1.0f &&
             unitnorm(negzero) == -1.0f && unitnorm(1e-30f) == 1.0f && unitnorm(-12345.0f) == -1.0f;
    if (!ok) { std::printf("00c92640_TEST FAIL\n"); return 1; }
    std::printf("00c92640_TEST PASS\n");
    return 0;
}