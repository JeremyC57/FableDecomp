// Behaviour test for 0x00E839F0 ~CFisticuffsMember: BooName, CheerName, then ShoutTimer (deregistered through the game
// interface with its index), then the entity-script base.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
class CGameScriptInterfaceBase {
public:
    CGameScriptInterfaceBase() : Next(5), LastDeregistered(-1) { }
    virtual long RegisterTimer() { return Next++; }
    virtual void DeregisterTimer(long index) { LastDeregistered = index; Log("T"); }
    long Next;
    long LastDeregistered;
};
static CGameScriptInterfaceBase g_game;
static CGameScriptInterfaceBase* pDSTGame = &g_game;
class CTimer { public: CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { } ~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); } long TimerIndex; };
class CCharString { public: CCharString(const char* tag) : Tag(tag) { } ~CCharString() { Log(Tag); } const char* Tag; };
class CEntityScriptBase { public: virtual ~CEntityScriptBase() { Log("E"); } };
class CFisticuffsMember : public CEntityScriptBase {
public:
    CFisticuffsMember() : CheerName("C"), BooName("B") { }
    void*       ParentClass;
    void*       PMasterData;
    bool        PerformingCheerOrBoo;
    long        FC_Status;
    CTimer      ShoutTimer;
    CCharString CheerName;
    CCharString BooName;
};
int main()
{
    g_log[0] = 0;
    long index;
    { CFisticuffsMember member; index = member.ShoutTimer.TimerIndex; }
    if (std::strcmp(g_log, "BCTE") != 0 || g_game.LastDeregistered != index || index != 5) { std::printf("00e839f0_TEST FAIL\n"); return 1; }
    std::printf("00e839f0_TEST PASS\n");
    return 0;
}