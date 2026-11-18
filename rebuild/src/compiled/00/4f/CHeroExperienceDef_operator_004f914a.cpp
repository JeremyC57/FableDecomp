// CHeroExperienceDef::operator=(const CHeroExperienceDef& other) -- the member-wise assignment (modelled as the compiler-generated one): the def base (CDefClassBase::operator= out of line), the three long vectors, the eight maps (each tree's operator= out of line) and the two floats in declaration order, then the attack-combo vector; returns *this
// Leaf: ??4CHeroExperienceDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F914A, 189 bytes, thiscall `ret 4`; size-optimised def TU.
// Name: Ego_r.pdb 0x0050C49F + FableWin.pdb 0x01B9BE44 ??4CHeroExperienceDef@@QAEAAV0@ABV0@@Z.
// Callees: CDefClassBase::operator= 0x00431F10, vector<long>::operator= 0x00454886 (x3), _Rb_tree<long, pair<const long, CDefString> ..>
//   ::operator= 0x004F9207 (x5), _Rb_tree<long, pair<const long, CComboMultiplierData> ..>::operator= 0x004F9660, _Rb_tree<EDamageAttribute,
//   pair<const EDamageAttribute, CHeroStatIncreaseData> ..>::operator= 0x004F9831 (q11179 / NF90), vector<CAttackHistoryCombo>::operator=
//   0x004F932C.
// Layout: engine/CHeroExperienceDef.h, FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER (the Ego_r.pdb STLport member types, retail 0xB4).
// Shape: the compiler-generated assignment; each map's implicit operator= is `_M_t = x._M_t` (the tree's out-of-line operator=), the
//   vectors' operator= is STLport's out-of-class _vector.c body (declared only: FABLE_RETAIL_STLP_VECTOR_ASSIGN).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CATTACKHISTORYCOMBO
#define FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_map.h"
#include "engine/CHeroExperienceDef.h"

namespace {
// Fidelity: scaffolding, not retail: the shared header declares no operator=, so the assignment is the compiler-generated one, whose
// member-wise order and out-of-line member calls retail matches (the PDB name sets list ??4 either way). This TU emits its
// out-of-line COMDAT through this use; retail's callers are outside this row.
void AssignHeroExperienceDef(CHeroExperienceDef& def, const CHeroExperienceDef& other)
{ def = other; }
}
