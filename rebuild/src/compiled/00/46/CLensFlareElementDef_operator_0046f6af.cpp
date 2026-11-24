// CLensFlareElementDef::operator=(const CLensFlareElementDef& other) -- memberwise copy: the CParentDefClassBase part out of line, then Radius, Colour, Position, Texture; returns *this
// Leaf: ??4CLensFlareElementDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046F6AF, 45 bytes, thiscall, `ret 4` (size-optimised def module).
// Identity: CLensFlareElementDef::Copy 0x0046F77C (slot 19 of the class's RTTI vftable; RTTI placement + both PDB name sets) is a `jmp` to it,
//   and the landed row 0x00476B17 already binds ??4CLensFlareElementDef@@QAEAAV0@ABV0@@Z here (reloc_bindings); the symbol is in Ego_r.pdb
//   and FableWin.pdb. The four copied dwords +0x3C..+0x48 are the class's members in engine/CLensFlareElementDef.h (Ego_r.pdb order).
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0.
// NF337 (2026-11-24): respelled from the made-up CBase_00431ed0_Derived_0046f6af / CBase_00431ed0 onto the shared class model
//   (engine/CLensFlareElementDef.h; FABLE_RETAIL_CLENSFLAREELEMENTDEF_COPY_DECLS declares the out-of-line operator=).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CLENSFLAREELEMENTDEF_COPY_DECLS
#include "engine/CLensFlareElementDef.h"

CLensFlareElementDef& CLensFlareElementDef::operator=(const CLensFlareElementDef& other)
{
    CParentDefClassBase::operator=(other);
    Radius = other.Radius;
    Colour = other.Colour;
    Position = other.Position;
    Texture = other.Texture;
    return *this;
}
