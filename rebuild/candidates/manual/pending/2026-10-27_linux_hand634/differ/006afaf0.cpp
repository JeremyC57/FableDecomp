// CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject(const CPhysicsCollideResponse& response, const CTCPhysicsBase& other, C3DVector& velocity) const -- flatten the collision normal to the horizontal plane (when its XY part is not tiny) and remove its component from the velocity
// Grade: DIFFER(249v249) -- x87 operand order only: the XY length sums load the newer product (`fld st0 ; fadd st2`, and `fadd st1` without retail's `fxch`) where retail loads the older (`fld st1 ; fadd st1`); and Y *= r is `fxch ; fmul st1 ; fxch` where retail has `fld st0 ; fmulp st2`
// Leaf: ?ImpulseRespondToCollisonWithImpassableObject@CTCPhysicsBase@@IBEXABVCPhysicsCollideResponse@@ABV1@AAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFAF0, 249 bytes, thiscall `ret 0Ch` (`this` and `other` are never read).
// Identity: Ego_r.pdb / FableWin.pdb `?ImpulseRespondToCollisonWithImpassableObject@CTCPhysicsBase@@IBEXABVCPhysicsCollideResponse@@ABV1@AAVC3DVector@@@Z`
//   (protected, const, the PDB keeps the "Collison" spelling); donor order next to ImpulseRespondToCollisonWithCylinder.
//   Only caller 0x00725728. This retest respells the parked pointer signature to the PDB references (no codegen change).
// Globals: g_InvSqrtMantissaTable 0x013CE558 (the fast inverse-sqrt mantissa table), the epsilon 0x0129BA3C (read twice for
//   its square: a non-literal const). Constants 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC.
// Shape: copy the normal, test its XY length squared against eps^2, zero Z and normalise in place through the inline fast
//   inverse square root (VC7.1 CSEs the XX / YY products of the test into the normalise sum and folds the zero Z term),
//   then velocity -= normal * Dot(normal, velocity) (the dot spilled to the dead first-argument slot).
// Chunk 634 retest (5 grades): the inline LengthSquaredXY() member as the condition (this form) fixes the dot spill of the
//   parked form (retail `fstp [esp+20h]` and memory multiplies) but changes Y *= r; the written operand order of the sums
//   and of the normalise sum (x,y,z or z,y,x) makes no difference: VC7.1 canonicalises commutative operands (micro tests
//   in NOTES.md). PDB reference parameters change nothing.
// Layout: this is modelled through the `#define CTCPhysicsBase CTCPhysicsBaseLayout` include of engine/CTCPhysicsBase.h.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern const float g_epsilon_0129ba3c;              // 0x0129BA3C (1e-4f)

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float LengthSquaredXY() const { return Y * Y + X * X; }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = z * z + y * y + x * x;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { float d = a.X * b.X + a.Y * b.Y; d += a.Z * b.Z; return d; }

class CPhysicsCollideResponse { public:
    C3DVector CollidePos;       // +0x00
    C3DVector CollideNormal;    // +0x0c
    float     Friction;         // +0x18
};

#define CTCPhysicsBase CTCPhysicsBaseLayout
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase

class CTCPhysicsBase : public CTCPhysicsBaseLayout {
protected:
    void ImpulseRespondToCollisonWithImpassableObject(const CPhysicsCollideResponse& response, const CTCPhysicsBase& other, C3DVector& velocity) const;
};

void CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject(const CPhysicsCollideResponse& response, const CTCPhysicsBase& other, C3DVector& velocity) const
{
    C3DVector normal(response.CollideNormal.X, response.CollideNormal.Y, response.CollideNormal.Z);
    if (normal.LengthSquaredXY() > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
        normal.Z = 0.0f;
        normal.Normalise();
    }
    float dot = Dot(normal, velocity);
    velocity -= normal * dot;
}
