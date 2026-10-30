#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CPatchTesselationEdgeStripBase {
};


class CPatchTesselationEdgeStrip { public:
    char pad00[0x40];
    void* p40;   /* +0x40 */
    char pad44[0x4c-0x44];
    void* p4c;   /* +0x4c */
    char pad50[0x58-0x50];
    void* p58;   /* +0x58 */
    char pad5c[0x90-0x5c];
    void* p90;   /* +0x90 */
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p90)
        free(self->p90);
    if (self->p58)
        free(self->p58);
    if (self->p4c)
        free(self->p4c);
    if (self->p40)
        free(self->p40);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}