// NHitLocations::CPhysicalPrimitiveSphere::GetBoundingSphere(CThing* thing, C3DVector& centre, float& radius) -- the world bounding sphere: the dummy-object origin and the radius (or the thing's physics radius when unset)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00814F90, length 124 bytes (+ int3 padding).
// Leaf: GetBoundingSphere
// Callees: CPhysicalPrimitiveSphere::GetDummyObjectOrigin(CThing*) @ 0x00814C70, CThingCreatureBase::GetCombatRadius @ 0x00661F90.
// Constant: 0.0001f @ 0x0129BA3C.
// Layout: Ego_r.pdb CPhysicalPrimitiveSphere {vptr, CDefString BaseName +4, float Radius +8}; the thing as in 0x00814D10.
// Shape: the centre is copied word by word from the returned hidden pointer (eax); the radius store sits in each of the
// three arms (`fstp [edx]` / `[eax]` / `[ecx]`), written as per-arm stores with early returns (the inline GetRadius used
// by 0x00814D10/0x00814E10 merges two of the exits); `ret 0Ch`.
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
struct CSphere {
    float     Radius;                              // +0x00
    C3DVector Base;                                // +0x04
    CSphere(float radius, const C3DVector& base) : Radius(radius), Base(base) { }
    bool GetFirstIntersectionWithFiniteLine(const C3DVector& start, const C3DVector& end, C3DVector* point, C3DVector* normal) const;   // 0x00A58790
};
struct CDefString { long TablePos; };
struct CPhysicalPrimitiveSphere {
    virtual void v00();
    CDefString BaseName;                           // +0x04
    float      Radius;                             // +0x08
    C3DVector GetDummyObjectOrigin(CThing* thing) const;   // 0x00814C70
    float GetRadius(CThing* thing) const
    {
        float radius = Radius;
        if (radius < 0.0001f)
            radius = thing->GetPhysicsRadius();
        return radius;
    }
    bool GetFirstIntersectionWithFiniteLine(CThing* thing, const C3DVector& start, const C3DVector& end, C3DVector* point, C3DVector* normal, long unused) const;
    bool IsPosInside(CThing* thing, const C3DVector& pos) const;
    void GetBoundingSphere(CThing* thing, C3DVector& centre, float& radius) const;
};

void CPhysicalPrimitiveSphere::GetBoundingSphere(CThing* thing, C3DVector& centre, float& radius) const
{
    centre = GetDummyObjectOrigin(thing);
    float r = Radius;
    if (r < 0.0001f) {
        float thingRadius = thing->m_pDef->CollisionRadius;
        if (thing->m_flags & 8) {
            radius = ((CThingCreatureBase*)thing)->GetCombatRadius();
            return;
        }
        radius = thingRadius;
        return;
    }
    radius = r;
}