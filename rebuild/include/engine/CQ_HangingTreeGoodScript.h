#ifndef FABLE_ENGINE_CQ_HANGINGTREEGOODSCRIPT_H
#define FABLE_ENGINE_CQ_HANGINGTREEGOODSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
#include "fable_string.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
// Layout: original complete PDB field order, native base0x48 and twelve-byte vectors.
// Retail opt-in FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT (chunk 744): the remaining PDB methods (Ego_r/FableWin name sets, all
// ...@CQ_HangingTreeGoodScript@NScript@@QAE..) with their retail addresses, and the ten nested entity scripts the Main 0x00D6F4B0
// binds (FableWin/Ego_r ?Alloc@CHTG_*@CQ_HangingTreeGoodScript@NScript@@SI...; Alloc address = the ClassAllocFunc stored by Main).
// Requires engine/CEntityScriptBase.h and engine/CCountedPointer.h before the include. The default text is unchanged.
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
#ifdef FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT
    void RemoveDregs();                                                  // 0x00D6FD00
    void CleanUp();                                                      // 0x00D72B70
    void RemoveParty(bool);                                              // 0x00D732F0
    void DoMission();                                                    // 0x00D74430
    void WatchForMissionEnd();                                           // 0x00D74CE0
    void MonitorHeroProximity();                                         // 0x00D74D30
    void FinalRound();                                                   // 0x00D75060
    void SetGuardsHealth();                                              // 0x00D766C0
    void DoConversations();                                              // 0x00D767F0
    void GetGuardsHealth();                                              // 0x00D77340
    void MonitorRegionPresence();                                        // 0x00D773E0
    void DoCutscene(long);                                               // 0x00D77690
    class CHTG_Prisoner : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeGoodScript&,CScriptDataBase*,const CScriptThing&); };           // 0x00D71980
    class CHTG_Guard : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeGoodScript&,CScriptDataBase*,const CScriptThing&); };              // 0x00D70520
    class CHTG_AmbushPoint : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeGoodScript&,CScriptDataBase*,const CScriptThing&); };        // 0x00D6FD70
    class CHTG_AmbushPointRear : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeGoodScript&,CScriptDataBase*,const CScriptThing&); };    // 0x00D6FF30
    class CHTG_AmbushPointCond : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeGoodScript&,CScriptDataBase*,const CScriptThing&); };    // 0x00D6FE50
    class CHTG_AmbushPointBackup : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeGoodScript&,CScriptDataBase*,const CScriptThing&); };  // 0x00D70010
    class CHTG_Executioner : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeGoodScript&,CScriptDataBase*,const CScriptThing&); };        // 0x00D731A0
    class CHTG_Audience : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeGoodScript&,CScriptDataBase*,const CScriptThing&); };           // 0x00D720E0
    class CHTG_Boss : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeGoodScript&,CScriptDataBase*,const CScriptThing&); };               // 0x00D724D0
    class CHTG_FreedomFighter : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeGoodScript&,CScriptDataBase*,const CScriptThing&); };     // 0x00D700E0
#endif
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
