// Leaf: ?CAnimComponentFlags_Clone@@YIPAVCAnimComponentFlags@@PAV1@@Z
#include "engine/CAnimComponentFlags.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentFlags* (__fastcall* CloneFlagsFn)(CAnimComponentFlags*);
CAnimComponentFlags* __fastcall CAnimComponentFlags_Clone(CAnimComponentFlags* component)
{
    CloneFlagsFn* vtable = (CloneFlagsFn*)component->__vftable;
    CAnimComponentFlags* copy = vtable[0x10 / 4](component);
    copy->Flags = component->Flags;
    return copy;
}
