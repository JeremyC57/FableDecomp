#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
#include "rebuild_abi.h"

// operator delete (retail 0x7a6f30) — relocation-masked call target
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14

class CPatchTesselationEdgeStrip;
// base destructor tail-called at end (relocation-masked target)
void __fastcall CPatchTesselationEdgeStrip_base_dtor(CPatchTesselationEdgeStrip* thisptr);

class CPatchTesselationEdgeStrip { public:
    char pad00[0x3c];
    void* p3c;   // +0x3c
    char pad40[0x48 - 0x40];
    void* p48;   // +0x48
    char pad4c[0x54 - 0x4c];
    void* p54;   // +0x54
    char pad58[0x60 - 0x58];
    void* p60;   // +0x60
    char pad64[0x6c - 0x64];
    void* p6c;   // +0x6c
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* thisptr)
{
    if (thisptr->p6c) { free(thisptr->p6c); }
    if (thisptr->p60) { free(thisptr->p60); }
    if (thisptr->p54) { free(thisptr->p54); }
    if (thisptr->p48) { free(thisptr->p48); }
    if (thisptr->p3c) { free(thisptr->p3c); }
    CPatchTesselationEdgeStrip_base_dtor(thisptr);
}