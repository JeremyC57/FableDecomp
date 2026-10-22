#include "engine/CAnimComponentSetCreatureMode.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentSetCreatureMode* (__fastcall* CloneCreatureModeFn)(CAnimComponentSetCreatureMode*);
CAnimComponentSetCreatureMode* __fastcall Clone(CAnimComponentSetCreatureMode* component)
{
    CloneCreatureModeFn* vtable = (CloneCreatureModeFn*)component->__vftable;
    CAnimComponentSetCreatureMode* copy = vtable[0x10 / 4](component);
    *(long*)copy->Mode = *(long*)component->Mode;
    return copy;
}
