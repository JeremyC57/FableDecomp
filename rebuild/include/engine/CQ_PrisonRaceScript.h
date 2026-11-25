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
#ifdef FABLE_RETAIL_CQ_PRISONRACESCRIPT_GUARD_PASS27
    // Both PDBs identify the quest constructor used by the binding factory.
    CQ_PrisonRaceScript(CScriptDataBase*, CGameScriptInterfaceBase&);
#endif
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
    class CPrisonRaceGuard : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_CQ_PRISONRACESCRIPT_GUARD_PASS27
        // FableWin 03873670 and the inlined retail construction at 00DDB4A0.
        CPrisonRaceGuard(CQ_PrisonRaceScript&, CScriptDataBase*, const CScriptThing&);
#endif
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
#ifdef FABLE_RETAIL_CQ_PRISONRACESCRIPT_WINNINGPOSTGUARD
    // Retail opt-in (chunk Q11, row 0x00DDC070): nested entity script CPrisonRaceWinningPostGuard (Ego_r.pdb, 28 bytes:
    // CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18; vtable 0x012DC9E8: 1 Main 0x00DDC070, 2 Init 0x00DDC040,
    // 3 GetParentScript 0x00DDC030; Alloc 0x00DDBF90). Names: Ego_r.pdb / FableWin.pdb ?Main / ?Init / ?GetParentScript /
    // ?Alloc@CPrisonRaceWinningPostGuard@CQ_PrisonRaceScript@NScript@@.
    class CPrisonRaceWinningPostGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonRaceScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DDBF90
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00DDC030
    private:
        virtual void Main();                                             // slot 1 0x00DDC070
        virtual void Init();                                             // slot 2 0x00DDC040
    protected:
        NScript::CQ_PrisonRaceScript & ParentClass;                      // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    };
#endif
#ifdef FABLE_RETAIL_CQ_PRISONRACESCRIPT_MAIN
    // Retail opt-in (Account B p25A, row 0x00DDAE40 CQ_PrisonRaceScript::Main): the spawned thread and the four other nested
    // entity scripts Main binds (Ego_r.pdb / FableWin.pdb ?WatchForHeroWinOrLose@CQ_PrisonRaceScript@NScript@@QAEXXZ and the
    // ?Alloc / ?Main / ?Init / ?GetParentScript@C..@CQ_PrisonRaceScript@NScript@@ name sets; Ego_r sizes 0x20 / 0x1c / 0x1c / 0x1c =
    // the retail `push 20h` / `push 1Ch` of the landed Allocs). Vtables: CPrisonRacer 0x012DCA2C, CPrisonRaceWarden 0x012DCA10,
    // CPrisonRaceStartingGuard 0x012DCA48, CWaypointSpeedCheck 0x012DCA64 (slot 1 Main, 2 Init, 3 GetParentScript).
    void WatchForHeroWinOrLose();                                        // 0x00DDB2F0
    class CPrisonRaceWarden : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonRaceScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DDC2D0
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00DDC370
    private:
        virtual void Main();                                             // slot 1 0x00DDD030
        virtual void Init();                                             // slot 2 0x00DDC380
    protected:
        NScript::CQ_PrisonRaceScript & ParentClass;                      // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    };
    class CPrisonRacer : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonRaceScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DDC3E0
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00DDC480
    private:
        virtual void Main();                                             // slot 1 0x00DDC980
        virtual void Init();                                             // slot 2 0x00DDC490
    protected:
        NScript::CQ_PrisonRaceScript & ParentClass;                      // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    private:
        long RaceWaypointIndex;                                          // +0x1c
    };
    class CPrisonRaceStartingGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonRaceScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DDC500
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00DDC5A0
    private:
        virtual void Main();                                             // slot 1 0x00DDD430
        virtual void Init();                                             // slot 2 0x00DDC5B0
    protected:
        NScript::CQ_PrisonRaceScript & ParentClass;                      // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    };
    class CWaypointSpeedCheck : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonRaceScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DDC5E0
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00DDC680
    private:
        virtual void Main();                                             // slot 1 0x00DDC6C0
        virtual void Init();                                             // slot 2 0x00DDC690
    protected:
        NScript::CQ_PrisonRaceScript & ParentClass;                      // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    };
#endif
#ifdef FABLE_RETAIL_CQ_PRISONRACESCRIPT_ALLOC
    // Retail opt-in (NF436, row 0x00DDDC30 Alloc): FableWin.pdb ??_7CQ_PrisonRaceScript@NScript@@6B@ / LF_CLASS (all public: the user ctor, static
    //   Alloc, the virtual overrides) for what this header does not declare yet: CQ_PrisonRaceScript, Alloc. Declarations only, no layout
    //   change; existing includers do not define the macro.
public:
#ifndef FABLE_RETAIL_CQ_PRISONRACESCRIPT_GUARD_PASS27
    CQ_PrisonRaceScript(CScriptDataBase* data, CGameScriptInterfaceBase& game); // FableWin.pdb ??0CQ_PrisonRaceScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
#endif
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00DDDC30
#endif
};
}
FABLE_STATIC_ASSERT_AT(75, sizeof(NScript::CQ_PrisonRaceScript) == 0x60);
FABLE_STATIC_ASSERT_AT(76, sizeof(NScript::CQ_PrisonRaceScript::CPrisonRaceGuard) == 0x20);
#ifdef FABLE_RETAIL_CQ_PRISONRACESCRIPT_WINNINGPOSTGUARD
FABLE_STATIC_ASSERT_AT(78, sizeof(NScript::CQ_PrisonRaceScript::CPrisonRaceWinningPostGuard) == 0x1c);
#endif
#ifdef FABLE_RETAIL_CQ_PRISONRACESCRIPT_MAIN
FABLE_STATIC_ASSERT_AT(139, sizeof(NScript::CQ_PrisonRaceScript::CPrisonRaceWarden) == 0x1c);
FABLE_STATIC_ASSERT_AT(140, sizeof(NScript::CQ_PrisonRaceScript::CPrisonRacer) == 0x20);
FABLE_STATIC_ASSERT_AT(141, sizeof(NScript::CQ_PrisonRaceScript::CPrisonRaceStartingGuard) == 0x1c);
FABLE_STATIC_ASSERT_AT(142, sizeof(NScript::CQ_PrisonRaceScript::CWaypointSpeedCheck) == 0x1c);
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_PRISONRACESCRIPT_H
