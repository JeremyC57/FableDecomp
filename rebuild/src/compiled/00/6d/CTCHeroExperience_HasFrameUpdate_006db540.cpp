// CTCHeroExperience::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHeroExperience@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DB540, thiscall, `ret`.
// Identity: slot 8 of CTCHeroExperience's vtable 0x0125F6C4 (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroExperience or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHeroExperience@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHeroExperience.h (opt-ins as in the landed row CTCHeroExperience_IsBlockRecoilAnimToConserveAnimationHandedness_006d68d0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_BLOCK_THRESHOLDS
#define FABLE_RETAIL_CHEROSTATSDEF_STATLEVELS
#define FABLE_RETAIL_CHEROCOMBATDEF_BLOCK_THRESHOLDS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
bool CTCHeroExperience::HasFrameUpdate() const
{
    return true;
}
