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
  char pad[0x60];
  void* p60;   /* +0x60 */
  char pad2[0x70 - 0x64];
  void* p70;   /* +0x70 */
  char pad3[0x80 - 0x74];
  void* p80;   /* +0x80 */
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
  if (self->p80) free(self->p80);
  if (self->p70) free(self->p70);
  if (self->p60) free(self->p60);
  ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}