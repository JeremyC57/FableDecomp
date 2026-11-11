// NUISystem::CListArrow::SetMinToDraw(long) -- stores MinToDraw
// Leaf: ?SetMinToDraw@CListArrow@NUISystem@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054C320, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 149 of NUISystem::CListArrow's vtable 0x01248F94 (the slot holds this body in 1 vtable(s), owner NUISystem::CListArrow); the Xbox 360 Test PDB virtual order (third source) aligned on 71 TLC-named slots of that vtable puts SetMinToDraw(long) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetMinToDraw@CListArrow@NUISystem@@UAEXJ@Z; `ret` matches the signature.
// Was: NUISystem::CListArrow::vfunc_149 (rebuild/src/compiled/00/54/gapscan_sub0054c320_0054c320.cpp).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CLISTARROW_VSLOTS
#define FABLE_RETAIL_CLISTARROW_MEMBERS
#include "engine/NUISystem_CListArrow.h"
namespace NUISystem {
void CListArrow::SetMinToDraw(long count)
{
    MinToDraw = count;
}
}
