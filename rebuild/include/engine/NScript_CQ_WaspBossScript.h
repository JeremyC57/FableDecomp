#ifndef FABLE_ENGINE_NSCRIPT_CQ_WASPBOSSSCRIPT_H
#define FABLE_ENGINE_NSCRIPT_CQ_WASPBOSSSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
// Layout: original PDB complete0x70 minus native CScriptBase shrink0x14 gives0x5c.
// Native e133ee/e135b9/e137d4 prove QueenHornetAttacks+4b, MissionSucceeded+48, PanickedVillagersScene+4e.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WaspBossScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_WaspBossScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual ~CQ_WaspBossScript();
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void EndMission();
    void Outro();
    bool MissionSucceeded;
    bool MissionFailed;
    bool DronesCreated;
    bool QueenHornetAttacks;
    bool CutsceneFinished;
    bool SaidLineOnce;
    bool PanickedVillagersScene;
    bool StartChase;
    long SavedVillagerCount;
    bool QuestStartScreened;
    CTimer ReachedWaspHelper;
#ifdef FABLE_RETAIL_CQ_WASPBOSSSCRIPT_ENTITIES
    // Chunk 772: nested entity scripts (Ego_r.pdb CQ_WaspBossScript::CWaspVictim, 0x1c: ParentClass +0x14, PMasterData +0x18;
    // vtable 0x012E0490, Alloc 0x00E11580). A row defines the class body after the include.
    class CWaspVictim;
#endif
#ifdef FABLE_RETAIL_CQ_WASPBOSSSCRIPT_WASPHELPER
    // Chunk 831: nested entity script CQ_WaspBossScript::CWaspHelper (Ego_r.pdb, 0x20: ParentClass +0x14, PMasterData +0x18,
    // PleadedToHero +0x1c, LeadToRegion +0x1d, WavedOver +0x1e; vtable 0x012E0284 stored by the landed Alloc 0x00E0F7F6, slot 1
    // Main 0x00E0F8A0). A row defines the class body after the include.
    class CWaspHelper;
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_WaspBossScript)==0x5c);
#endif
