// Leaf: ?CResurrectionItemDef_Copy@@YIXPAVCResurrectionItemDef@@HPBV1@@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResurrectionItemDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CSubField { int v; };
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

void __fastcall CResurrectionItemDef_Copy(CResurrectionItemDef* self, int _edx, const CResurrectionItemDef* other)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)other);
    self->OnUseParticleEffect = other->OnUseParticleEffect;
}
// Stand-in (owner rule 2026-11-05): CResurrectionItemDef_Copy for CResurrectionItemDef::Copy; RTTI vtable slot ?Copy@CResurrectionItemDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (fablewin).
