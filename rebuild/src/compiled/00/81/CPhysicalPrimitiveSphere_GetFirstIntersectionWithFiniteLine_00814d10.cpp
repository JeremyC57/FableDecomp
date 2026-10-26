// CPhysicalPrimitiveSphere::GetFirstIntersectionWithFiniteLine(CThing* thing, const C3DVector& start, const C3DVector& end, C3DVector* point, C3DVector* normal, long unused) -- build the world sphere (dummy-object origin, radius or the thing's physics radius when unset) and intersect the segment with it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00814D10, length 127 bytes.
// Leaf: GetFirstIntersectionWithFiniteLine
// Callees: CPhysicalPrimitiveSphere::GetDummyObjectOrigin(CThing*) @ 0x00814C70 (C3DVector by hidden pointer),
// CThingCreatureBase::GetCombatRadius @ 0x00661F90, CSphere::GetFirstIntersectionWithFiniteLine @ 0x00A58790.
// Constant: 0.0001f @ 0x0129BA3C (the "radius unset" threshold).
// Layout: Ego_r.pdb CPhysicalPrimitiveSphere {vptr, CDefString BaseName +4, float Radius +8}; CSphere {Radius, Base}.
// The thing is modelled locally (retail CThing is smaller than the PDB one): def +0x60 (collision radius at def+0x34),
// flags byte +0x6c (bit 3 = creature).
// Shape: the radius stays on the x87 stack (`fcom` without pop; the then-arm drops it with `fstp st(0)`); the thing's
// radius reads the def radius first and replaces it by the creature's combat radius under the flag (an inline
// GetPhysicsRadius); the CSphere temporary takes the radius then the centre; the sixth argument is unused; `ret 18h`.
class C3DVector { public:
    float x, y, z;
};
struct CThingDef_Physics {                         // the thing's definition (only the collision radius is read)
    unsigned char _pad[0x34];
    float         CollisionRadius;                 // +0x34
};
class CThing { public:
    unsigned char      _pad00[0x60];
    CThingDef_Physics* m_pDef;                     // +0x60
    unsigned char      _pad64[0x08];
    unsigned char      m_flags;                    // +0x6c (bit 3: a creature)
    float GetPhysicsRadius();
};
class CThingCreatureBase : public CThing { public:
public:
    float GetCombatRadius() const;   // 0x00661F90
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
class CDefString { public: long TablePos; };
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

bool CPhysicalPrimitiveSphere::GetFirstIntersectionWithFiniteLine(CThing* thing, const C3DVector& start, const C3DVector& end, C3DVector* point, C3DVector* normal, long unused) const
{
    C3DVector centre = GetDummyObjectOrigin(thing);
    CSphere sphere(GetRadius(thing), centre);
    return sphere.GetFirstIntersectionWithFiniteLine(start, end, point, normal);
}