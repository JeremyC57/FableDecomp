#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static bool g_held = false, g_isAction = false; static long g_action = 0, g_mode = 0; static const void* g_event = 0;
class CInputEvent { public: long m_kind; };
class CProcessedInput { public: long Player; long Type; unsigned char GameEvents[0xa0]; unsigned char NoGameEvents; unsigned char _pad[3]; long Priority; void SetHandled() { NoGameEvents = 0; Type = 2; std::strcat(g_seq, "H"); } };
class CPlayer { public: void RemoveMode(long mode) { std::strcat(g_seq, "R"); g_mode = mode; } };
struct CInputProcessGameBase { virtual void v00() { } unsigned char _pad04[0x10]; void* World; CPlayer* MyPlayer; void* DefinitionManager; void* Component; void* PGamePlayerInterface;
    bool IsEventButtonHeld(const CInputEvent& event) { std::strcat(g_seq, "B"); g_event = &event; return g_held; }
    bool IsEventGameAction(long action, const CInputEvent& event) { std::strcat(g_seq, "A"); g_action = action; g_event = &event; return g_isAction; } };
class CInputProcessFirstPersonTargeting : public CInputProcessGameBase { public:
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed) { if (IsEventButtonHeld(event)) { if (!IsEventGameAction(10, event)) { MyPlayer->RemoveMode(0x16); processed.SetHandled(); } } }
};
int main()
{
    CPlayer player; CInputProcessFirstPersonTargeting t; t.MyPlayer = &player; CInputEvent e; CProcessedInput p; p.Type = 0; p.NoGameEvents = 1;
    t.ProcessInput(e, p); if (std::strcmp(g_seq, "B") || g_event != &e || p.Type != 0) { std::printf("00690390_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_held = true; g_isAction = true; t.ProcessInput(e, p); if (std::strcmp(g_seq, "BA") || g_action != 10 || p.Type != 0) { std::printf("00690390_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_isAction = false; t.ProcessInput(e, p); if (std::strcmp(g_seq, "BARH") || g_mode != 0x16 || p.Type != 2 || p.NoGameEvents != 0) { std::printf("00690390_TEST FAIL\n"); return 1; }
    std::printf("00690390_TEST PASS\n");
    return 0;
}