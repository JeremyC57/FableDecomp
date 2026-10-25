// Behaviour test for 0x00C94F90 libvorbis _float32_unpack (copy of the logic, 32-bit word semantics).
#include <cstdio>
#include <cmath>
static float float32_unpack(int val)
{
    double mant = val & 0x1fffff;
    int sign = val & 0x80000000;
    int exp = (val & 0x7fe00000) >> 21;
    if (sign) mant = -mant;
    return (float)std::ldexp(mant, exp - 20 - 768);
}
int main()
{
    int ok = float32_unpack(0x60100000) == 1.0f &&                 /* 2^20 * 2^-20 */
             float32_unpack((int)0xE0100000u) == -1.0f &&
             float32_unpack(0x60180000) == 1.5f &&                  /* mant 1.5*2^20 */
             float32_unpack(0x60000003) == 3 / 1048576.0f &&
             float32_unpack(0x60000000 + (2 << 21) + 0x180000) == 6.0f; /* exp 770: 1.5*2^20 * 2^-18 */
    if (float32_unpack(0) != 0.0f) ok = 0;
    if (!ok) { std::printf("00c94f90_TEST FAIL\n"); return 1; }
    std::printf("00c94f90_TEST PASS\n");
    return 0;
}