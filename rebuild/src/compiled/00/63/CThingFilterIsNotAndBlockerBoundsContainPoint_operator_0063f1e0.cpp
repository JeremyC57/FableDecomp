// CThingFilter_IsNotAndBlockerBoundsContainPoint::operator()(const CThing& thing) const -- the thing is not the excluded one, can block navigation, has a physics mesh and that mesh's bounding box contains the point
// Leaf: ??RCThingFilter_IsNotAndBlockerBoundsContainPoint@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063F1E0, 150 bytes, `ret 4`. No direct or absolute references (an
// unreferenced COMDAT copy, like 0x0063F150 beside it).
// Identity: the composite of an out-of-line CThingFilter_IsNot::operator() 0x006A76A0 (through a pointer at +0) and the
// 0x0063F150 test held BY VALUE at +4 (its point at +8). The Ego_r.pdb CFilter_Bind2<F1,F2> holds both filters by pointer
// ({F1* F1; F2* F2}), so this layout is not a Bind2; the class name is descriptive. Catalog label
// "CThingFilter_IsNot_LoadPhysicsMesh" is a propagation.
// Callees: CThingFilter_IsNot::operator() 0x006A76A0, then the 0x0063F150 body inline (CTCPhysicsBase slot 42 / 93,
// CanBlockNavigatingCreature slot 55 on both arms, GetPhysicsMeshObject 0x006B0850, IsPointContainedInBoundingBox
// 0x00A12890, ~CPhysicsMeshObject 0x00A127D0).
// Shape: `return (*NotFilter)(thing) && Inner(thing);` with the inner test `__forceinline` (plain inline stays a call).
// ---- local models. CThingPhysical: retail CThing sits 0x10 below the Ego_r.pdb layout from +0x48 on, so PhysicsTC is +0x60
// (the landed 0x006A4D90 CanBlockNavigatingCreature model; the generated CThingPhysical.h mixes PDB and retail offsets there);
// vtable slot 55 (+0xDC) is CThingPhysical::CanBlockNavigatingCreature 0x006A4D90 (rtti-vtables CThingPhysical 0x01244E84).
// CTCPhysicsBase (vtable 0x0125D2C4): slot 42 (+0xA8) a flag query (base 0x006B1100 returns false, CTCPhysicsStandard
// 0x00724BC0 returns bit 4 of +0x85), slot 93 (+0x174) GetPhysicsMeshInfo (the landed 0x006B0850 notes). CPhysicsMeshObject
// (Ego_r.pdb, 0x40 bytes: vptr, Mesh +4, PositionMatrix +8, Scale +0x38, OO_Scale +0x3C) is modelled opaque.
class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CThingPhysicsMeshInfo;

class CPhysicsMeshObject
{
public:
    virtual ~CPhysicsMeshObject();                                                  // 0x00A127D0
    bool IsPointContainedInBoundingBox(const C3DVector& pos) const;                 // 0x00A12890
    unsigned char _body[0x3C];                                                       // +0x04 .. +0x40
};

class CTCPhysicsBase
{
public:
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const; virtual void v007() const; virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const; virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const; virtual void v040() const; virtual void v041() const;
    virtual bool IsFlag42() const;                                                  // slot 42 (+0xA8)
    virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const; virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const; virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const; virtual void v070() const; virtual void v071() const; virtual void v072() const; virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const; virtual void v079() const; virtual void v080() const; virtual void v081() const; virtual void v082() const; virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const; virtual void v089() const; virtual void v090() const; virtual void v091() const; virtual void v092() const;
    virtual CThingPhysicsMeshInfo* GetPhysicsMeshInfo() const;                      // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject() const;                                // 0x006B0850
};

class CThing
{
public:
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const; virtual void v007() const; virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const; virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const; virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const;
    virtual bool CanBlockNavigatingCreature() const;                                // slot 55 (+0xDC) 0x006A4D90
    unsigned char   _pad04[0x5C];
    CTCPhysicsBase* PhysicsTC;                                                      // +0x60
};

class CThingFilter_IsNot
{
public:
    CThing* Thing;                                                                   // +0x00 (Ego_r.pdb)
    bool operator()(const CThing& thing) const;                                      // 0x006A76A0
};

// The mesh half of the test as a __forceinline helper: the mesh object is a named local built before the point is pushed
// and destroyed at the helper's end (no conditional-destruction flag, as a temporary inside the `&&` would need).
__forceinline bool MeshBoundsContainPoint(const CTCPhysicsBase* tc, const C3DVector& pos)
{
    if (tc->GetPhysicsMeshInfo() == 0)
        return false;
    CPhysicsMeshObject mesh = tc->GetPhysicsMeshObject();
    return mesh.IsPointContainedInBoundingBox(pos);
}

class CThingFilter_BlockerBoundsContainPoint
{
public:
    const void*      Owner;                                                          // +0x00 (not read)
    const C3DVector* Pos;                                                            // +0x04
    __forceinline bool operator()(const CThing& thing) const
    {
        CTCPhysicsBase* physics = thing.PhysicsTC;
        bool blocks = (physics && physics->IsFlag42()) ? thing.CanBlockNavigatingCreature() : thing.CanBlockNavigatingCreature();
        return blocks && MeshBoundsContainPoint(thing.PhysicsTC, *Pos);
    }
};

class CThingFilter_IsNotAndBlockerBoundsContainPoint
{
public:
    const CThingFilter_IsNot*              NotFilter;                                // +0x00
    CThingFilter_BlockerBoundsContainPoint Inner;                                    // +0x04
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsNotAndBlockerBoundsContainPoint::operator()(const CThing& thing) const
{
    return (*NotFilter)(thing) && Inner(thing);
}