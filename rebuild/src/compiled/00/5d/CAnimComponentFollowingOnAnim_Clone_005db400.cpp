#include "engine/CAnimComponentFollowingOnAnim.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentFollowingOnAnim* (__fastcall* CloneFollowingAnimFn)(CAnimComponentFollowingOnAnim*);
CAnimComponentFollowingOnAnim* __fastcall Clone(CAnimComponentFollowingOnAnim* component)
{
    CloneFollowingAnimFn* vtable = (CloneFollowingAnimFn*)component->__vftable;
    CAnimComponentFollowingOnAnim* copy = vtable[0x10 / 4](component);
    copy->NextAnimName_TablePos = component->NextAnimName_TablePos;
    return copy;
}
