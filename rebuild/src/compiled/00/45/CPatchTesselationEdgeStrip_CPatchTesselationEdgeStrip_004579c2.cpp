#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CBase { };
class CPatchTesselationEdgeStrip { public:
    unsigned char _pad0[0x3c];
    void* p3c;   // +0x3c
    unsigned char _pad2[0x48-0x40];
    void* p48;   // +0x48
    unsigned char _pad3[0x54-0x4c];
    void* p54;   // +0x54
};
void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p54) free(self->p54);
    if (self->p48) free(self->p48);
    if (self->p3c) free(self->p3c);
    ((CParentDefClassBase*)((CBase*)self))->CParentDefClassBase::~CParentDefClassBase();
}