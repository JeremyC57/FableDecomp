// GetRandomPositionInsideRadius(const C3DVector& centre, float radius) -- a random point at a random distance (up to radius) and random XY bearing from the centre
// Leaf: ?GetRandomPositionInsideRadius@@YI?AVC3DVector@@ABV1@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE693, 131 bytes (`ret 4`, hidden C3DVector result in ecx).
// Identity: Ego_r.pdb `?GetRandomPositionInsideRadius@@YI?AVC3DVector@@ABV1@M@Z`. The manifest label
//   "Particle_CalcRandomOffset" is propagated and wrong.
// Callees: GFFloatRandom(float range, seed*) 0x004978A0 (__fastcall: the float on the stack, the seed in ecx) with the
//   script random seed 0x013AC834; the (double)2pi pool 0x0124F2B8.
// Shape: length = random(radius) (stored back over the radius slot), bearing = random(1.0) turns, then the same polar
//   helper as 0x00CBE5F0 and + centre.
// Helper: the PDB's C3DVector::InitialiseFromAngles(angleXY, angleYZ, length) (`?InitialiseFromAngles@C3DVector@@
//   QAEABV1@MMM@Z`) as a __forceinline member: cos(angleYZ) a float local (homed in [ebp-4]), the angles multiplied by
//   the (double)2pi inline, and `sin(xy) * (cosYZ * length)` grouped so cos*length is one CSE kept on the x87 stack
//   (`fld [ebp-4] ; fmul len`, then `fld st(1) ; fmulp` per component); ungrouped `* cosYZ * length` homes the angle
//   instead (130v131).
#pragma optimize("s", on)
#include <math.h>
#pragma intrinsic(sin, cos)
static const float TWO_PI = 6.2831855f;
class C3DVector {
public:
    float X, Y, Z;
    __forceinline C3DVector() { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    // Ego_r.pdb ?InitialiseFromAngles@C3DVector@@QAEABV1@MMM@Z: angles in turns, length
    __forceinline const C3DVector& InitialiseFromAngles(float angleXY, float angleYZ, float length)
    {
        float cosYZ = (float)cos(angleYZ * (double)TWO_PI);
        X = (float)(sin(angleXY * (double)TWO_PI) * (cosYZ * length));
        Y = (float)(cos(angleXY * (double)TWO_PI) * (cosYZ * length));
        Z = (float)(sin(angleYZ * (double)TWO_PI) * length);
        return *this;
    }
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
extern unsigned long g_ScriptRandomSeed;                            // 0x013AC834
// angles are fractions of a full turn; fills the vector in place
C3DVector __fastcall GetRandomPositionInsideRadius(const C3DVector& centre, float radius)
{
    float length = GFFloatRandom(radius, &g_ScriptRandomSeed);
    C3DVector offset;
    offset.InitialiseFromAngles(GFFloatRandom(1.0f, &g_ScriptRandomSeed), 0.0f, length);
    return offset + centre;
}