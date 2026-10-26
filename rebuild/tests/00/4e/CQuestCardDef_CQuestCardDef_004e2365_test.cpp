// Behaviour test for 0x004E2365 CQuestCardDef::~CQuestCardDef: the map is destroyed first, then the
// RewardObjects buffer is freed when non-null, then the CDefPointeeBase and root destructors run.
#include <cstdio>

static int g_log[16];
static int g_n;
static void* g_freed;
static void Log(int v) { g_log[g_n++] = v; }
static void test_free(void* p) { Log(1); g_freed = p; }

struct CRetailVectorLong {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    ~CRetailVectorLong() { if (m_pFirst != 0) test_free(m_pFirst); }
};
struct CRetailMapPrerequisites {
    void* m_pHead;
    ~CRetailMapPrerequisites() { Log(2); }
};
struct CResource_Calls { ~CResource_Calls() { Log(3); } };
class CDefPointeeBase : public CResource_Calls { public: ~CDefPointeeBase() { Log(4); } };
class CQuestCardDef : public CDefPointeeBase { public:
    CRetailVectorLong       RewardObjects;
    bool                    IsCore, IsVignette, IsExclusive, MakeVignetteRouteAppearOnMinimap;
    CRetailMapPrerequisites Prerequisites;
};

static int Check(const int* want, int n)
{
    if (g_n != n) return 0;
    for (int i = 0; i < n; ++i) if (g_log[i] != want[i]) return 0;
    return 1;
}

int main()
{
    long buffer[4];
    g_n = 0; g_freed = 0;
    {
        CQuestCardDef def;
        def.RewardObjects.m_pFirst = buffer;
        def.Prerequisites.m_pHead = 0;
    }
    const int full[] = { 2, 1, 4, 3 };
    if (!Check(full, 4) || g_freed != buffer) { std::printf("004e2365_TEST FAIL\n"); return 1; }
    g_n = 0; g_freed = 0;
    {
        CQuestCardDef def;
        def.RewardObjects.m_pFirst = 0;
        def.Prerequisites.m_pHead = 0;
    }
    const int empty[] = { 2, 4, 3 };
    if (!Check(empty, 3) || g_freed != 0) { std::printf("004e2365_TEST FAIL\n"); return 1; }
    std::printf("004e2365_TEST PASS\n");
    return 0;
}