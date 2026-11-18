// CHeroSuitDef::Copy(const CDefClassBase* other) -- slot 19 of the hero-suit def: `*this = *static_cast<const CHeroSuitDef*>(other)` through the
//   implicit assignment, expanded in place: CDefClassBase::operator= 0x00431F10 (CSubDefClassBase's implicit assignment), then the SuitParts
//   map<EClothingSuitPart,long> +0x28 through its out-of-line STLport _Rb_tree::operator= 0x004E54CC (map's own operator= is implicit and inline)
// Leaf: ?Copy@CHeroSuitDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E54AD, 31 bytes, thiscall `ret 4`, no return value.
// Identity: FableWin.pdb ?Copy@CHeroSuitDef@@UAEXPBVCDefClassBase@@@Z (01BA9EC9); engine/CHeroSuitDef.h (SuitParts +0x28 on the 0x25-byte
//   CSubDefClassBase). Was a made-up free function `CHeroSuitDef_Copy` over local stand-ins (SubDef::Copy for the tree assignment).
// NF70 (round 7, land1117fu q10781): moved onto the shared header (overlay opt-in FABLE_RETAIL_CHEROSUITDEF_COPY declares the override).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CHEROSUITDEF_COPY
#include "engine/CHeroSuitDef.h"

void CHeroSuitDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CHeroSuitDef*>(other);
}
