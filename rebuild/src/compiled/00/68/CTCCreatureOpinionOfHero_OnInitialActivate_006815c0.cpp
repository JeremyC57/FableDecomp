// CTCCreatureOpinionOfHero::OnInitialActivate() -- the base initial activation (tail call to CTCBase::OnInitialActivate 0x00686870)
// Leaf: ?OnInitialActivate@CTCCreatureOpinionOfHero@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006815C0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 6 of CTCCreatureOpinionOfHero's vtable 0x0125BC3C (the slot holds this body in 1 vtable(s), owner CTCCreatureOpinionOfHero); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnInitialActivate@CTCCreatureOpinionOfHero@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCCreatureOpinionOfHero::vfunc_6 (rebuild/src/compiled/00/68/gapscan_sub006815c0_006815c0.cpp).
// Opt-ins as in the landed row CTCCreatureOpinionOfHero_HasFrameUpdate_006866b0.cpp.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#include "engine/CTCCreatureOpinionOfHero.h"
void CTCCreatureOpinionOfHero::OnInitialActivate()
{
    CTCBase::OnInitialActivate();
}
