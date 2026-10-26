// Behaviour test for 00cdb3c0 ~CCrateTeamManager: both script things release their counted PImp (the
// shared block dies with its last reference), the base destructors run carrier first, and the timer
// index is returned to the thing manager last.
#include <cstdio>

struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static int g_destroyed = 0; static int g_order[8]; static int g_n = 0;
static void Destroy(void*) { ++g_destroyed; }
struct CCountedPointer {
    void* m_pObject; SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
        m_pObject = 0; m_pCount = 0;
    }
};
struct CBase { int m_tag; virtual ~CBase() { g_order[g_n++] = m_tag; } };
class CScriptThing : public CBase { public: CCountedPointer PImp; virtual ~CScriptThing() { } };
static long g_releasedTimer = -1;
class CTimer { public: long TimerIndex; ~CTimer() { g_releasedTimer = TimerIndex; g_order[g_n++] = 99; } };
struct CCrateTeamManager { long MemberCount; CTimer Timer; CScriptThing CrateDropPos; CScriptThing TeamCrateCarrier; };

int main()
{
    SCountBlock* shared = new SCountBlock; shared->m_references = 2; shared->m_pfnDestroy = Destroy; shared->m_pObject = 0;
    {
        CCrateTeamManager m;
        m.Timer.TimerIndex = 17;
        m.CrateDropPos.m_tag = 1; m.CrateDropPos.PImp.m_pObject = 0; m.CrateDropPos.PImp.m_pCount = shared;
        m.TeamCrateCarrier.m_tag = 2; m.TeamCrateCarrier.PImp.m_pObject = 0; m.TeamCrateCarrier.PImp.m_pCount = shared;
    }
    bool ok = g_destroyed == 1 && g_releasedTimer == 17 && g_n == 3 && g_order[0] == 2 && g_order[1] == 1 && g_order[2] == 99;
    if (!ok) { std::printf("00cdb3c0_TEST FAIL\n"); return 1; }
    std::printf("00cdb3c0_TEST PASS\n");
    return 0;
}