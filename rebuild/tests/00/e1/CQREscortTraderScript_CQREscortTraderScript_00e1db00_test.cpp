// Behaviour test for the CQR_EscortTraderScript constructor (0x00E1DB00): arguments stored, the four timers registered
// in declaration order through the global interface, all 18 strings constructed and the six route vectors empty.
#include <cstdio>
#include <cstring>
#include <new>

static int g_strings = 0;
struct CCharString { void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
struct CGameScriptInterface {
    long next;
    virtual long RegisterTimer() { return next++; }
};
static CGameScriptInterface g_gsi;
static CGameScriptInterface* g_pGameScriptInterface = &g_gsi;
struct CTimer { long Index; CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { } };
struct CScriptBase { virtual void v00() {} unsigned char _body[0x3c]; CScriptBase() { } };
struct CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame; void* m_pOwner;
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};
template <class T> struct CRetailVector { T* a; T* b; T* c; CRetailVector() : a(0), b(0), c(0) { } };
struct CQR_EscortTraderScript : CScriptObject {
    bool MissionSucceeded, MissionFailed;
    long BanditSpawnCount;
    CCharString BonusObject;
    CTimer TraderComplainTimer, TraderExpressionDetectTimer, TraderHitDetectTimer, TraderRandomSpeechTimer;
    CCharString RandomSpeechFollow[10];
    CCharString RandomSpeechWait[5];
    long TradeRoute;
    CRetailVector<CCharString> RouteDynamic[3];
    CRetailVector<CCharString> RouteStatic[3];
    CCharString PreviousLoadedRegion, CurrentLoadedRegion;
    virtual void v00() {}
    CQR_EscortTraderScript(void* owner, CGameScriptInterface* game);
};
CQR_EscortTraderScript::CQR_EscortTraderScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}
int main()
{
    static double storage[(sizeof(CQR_EscortTraderScript) + 7) / 8];
    std::memset(storage, 0xcd, sizeof(storage));
    g_gsi.next = 40;
    int owner = 0; CGameScriptInterface game;
    CQR_EscortTraderScript* s = new (storage) CQR_EscortTraderScript(&owner, &game);
    int bad = 0;
    if (s->m_pGame != &game || s->m_pOwner != &owner || g_strings != 18) bad = 1;
    if (s->TraderComplainTimer.Index != 40 || s->TraderExpressionDetectTimer.Index != 41 ||
        s->TraderHitDetectTimer.Index != 42 || s->TraderRandomSpeechTimer.Index != 43) bad = 1;
    for (int i = 0; i < 3; ++i)
        if (s->RouteDynamic[i].a || s->RouteDynamic[i].c || s->RouteStatic[i].b || s->RouteStatic[i].c) bad = 1;
    if (s->RandomSpeechFollow[9].PStringData || s->RandomSpeechWait[4].PStringData || s->CurrentLoadedRegion.PStringData) bad = 1;
    if (bad) { std::printf("00e1db00_TEST FAIL\n"); return 1; }
    std::printf("00e1db00_TEST PASS\n");
    return 0;
}