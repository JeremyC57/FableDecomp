#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
#include "rebuild_abi.h"

// operator delete (relocation-masked target)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14

// base-class destructor tail-called at end (relocation-masked target)
class CPatchTesselationEdgeStrip;

class CPatchTesselationEdgeStrip { public:
    char _pad[0x6c + 4];
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* thisptr)
{
    void* p;

    p = *(void**)((char*)thisptr + 0x6c);
    if (p) free(p);

    p = *(void**)((char*)thisptr + 0x60);
    if (p) free(p);

    p = *(void**)((char*)thisptr + 0x54);
    if (p) free(p);

    p = *(void**)((char*)thisptr + 0x48);
    if (p) free(p);

    p = *(void**)((char*)thisptr + 0x3c);
    if (p) free(p);

    ((CParentDefClassBase*)(thisptr))->CParentDefClassBase::~CParentDefClassBase();
}