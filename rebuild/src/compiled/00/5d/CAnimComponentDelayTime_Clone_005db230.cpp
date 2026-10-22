#include "engine/CAnimComponentDelayTime.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentDelayTime* (__fastcall* CloneDelayFn)(CAnimComponentDelayTime*);
CAnimComponentDelayTime* __fastcall Clone(CAnimComponentDelayTime* component)
{
    CloneDelayFn* vtable = (CloneDelayFn*)component->__vftable;
    CAnimComponentDelayTime* copy = vtable[0x10 / 4](component);
    copy->Delay = component->Delay;
    return copy;
}
