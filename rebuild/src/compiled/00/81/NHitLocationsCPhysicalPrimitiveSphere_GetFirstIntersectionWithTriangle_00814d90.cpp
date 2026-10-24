// NHitLocations::CPhysicalPrimitiveSphere::GetFirstIntersectionWithTriangle(const CThing& thing, const vector<C3DVector>& triangle, C3DVector& point, C3DVector& normal, float unused) const -- build the world sphere (dummy-object origin, radius or the thing's physics radius when unset) and intersect the triangle with it
// Leaf: ?GetFirstIntersectionWithTriangle@CPhysicalPrimitiveSphere@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00814D90, 122 bytes (+6 int3 padding), `ret 14h`.
// Name: FableWin.pdb ?GetFirstIntersectionWithTriangle@CPhysicalPrimitiveSphere@NHitLocations@@UBE_NABVCThing@@ABV?$vector@VC3DVector@@...@std@@AAVC3DVector@@2M@Z
// (vtable slot 8, catalog vfunc_8); the twin of the landed GetFirstIntersectionWithFiniteLine 0x00814D10 (same model).
// Callees: CPhysicalPrimitiveSphere::GetDummyObjectOrigin @ 0x00814C70 (C3DVector by hidden pointer),
// CThingCreatureBase::GetCombatRadius @ 0x00661F90, NBBBMaths::CSphere::GetFirstIntersectionWithTriangle @ 0x00A58B50.
// Constant: 0.0001f @ 0x0129BA3C (the "radius unset" threshold).
// Layout: Ego_r.pdb CPhysicalPrimitiveSphere {vptr, CDefString BaseName +4, float Radius +8}; CSphere {Radius, Base}.
// The thing is modelled as in the landed twin: def +0x60 (collision radius at def+0x34), flags byte +0x6c (bit 3 = creature).
// Shape: radius kept on the x87 stack across the test, inline GetPhysicsRadius, the CSphere temporary takes the radius then
// the centre; the trailing float argument is unused.
struct C3DVector {
    float x, y, z;
};
template<class T> struct vector {                  // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
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
    bool GetFirstIntersectionWithTriangle(const vector<C3DVector>& triangle, C3DVector& point, C3DVector& normal) const;   // 0x00A58B50
};
namespace NHitLocations {
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
    virtual bool GetFirstIntersectionWithTriangle(const CThing& thing, const vector<C3DVector>& triangle, C3DVector& point, C3DVector& normal, float unused) const;
};

bool CPhysicalPrimitiveSphere::GetFirstIntersectionWithTriangle(const CThing& thing, const vector<C3DVector>& triangle, C3DVector& point, C3DVector& normal, float unused) const
{
    C3DVector centre = GetDummyObjectOrigin(const_cast<CThing*>(&thing));
    CSphere sphere(GetRadius(const_cast<CThing*>(&thing)), centre);
    return sphere.GetFirstIntersectionWithTriangle(triangle, point, normal);
}
} // namespace NHitLocations