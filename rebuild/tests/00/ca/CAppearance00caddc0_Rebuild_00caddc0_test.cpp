#include <cstdio>

static int g_depth = 0, g_enters = 0, g_storesInside = 1;
struct CRITICAL_SECTION_T { int locked; };
static void Enter(CRITICAL_SECTION_T* cs) { cs->locked = 1; ++g_enters; }
static void Leave(CRITICAL_SECTION_T* cs) { cs->locked = 0; }
struct CAppearance {
    CRITICAL_SECTION_T m_lock; long m_count0, m_count1, m_count2, m_count3, m_dirty;
    long Rebuild()
    {
        Enter(&m_lock);
        m_count0 = 0;
        m_count1 = 0;
        m_count2 = 0;
        m_count3 = 0;
        m_dirty = 1;
        Leave(&m_lock);
        return 0;
    }
};

int main()
{
    CAppearance a; a.m_lock.locked = 0; a.m_count0 = 1; a.m_count1 = 2; a.m_count2 = 3; a.m_count3 = 4; a.m_dirty = 0;
    if (a.Rebuild() != 0 || a.m_count0 || a.m_count1 || a.m_count2 || a.m_count3 || a.m_dirty != 1 || a.m_lock.locked || g_enters != 1)
        { std::printf("00caddc0_TEST FAIL\n"); return 1; }
    std::printf("00caddc0_TEST PASS\n");
    return 0;
}