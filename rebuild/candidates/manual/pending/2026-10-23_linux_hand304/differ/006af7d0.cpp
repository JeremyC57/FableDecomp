// Grade: DIFFER(614v612) -- 130 difflines: retail reads the ternary axis temp through eax=&axis from memory at every use (frame 0x3c); ours caches the axis in x87 registers (frame 0x30), cascading through the cross products. Tried value/ref ternary, sequential-member cross product, if/else axis stores.
// CTCPhysicsBase::ImpulseRespondToCollision(const CPhysicsCollideResponse& response, C3DVector& velocity) const -- with the global response switch set, split the velocity into the collide normal and two tangents and rebuild it with the normal part clamped to non-negative; otherwise remove the velocity's component along the normal
// Leaf: ?ImpulseRespondToCollision@CTCPhysicsBase@@IBEXABVCPhysicsCollideResponse@@AAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF7D0, 610 bytes, `ret 8` (this unused).
// Callees: C3DVector::Normalise 0x00A14440 (twice). Globals: bool 0x013B86EC (as the landed CTCPhysicsBase code, named
//   descriptively), 0.0f 0x0122DEDC.
// Layout: CPhysicsCollideResponse (PDB, 0x1c): CollidePos +0x00, CollideNormal +0x0c, Friction +0x18.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector& operator-=(const C3DVector& v) { X -= v.X; Y -= v.Y; Z -= v.Z; return *this; }
    void Normalise();                               // 0x00A14440
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
inline float DotProduct(const C3DVector& a, const C3DVector& b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

struct CPhysicsCollideResponse {
    C3DVector CollidePos;                           // +0x00
    C3DVector CollideNormal;                        // +0x0c
    float     Friction;                             // +0x18
};
extern bool g_flipMeshCollideNormals_013b86ec;      // 0x013B86EC

class CTCPhysicsBase {
public:
    virtual void v000();
    void ImpulseRespondToCollision(const CPhysicsCollideResponse& response, C3DVector& velocity) const;
};

void CTCPhysicsBase::ImpulseRespondToCollision(const CPhysicsCollideResponse& response, C3DVector& velocity) const
{
    if (g_flipMeshCollideNormals_013b86ec) {
        C3DVector normal = response.CollideNormal;
        const C3DVector& axis = (normal.Y > normal.Z) ? C3DVector(0.0f, 0.0f, 1.0f) : C3DVector(0.0f, 1.0f, 0.0f);
        C3DVector tangent1 = CrossProduct(normal, axis);
        tangent1.Normalise();
        C3DVector tangent2 = CrossProduct(normal, tangent1);
        tangent2.Normalise();
        float normalPart = DotProduct(normal, velocity);
        float part1 = DotProduct(tangent1, velocity);
        float part2 = DotProduct(tangent2, velocity);
        if (normalPart < 0.0f)
            normalPart = 0.0f;
        velocity = normal * normalPart + tangent1 * part1 + tangent2 * part2;
    } else {
        velocity -= response.CollideNormal * DotProduct(response.CollideNormal, velocity);
    }
}
