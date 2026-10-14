#include <cstdio>
struct CGameEvent { long m_type; };
static CGameEvent g_events[3] = { { 0 }, { 1 }, { 0 } };
struct CGameEventPackage { unsigned long GetNoEvents() { return 3; } CGameEvent* GetEvent(unsigned long i) { return &g_events[i]; } };
struct __multiple_inheritance CDisplayEngine;
struct SGameEventHandler { unsigned char _pad[0x30]; void (CDisplayEngine::*m_pHandler)(CGameEvent* event); unsigned char _pad2[8]; };
SGameEventHandler g_GameEventHandlers[2];
struct CDisplayEngineBaseA { unsigned char _body[4]; };
struct CDisplayEngineBaseB { unsigned char _body[4]; };
static int g_handled = 0; static const void* g_lastEvent = 0;
struct CDisplayEngine : CDisplayEngineBaseA, CDisplayEngineBaseB {
    void OnEvent(CGameEvent* e) { ++g_handled; g_lastEvent = e; }
    void ConfirmFeedbackGameEventPackage(CGameEventPackage& package) {
        for (unsigned long i = 0; i < package.GetNoEvents(); ++i) {
            CGameEvent* event = package.GetEvent(i); SGameEventHandler& h = g_GameEventHandlers[event->m_type];
            if (h.m_pHandler) (this->*h.m_pHandler)(event);
        }
    }
};
int main()
{
    g_GameEventHandlers[0].m_pHandler = &CDisplayEngine::OnEvent; g_GameEventHandlers[1].m_pHandler = 0;
    CDisplayEngine e; CGameEventPackage p; e.ConfirmFeedbackGameEventPackage(p);
    if (g_handled != 2 || g_lastEvent != &g_events[2] || sizeof(SGameEventHandler) != 64) { std::printf("00434a60_TEST FAIL\n"); return 1; }
    std::printf("00434a60_TEST PASS\n"); return 0;
}