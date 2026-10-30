#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
// ~CPatchTesselationEdgeStrip
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
struct CMember { void* v; void __fastcall dtor(); };

class CPatchTesselationEdgeStrip { public:
    unsigned char _pad_0x0[0x3c];
    void* p3c;      // +0x3c
    unsigned char _pad_0x40[0x18]; // to 0x58
    void* p58;      // +0x58
    unsigned char _pad_0x5c[0x8];  // to 0x64
    CMember m64;    // +0x64
    CMember m68;    // +0x68
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    self->m68.dtor();
    self->m64.dtor();
    if (self->p58) free(self->p58);
    if (self->p3c) free(self->p3c);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}