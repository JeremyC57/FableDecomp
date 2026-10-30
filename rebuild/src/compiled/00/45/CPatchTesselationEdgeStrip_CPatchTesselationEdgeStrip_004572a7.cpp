#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CBase { int base_pad; };
class CPatchTesselationEdgeStrip { public:
    char pad[0x68];
    void* p68;   /* +0x68 */
    char pad2[0x74 - 0x68 - 4];
    void* p74;   /* +0x74 */
};
void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p74)
        free(self->p74);
    if (self->p68)
        free(self->p68);
    ((CParentDefClassBase*)((CBase*)self))->CParentDefClassBase::~CParentDefClassBase();
}