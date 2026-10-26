#pragma optimize("s",on)
#include "rebuild_abi.h"

class CBrainDef { public:
    void *vtbl;
};

// scalar destructor (the masked call target)
extern "C" void __fastcall CBrainDef_dtor(CBrainDef *thisptr);
// operator delete

// vector deleting destructor, retail 0x00459aa2
// __fastcall(this, unsigned int flags) modeled as __fastcall(this, edx, flags)
void * __fastcall CBrainDef_vector_deleting_dtor(
        CBrainDef *thisptr, int /*edx*/, unsigned int flags)
{
    CBrainDef *def = thisptr;
    CBrainDef_dtor(def);
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}