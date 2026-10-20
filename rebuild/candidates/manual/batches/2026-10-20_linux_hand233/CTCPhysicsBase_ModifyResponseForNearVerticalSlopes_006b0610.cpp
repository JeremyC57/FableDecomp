// CTCPhysicsBase::ModifyResponseForNearVerticalSlopes(CPhysicsCollideResponse& response) const -- for class-2 things, flatten a collision normal that is steeper than the physics def's clamped maximum slope into a horizontal unit normal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B0610, length 286 bytes (+ int3 padding), `ret 4`. FableWin.pdb:
// ?ModifyResponseForNearVerticalSlopes@CTCPhysicsBase@@IBEXAAVCPhysicsCollideResponse@@@Z (protected, const).
// Callees: CTCBase::GetDefinitions() const @ 0x006869D0 (the definition holder; its +0xD0 def pointer holds the slope
// angle in degrees at +0x134). Constants: 0.0f 0x0122DEDC, 45.0f 0x0125D2B8 / 89.0f 0x0125D2B4 (angle clamp), 1/360
// 0x01238E00, the double 6.2831854820251465 0x0124F2B8 (2*pi as a float, widened), 3.0f 0x0122DED4, 0.5f 0x0122F59C,
// the reciprocal-square-root mantissa table 0x013CE558.
// Shape: the up component is the inline Dot with (0, 0, 1) (VC7.1 folds the two zero terms into `(y + x) * 0 + z`) and is
// homed in the dead `response` argument slot; the clamp is an if / else-if pair on a float local; `cos` is the intrinsic on
// `angle / 360.0f * 2pi` (double product, `fcos`) assigned to a float local `limit` (that gives retail's `fxch ; fxch`
// before the fcompp; comparing against the cos expression directly drops the pair); the normal's Z is zeroed and the
// inline fast Normalise runs on the member in place (x, y, z locals loaded and popped, the length sum grouped
// `x*x + (y*y + z*z)` for retail's ZZ, YY, add, XX, add order; the last scale consumes the reciprocal: `fmul [esi+14h]`).
// Layout: CTCBase ParentThing +0x04; CThing +0x10 a class byte (2 tested); CPhysicsCollideResponse (PDB) CollidePos +0x00,
// CollideNormal +0x0c, Friction +0x18. engine/CTCPhysicsBase.h flattens the vectors into floats and has no CTCBase
// members, so the class is modelled locally.
#include <math.h>
#pragma intrinsic(cos)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + (y * y + z * z);
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

struct CPhysicsCollideResponse {
    C3DVector CollidePos;       // +0x00
    C3DVector CollideNormal;    // +0x0c
    float     Friction;         // +0x18
};

struct CPhysicsGlobalDef {
    unsigned char _pad000[0x134];
    float         MaxSlopeAngle;                // +0x134 degrees
};
struct CDefinitionHolder {
    unsigned char      _pad00[0xd0];
    CPhysicsGlobalDef* PhysicsDef;              // +0xd0 CDefPointer object
};

struct CThing {
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                   // +0x10
};

struct CTCPhysicsBase {
    void*     __vftable;                        // +0x00
    CThing*   ParentThing;                      // +0x04
    CDefinitionHolder* GetDefinitions() const;  // 0x006869D0 (CTCBase)
    void ModifyResponseForNearVerticalSlopes(CPhysicsCollideResponse& response) const;
};

void CTCPhysicsBase::ModifyResponseForNearVerticalSlopes(CPhysicsCollideResponse& response) const
{
    if (ParentThing->ThingClass == 2) {
        float up = Dot(response.CollideNormal, C3DVector(0.0f, 0.0f, 1.0f));
        float angle = GetDefinitions()->PhysicsDef->MaxSlopeAngle;
        if (angle < 45.0f)
            angle = 45.0f;
        else if (angle > 89.0f)
            angle = 89.0f;
        float limit = cos(angle / 360.0f * 6.2831854820251465);
        if (fabsf(up) < limit) {
            response.CollideNormal.Z = 0.0f;
            response.CollideNormal.Normalise();
        }
    }
}
