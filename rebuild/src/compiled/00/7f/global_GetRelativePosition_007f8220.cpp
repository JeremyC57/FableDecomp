// GetRelativePosition(long index, long count) -- the levitation slot of item `index` of `count` around the creature: the fixed offset rotated toward a point on a horizontal circle (azimuth index/count of a turn, elevation a quarter of that)
// Leaf: ?GetRelativePosition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8220, 221 bytes (`ret 4` + int3 padding), `__fastcall` returning the C3DVector
//   through the hidden pointer (hidden result in ecx, index in edx, count on the stack). FableWin.pdb / Ego_r.pdb
//   `?GetRelativePosition@?A0x6dc84f21@@YI?AVC3DVector@@JJ@Z`: a function in the CTCCreatureItemLevitationBase TU's
//   ANONYMOUS namespace (the leaf is therefore the name prefix: our TU's namespace hash differs). No direct callers
//   (the out-of-line copy of an inlined helper). Catalog label `Ellipsoid_ComputePoint` was descriptive.
// Data: the levitation offset C3DVector (0, 0, 2.5) @ 0x01384494 (also read by CTCCreatureItemLevitationBase::FrameUpdate
//   0x007F8300); constants 0.25f @ 0x0123078C, (double)(2 pi as a float) @ 0x0124F2B8, 0.0f @ 0x0122DEDC.
// Shape: fraction = index / count (`fild ; fidiv`); the elevation fraction (a quarter) is a float local homed in the dead
//   `count` slot; the horizontal direction is `XAxis * cos(a) + YAxis * sin(a)` from literal C3DVectors (0 * s computed once
//   and reused, 1 * s folded), the result `offset * cos(e) + dir * sin(e)` (the right operand of each + built first).
//   Each angle product is written inside BOTH its sin and cos (VC CSEs them): with one named angle local the elevation is
//   forward-substituted to its use and the fraction stays on the x87 stack instead of being consumed early.
extern "C" double __cdecl sin(double x);
extern "C" double __cdecl cos(double x);
#pragma intrinsic(sin, cos)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};

extern C3DVector g_LevitationOffset;                            // 0x01384494 (0, 0, 2.5)

namespace {
C3DVector __fastcall GetRelativePosition(long index, long count)
{
    float fraction = (float)index / count;
    float elevation = fraction * 0.25f;
    C3DVector dir = C3DVector(1.0f, 0.0f, 0.0f) * (float)cos(fraction * 6.2831854820251465)
                  + C3DVector(0.0f, 1.0f, 0.0f) * (float)sin(fraction * 6.2831854820251465);
    return g_LevitationOffset * (float)cos(elevation * 6.2831854820251465) + dir * (float)sin(elevation * 6.2831854820251465);
}
}

C3DVector (__fastcall* g_pGetRelativePosition)(long, long) = GetRelativePosition;   // keeps the anonymous helper emitted