#include <cstdio>
#include <cstring>
// Behaviour of 0x00C3EB80 (chunk 901 piece): -9 without a device, else the device's slot 9 with all four arguments
static long g_seen = 0;
struct CAudioDevice_Retail {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual long v03() { return 3; }
    virtual long v04() { return 4; }
    virtual long v05() { return 5; }
    virtual long v06() { return 6; }
    virtual long v07() { return 7; }
    virtual long v08() { return 8; }
    virtual long vfunc_9(long a0, long a1, long a2, long a3) { g_seen = a0 + a1 + a2 + a3; return 42; }
};
struct CAudioOwner_Retail {
    unsigned char _pad_0x00[0x1d4];
    CAudioDevice_Retail* m_pDevice;
    long func_00c3eb80(long a0, long a1, long a2, long a3);
};
long CAudioOwner_Retail::func_00c3eb80(long a0, long a1, long a2, long a3) { if (m_pDevice) return m_pDevice->vfunc_9(a0, a1, a2, a3); return -9; }
int main()
{
    CAudioDevice_Retail d; CAudioOwner_Retail o; o.m_pDevice = 0;
    long a = o.func_00c3eb80(1, 2, 3, 4); o.m_pDevice = &d; long b = o.func_00c3eb80(1, 2, 3, 4);
    if (a != -9 || b != 42 || g_seen != 10) { std::printf("00c3eb80_TEST FAIL\n"); return 1; }
    std::printf("00c3eb80_TEST PASS\n"); return 0;
}