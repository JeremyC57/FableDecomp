// Behaviour test for CQuestManager::OnWorldDestruction() @ 0x004B49E0
#include <cstdio>

static long g_frame = 4321;
static long GFGetCurrentFrame() { return g_frame; }
static char g_log[8];
static int  g_logCount = 0;

struct CBoastList {
    char m_tag; int m_size;
    void clear() { m_size = 0; if (g_logCount < 7) g_log[g_logCount++] = m_tag; }
};

struct CQuestManager {
    unsigned long FrameBoastsLastUpdated;
    CBoastList    Boasts;
    CBoastList    AcceptedBoasts;
    bool          WorldDestructed;
    void DeactivateAllQuests() { if (g_logCount < 7) g_log[g_logCount++] = 'D'; }
    void RemoveAllBoasts()
    {
        Boasts.clear();
        AcceptedBoasts.clear();
        FrameBoastsLastUpdated = GFGetCurrentFrame();
    }
    void OnWorldDestruction()
    {
        DeactivateAllQuests();
        RemoveAllBoasts();
        WorldDestructed = true;
    }
};

int main()
{
    CQuestManager m;
    m.FrameBoastsLastUpdated = 0; m.WorldDestructed = false;
    m.Boasts.m_tag = 'B'; m.Boasts.m_size = 3; m.AcceptedBoasts.m_tag = 'A'; m.AcceptedBoasts.m_size = 2;
    m.OnWorldDestruction();
    if (!m.WorldDestructed || m.FrameBoastsLastUpdated != 4321 || m.Boasts.m_size != 0 || m.AcceptedBoasts.m_size != 0) { std::printf("004b49e0_TEST FAIL\n"); return 1; }
    if (g_logCount != 3 || g_log[0] != 'D' || g_log[1] != 'B' || g_log[2] != 'A') { std::printf("004b49e0_TEST FAIL\n"); return 1; }
    std::printf("004b49e0_TEST PASS\n");
    return 0;
}
