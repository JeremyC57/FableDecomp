// CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject(CPhysicsCollideResponse*, CTCPhysicsBase*, C3DVector*) -- flatten the collision normal to the horizontal plane and remove its component from the velocity
// Grade: DIFFER(249v249) -- x87 operand order only: the XY length-squared sum (and the normalise's reuse of it) loads X*X first (fld st0 ; fadd st2) where retail loads Y*Y first (fld st1 ; fadd st1, then fxch before the normalise sum), and the normalise scale keeps r on top differently (fxch/fmul st1 v fld st0 ; fmulp)
// Leaf: ?ImpulseRespondToCollisonWithImpassableObject@
// Flags: none
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern const float g_epsilon_0129ba3c;              // 0x0129BA3C (1e-4f)

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float LengthSquaredXY() const { float squared = Y * Y; squared += X * X; return squared; }
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
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { float d = a.X * b.X + a.Y * b.Y; d += a.Z * b.Z; return d; }

struct CPhysicsCollideResponse {
    C3DVector CollidePos;       // +0x00
    C3DVector CollideNormal;    // +0x0c
    float     Friction;         // +0x18
};

struct CTCPhysicsBase {
    void ImpulseRespondToCollisonWithImpassableObject(CPhysicsCollideResponse* response, CTCPhysicsBase* other, C3DVector* velocity);
};

void CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject(CPhysicsCollideResponse* response, CTCPhysicsBase* other, C3DVector* velocity)
{
    C3DVector normal(response->CollideNormal.X, response->CollideNormal.Y, response->CollideNormal.Z);
    if (normal.LengthSquaredXY() > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
        normal.Z = 0.0f;
        normal.Normalise();
    }
    float dot = Dot(normal, *velocity);
    *velocity -= normal * dot;
}
