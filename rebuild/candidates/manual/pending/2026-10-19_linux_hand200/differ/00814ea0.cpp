// Grade: DIFFER(ours v retail) -- 238v238 x87 operand order only in the z term of `offset * radius + centre`: retail `fld radius ; fmul z` then `fld tmp ; fadd centre.z`, ours `fld z ; fmul radius` / `fld centre.z ; fadd tmp` (s*z spelling did not change it)
// CPhysicalPrimitiveSphere::GetClosestPointToPos2D(CThing* thing, const C3DVector& pos, C3DVector* direction) -- the point on the sphere's horizontal circle nearest `pos`: the flattened unit direction from the centre (returned through `direction`) scaled by the radius
// Leaf: GetClosestPointToPos2D
// Retail: Fable.exe (TLC, VC7.1) @ 0x00814EA0, 238 bytes, `ret 10h` (hidden C3DVector result + 3 arguments); vtable slot 10.
// Callees: CPhysicalPrimitiveSphere::GetDummyObjectOrigin(CThing*) 0x00814C70, CThingCreatureBase::GetCombatRadius
//   0x00661F90, C3DVector::Normalise 0x00A14440 (catalogued "C3DVector::GetScaled"). Constant: 0.0001f @ 0x0129BA3C.
// Shape: the landed sphere family (IsPosInside 0x00814E10): the radius falls back to the thing's physics radius when
//   below 1e-4 (written out in the body: through the landed inline GetRadius the def radius stays on the x87 stack
//   instead of retail's dword copy) and is homed in the dead `thing` argument slot. The offset is a C3DVector(dx, dy, 0) local; it is
//   normalised only when its 2D squared length (y term first: an `s = y*y; s += x*x;` chain, a single expression is
//   reordered) exceeds eps*eps -- `fld eps ; fmul eps`, so eps is an
//   out-of-TU constant (a literal would fold the product) and the radius test uses the same symbol. The direction is
//   copied out dword by dword and the result is `offset * radius + centre` built straight into the hidden result.
struct C3DVector {
    float x, y, z;
    C3DVector() {}
    C3DVector(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float GetLengthSquared2D() const { float s = y * y; s += x * x; return s; }
    void Normalise();                              // 0x00A14440
    C3DVector operator*(float s) const { return C3DVector(x * s, y * s, z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
};
extern const float g_epsilon_0129ba3c;             // 0x0129BA3C (1e-4f)

struct CThingDef_Physics {                         // the thing's definition (only the collision radius is read)
    unsigned char _pad[0x34];
    float         CollisionRadius;                 // +0x34
};
struct CThing {
    unsigned char      _pad00[0x60];
    CThingDef_Physics* m_pDef;                     // +0x60
    unsigned char      _pad64[0x08];
    unsigned char      m_flags;                    // +0x6c (bit 3: a creature)
    float GetPhysicsRadius();
};
struct CThingCreatureBase : CThing {
    float GetCombatRadius();                       // 0x00661F90
};
inline float CThing::GetPhysicsRadius()
{
    float radius = m_pDef->CollisionRadius;
    if (m_flags & 8)
        radius = ((CThingCreatureBase*)this)->GetCombatRadius();
    return radius;
}
struct CDefString { long TablePos; };
struct CPhysicalPrimitiveSphere {
    virtual void v00();
    CDefString BaseName;                           // +0x04
    float      Radius;                             // +0x08
    C3DVector GetDummyObjectOrigin(CThing* thing) const;   // 0x00814C70
    float GetRadius(CThing* thing) const
    {
        float radius = Radius;
        if (radius < g_epsilon_0129ba3c)
            radius = thing->GetPhysicsRadius();
        return radius;
    }
    C3DVector GetClosestPointToPos2D(CThing* thing, const C3DVector& pos, C3DVector* direction) const;
};

C3DVector CPhysicalPrimitiveSphere::GetClosestPointToPos2D(CThing* thing, const C3DVector& pos, C3DVector* direction) const
{
    C3DVector centre = GetDummyObjectOrigin(thing);
    float radius = Radius;
    if (radius < g_epsilon_0129ba3c) {
        radius = thing->m_pDef->CollisionRadius;
        if (thing->m_flags & 8)
            radius = ((CThingCreatureBase*)thing)->GetCombatRadius();
    }
    C3DVector offset(pos.x - centre.x, pos.y - centre.y, 0.0f);
    if (offset.GetLengthSquared2D() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
        offset.Normalise();
    *direction = offset;
    return offset * radius + centre;
}
