// C3DBoundingBox::CalcShortestDistanceToPoint(const C3DVector& point) const -- distance from a point to the box (0 inside): the square root of the inlined squared form (SSE kernel or x87 clamp path)
// Leaf: ?CalcShortestDistanceToPoint@C3DBoundingBox@@QBEMABVC3DVector@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Grade: RELOCATION_MATCH over the true extent (286 bytes; catalog 328 = int3 padding + the next body): sqrt of the
//   inlined CalcShortestDistanceToPointSquared (landed 0x00BF7DF0 model, retail_asm SSE kernel). Parked 'inline-asm SSE
//   lane gap' disproved; the catalog label is propagated from the fsqrt-less twin.
// Retail-asm: 0x00B9C50F..0x00B9C553 `movss xmm0,[ecx+8] ; movhps xmm0,[ecx] ; ... ; maxps xmm2,xmm0 ; minps xmm2,xmm1 ; subps xmm2,xmm3 ; mulps xmm2,xmm2 ; shufps xmm2,xmm2,0B0h ; addps xmm2,xmm3 ; shufps xmm2,xmm2,3 ; addss xmm2,xmm3 ; movss [esp+4],xmm2` -- the hand SSE kernel behind g_SSEAvailable 0x013D2880 (`this` spilled to [esp] and reloaded, the point reloaded from its argument slot, because the asm names them; VC7.1 C++ emits no packed SSE); the x87 branch is the C++ fallback. The same kernel as landed 0x00BF7DF0.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared engine/C3DBoundingBoxSSE.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SSE_BOX.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9C500, 286 bytes (`ret 4`, float in st0, ends in fsqrt); the catalog's 328 runs over int3
//   padding into the next body (0x00B9C620): BOUNDS_FIX.tsv. Identity: Ego_r.pdb / FableWin.pdb
//   ?CalcShortestDistanceToPoint@C3DBoundingBox@@QBEMABVC3DVector@@@Z (the catalog label CalcShortestDistanceToPointSquared is
//   propagated from the fsqrt-less twin 0x00B49810).
#include "fable_compiler.h"
#include "engine/C3DBoundingBoxSSE.h"
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)
extern bool g_SSEAvailable;                             // 0x013D2880

#include "engine/C3DVector.h"
class C3DBoundingBox {
public:
    C3DVector MinCoord;                                 // +0x00
    C3DVector MaxCoord;                                 // +0x0c
    FABLE_FORCEINLINE float CalcShortestDistanceToPointSquared(const C3DVector& point) const;
    float CalcShortestDistanceToPoint(const C3DVector& point) const;
};
FABLE_FORCEINLINE float C3DBoundingBox::CalcShortestDistanceToPointSquared(const C3DVector& point) const
{
    if (g_SSEAvailable) {
        float result;
        GF_SSE_BOX_DISTANCE_SQUARED(this, point, result);
        return result;
    }
    C3DVector closest(point);
    if (closest.X < MinCoord.X)
        closest.X = MinCoord.X;
    else if (closest.X > MaxCoord.X)
        closest.X = MaxCoord.X;
    if (closest.Y < MinCoord.Y)
        closest.Y = MinCoord.Y;
    else if (closest.Y > MaxCoord.Y)
        closest.Y = MaxCoord.Y;
    if (closest.Z < MinCoord.Z)
        closest.Z = MinCoord.Z;
    else if (closest.Z > MaxCoord.Z)
        closest.Z = MaxCoord.Z;
    return (point - closest).GetSquaredMagnitude();
}

float C3DBoundingBox::CalcShortestDistanceToPoint(const C3DVector& point) const
{
    return (float)sqrt(CalcShortestDistanceToPointSquared(point));
}