#pragma optimize("s",on)
#include "rebuild_abi.h"

class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CLandscapeBackgroundPatch;

// scalar destructor (this-only member), relocation-masked call target
// operator delete (cdecl), relocation-masked call target

// `vector_deleting_destructor' @ 0x0046ef6b
// __fastcall void* dtor(unsigned int flags) modeled as __fastcall(this, edx, flags)
void* __fastcall CQuickAccessItemButtonGuiDef_scalar_deleting_destructor(
    CLandscapeBackgroundPatch* thisptr, int /*edx*/, unsigned int flags)
{
    ((CParentDefClassBase*)(thisptr))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(thisptr);
    return thisptr;
}