// CEnvironmentSpellEffectInterpolator::ScreenFilterSetFlags(const std::vector<CThingGraphicArgPasser>&, void*) -- set screen-filter bypass flags for things, via the priority stack
// Leaf: ?ScreenFilterSetFlags@CEnvironmentSpellEffectInterpolator@@QAEXABV?$vector@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@@std@@@std@@PAX@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B83E0, 11 bytes: `add ecx,90h ; jmp 0x00881A40` (ScreenEffectPriorityStack, tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?ScreenFilterSetFlags@CEnvironmentSpellEffectInterpolator@@..PAX@Z (Ego_r 0x006F4380)
//   with Ego_r ScreenEffectPriorityStack at +0x90 (the offset forwarded); the jump target 0x00881A40 is
//   NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags (manifest label, same argument list in both PDBs); the callers are
//   the landed CGameScriptInterface::SetThingAndCarriedItemsNotAffectedByScreenFilter 0x008A99F0 (World->PEnvironment +0x1964 spell
//   effect object, one-pointer vector plus the bypass handle) and CTCCarrying::SetAllCarriedThingsAsBypassFilter's caller 0x006A23E0.
//   The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?ScreenFilterSetFlags@CScreenEffectPriorityStack@NScreenEffect@@QAEXABV?$vector@..@std@@PAX@Z 0x00881A40.
// Header: engine/CEnvironmentSpellEffectInterpolator.h (new), engine/NScreenEffect_CScreenEffectPriorityStack.h (new).
#include "engine/CEnvironmentSpellEffectInterpolator.h"
void CEnvironmentSpellEffectInterpolator::ScreenFilterSetFlags(const NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers& things, void* handle)
{
    ScreenEffectPriorityStack.ScreenFilterSetFlags(things, handle);
}