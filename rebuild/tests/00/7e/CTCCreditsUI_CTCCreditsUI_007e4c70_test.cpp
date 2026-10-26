// Behaviour test for 0x007E4C70 CTCCreditsUI::~CTCCreditsUI: CreditPart destroyed, the Components buffer
// freed when non-null, then ~CTCBase.
#include <cstdio>

static int g_log[16];
static int g_n;
static void* g_freed;
static void Log(int v) { g_log[g_n++] = v; }
static void test_free(void* p) { Log(2); g_freed = p; }

class CCharString { public: void* PStringData; ~CCharString() { Log(1); } };
struct CComponentVector {
    void** m_pFirst; void** m_pLast; void** m_pEnd;
    ~CComponentVector() { if (m_pFirst != 0) test_free(m_pFirst); }
};
struct CTCBase_Calls { void* ParentThing; ~CTCBase_Calls() { Log(3); } };
class CTCCreditsUI : public CTCBase_Calls { public:
    bool             Active;
    CComponentVector Components;
    bool             Quitting;
    CCharString      CreditPart;
    double           LastRenderTime;
};

int main()
{
    void* items[2];
    g_n = 0;
    { CTCCreditsUI ui; ui.Components.m_pFirst = items; }
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || g_freed != items) { std::printf("007e4c70_TEST FAIL\n"); return 1; }
    g_n = 0; g_freed = 0;
    { CTCCreditsUI ui; ui.Components.m_pFirst = 0; }
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 3 || g_freed != 0) { std::printf("007e4c70_TEST FAIL\n"); return 1; }
    std::printf("007e4c70_TEST PASS\n");
    return 0;
}