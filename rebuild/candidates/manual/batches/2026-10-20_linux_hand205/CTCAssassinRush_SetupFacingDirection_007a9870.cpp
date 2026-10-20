// CTCAssassinRush::SetupFacingDirection(C3DVector& direction) -- normalise the rush direction; when it points within 60 degrees of straight up, use the thing's facing instead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A9870, length 222 bytes, `ret 4`. Called once, from
// 0x007AAD0A; 0x007A6E20 is a byte-identical copy in another TU.
// Callees: the physics body's virtual GetFacing (slot 70, +0x118, C3DVector by hidden pointer, on ParentThing+0x60).
// Globals: the 128-entry reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC,
// the double 1.0471976 (60 degrees as a float radian) 0x01261DC8 fed to the fcos intrinsic.
// Shape: C3DVector's inline fast Normalise (the out-of-line copy is 0x00486F60: exponent (380 - e) / 2, table mantissa, one
// Newton step) writes the components back with `fst`, the inline dot with (0, 0, 1) keeps them on the x87 stack (VC7.1
// folds the two zero terms into `(x + y) * 0`), and `cos` stays an intrinsic on the constant.
#include <math.h>
#pragma intrinsic(cos)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CPhysicsBody {                           // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();              // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsBody* PhysicsBody;                  // +0x60
};
class CTCAssassinRush {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    void SetupFacingDirection(C3DVector& direction);
};

void CTCAssassinRush::SetupFacingDirection(C3DVector& direction)
{
    direction.Normalise();
    if (Dot(direction, C3DVector(0.0f, 0.0f, 1.0f)) > cos(1.0471976115464443))
        direction = ParentThing->PhysicsBody->GetFacing();
}
