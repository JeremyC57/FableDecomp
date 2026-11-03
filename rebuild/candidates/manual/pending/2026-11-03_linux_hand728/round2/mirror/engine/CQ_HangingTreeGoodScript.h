#ifndef FABLE_ENGINE_CQ_HANGINGTREEGOODSCRIPT_H
#define FABLE_ENGINE_CQ_HANGINGTREEGOODSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
#include "fable_string.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
// Layout: original complete PDB field order, native base0x48 and twelve-byte vectors.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeGoodScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HangingTreeGoodScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual ~CQ_HangingTreeGoodScript();
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    bool UpdateWayPoint();
    void ReassignLeader();
    void WatchForHeroNearExit();
    void SpawnParty(long RemainingGuards,CScriptThing& SpawnPoint);
    bool LeaderSpokenTo; // +0x48
    bool PartySuccessfullySetUp; // +0x49
    bool MissionSucceeded; // +0x4a
    bool MissionFailed; // +0x4b
    bool SwitchingRegions; // +0x4c
    bool ReassignLeaderNow; // +0x4d
    bool PartyDefeated; // +0x4e
    bool SpawnHere; // +0x4f
    bool AtWayPoint; // +0x50
    bool HeroKilledPrisoner; // +0x51
    bool QuestStartScreenShown; // +0x52
    bool PartyReadyToTransition; // +0x53
    bool BanditYelledOut; // +0x54
    bool ExecutionerBeenAttacked; // +0x55
    bool ExecutionerBeenAttackedByHero; // +0x56
    bool BanditsFlee; // +0x57
    bool PrisonerEscaping; // +0x58
    bool ExitCutsceneStart; // +0x59
    long HTG_State; // +0x5c
    long CurrentWayPoint; // +0x60
    long CurrentGuards; // +0x64
    CScriptThing Leader; // +0x68
    CScriptThing Prisoner; // +0x74
    CScriptThing PartySpawnPoint; // +0x80
    CScriptThing ActiveWayPoint; // +0x8c
    CCharString CurrentLevel; // +0x98
    CCharString ReasonForFailing; // +0x9c
    std::vector<CScriptThing,std::allocator<CScriptThing> > CurrentEnemyVector; // +0xa0
    std::vector<CScriptThing,std::allocator<CScriptThing> > CurrentGuardVector; // +0xac
    std::vector<CCharString,std::allocator<CCharString> > RegionsWhereSpawned; // +0xb8
    std::vector<CCharString,std::allocator<CCharString> > AreasSpawnedIn; // +0xc4
    std::vector<float,std::allocator<float> > GuardsHealthVector; // +0xd0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HangingTreeGoodScript)==0xdc);
#endif
