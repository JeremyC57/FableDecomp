// CTCWaterWader::IsInWater(bool& inWater, bool& deep, float& waterHeight) const -- clear the outputs; unless the world's mode object says always-check, skip things more than 20 units from the camera point or not rendered last frame; skip things whose slot-39 query is set, that lack the 0x31 component or whose 0x31 kind is 0x1A; then read the water height at the thing, and when it is above 0.05 report water deeper than 0.1 over the ground and more than 0.7 above the thing's feet
// Leaf: IsInWater
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B5CA0, length 417 bytes (+ int3 padding; `ret 0Ch`).
// Mangled (Ego_r.pdb): ?IsInWater@CTCWaterWader@@ABEXAA_N0AAM@Z.
// Callees: CTCBase::GetWorld() 0x00686A70; the world's +0x38 object slot 395 (+0x62C); the world's slot 8 (+0x20, by-value
//   C3DVector reference point); CThing::GetPosition 0x004C73D0; CTCGraphicAppearance::WasRenderedLastFrame 0x004BD200;
//   the parent's slot 39 (+0x9C); CVectorMap LowerBound 0x004365B0 (const); the 0x31 component's getter 0x0073A6E0 and its
//   result's slot 1; CTCBase::GetWorldMap() 0x00686D20; CWorldMap::GetWaterSizeZAt 0x0063A420, GetGroundSizeZAt 0x0063A300.
// Data: float pool 400.0f 0x0125D824, 0.05f 0x0125A15C, 0.1f 0x0126D948, 0.7f 0x0126D94C.
// Layout (retail, local models): CTCBase ParentThing +0x04; CThing component bitset +0x20, component map +0x44, graphic
//   appearance +0x64 (landed CCreatureAction_SnapDragon model).
// Shape: the reference-distance test is a `__forceinline` helper with named `world` then `parent` locals (the parent is the only
//   value held in ebx, pushed and popped inside that branch; it loads between GetWorld and the virtual call); the squared length
//   is `X*X + (Y*Y + Z*Z)` (retail's Z2,Y2,X2 order); the mode object goes through an inline getter (straight into ecx); the
//   inlined component lookup takes `map = &m_tcs` first; the ground height is a named float local (`fld [ebp] ; fsub st(1)`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
};
class CTCBase;
struct STCEntry {
    long     m_kind;
    CTCBase* m_pTC;
};
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind) const;   // 0x004365B0
};
struct CTCGraphicAppearance {
    bool WasRenderedLastFrame() const;                      // 0x004BD200
};
class CThing { public:
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38)
    virtual long GetOwnerQuery() const;                     // slot 39 (+0x9c)
    unsigned char  _pad04[0x1c];
    unsigned long  m_tcMask[8];                             // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                                   // +0x44
    unsigned char  _pad50[0x64 - 0x50];
    CTCGraphicAppearance* PGraphicAppearance;               // +0x64
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase* GetTC(long kind) const
    {
        const CTCMap* map = &m_tcs;
        STCEntry* it = map->LowerBound((unsigned long)kind);
        return (it == map->m_pLast || kind < it->m_kind ? map->m_pLast : it)->m_pTC;
    }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    float GetDistanceSquaredTo(const C3DVector& point) const { return (GetPosition() - point).GetLengthSquared(); }
};
struct CKindObject {
    virtual void v00();
    virtual long GetKind() const;                           // slot 1 (+4)
};
struct CTC31 {                                              // component 0x31
    CKindObject* GetKindObject() const;                     // 0x0073A6E0
};
struct CWorldModeObject {
    VF(000) VF(001) VF(002) VF(003) VF(004) VF(005) VF(006) VF(007) VF(008) VF(009)
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(01) VF10(02) VF10(03) VF10(04) VF10(05) VF10(06) VF10(07) VF10(08) VF10(09)
    VF10(10) VF10(11) VF10(12) VF10(13) VF10(14) VF10(15) VF10(16) VF10(17) VF10(18) VF10(19)
    VF10(20) VF10(21) VF10(22) VF10(23) VF10(24) VF10(25) VF10(26) VF10(27) VF10(28) VF10(29)
    VF10(30) VF10(31) VF10(32) VF10(33) VF10(34) VF10(35) VF10(36) VF10(37) VF10(38)
    VF(390) VF(391) VF(392) VF(393) VF(394)
    virtual bool IsAlwaysChecking() const;                  // slot 395 (+0x62c)
};
class CWorld { public:
    VF(w0) VF(w1) VF(w2) VF(w3) VF(w4) VF(w5) VF(w6) VF(w7)
    virtual C3DVector GetReferencePoint() const;            // slot 8 (+0x20)
    unsigned char _pad04[0x34];
    CWorldModeObject* PModeObject;                          // +0x38
    CWorldModeObject* GetModeObject() const { return PModeObject; }
};
class CWorldMap { public:
    float GetWaterSizeZAt(const C3DVector& pos) const;       // 0x0063A420
    float GetGroundSizeZAt(const C3DVector& pos) const;      // 0x0063A300
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    CWorld* GetWorld_00686a70() const;                               // 0x00686A70
    CWorldMap* GetWorldMap() const;                         // 0x00686D20
};
struct CTCWaterWader : CTCBase {
    void IsInWater(bool& inWater, bool& deep, float& waterHeight) const;
    __forceinline float GetDistanceSquaredFromReferencePoint() const
    {
        CWorld* world = GetWorld_00686a70();
        const CThing* parent = ParentThing;
        return (parent->GetPosition() - world->GetReferencePoint()).GetLengthSquared();
    }
};

void CTCWaterWader::IsInWater(bool& inWater, bool& deep, float& waterHeight) const
{
    inWater = false;
    deep = false;
    waterHeight = 0.0f;
    if (!GetWorld_00686a70()->GetModeObject()->IsAlwaysChecking())
    {
        if (GetDistanceSquaredFromReferencePoint() > 400.0f)
            return;
        if (!ParentThing->PGraphicAppearance->WasRenderedLastFrame())
            return;
    }
    if (ParentThing->GetOwnerQuery() != 0)
        return;
    if (!ParentThing->HasTC(0x31))
        return;
    if (((CTC31*)ParentThing->GetTC(0x31))->GetKindObject()->GetKind() == 0x1a)
        return;
    waterHeight = GetWorldMap()->GetWaterSizeZAt(ParentThing->GetPosition());
    if (waterHeight > 0.05f)
    {
        float groundHeight = GetWorldMap()->GetGroundSizeZAt(ParentThing->GetPosition());
        inWater = waterHeight - groundHeight > 0.1f;
        deep = waterHeight - ParentThing->GetPosition().Z > 0.7f;
    }
}