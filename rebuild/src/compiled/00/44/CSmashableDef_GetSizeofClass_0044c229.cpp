// Leaf: ?CSmashableDef_GetSizeofClass@@YIKPBVCSmashableDef@@@Z
#pragma optimize("s",on)

#include "engine/CSmashableDef.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned long __fastcall CSmashableDef_GetSizeofClass(const CSmashableDef* self)
{
    (void)self;
    return 0x30;
}
// Stand-in (owner rule 2026-11-05): CSmashableDef_GetSizeofClass for CSmashableDef::GetSizeofClass; RTTI vtable slot ?GetSizeofClass@CSmashableDef@@UAEKXZ + PDB name set (egor/fablewin).
