#ifndef FABLE_ENGINE_CQ_PRISONRACESCRIPT_H
#define FABLE_ENGINE_CQ_PRISONRACESCRIPT_H
// Retail opt-in (chunk 783): NScript::CQ_PrisonRaceScript, new header (no includer before chunk 783).
// Layout: Ego_r.pdb CQ_PrisonRaceScript (struct_layouts_egor.tsv, 116 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): HeroWonRace +0x48, RaceStarted +0x49, RaceEnded +0x4a, RacerSpeed +0x4c, HeroWaypointIndex +0x50,
// NearestRaceMarkerDistanceToHero +0x54, NearestRaceMarker +0x58, ConversationIndex +0x5c; retail sizeof 0x60 (the landed
// Alloc 0x00DDDC30 `push 60h`, the landed rows' layout lines). Retail methods (landed): ctor 0x00DDAD30, Init 0x00DDAD60,
// OnPersist 0x00DDAD90, RegisterMain 0x00DDADA0, PlaceHeroBackInCells 0x00DDB450, MakeRacerComment 0x00DDCF50.
// Nested entity script: only CPrisonRaceGuard so far (Ego_r.pdb CPrisonRaceGuard, 32 bytes: CEntityScriptBase 0x14, ParentClass
// +0x14, PMasterData +0x18, DistanceToPlayAnim +0x1c; vtable 0x012DC97C: 1 Main 0x00DDB720, 2 Init 0x00DDB570, 3 GetParentScript
// 0x00DDB540; Alloc 0x00DDB4A0 -- the landed Init stores DistanceToPlayAnim +0x1c).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> as the Sunnyvale quest scripts; ParentClass a reference as in the other quest
// headers (engine/CV_DemonDoorsScript.h).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_PrisonRaceScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonRaceScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00DDADA0
    virtual void Main();
    virtual void Init();                                                 // 0x00DDAD60
    virtual void OnPersist(CPersistContext&);                            // 0x00DDAD90
    void PlaceHeroBackInCells();                                         // 0x00DDB450
    void MakeRacerComment();                                             // 0x00DDCF50
    bool HeroWonRace;                                                    // +0x48
    bool RaceStarted;                                                    // +0x49
    bool RaceEnded;                                                      // +0x4a
    long RacerSpeed;                                                     // +0x4c
    long HeroWaypointIndex;                                              // +0x50
    float NearestRaceMarkerDistanceToHero;                               // +0x54
    long NearestRaceMarker;                                              // +0x58
    long ConversationIndex;                                              // +0x5c
#ifdef FABLE_RETAIL_CQ_PRISONRACESCRIPT_MAIN
    // Retail opt-in (chunk 837): the Main 0x00DDAE40 spawns "WatchForHeroWinOrLose" (Ego_r.pdb
    // ?WatchForHeroWinOrLose@CQ_PrisonRaceScript@NScript@@QAEXXZ; the CSpawnedFunc member pointer 0x00DDB2F0) and binds the other
    // five nested entity scripts (Ego_r.pdb ?Alloc@<Class>@CQ_PrisonRaceScript@NScript@@SI...; the landed Alloc rows; the binding-name
    // literal is the class name less its `C`), forward-declared only.
    void WatchForHeroWinOrLose();                                        // 0x00DDB2F0
    class CPrisonRaceWinningPostGuard;                                   // Alloc 0x00DDBF90
    class CPrisonRaceWarden;                                             // Alloc 0x00DDC2D0
    class CPrisonRacer;                                                  // Alloc 0x00DDC3E0
    class CPrisonRaceStartingGuard;                                      // Alloc 0x00DDC500
    class CWaypointSpeedCheck;                                           // Alloc 0x00DDC5E0
#endif
    class CPrisonRaceGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonRaceScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DDB4A0
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00DDB540
    private:
        virtual void Main();                                             // slot 1 0x00DDB720
        virtual void Init();                                             // slot 2 0x00DDB570
    protected:
        NScript::CQ_PrisonRaceScript & ParentClass;                      // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    private:
        long DistanceToPlayAnim;                                         // +0x1c
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_PrisonRaceScript) == 0x60);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_PrisonRaceScript::CPrisonRaceGuard) == 0x20);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_PRISONRACESCRIPT_H
