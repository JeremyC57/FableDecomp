#ifndef FABLE_ENGINE_CQ_PRISONESCAPERESCUEMOTHERSCRIPT_H
#define FABLE_ENGINE_CQ_PRISONESCAPERESCUEMOTHERSCRIPT_H
// Ego_r TPI0x10c3e; native constructor00dd26e0, OnPersist00dd2c80 and script vtable012db86c.
// Layout: original CLevelScriptBase is0x48 in retail, followed by every original field; total0x80.
#include "engine/CLevelScriptBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_PrisonEscapeRescueMotherScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    virtual void Main();
    bool MakePrisonerComment(const CScriptThing&,const CCharString&);
    void ResetCells();
    void WatchHeroEscapeStatus();
    void SplittingUpCutscene();
    void PrisonLifeIntro();
    void ProcessRaces();
    long CurrentTask; // +0x48
    long TasksDone[3]; // +0x4c
    long TargetNumber[3]; // +0x58
    bool HeroAgreesToTask; // +0x64
    bool HeroHasFoundWeapons; // +0x65
    bool MotherCellDoorOpen; // +0x66
    bool MotherFollowing; // +0x67
    bool MotherDied; // +0x68
    bool WatchingHero; // +0x69
    bool PrisonCutsceneFinished; // +0x6a
    bool RaceStarting; // +0x6b
    long YearsPassedInCell; // +0x6c
    long PrisonerConversationIndex; // +0x70
    long ConversationIndex; // +0x74
    bool HeroEscapedCell; // +0x78
    bool AlarmSounded; // +0x79
    bool MotherRescuedFromPrisonAutoSaveCheckPoint; // +0x7a
    CTimer RunningTimer; // +0x7c
    class CPrisonKeyDoor;
    class CCellDoor;
    class CCellDoorHero;
    class CTortureDoorCourtyard;
    class CTortureDoorPassage;
    class CInventoryChest;
    class CGuardMotherEscape;
    class CMotherTorture;
    class CPrisonEscapeGuard;
    class CPrisoner;
    class CPrisonerAccomplice;
    class CPrisonGuard;
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_PrisonEscapeRescueMotherScript) == 0x80);
#ifdef FABLE_RETAIL_CQ_PRISONESCAPERESCUEMOTHERSCRIPT_MOTHERTORTURE
// Retail opt-in (chunk 811): the MotherTorture entity script. Ego_r.pdb layout (struct_layouts_egor.tsv, entity offsets equal
// retail's: CEntityScriptBase is 0x14 in both): CMotherTorture 32 bytes (ParentClass +0x14, PMasterData +0x18, BrainState +0x1c);
// retail Main 0x00DD6A60 reads BrainState +0x1c and PMasterData->MotherDodgingKraken +0x84. Entity vtable 0x012DC108 (0 scalar
// deleting dtor 0x00DD6A40, 1 Main 0x00DD6A60, 2 Init 0x00DD6950, 3 GetParentScript 0x00DD6940, 4 OnPersist 0x00DD94F0,
// 5 OnPredicateFail 0x00DD6A00) that Alloc stores; Main / Init / OnPersist / OnPredicateFail are private (Ego_r.pdb and FableWin.pdb
// ?Main@CMotherTorture@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ and the other spellings).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_PrisonEscapeRescueMotherScript::CMotherTorture : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonEscapeRescueMotherScript&, CScriptDataBase*, const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;                        // 0x00DD6940
private:
    virtual void Main();                                                 // 0x00DD6A60
    virtual void Init();                                                 // 0x00DD6950
    virtual void OnPersist(CPersistContext&);                            // 0x00DD94F0
    virtual void OnPredicateFail();                                      // 0x00DD6A00
protected:
    CQ_PrisonEscapeRescueMotherScript& ParentClass;                      // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    long BrainState;                                                     // +0x1c
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture) == 0x20);
#endif
#endif
