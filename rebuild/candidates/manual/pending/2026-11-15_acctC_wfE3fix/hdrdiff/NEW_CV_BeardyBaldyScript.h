#ifndef FABLE_ENGINE_CV_BEARDYBALDYSCRIPT_H
#define FABLE_ENGINE_CV_BEARDYBALDYSCRIPT_H
// Retail opt-in (chunk 815): NScript::CV_BeardyBaldyScript, new header (the landed sibling rows keep their local models).
// Layout: Ego_r.pdb CV_BeardyBaldyScript (struct_layouts_egor.tsv, 184 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): the offsets the landed ctor 0x00E4F5B0 writes (RequiredHairdo +0x58 .. RandomSpeech[10] +0x78,
// BeardyBaldyPhase +0xa0, `push 0A4h` in the factory 0x00E54260) and CBB_BeardyBaldyMan::Main 0x00E50FD0 reads (QuestComplete
// +0x49, QuestPhase +0x4c, InitialisePhase +0x50, IntroComplete +0x51, Phase1..3RequirementsComplete +0x52..+0x54, AllHairChanged
// +0x67, BeardyBaldyLeft +0x6a, IncorrectHairComboCount +0x6c, AttackedByHero +0x70, RandomSpeechTimer +0x74).
// BeardyBaldyPhase: the PDB names the member type (unnamed enum) but no enumerators.
// Nested CBB_BeardyBaldyMan (Ego_r.pdb, 36 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18,
// lastRandomSpeechIdx1 +0x1c, lastRandomSpeechIdx2 +0x20 (the landed Init 0x00E50890 writes both). Entity vtable 0x012E5AEC (stored by
// Alloc 0x00E507E0 at 0x00E50816): 0 scalar deleting dtor 0x00E508F0, 1 Main 0x00E50FD0, 2 Init 0x00E50890, 3 GetParentScript
// 0x00E50880, 4 OnPersist 0x00E508D0, 5 OnPredicateFail 0x00E508E0. Spellings: Ego_r.pdb / FableWin.pdb
// ?<Name>@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@ (Main / Init / OnPersist / OnPredicateFail EAE, GetRandomSpeech AAE,
// GetParentScript UBE) and ?<Name>@CV_BeardyBaldyScript@NScript@@ for the parent helpers (QAE).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
class C3DVector;
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BeardyBaldyScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void ResetRandomSpeechTime();                                        // 0x00E53C70
    void SetWanderPointAndDistance(const CScriptThing&, const C3DVector&); // 0x00E53F60
    void WatchForAttack(CScriptThing);                                   // 0x00E53D90
    bool IsHeroWearingAnyBeard();                                        // 0x00E53670
    bool IsHeroWearingAnyTash();                                         // 0x00E538F0
    bool IsHeroWearingAnyOddHairdo();                                    // 0x00E53AD0
    bool QuestFinished;                                                  // +0x48
    bool QuestComplete;                                                  // +0x49
    bool QuestActivated;                                                 // +0x4a
    long QuestPhase;                                                     // +0x4c
    bool InitialisePhase;                                                // +0x50
    bool IntroComplete;                                                  // +0x51
    bool Phase1RequirementsComplete;                                     // +0x52
    bool Phase2RequirementsComplete;                                     // +0x53
    bool Phase3RequirementsComplete;                                     // +0x54
    CCharString RequiredHairdo;                                          // +0x58
    CCharString RequiredBeard;                                           // +0x5c
    CCharString RequiredTash;                                            // +0x60
    bool AcquiredNewHairdo;                                              // +0x64
    bool AcquiredNewBeard;                                               // +0x65
    bool AcquiredNewTash;                                                // +0x66
    bool AllHairChanged;                                                 // +0x67
    bool BeardyBaldyKilled;                                              // +0x68
    bool BarberLadyKilled;                                               // +0x69
    bool BeardyBaldyLeft;                                                // +0x6a
    long IncorrectHairComboCount;                                        // +0x6c
    bool AttackedByHero;                                                 // +0x70
    CTimer RandomSpeechTimer;                                            // +0x74
    CCharString RandomSpeech[10];                                        // +0x78
    enum { } BeardyBaldyPhase;                                           // +0xa0
    class CBB_BeardyBaldyMan : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BeardyBaldyScript&, CScriptDataBase*, const CScriptThing&); // 0x00E507E0
        virtual CScriptBase* GetParentScript() const;                    // 0x00E50880
    private:
        virtual void Main();                                             // 0x00E50FD0
        virtual void Init();                                             // 0x00E50890
        virtual void OnPersist(CPersistContext&);                        // 0x00E508D0
        virtual void OnPredicateFail();                                  // 0x00E508E0
        CCharString GetRandomSpeech();                                   // 0x00E53CB0
    protected:
        CV_BeardyBaldyScript& ParentClass;                               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        long lastRandomSpeechIdx1;                                       // +0x1c
        long lastRandomSpeechIdx2;                                       // +0x20
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BeardyBaldyScript) == 0xA4);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan) == 0x24);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_BEARDYBALDYSCRIPT_H
