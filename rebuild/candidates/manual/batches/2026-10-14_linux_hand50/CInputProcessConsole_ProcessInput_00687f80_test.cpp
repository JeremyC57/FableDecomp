#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static bool g_consoleTakes = false; static const void* g_event = 0;
struct CInputEvent { long m_kind; };
struct CProcessedInput { long Player; long Type; unsigned char GameEvents[0xa0]; unsigned char NoGameEvents; unsigned char _pad[3]; long Priority; void SetHandled() { NoGameEvents = 0; Type = 2; std::strcat(g_seq, "H"); } };
struct CConsole { virtual void v00() { } unsigned char _body[0x98]; CConsole() { std::strcat(g_seq, "N"); } bool ProcessInput(const CInputEvent& event) { std::strcat(g_seq, "P"); g_event = &event; return g_consoleTakes; } };
struct CCountedPointer_CConsole { CConsole* m_pObject; void* m_pCount; void Reset(CConsole* object) { m_pObject = object; std::strcat(g_seq, "R"); } };
static CCountedPointer_CConsole g_pConsole = { 0, 0 };
struct CInputProcessConsole {
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        if (g_pConsole.m_pObject == 0) g_pConsole.Reset(new CConsole());
        if (g_pConsole.m_pObject->ProcessInput(event)) processed.SetHandled();
    }
};
int main()
{
    CInputProcessConsole c; CInputEvent e; CProcessedInput p; p.NoGameEvents = 3; p.Type = 0;
    c.ProcessInput(e, p); if (std::strcmp(g_seq, "NRP") || g_event != &e || !g_pConsole.m_pObject || p.NoGameEvents != 3 || p.Type != 0) { std::printf("00687f80_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_consoleTakes = true; c.ProcessInput(e, p); if (std::strcmp(g_seq, "PH") || p.NoGameEvents != 0 || p.Type != 2) { std::printf("00687f80_TEST FAIL\n"); return 1; }
    delete g_pConsole.m_pObject;
    std::printf("00687f80_TEST PASS\n");
    return 0;
}
