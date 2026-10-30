#pragma optimize("s",on)
#include "rebuild_abi.h"

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
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