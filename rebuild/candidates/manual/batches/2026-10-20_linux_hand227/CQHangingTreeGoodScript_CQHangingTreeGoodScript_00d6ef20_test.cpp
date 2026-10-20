// Behaviour test for the CQ_HangingTreeGoodScript constructor (0x00D6EF20): the base runs, the owner/interface pair
// is stored, the four script handles, two strings and five vectors start empty; the plain bools/longs are left alone.
#include <cstdio>
#include <cstring>
#include <new>

struct CGameScriptInterface { int dummy; };
static int g_baseCtors = 0, g_thingRoots = 0, g_strings = 0;
struct CScriptBase {
    virtual void v00() {}
    unsigned char _body[0x3c];
    CScriptBase() { ++g_baseCtors; }
};
struct CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;
    void* m_pOwner;
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};
struct CBase { CBase() { ++g_thingRoots; } virtual ~CBase() {} };
struct CScriptThing;
struct CCountedPointer_CScriptThing {
    CScriptThing* Data; void* Info;
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};
struct CScriptThing : CBase { CCountedPointer_CScriptThing PImp; CScriptThing() { } };
struct CCharString { void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end(0) { }
};
struct CQ_HangingTreeGoodScript : CScriptObject {
    bool LeaderSpokenTo, PartySuccessfullySetUp, MissionSucceeded, MissionFailed, SwitchingRegions, ReassignLeaderNow,
         PartyDefeated, SpawnHere, AtWayPoint, HeroKilledPrisoner, QuestStartScreenShown, PartyReadyToTransition,
         BanditYelledOut, ExecutionerBeenAttacked, ExecutionerBeenAttackedByHero, BanditsFlee, PrisonerEscaping, ExitCutsceneStart;
    long HTG_State, CurrentWayPoint, CurrentGuards;
    CScriptThing Leader, Prisoner, PartySpawnPoint, ActiveWayPoint;
    CCharString CurrentLevel, ReasonForFailing;
    CRetailVector<CScriptThing> CurrentEnemyVector, CurrentGuardVector;
    CRetailVector<CCharString> RegionsWhereSpawned, AreasSpawnedIn;
    CRetailVector<float> GuardsHealthVector;
    virtual void v00() {}
    CQ_HangingTreeGoodScript(void* owner, CGameScriptInterface* game);
};
CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}
int main()
{
    static double storage[(sizeof(CQ_HangingTreeGoodScript) + 7) / 8];
    std::memset(storage, 0xcd, sizeof(storage));
    CGameScriptInterface gi; int owner = 0;
    CQ_HangingTreeGoodScript* s = new (storage) CQ_HangingTreeGoodScript(&owner, &gi);
    int bad = 0;
    if (s->m_pGame != &gi || s->m_pOwner != &owner || g_baseCtors != 1 || g_thingRoots != 4 || g_strings != 2) bad = 1;
    CScriptThing* things[4] = { &s->Leader, &s->Prisoner, &s->PartySpawnPoint, &s->ActiveWayPoint };
    for (int i = 0; i < 4; ++i) if (things[i]->PImp.Data || things[i]->PImp.Info) bad = 1;
    if (s->CurrentLevel.PStringData || s->ReasonForFailing.PStringData) bad = 1;
    if (s->CurrentEnemyVector._M_start || s->CurrentEnemyVector._M_end || s->CurrentGuardVector._M_finish ||
        s->RegionsWhereSpawned._M_start || s->AreasSpawnedIn._M_end || s->GuardsHealthVector._M_finish) bad = 1;
    unsigned char b; std::memcpy(&b, &s->LeaderSpokenTo, 1);
    if (b != 0xcd) bad = 1;                          // plain members are not initialised
    if (bad) { std::printf("00d6ef20_TEST FAIL\n"); return 1; }
    std::printf("00d6ef20_TEST PASS\n");
    return 0;
}
