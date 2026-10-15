// CPhysicalPrimitiveCylinder::GetFirstIntersectionWithTriangle(CThing* thing, const void* triangle, C3DVector* point, C3DVector* normal, long unused) -- build the world cylinder (the transformed dummy-object axis, radius or the thing's physics radius when unset) and intersect the triangle with it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815C40, length 153 bytes (+ int3 padding).
// Leaf: GetFirstIntersectionWithTriangle
// Callees: CPhysicalPrimitiveCylinder::GetDummyObjectOriginTransformed(CThing*, C3DVector&, C3DVector&) @ 0x008158D0,
// CThingCreatureBase::GetCombatRadius @ 0x00661F90, CCylinder::GetFirstIntersectionWithTriangle @ 0x00A593F0.
// Constant: 0.0001f @ 0x0129BA3C (the "radius unset" threshold).
// Layout: Ego_r.pdb CPhysicalPrimitiveCylinder {vptr, CDefString BaseName +4, float Radius +8, float Height +0xc};
// CCylinder {Radius, AxisBottom, AxisTop}; the thing as in CPhysicalPrimitiveSphere 0x00814D10 (def +0x60, flags +0x6c).
// Shape: as the sphere rows -- the radius kept on the x87 stack (`fcom`, then-arm `fstp st(0)`), the inline physics radius
// reading the def radius before the creature-flag test; the CCylinder temporary takes the radius, then both axis points;
// the last argument is unused; `ret 14h`.
struct C3DVector {
    float x, y, z;
};
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
struct CCylinder {
    float     Radius;                              // +0x00
    C3DVector AxisBottom;                          // +0x04
    C3DVector AxisTop;                             // +0x10
    CCylinder(float radius, const C3DVector& bottom, const C3DVector& top) : Radius(radius), AxisBottom(bottom), AxisTop(top) { }
    bool GetFirstIntersectionWithFiniteLine(const C3DVector& start, const C3DVector& end, C3DVector* point, C3DVector* normal) const;   // 0x00A59FD0
    bool GetFirstIntersectionWithTriangle(const void* triangle, C3DVector* point, C3DVector* normal) const;                           // 0x00A593F0
};
struct CDefString { long TablePos; };
struct CPhysicalPrimitiveCylinder {
    virtual void v00();
    CDefString BaseName;                           // +0x04
    float      Radius;                             // +0x08
    float      Height;                             // +0x0c
    void GetDummyObjectOriginTransformed(CThing* thing, C3DVector& bottom, C3DVector& top) const;   // 0x008158D0
    float GetRadius(CThing* thing) const
    {
        float radius = Radius;
        if (radius < 0.0001f)
            radius = thing->GetPhysicsRadius();
        return radius;
    }
    bool GetFirstIntersectionWithFiniteLine(CThing* thing, const C3DVector& start, const C3DVector& end, C3DVector* point, C3DVector* normal, long unused) const;
    bool GetFirstIntersectionWithTriangle(CThing* thing, const void* triangle, C3DVector* point, C3DVector* normal, long unused) const;
};

bool CPhysicalPrimitiveCylinder::GetFirstIntersectionWithTriangle(CThing* thing, const void* triangle, C3DVector* point, C3DVector* normal, long unused) const
{
    C3DVector bottom;
    C3DVector top;
    GetDummyObjectOriginTransformed(thing, bottom, top);
    CCylinder cylinder(GetRadius(thing), bottom, top);
    return cylinder.GetFirstIntersectionWithTriangle(triangle, point, normal);
}
