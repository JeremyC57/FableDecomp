#include <cstdio>
static int g_depth = 0, g_locks = 0;
static unsigned char g_section[1];
struct CProfileLock {
    void* m_pSection; long m_entryTick;
    CProfileLock(void* section) : m_pSection(section), m_entryTick(0) { ++g_locks; ++g_depth; }
    ~CProfileLock() { --g_depth; }
};
struct SAudioOwnerRecord { unsigned char _pad[0x14]; unsigned short m_shortValue; };
struct CAudioOwner {
    SAudioOwnerRecord* m_pOwner;
    long GetShortValue() { CProfileLock lock(g_section); return m_pOwner->m_shortValue; }
};
int main()
{
    SAudioOwnerRecord r; r.m_shortValue = 0xFFFE;
    CAudioOwner o; o.m_pOwner = &r;
    long v = o.GetShortValue();
    r.m_shortValue = 5;
    long w = o.GetShortValue();
    if (v != 65534L || w != 5 || g_locks != 2 || g_depth != 0) { std::printf("00c1e4d0_TEST FAIL\n"); return 1; }
    std::printf("00c1e4d0_TEST PASS\n");
    return 0;
}