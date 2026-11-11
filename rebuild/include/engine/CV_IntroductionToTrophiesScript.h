#ifndef FABLE_ENGINE_CV_INTRODUCTIONTOTROPHIESSCRIPT_H
#define FABLE_ENGINE_CV_INTRODUCTIONTOTROPHIESSCRIPT_H
// Retail opt-in (chunk 809): NScript::CV_IntroductionToTrophiesScript, new header (the landed sibling rows keep their local models).
// Layout: Ego_r.pdb CV_IntroductionToTrophiesScript (struct_layouts_egor.tsv, 128 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): DoneBriarIntroSpeech +0x48 .. OutsideTavernTimer +0x68, sizeof 0x6C.
// Checked against retail: CBriarRose::Main 0x00E9E420 reads DoneBriarIntroSpeech +0x48, DoneBriarTrophySpeech +0x49, AttackedByHero
// +0x4a, DoneApproachMessage1 / 2 +0x4b / +0x4c, AttackSpeechComplete +0x4d, HitByHeroCount +0x50, HitWarningComplete +0x54,
// InsideTavern +0x55, OpinionCurrent / OpinionRequired +0x58 / +0x5c (compared with 0, 1, 2), CutsceneStarted +0x60.
// Nested CBriarRose (Ego_r.pdb, 32 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18, AnimTimer +0x1c (the landed
// Alloc 0x00E9D9D0 news 0x20; entity vtable 0x012ED67C: 0 scalar deleting dtor 0x00E9DA90, 1 Main 0x00E9E420, 2 Init 0x00E9DAC0,
// 3 GetParentScript 0x00E9DA70, 5 0x00E9DA80). Main / Init private (Ego_r.pdb / FableWin.pdb
// ?Main@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ).
// Retail methods: the Ego_r.pdb name set ?<Name>@CV_IntroductionToTrophiesScript@NScript@@...; addresses from the landed rows.
// Both original PDB enum records prove all three original BriarRoseOpinion enumerators; row PDB_OPINION_ENUM.json.
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_IntroductionToTrophiesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
#ifdef FABLE_RETAIL_CV_INTRODUCTIONTOTROPHIESSCRIPT_ORIGINAL_OPINION_ENUM
    enum BriarRoseOpinion { BRIAR_ROSE_OPINION_LOUD=0, BRIAR_ROSE_OPINION_MEDIUM=1, BRIAR_ROSE_OPINION_QUIET=2 };
#else
    enum BriarRoseOpinion { };
#endif
    virtual void RegisterMain();                                         // 0x00E9C410
    virtual void Main();                                                 // 0x00E9C4B0
    virtual void Init();                                                 // 0x00E9C3B0
    virtual void OnPersist(CPersistContext&);                            // 0x00E9E110
    void WatchForAttack(CScriptThing);                                   // 0x00E9DC80
    void WatchForCrowdVolume();                                          // 0x00E9C8D0
    void WatchForInsideTavern();                                         // 0x00E9CB20
    void WatchForQuestComplete();
    bool DoneBriarIntroSpeech;                                           // +0x48
    bool DoneBriarTrophySpeech;                                          // +0x49
    bool AttackedByHero;                                                 // +0x4a
    bool DoneApproachMessage1;                                           // +0x4b
    bool DoneApproachMessage2;                                           // +0x4c
    bool AttackSpeechComplete;                                           // +0x4d
    long HitByHeroCount;                                                 // +0x50
    bool HitWarningComplete;                                             // +0x54
    bool InsideTavern;                                                   // +0x55
    bool QuestComplete;                                                  // +0x56
    BriarRoseOpinion OpinionCurrent;                                     // +0x58
    BriarRoseOpinion OpinionRequired;                                    // +0x5c
    bool CutsceneStarted;                                                // +0x60
    bool OpinionTimerReset;                                              // +0x61
    bool OutsideTavernTimerReset;                                        // +0x62
    CTimer OpinionTimer;                                                 // +0x64
    CTimer OutsideTavernTimer;                                           // +0x68
    class CBriarRose : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_IntroductionToTrophiesScript&, CScriptDataBase*, const CScriptThing&); // 0x00E9D9D0
        virtual CScriptBase* GetParentScript() const;                    // 0x00E9DA70
    protected:
        CV_IntroductionToTrophiesScript& ParentClass;                    // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        CTimer AnimTimer;                                                // +0x1c
        virtual void Init();                                             // 0x00E9DAC0
        virtual void Main();                                             // 0x00E9E420
    };
    class CTrophiesBriarTrophy;                                          // Alloc 0x00E9D700
    class CTrophiesIntroTable;                                           // Alloc 0x00E9D8E0
    class CTrophiesIntroWatcher;                                         // Alloc 0x00E9CEE0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_IntroductionToTrophiesScript) == 0x6C);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_IntroductionToTrophiesScript::CBriarRose) == 0x20);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_INTRODUCTIONTOTROPHIESSCRIPT_H
