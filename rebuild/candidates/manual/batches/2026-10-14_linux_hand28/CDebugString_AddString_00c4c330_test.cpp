#include <cstdio>
struct CProfileSection { unsigned char _opaque[4]; };
static int g_locked = 0, g_unlocked = 0, g_countWhileLocked = -1;
static long* g_pCount = 0;
struct CProfileLock {
    CProfileSection* m_pSection; long m_entryTick;
    CProfileLock(CProfileSection* s) : m_pSection(s), m_entryTick(7) { ++g_locked; }
    ~CProfileLock() { ++g_unlocked; g_countWhileLocked = (int)*g_pCount; }
};
struct CDebugString {
    unsigned char _pad[8]; long m_count; unsigned char _pad2[4]; CProfileSection m_section;
    void AddString() { CProfileLock lock(&m_section); ++m_count; }
};
int main()
{
    CDebugString d; d.m_count = 4; g_pCount = &d.m_count;
    d.AddString();
    if (d.m_count != 5 || g_locked != 1 || g_unlocked != 1 || g_countWhileLocked != 5 || sizeof(CProfileLock) != 8) { std::printf("00c4c330_TEST FAIL\n"); return 1; }
    std::printf("00c4c330_TEST PASS\n"); return 0;
}
