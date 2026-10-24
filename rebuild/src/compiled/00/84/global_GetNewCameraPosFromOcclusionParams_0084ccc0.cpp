// GetNewCameraPosFromOcclusionParams(const C3DVector&, const C3DVector&, float, const C3DVector&) -- the camera position pushed out along a direction: start + direction * distance (the fourth vector is unused)
// Leaf: ?GetNewCameraPosFromOcclusionParams@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084CCC0, 77 bytes, __fastcall (ecx = hidden result, edx = start; the direction,
//   the distance and the unused vector on the stack), `ret 0Ch`.
// Identity: Ego_r.pdb ?GetNewCameraPosFromOcclusionParams@?A0x49b2e1a3@@YI?AVC3DVector@@ABV2@0M0@Z (an anonymous-namespace
//   free function of the camera TU that also holds C3DFrustrum); callers 0x0084D6B9 / 0x0084D766. The manifest label
//   is the same name.
// Shape: inline C3DVector operators with a user constructor (the landed CTools::GetRollingAverage<C3DVector> 0x0084EAF0
//   model) plus a memberwise user copy constructor: `direction * distance` first (x stays on the FPU stack, y / z
//   spilled), then `start + that` into a NAMED local, returned by copy (VC7.1 has no NRVO: z goes through memory as an
//   integer move, x / y are `fxch` + `fstp`); `return start + direction * distance;` stores the three sums directly.
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
    C3DVector operator*(float f) const { return C3DVector(x * f, y * f, z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
};

namespace {
C3DVector __fastcall GetNewCameraPosFromOcclusionParams(const C3DVector& start, const C3DVector& direction, float distance,
                                                        const C3DVector& unused)
{
    C3DVector pos = start + direction * distance;
    return pos;
}
}

C3DVector (__fastcall *g_keepGetNewCameraPos)(const C3DVector&, const C3DVector&, float, const C3DVector&) =
    &GetNewCameraPosFromOcclusionParams;   // keeps the internal-linkage body emitted; not part of the retail function