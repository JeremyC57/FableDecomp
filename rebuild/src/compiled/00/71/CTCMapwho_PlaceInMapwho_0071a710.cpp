// Byte-exact reconstruction of CTCMapwho::PlaceInMapwho @ 0x0071a710
// Leaf: ?CTCMapwho_PlaceInMapwho@@YIXPAVCTCMapwho@@HPAVC3DVector@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.

#include "engine/C3DVector.h"

struct Helper;
struct Slot;

class CTCMapwho { public:
    char pad[0x50];
    unsigned char flags;       // +0x50
};

// global returning Helper* (0x686d30) -- no args, cdecl
extern "C" Helper* __cdecl GetMapwhoHelper(void);

// The object stored at Helper+0x10. Retail method 0x637fc0 is a native __fastcall:
// this in ecx (= Helper+0x10), and two stack args (mapwho, vector). Modeled as a
// real C++ member so VC7.1 emits a native thiscall (this in ecx, args on stack) --
// no __fastcall keyword, so the harness cannot rewrite it and no edx is touched.
struct Slot {
    void Place(CTCMapwho* mw, C3DVector* v);
};

// Helper layout: pointer to Slot at +0x10
struct Helper {
    char pad[0x10];
    Slot* slot;                // +0x10
};

// Retail PlaceInMapwho is __fastcall: this in ecx, the C3DVector* on the stack.
// The harness rewrites __fastcall->__fastcall, so model the this pointer in
// ecx and force the vector onto the stack by consuming edx with a dummy arg.
void __fastcall CTCMapwho_PlaceInMapwho(CTCMapwho* mapwho, int /*edx*/, C3DVector* v)
{
    if (!(mapwho->flags & 1)) {
        Helper* h = GetMapwhoHelper();
        h->slot->Place(mapwho, v);
    }
}