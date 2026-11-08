// CLandscapeBackgroundPatch::GetPackedNormalFromTargetVertex(const CTVertexLandscapeBackground& vertex) -- pack the vertex's float normal into the 11:11:10 word (x*1023 bits 0-10, y*1023 bits 11-21, z*511 bits 22-31, each rounded)
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction inline-asm (no control flow) + the Retail-asm evidence line
// Retail-asm: three expansions of the engine's inline-assembly ROUND helper (each component product stored to the by-value copy in a dead argument slot, reloaded and rounded with a bare `fistp dword`), the same helper as the landed retail_asm PackVertexNormal 0x00BE6790/0x00BFDF60. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's inline-asm.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongNear.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Leaf: ?GetPackedNormalFromTargetVertex@CLandscapeBackgroundPatch@@SIKABVCTVertexLandscapeBackground@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail-asm evidence: 0x00BE681A `fstp dword ptr [esp] ; fld dword ptr [esp] ; fistp dword ptr [esp+8]` (x3, z then y then x), the same
// helper the landed CPackedNormalOwner_00be6790::PackVertexNormal (grade retail_asm) expands; VC7.1 C++ `(long)x` calls __ftol2 and
// /QIfist gives `fistp qword` without the reload (GOTCHAS wave 25 chunk 145).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE6800, 113 bytes. Ego_r.pdb:
// ?GetPackedNormalFromTargetVertex@CLandscapeBackgroundPatch@@SIKABVCTVertexLandscapeBackground@@@Z (static __fastcall, vertex in ecx).
// Float pool: 0x0129EF90 = 1023.0f, 0x0129EF94 = 511.0f. No callers by direct call (reached through an inlined/indirect path).
// Shape: the normal is first copied into a C3DVector local (X/Y as integer moves into the frame, Z kept on the FPU stack), then
// PackVertexNormal's body runs inline: z, y, x rounded into a 3-word frame and combined as ((y & 0x7ff) | (z << 11)) << 11 | (x & 0x7ff).
// The ROUND helper is the one of the landed retail_asm source 0x00BE6790.
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class CTVertexLandscapeBackground {
public:
    unsigned short X, Y;                           // +0x00 / +0x02
    float          Z;                              // +0x04
    float          NormalX, NormalY, NormalZ;      // +0x08 / +0x0c / +0x10
    unsigned char  UVPad1;                         // +0x14
};
// the engine's inline-assembly ROUND helper (round to nearest, the FPU default); expanded in place as a __forceinline
#include "engine/GFFloatToLongNear.h"

FABLE_FORCEINLINE unsigned long PackVertexNormal(const C3DVector& normal)
{
    long z = GFFloatToLongNear(normal.Z * 511.0f);
    long y = GFFloatToLongNear(normal.Y * 1023.0f);
    long x = GFFloatToLongNear(normal.X * 1023.0f);
    return (((y & 0x7ff) | (z << 11)) << 11) | (x & 0x7ff);
}

class CLandscapeBackgroundPatch {
public:
    static unsigned long __fastcall GetPackedNormalFromTargetVertex(const CTVertexLandscapeBackground& vertex);
};

unsigned long __fastcall CLandscapeBackgroundPatch::GetPackedNormalFromTargetVertex(const CTVertexLandscapeBackground& vertex)
{
    C3DVector normal(vertex.NormalX, vertex.NormalY, vertex.NormalZ);
    return PackVertexNormal(normal);
}