#ifndef FABLE_ENGINE_CV_SWORDINTHESTONESCRIPT_H
#define FABLE_ENGINE_CV_SWORDINTHESTONESCRIPT_H
// Retail opt-in (chunk 798): NScript::CV_SwordInTheStoneScript, new header (no includer before chunk 798; the 20 landed rows keep
// their local models).
// Layout: Ego_r.pdb CV_SwordInTheStoneScript (struct_layouts_egor.tsv, 148 bytes) at retail offsets PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container member): SwordRemovalAttemptCount +0x48, SwordCutscene +0x4c ... AttackedByHero
// +0x61, CrowdMemberPullingCurrent +0x64, CrowdMemberPullingNull +0x70, CrowdSwordTimer +0x7c; 0x80 bytes. Checked against retail:
// the landed rows name the same offsets (WatchForSwordPickup 0x00ED57E0 +0x4c, ResetCrowdSwordTimer 0x00ED72F0 +0x7c, Main
// 0x00ED4D20); SwordPullCutscene 0x00ED5830 reads StatsInitialised +0x60, writes HeroStrengthPhysique / Toughness / Health
// +0x54 / +0x58 / +0x5c, HeroHasTakenSword +0x52, SwordCutscene +0x4c and SwordRemovalAttemptCount +0x48.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the Sunnyvale village scripts' base, as engine/CV_BanditCampPathScript.h).
// Methods (Ego_r.pdb): RegisterMain 0x00ED4B90, Main 0x00ED4D20, Init 0x00ED4B10, OnPersist; threads WatchForSwordPickup
// 0x00ED57E0, WatchForDeathDuringSwordPull 0x00ED7320; SwordPullCutscene 0x00ED5830, ResetCrowdSwordTimer 0x00ED72F0. Nested
// entity scripts CSwordOfStrength (Alloc 0x00ED5130), CSwordCrowd (Alloc 0x00ED5260).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CV_SwordInTheStoneScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SwordInTheStoneScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00ED4B90
    virtual void Main();                                                 // 0x00ED4D20
    virtual void Init();                                                 // 0x00ED4B10
    virtual void OnPersist(CPersistContext&);                            // 0x00ED4C50
    void WatchForSwordPickup();                                          // 0x00ED57E0
    void SwordPullCutscene();                                            // 0x00ED5830
    void ResetCrowdSwordTimer();                                         // 0x00ED72F0
    void WatchForDeathDuringSwordPull();                                 // 0x00ED7320
    long SwordRemovalAttemptCount;                                       // +0x48
    bool SwordCutscene;                                                  // +0x4c
    bool CrowdAttemptingToPullSword;                                     // +0x4d
    bool CrowdSwordPullActive;                                           // +0x4e
    bool HeroHasBeenToldSwordLegend;                                     // +0x4f
    bool HeroCongratulated;                                              // +0x50
    bool CrowdDisperse;                                                  // +0x51
    bool HeroHasTakenSword;                                              // +0x52
    bool SwordInTheStoneIntroComplete;                                   // +0x53
    long HeroStrengthPhysique;                                           // +0x54
    long HeroStrengthToughness;                                          // +0x58
    long HeroStrengthHealth;                                             // +0x5c
    bool StatsInitialised;                                               // +0x60
    bool AttackedByHero;                                                 // +0x61
    CScriptThing CrowdMemberPullingCurrent;                              // +0x64
    CScriptThing CrowdMemberPullingNull;                                 // +0x70
    CTimer CrowdSwordTimer;                                              // +0x7c
    class CSwordOfStrength;                                              // Alloc 0x00ED5130
    class CSwordCrowd;                                                   // Alloc 0x00ED5260
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_SwordInTheStoneScript) == 0x80);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_SWORDINTHESTONESCRIPT_H
