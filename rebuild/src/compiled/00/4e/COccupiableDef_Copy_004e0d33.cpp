// Leaf: ?COccupiableDef_Copy@@YIXPAVCOccupiableDef@@HPBV1@@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/COccupiableDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CSubField { int v; };
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

void __fastcall COccupiableDef_Copy(COccupiableDef* self, int _edx, const COccupiableDef* other)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)other);
    self->TypeFlags = other->TypeFlags;
}
// Stand-in (owner rule 2026-11-05): COccupiableDef_Copy for COccupiableDef::Copy; RTTI vtable slot ?Copy@COccupiableDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (fablewin).
