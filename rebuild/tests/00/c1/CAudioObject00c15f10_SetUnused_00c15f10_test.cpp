#include <cstdio>
static int g_depth = 0, g_locks = 0;
static unsigned char g_section[1];
struct CProfileLock {
    void* m_pSection; long m_entryTick;
    CProfileLock(void* section) : m_pSection(section), m_entryTick(0) { ++g_locks; ++g_depth; }
    ~CProfileLock() { --g_depth; }
};
struct CAudioObject {
    long m_state;
    void SetUnused(long) { CProfileLock lock(g_section); }
};
int main()
{
    CAudioObject o; o.m_state = 7;
    o.SetUnused(42); o.SetUnused(-1);
    if (o.m_state != 7 || g_locks != 2 || g_depth != 0) { std::printf("00c15f10_TEST FAIL\n"); return 1; }
    std::printf("00c15f10_TEST PASS\n");
    return 0;
}