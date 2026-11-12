#ifndef FABLE_ENGINE_CTCMELEETRAP_H
#define FABLE_ENGINE_CTCMELEETRAP_H
// NEW (agentNF12, 2026-11-11, rows 0x00788406 TriggerTrap / 0x007884F5 ResetTrap / 0x0078809B InitialiseAnimation): the melee trap
// component. RTTI .?AVCTCMeleeTrap@@ : CTCTrapBase (ghidra_out/rtti_bases.tsv), vtable 0x0126B884 (slots 28..31 InitTrap
// 0x007883C3, UpdateTrap 0x00788B0A, TriggerTrap 0x00788406, ResetTrap 0x007884F5). Members: Ego_r.pdb CTCMeleeTrap
// (struct_layouts_egor.tsv: PlayingContinuous / StrikeAnimation right after the base, Ego_r +108 / +109 = retail +0x60 / +0x61;
// ResetTrap stores `mov byte ptr [esi+60h],0`, 0x0078850A). Methods: Ego_r.pdb / FableWin.pdb spellings (the trap overrides
// protected virtual MAEXXZ; InitialiseAnimation private AAEXABVCCharString@@_N@Z; InitialiseStrikeAnimation private
// AAEXABVCAnimationEntry@@@Z). Needs FABLE_RETAIL_CTCTRAPBASE (and
// FABLE_RETAIL_DEF_CLASS_BASE for engine/CDefPointer.h).
#ifndef FABLE_RETAIL_CTCTRAPBASE
#error "engine/CTCMeleeTrap.h needs FABLE_RETAIL_CTCTRAPBASE"
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCTrapBase.h"
class CCharString;
class CAnimationEntry;
class CTCMeleeTrap : public CTCTrapBase {
protected:
    virtual void TriggerTrap();                                          // 30 0x00788406
    virtual void ResetTrap();                                            // 31 0x007884F5
private:
    void InitialiseStrikeAnimation(const CAnimationEntry& entry);       // 0x00787F29 (Ego_r donor 0x0078F4C9, before InitialiseAnimation)
    void InitialiseAnimation(const CCharString& name, bool loop);       // 0x0078809B
public:
    bool PlayingContinuous;                                              // +0x60
    bool StrikeAnimation;                                                // +0x61
#ifdef FABLE_RETAIL_CTCMELEETRAP_VSLOTS3
// agentFAM05 (2026-11-11): retail virtual(s) this declaration lacks, declaration only (no layout change; the vtable exists). Slots from the TLC
// vtable (agentFAM03 proof, re-verified by agentFAM05); spellings from the Ego_r / FableWin name sets.
protected:
    virtual void UpdateTrap();                                           // 29 0x00788B0A ?UpdateTrap@CTCMeleeTrap@@MAEXXZ
private:
    void UpdateAnimation();                                              // 0x00788555 ?UpdateAnimation@CTCMeleeTrap@@AAEXXZ (Ego_r donor 0x0078FA39)
    void UpdateStrikeAction();                                           // 0x00788AEF ?UpdateStrikeAction@CTCMeleeTrap@@AAEXXZ (Ego_r donor 0x0078FF16, 0x1B bytes before UpdateTrap in both)
public:
#endif // FABLE_RETAIL_CTCMELEETRAP_VSLOTS3
};
FABLE_STATIC_ASSERT(offsetof(CTCMeleeTrap, PlayingContinuous) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CTCMeleeTrap, StrikeAnimation) == 0x61);
#endif // FABLE_ENGINE_CTCMELEETRAP_H
