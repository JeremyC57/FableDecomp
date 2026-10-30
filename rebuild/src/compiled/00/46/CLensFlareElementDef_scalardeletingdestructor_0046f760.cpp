#pragma optimize("s",on)
#include "rebuild_abi.h"

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
/* 0x0046f760  CLandscapeBackgroundPatch::`vector deleting destructor'
 * Standard MSVC vector-deleting-destructor thunk:
 *   push esi; mov esi,ecx
 *   call <scalar destructor>          ; this in ecx
 *   test [esp+8], 1                   ; flags & 1 ?
 *   if set: push esi; call operator delete; pop ecx
 *   mov eax,esi; pop esi; ret 4
 * Modeled __fastcall(this, edx, flags) == __fastcall(this, flags).
 */

class CLandscapeBackgroundPatch;

/* engine callees (addresses relocation-masked) */

void* __fastcall CLensFlareElementDef_vector_deleting_dtor(
        CLandscapeBackgroundPatch* thisptr, int /*edx*/, unsigned int flags)
{
    ((CParentDefClassBase*)(thisptr))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(thisptr);
    return thisptr;
}