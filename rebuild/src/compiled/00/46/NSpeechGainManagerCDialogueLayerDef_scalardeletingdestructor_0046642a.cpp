#pragma optimize("s",on)
#include "rebuild_abi.h"

// vector deleting destructor @ 0x0046642a (CLandscapeBackgroundPatch)
// this in ecx, flags on stack; sets vftable, calls dtor, conditionally frees, returns this; ret 4

class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
class CLandscapeBackgroundPatch { public:
    void** vftable;
};

extern void* g_CLandscapeBackgroundPatch_vftable; // vtable @ 0x1230ba0

void* __fastcall CDialogueLayerDef_vector_dtor(
    CLandscapeBackgroundPatch* thisptr, int /*edx*/, unsigned int flags)
{
    thisptr->vftable = (void**)&g_CLandscapeBackgroundPatch_vftable;
    ((CResource*)(thisptr))->CResource::~CResource();
    if (flags & 1) {
        operator delete(thisptr);
    }
    return thisptr;
}