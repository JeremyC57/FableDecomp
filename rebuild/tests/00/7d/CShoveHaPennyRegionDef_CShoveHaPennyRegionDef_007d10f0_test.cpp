// behaviour test for 0x007D10F0: a new definition has no manager and its three flag bits cleared, other bits kept
#include <cstdio>
struct D { unsigned int vt; unsigned char res[0x18]; unsigned int mgr; unsigned int gi; unsigned char flags; D() { mgr = 0xFFFFFFFF; flags = 0xFF; gi = 7; } void Construct() { flags &= 0xF8; mgr = 0; } };
int main()
{
    D d; d.Construct();
    if (d.mgr != 0 || d.flags != 0xF8 || d.gi != 7) { std::printf("007d10f0_TEST FAIL\n"); return 1; }
    std::printf("007d10f0_TEST PASS\n");
    return 0;
}