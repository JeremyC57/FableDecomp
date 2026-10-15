#include <cstdio>
static int g_depth = 0, g_locks = 0; static int g_readDepth = -1;
static unsigned char g_section[1];
struct CProfileLock {
    void* m_pSection; long m_entryTick;
    CProfileLock(void* section) : m_pSection(section), m_entryTick(0) { ++g_locks; ++g_depth; }
    ~CProfileLock() { --g_depth; }
};
struct CAudioListener {
    float m_value;
    float Read() { g_readDepth = g_depth; return m_value; }
    float GetValue() { CProfileLock lock(g_section); return Read(); }
};
int main()
{
    CAudioListener l; l.m_value = 2.5f;
    float v = l.GetValue();
    if (v != 2.5f || g_locks != 1 || g_depth != 0 || g_readDepth != 1) { std::printf("00c11150_TEST FAIL\n"); return 1; }
    std::printf("00c11150_TEST PASS\n");
    return 0;
}