// C3DBoundingBox::InitialiseFromSphere(const C3DVector& centre, const C3DVector& extent) -- MinCoord = centre - extent, MaxCoord = centre + extent
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local free operator- removed (the header member operator- is the same inline member-wise computation); local free operator+ removed (the header member operator+ is the same inline member-wise computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A31530, 120 bytes, no calls (`ret 8`).
// Layout from the PDB (C3DBoundingBox: MinCoord +0, MaxCoord +0xC). Each side is a by-value C3DVector
// temporary built on the frame by the inline operator (user constructor) and then assigned word by word
// through integer registers.
#include "engine/C3DVector.h"


class C3DBoundingBox { public:
    C3DVector MinCoord;     // +0x00
    C3DVector MaxCoord;     // +0x0C
    void InitialiseFromSphere(const C3DVector& centre, const C3DVector& extent);
};

void C3DBoundingBox::InitialiseFromSphere(const C3DVector& centre, const C3DVector& extent)
{
    MinCoord = centre - extent;
    MaxCoord = centre + extent;
}