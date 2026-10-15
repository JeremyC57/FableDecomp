// CPhysicalPrimitiveSphere::IsPosInside(CThing* thing, const C3DVector& pos) -- true when `pos` lies strictly inside the world sphere (dummy-object origin, radius or the thing's physics radius when unset)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00814E10, length 140 bytes (+ int3 padding).
// Leaf: IsPosInside
// Callees: CPhysicalPrimitiveSphere::GetDummyObjectOrigin(CThing*) @ 0x00814C70, CThingCreatureBase::GetCombatRadius @ 0x00661F90.
// Constant: 0.0001f @ 0x0129BA3C.
// Layout: Ego_r.pdb CPhysicalPrimitiveSphere {vptr, CDefString BaseName +4, float Radius +8}; the thing as in 0x00814D10.
// Shape: the radius stays on the x87 stack while the offset components are formed (x, y, z order) and squared z first;
// the compare `r*r` against the squared length ends in `test ah,41h ; jne` (false) with separate `mov al,1` / `xor al,al`
// exits; `ret 8`.
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

bool CPhysicalPrimitiveSphere::IsPosInside(CThing* thing, const C3DVector& pos) const
{
    C3DVector centre = GetDummyObjectOrigin(thing);
    float radius = GetRadius(thing);
    float dx = pos.x - centre.x;
    float dy = pos.y - centre.y;
    float dz = pos.z - centre.z;
    if (dx * dx + dy * dy + dz * dz < radius * radius)
        return true;
    return false;
}
