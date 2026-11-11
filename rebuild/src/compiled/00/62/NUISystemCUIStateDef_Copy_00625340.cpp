// NUISystem::CUIStateDef::Copy(CDefClassBase const*) -- copies another UI state def into this one through the implicit assignment (tail jump to operator= 0x00625350)
// Leaf: ?Copy@CUIStateDef@NUISystem@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00625340, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 19 of NUISystem::CUIStateDef's vtable 0x0125871C (the slot holds this body in 1 vtable(s), owner NUISystem::CUIStateDef); the Xbox 360 Test PDB virtual order (third source) aligned on 22 TLC-named slots of that vtable puts Copy(CDefClassBase const *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?Copy@CUIStateDef@NUISystem@@UAEXPBVCDefClassBase@@@Z; no `ret` to check (NORET).
// Was: NUISystem::CUIStateDef::vfunc_19 (rebuild/src/compiled/00/62/gapscan_sub00625340_00625340.cpp).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_RETAIL_CUISTATEDEF_VSLOTS
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/NUISystem_CUIStateDef.h"
namespace NUISystem {
void CUIStateDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CUIStateDef*>(def);
}
}
