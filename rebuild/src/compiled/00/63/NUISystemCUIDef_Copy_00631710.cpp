// NUISystem::CUIDef::Copy(CDefClassBase const*) -- copies another UI def into this one through the implicit assignment (tail jump to operator= 0x00631720)
// Leaf: ?Copy@CUIDef@NUISystem@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00631710, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 19 of NUISystem::CUIDef's vtable 0x01259F8C (the slot holds this body in 1 vtable(s), owner NUISystem::CUIDef); the Xbox 360 Test PDB virtual order (third source) aligned on 22 TLC-named slots of that vtable puts Copy(CDefClassBase const *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?Copy@CUIDef@NUISystem@@UAEXPBVCDefClassBase@@@Z; no `ret` to check (NORET).
// Was: NUISystem::CUIDef::vfunc_19 (rebuild/src/compiled/00/63/gapscan_sub00631710_00631710.cpp).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/NUISystem_CUIStateDef.h"
#include "engine/NUISystem_CUIDef.h"
namespace NUISystem {
void CUIDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CUIDef*>(def);
}
}
