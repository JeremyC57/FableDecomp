#include <cstdio>
static int g_depth = 0, g_maxDepth = 0, g_locks = 0;
static unsigned char g_section[1];
struct CProfileLock {
    void* m_pSection; long m_entryTick;
    CProfileLock(void* section) : m_pSection(section), m_entryTick(0) { ++g_locks; if (++g_depth > g_maxDepth) g_maxDepth = g_depth; }
    ~CProfileLock() { --g_depth; }
};
struct CAudioObject {
    long GetElementCount() { CProfileLock lock(g_section); return 8; }
};
int main()
{
    CAudioObject o;
    long n = o.GetElementCount();
    if (n != 8 || g_locks != 1 || g_depth != 0 || g_maxDepth != 1) { std::printf("00c0ac00_TEST FAIL\n"); return 1; }
    std::printf("00c0ac00_TEST PASS\n");
    return 0;
}