// GetRandomPositionInsideRadius(const C3DVector& centre, float radius) -- a random point at a random distance (up to radius) and random XY bearing from the centre
// Grade: DIFFER(123v131) -- the polar helper: with cos(z) as a multi-use float local VC7.1 homes it in [ebp-4] like retail but hoists fldz ; fcos above the second GFFloatRandom call and reassociates sin(xy)*cos(z)*length (retail: one cos*length product kept on the x87 stack)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE693, 131 bytes (`ret 4`, hidden C3DVector result in ecx).
// Identity: Ego_r.pdb `?GetRandomPositionInsideRadius@@YI?AVC3DVector@@ABV1@M@Z`. The manifest label
//   "Particle_CalcRandomOffset" is propagated and wrong.
// Callees: GFFloatRandom(float range, seed*) 0x004978A0 (__fastcall: the float on the stack, the seed in ecx) with the
//   script random seed 0x013AC834; the (double)2pi pool 0x0124F2B8.
// Shape: length = random(radius) (stored back over the radius slot), bearing = random(1.0) turns, then the same polar
//   helper as 0x00CBE5F0 and + centre.
// Tried: xy double first (124v131), float xy + flat*sin (124v131), cos(z) first + double xy (124v131), cos(z) used
//   twice in `cosZ * length * sin(xy)` (123v131, this file).
// Leaf: ?GetRandomPositionInsideRadius@@YI?AVC3DVector@@ABV1@M@Z
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
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
extern unsigned long g_ScriptRandomSeed;                            // 0x013AC834
// angles are fractions of a full turn; fills the vector in place
__forceinline void SetFromAngles(C3DVector& v, float angleXY, float angleZ, float length)
{
    float cosZ = (float)cos(angleZ * (double)TWO_PI);
    double xy = angleXY * (double)TWO_PI;
    v.X = cosZ * length * (float)sin(xy);
    v.Y = cosZ * length * (float)cos(xy);
    v.Z = (float)sin(angleZ * (double)TWO_PI) * length;
}

C3DVector __fastcall GetRandomPositionInsideRadius(const C3DVector& centre, float radius)
{
    float length = GFFloatRandom(radius, &g_ScriptRandomSeed);
    C3DVector offset;
    SetFromAngles(offset, GFFloatRandom(1.0f, &g_ScriptRandomSeed), 0.0f, length);
    return offset + centre;
}
