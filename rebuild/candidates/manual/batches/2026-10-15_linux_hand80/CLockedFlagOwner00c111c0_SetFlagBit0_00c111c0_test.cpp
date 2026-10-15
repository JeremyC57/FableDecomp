#include <cstdio>
#include <cstring>
// Behaviour model of 00c111c0: the flag word's bit 0 is set while the scoped lock is held.
static int g_depth = 0, g_maxDepth = 0;
struct CGlobalLockTarget { int dummy; };
static CGlobalLockTarget g_lock;
struct CScopedLock {
    CGlobalLockTarget* m_pTarget; long m_state;
    CScopedLock(CGlobalLockTarget* t) : m_pTarget(t), m_state(0) { ++g_depth; if (g_depth > g_maxDepth) g_maxDepth = g_depth; }
    ~CScopedLock() { --g_depth; }
};
static int g_bitSetUnderLock = 0;
struct COwner {
    unsigned char _pad[0x7c8];
    unsigned long m_bit0 : 1;
    unsigned long m_bit1 : 1;
    unsigned long m_bit2 : 1;
    unsigned long m_rest : 29;
    void SetFlagBit0() { CScopedLock lock(&g_lock); m_bit0 = 1; g_bitSetUnderLock = (g_depth == 1); }
};
int main()
{
    COwner o; std::memset(&o, 0, sizeof(o));
    o.m_rest = 0x155;
    o.SetFlagBit0();
    if (!o.m_bit0 || o.m_rest != 0x155 || !g_bitSetUnderLock || g_depth != 0 || g_maxDepth != 1) { std::printf("00c111c0_TEST FAIL\n"); return 1; }
    if ((0 == 0 && (o.m_bit1 || o.m_bit2)) || (0 == 1 && (o.m_bit0 || o.m_bit2))) { std::printf("00c111c0_TEST FAIL\n"); return 1; }
    o.SetFlagBit0();   // idempotent
    if (!o.m_bit0 || o.m_rest != 0x155) { std::printf("00c111c0_TEST FAIL\n"); return 1; }
    std::printf("00c111c0_TEST PASS\n");
    return 0;
}
