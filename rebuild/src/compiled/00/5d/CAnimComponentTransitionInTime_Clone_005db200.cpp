#include "engine/CAnimComponentTransitionInTime.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentTransitionInTime* (__fastcall* CloneTransitionFn)(CAnimComponentTransitionInTime*);
CAnimComponentTransitionInTime* __fastcall Clone(CAnimComponentTransitionInTime* component)
{
    CloneTransitionFn* vtable = (CloneTransitionFn*)component->__vftable;
    CAnimComponentTransitionInTime* copy = vtable[0x10 / 4](component);
    copy->TransitionInTime = component->TransitionInTime;
    return copy;
}
