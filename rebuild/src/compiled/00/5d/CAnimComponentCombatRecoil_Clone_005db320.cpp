// Leaf: ?CAnimComponentCombatRecoil_Clone@@YIPAVCAnimComponentCombatRecoil@@PAV1@@Z
#include "engine/CAnimComponentCombatRecoil.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentCombatRecoil* (__fastcall* CloneRecoilFn)(CAnimComponentCombatRecoil*);
CAnimComponentCombatRecoil* __fastcall CAnimComponentCombatRecoil_Clone(CAnimComponentCombatRecoil* component)
{
    CloneRecoilFn* vtable = (CloneRecoilFn*)component->__vftable;
    CAnimComponentCombatRecoil* copy = vtable[0x10 / 4](component);
    copy->RecoilAnimIndex = component->RecoilAnimIndex;
    return copy;
}
