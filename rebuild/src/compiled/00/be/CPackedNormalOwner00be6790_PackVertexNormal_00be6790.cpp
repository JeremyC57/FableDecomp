// CPackedNormalOwner_00be6790::PackVertexNormal(const C3DVector& normal) -- pack a unit normal into 11:11:10 bits (x*1023 -> bits 0-10, y*1023 -> bits 11-21, z*511 -> bits 22-31, each rounded with fistp); returns the packed word
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Owner: the class is unrecovered (the CLandscapeBackgroundPatch label was propagated); one inline helper expanded in four TUs, so each address gets its own stand-in owner.
// Retail-asm: 0x00BE67A0 `fstp [esp+10h] ; fld [esp+10h] ; fistp dword ptr [esp+4]` (x3) -- the engine's inline-assembly ROUND helper: each product is homed in the dead `normal` argument slot and reloaded for a bare dword fistp; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` without the reload.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongNear.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE6790, length 106 bytes.
// Callees: none. Float pool: 0x0129EF90 = 1023.0f, 0x0129EF94 = 511.0f (the z scale).
// Shape: the three rounded components land in a 3-word frame (y at [esp], z at [esp+4], x at [esp+8]) and are
// combined as ((y & 0x7ff) | (z << 11)) << 11 | (x & 0x7ff).
#include "engine/C3DVector.h"
struct CPackedNormalOwner_00be6790 {
    unsigned long PackVertexNormal(const C3DVector& normal);
};
// the engine's inline-assembly float-to-long helper, expanded in place (the by-value parameter is homed in a dead slot)
#include "engine/GFFloatToLongNear.h"
unsigned long CPackedNormalOwner_00be6790::PackVertexNormal(const C3DVector& normal)
{
    long z = GFFloatToLongNear(normal.Z * 511.0f);
    long y = GFFloatToLongNear(normal.Y * 1023.0f);
    long x = GFFloatToLongNear(normal.X * 1023.0f);
    return (((y & 0x7ff) | (z << 11)) << 11) | (x & 0x7ff);
}