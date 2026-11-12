// NUISystem::CUIMiscThingsDef::Copy(CDefClassBase const*) -- copies another misc-things def into this one through the implicit assignment (tail jump to operator= 0x004321F0)
// Leaf: ?Copy@CUIMiscThingsDef@NUISystem@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004321E0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 19 of NUISystem::CUIMiscThingsDef's vtable 0x01230EEC (the slot holds this body in 1 vtable(s), owner NUISystem::CUIMiscThingsDef); the Xbox 360 Test PDB virtual order (third source) aligned on 22 TLC-named slots of that vtable puts Copy(CDefClassBase const *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?Copy@CUIMiscThingsDef@NUISystem@@UAEXPBVCDefClassBase@@@Z; no `ret` to check (NORET).
// Was: NUISystem::CUIMiscThingsDef::vfunc_19 (rebuild/src/compiled/00/43/gapscan_sub004321e0_004321e0.cpp).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#include "engine/NUISystem_CUIMiscThingsDef.h"
namespace NUISystem {
void CUIMiscThingsDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CUIMiscThingsDef*>(def);
}
}
