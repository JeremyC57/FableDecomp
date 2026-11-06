#ifndef FABLE_ENGINE_CQ_GUILDTRAININGSCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGSCRIPT_H
// Original Ego_r TPI110ab/110ca. Native CWillDummy factory00d43360/vtable012ce364.
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain(); virtual void Main(); virtual void Init(); virtual void OnPersist(CPersistContext&);
    long GameState;
    bool SleepNeeded;
    bool ReachedPlatform;
    bool DomeCutsceneStart;
    long HeroWarnings;
    long CurrentBirdsKilled;
    long StartedTesting;
    bool StartedMeleeTesting;
    CTimer WillHelpTimer;
    bool FightFinished;
    bool DisplayBirdKilledMessage;
#ifdef FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
    // Retail opt-in (chunk 800): the members Main 0x00D3BC60 calls (Ego_r.pdb ?<Name>@CQ_GuildTrainingScript@NScript@@QAEXXZ, the
    //   CSpawnedFunc thread-name literals "RunTutorials" .. "KeepBookcaseExitRemoved" next to each member pointer) and the nested
    //   entity scripts its bindings allocate (Ego_r.pdb ?Alloc@<Class>@CQ_GuildTrainingScript@NScript@@SI..., binding-name literals);
    //   declarations only, no layout change (vtable 0x012CD458: 0 dtor 0x00D50650, 1 RegisterMain 0x00D3BB50, 2 Main 0x00D3BC60,
    //   3 Init 0x00D3B3D0, 4 OnPersist 0x00D3BC10).
    void RunArrivalCutscene();                                           // 0x00D44CB0
    void RunTutorials();                                                 // 0x00D45DD0
    void CheckFriendlyAttacks();                                         // 0x00D45060
    void KeepTabsOnWhisper();                                            // 0x00D3CBD0
    void WatchForSparrowKilled();                                        // 0x00D3C840
    void KeepBookcaseExitRemoved();                                      // 0x00D3C8F0
    class CAppleGirl;                                                    // Alloc 0x00D3D060
    class CApprenticeSpeedTest;                                          // Alloc 0x00D3E1F0
    class CSpeedFriend;                                                  // Alloc 0x00D407C0
    class CRaceMarker;                                                   // Alloc 0x00D409E0
    class CMeleeApprentice;                                              // Alloc 0x00D40C10
    class CCombatApprentice;                                             // Alloc 0x00D41A30
    class CSkillApprentice;                                              // Alloc 0x00D41B10
    class CWillApprentice;                                               // Alloc 0x00D43280
    class CHeroBed;                                                      // Alloc 0x00D40B30
    class CSkillTarget;                                                  // Alloc 0x00D41BF0
    class CBirdKiller;                                                   // Alloc 0x00D42F40
    class CKillBird;                                                     // Alloc 0x00D43040
    class CPreMeleeMaze;                                                 // Alloc 0x00D43CD0
#endif
    class CWillDummy : public CEntityScriptBase {
    public: virtual CScriptBase* GetParentScript() const;
#ifdef FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D43360 (chunk 800)
#endif
    protected: CQ_GuildTrainingScript& ParentClass; CQ_SunnyvaleMasterData* PMasterData;
    private: virtual void Main(); virtual void Init(); virtual void OnPredicateFail();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingScript)==0x68);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingScript::CWillDummy)==0x1c);
#endif
