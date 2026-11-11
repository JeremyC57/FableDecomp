// CTCCreatureOpinionOfHero::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreatureOpinionOfHero@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006866B0, thiscall, `ret`.
// Identity: slot 8 of CTCCreatureOpinionOfHero's vtable 0x0125BC3C (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureOpinionOfHero or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureOpinionOfHero@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCCreatureOpinionOfHero.h (opt-ins as in the landed row CTCCreatureOpinionOfHero_IsFactionEnemyToHeroOverriddenOnOpinion_00681850.cpp).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#include "engine/CTCCreatureOpinionOfHero.h"
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"
bool CTCCreatureOpinionOfHero::HasFrameUpdate() const
{
    return true;
}
