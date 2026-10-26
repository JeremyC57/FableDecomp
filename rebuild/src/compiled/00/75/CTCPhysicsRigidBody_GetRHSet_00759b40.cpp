// CTCPhysicsRigidBody::GetRHSet(float) -- the right-handed set interpolated between the old and the current frame by t, both axes fast-normalised
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759b40, length 494 bytes, `ret 8` (hidden CRightHandedSet result + t).
// Vtable slot 71 of CTCPhysicsRigidBody (0x0126996C); same body as CTCPhysicsStandard::GetRHSet 0x00723C80 with the members 0x18 higher.
// Callees: none. Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: engine/CTCPhysicsRigidBody.h puts RHSet/OldRHSet at +0x74/+0x8C, but its three vectors before them are 16-byte PDB vectors; retail STLport vectors are 12 bytes, so retail has RHSet +0x68 and OldRHSet +0x80 (the fld/fsub displacements).
// Shape: each axis is `(current - old) * t + old` through C3DVector operators that return `C3DVector(...)`, so the
//   three differences sit on the x87 stack before the scale; the inline fast Normalise is the engine's (exponent
//   (0x17C - e) / 2, table mantissa, one Newton step). The Newton step written `r = (3 - r*r*l) * r; r *= 0.5f;` is
//   what places `mov edx,0x17c` between `fld st(2)` and `fmul st(3)` in the second normalise (a single expression
//   schedules it one instruction later). The result is built by value from the two locals (user C3DVector copy ctor).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z;
        lengthSquared += Y * Y;
        lengthSquared += X * X;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
class CRightHandedSet { public:
    C3DVector Up;           // +0x00
    C3DVector Forward;      // +0x0c
    CRightHandedSet() {}
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};
struct CTCPhysicsRigidBody {
    unsigned char _pad[0x68];
    CRightHandedSet RHSet;      // +0x68
    CRightHandedSet OldRHSet;   // +0x80
    CRightHandedSet GetRHSet(float t) const;
};

CRightHandedSet CTCPhysicsRigidBody::GetRHSet(float t) const
{
    C3DVector up = (RHSet.Up - OldRHSet.Up) * t + OldRHSet.Up;
    C3DVector forward = (RHSet.Forward - OldRHSet.Forward) * t + OldRHSet.Forward;
    up.Normalise();
    forward.Normalise();
    return CRightHandedSet(up, forward);
}