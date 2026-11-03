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
#endif
