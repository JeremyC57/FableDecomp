// Member setter at this+0x25: one stack arg, `ret 4`. __fastcall this=ecx.
// Leaf: ?SetInstantiationName@CParentDefClassBase_Methods@@QAEXH@Z
#include "engine/CParentDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CParentDefClassBase_Methods : CParentDefClassBase {
    void SetInstantiationName(int value);
};
#pragma pack(pop)
void CParentDefClassBase_Methods::SetInstantiationName(int value) { this->InstantiationName_TablePos = value; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?SetInstantiationName@CParentDefClassBase@@UAEXVCDefString@@@Z); evidence: RTTI vtable slot + PDB name set.
