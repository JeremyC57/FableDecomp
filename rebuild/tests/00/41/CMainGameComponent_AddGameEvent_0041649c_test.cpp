#include <cstdio>
#include "engine/CNetworkClient.h"
#include "engine/CWorld.h"
#include "engine/CDisplayEngine.h"
class CGameEvent;
struct CWorld_Methods : CWorld {
    bool EventIsWorldEvent(const CGameEvent* event) const;           // 0x0049D8C0
    void ReceiveLocalEvent(const CGameEvent* event);                 // 0x0049E1D0
};
struct CDisplayEngine_Methods : CDisplayEngine {
    void ReceiveFeedbackGameEvent(const CGameEvent* event);          // 0x00434A30
};
struct CNetworkClient_Methods : CNetworkClient {
    void SendGameEvent(const CGameEvent* event);                     // 0x004AE9A0
};
class CMainGameComponent { public:
    unsigned char             _pad00[0x24];
    CWorld_Methods*           PWorld;                    // +0x24 scoped_ptr<CWorld>
    CDisplayEngine_Methods*   PDisplayEngine;            // +0x28 scoped_ptr<CDisplayEngine>
    unsigned char             _pad2c[0x13ab8 - 0x2c];
    CNetworkClient_Methods    Client;                    // +0x13ab8
    void AddGameEvent(const CGameEvent& event);
protected:
    bool EventIsSystemEvent(const CGameEvent* event);    // 0x00415FF2
    friend struct CMainGameComponent_Test;               // behaviour-test hook
};
void CMainGameComponent::AddGameEvent(const CGameEvent& event)
{
    if (PWorld->EventIsWorldEvent(&event) || EventIsSystemEvent(&event))
        Client.SendGameEvent(&event);
    PWorld->ReceiveLocalEvent(&event);
    PDisplayEngine->ReceiveFeedbackGameEvent(&event);
}

static char g_log[8]; static int g_n = 0; static bool g_world = false, g_system = false; static const CGameEvent* g_ev = 0; static bool g_ok = true;
bool CWorld_Methods::EventIsWorldEvent(const CGameEvent* e) const { if (e != g_ev) g_ok = false; g_log[g_n++] = 'W'; return g_world; }
void CWorld_Methods::ReceiveLocalEvent(const CGameEvent* e) { if (e != g_ev) g_ok = false; g_log[g_n++] = 'L'; }
void CDisplayEngine_Methods::ReceiveFeedbackGameEvent(const CGameEvent* e) { if (e != g_ev) g_ok = false; g_log[g_n++] = 'D'; }
void CNetworkClient_Methods::SendGameEvent(const CGameEvent* e) { if (e != g_ev) g_ok = false; g_log[g_n++] = 'N'; }
bool CMainGameComponent::EventIsSystemEvent(const CGameEvent* e) { if (e != g_ev) g_ok = false; g_log[g_n++] = 'S'; return g_system; }
static bool Seq(const char* want) { for (int i = 0; i < g_n; ++i) if (g_log[i] != want[i]) return false; return want[g_n] == 0; }
int main()
{
    static CMainGameComponent c; static unsigned char wb[sizeof(CWorld_Methods)], db[sizeof(CDisplayEngine_Methods)];
    c.PWorld = (CWorld_Methods*)wb; c.PDisplayEngine = (CDisplayEngine_Methods*)db;
    static int evbuf[4]; g_ev = (const CGameEvent*)evbuf;
    c.AddGameEvent(*g_ev); bool ok = Seq("WSLD");
    g_n = 0; g_world = true; c.AddGameEvent(*g_ev); ok = ok && Seq("WNLD");
    g_n = 0; g_world = false; g_system = true; c.AddGameEvent(*g_ev); ok = ok && Seq("WSNLD");
    if (ok && g_ok) std::printf("0041649c_TEST PASS\n");
    return 0;
}