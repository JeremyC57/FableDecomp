#include "engine/CAnimComponentFollowingOnAnimFilter.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentFollowingOnAnimFilter* (__fastcall* CloneFollowingFilterFn)(CAnimComponentFollowingOnAnimFilter*);
CAnimComponentFollowingOnAnimFilter* __fastcall Clone(CAnimComponentFollowingOnAnimFilter* component)
{
    CloneFollowingFilterFn* vtable = (CloneFollowingFilterFn*)component->__vftable;
    CAnimComponentFollowingOnAnimFilter* copy = vtable[0x10 / 4](component);
    copy->NextFilter_TablePos = component->NextFilter_TablePos;
    return copy;
}
