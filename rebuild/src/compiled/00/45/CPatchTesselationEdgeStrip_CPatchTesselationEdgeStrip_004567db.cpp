#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14
class CPatchTesselationEdgeStrip { public:
    char pad0[0x3c];
    void* p3c;
    char pad1[0x48 - 0x3c - 4];
    void* p48;
};


void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p48)
        free(self->p48);
    if (self->p3c)
        free(self->p3c);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}