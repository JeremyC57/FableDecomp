#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
struct CSub { };
extern void __fastcall CSub_dtor(CSub* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CPatchTesselationEdgeStrip { public:
    char pad0[0x60];
    void* p60;
    char pad1[0x08];
    void* p6c;
    char pad2[0x08];
    void* p78;
    char pad3[0x08];
    void* p84;
    char pad4[0x08];
    CSub sub90;
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    CSub_dtor(&self->sub90);
    if (self->p84) free(self->p84);
    if (self->p78) free(self->p78);
    if (self->p6c) free(self->p6c);
    if (self->p60) free(self->p60);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}