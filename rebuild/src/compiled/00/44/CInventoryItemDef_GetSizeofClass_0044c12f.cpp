// Leaf: ?CInventoryItemDef_GetSizeofClass@@YIKPBVCInventoryItemDef@@@Z
#pragma optimize("s",on)

#include "engine/CInventoryItemDef.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned long __fastcall CInventoryItemDef_GetSizeofClass(const CInventoryItemDef* self)
{
    (void)self;
    return 0x70;
}
// Stand-in (owner rule 2026-11-05): CInventoryItemDef_GetSizeofClass for CInventoryItemDef::GetSizeofClass; RTTI vtable slot ?GetSizeofClass@CInventoryItemDef@@UAEKXZ + PDB name set (fablewin).
