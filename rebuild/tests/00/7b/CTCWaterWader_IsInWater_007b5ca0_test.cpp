// Behaviour test for 0x007B5CA0: the reconstructed body (pragmas stripped) driven through recording stubs.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
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
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
};
struct CTCBase;
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
struct CThing {
#define VF(n) virtual void v##n() {}
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
struct CWorld {
    VF(w0) VF(w1) VF(w2) VF(w3) VF(w4) VF(w5) VF(w6) VF(w7)
    virtual C3DVector GetReferencePoint() const;            // slot 8 (+0x20)
    unsigned char _pad04[0x34];
    CWorldModeObject* PModeObject;                          // +0x38
    CWorldModeObject* GetModeObject() const { return PModeObject; }
};
struct CWorldMap {
    float GetWaterSizeZAt(const C3DVector& pos) const;       // 0x0063A420
    float GetGroundSizeZAt(const C3DVector& pos) const;      // 0x0063A300
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    CWorld* GetWorld() const;                               // 0x00686A70
    CWorldMap* GetWorldMap() const;                         // 0x00686D20
};
struct CTCWaterWader : CTCBase {
    void IsInWater(bool& inWater, bool& deep, float& waterHeight) const;
    __forceinline float GetDistanceSquaredFromReferencePoint() const
    {
        CWorld* world = GetWorld();
        const CThing* parent = ParentThing;
        return (parent->GetPosition() - world->GetReferencePoint()).GetLengthSquared();
    }
};

void CTCWaterWader::IsInWater(bool& inWater, bool& deep, float& waterHeight) const
{
    inWater = false;
    deep = false;
    waterHeight = 0.0f;
    if (!GetWorld()->GetModeObject()->IsAlwaysChecking())
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

static C3DVector g_pos(10.0f, 0.0f, 5.0f);
static C3DVector g_ref(0.0f, 0.0f, 5.0f);
static bool g_always = false, g_rendered = true;
static long g_query = 0, g_kind = 3;
static float g_water = 6.0f, g_ground = 5.5f;
const C3DVector& CThing::GetPosition() const { return g_pos; }
long CThing::GetOwnerQuery() const { return g_query; }
STCEntry* CTCMap::LowerBound(const unsigned long& kind) const
{
    STCEntry* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it;
    return it;
}
bool CTCGraphicAppearance::WasRenderedLastFrame() const { return g_rendered; }
void CKindObject::v00() {}
long CKindObject::GetKind() const { return g_kind; }
static CKindObject g_kindObject;
CKindObject* CTC31::GetKindObject() const { return &g_kindObject; }
bool CWorldModeObject::IsAlwaysChecking() const { return g_always; }
C3DVector CWorld::GetReferencePoint() const { return g_ref; }
float CWorldMap::GetWaterSizeZAt(const C3DVector&) const { return g_water; }
float CWorldMap::GetGroundSizeZAt(const C3DVector&) const { return g_ground; }
static CWorldModeObject g_mode;
static CWorld g_world;
static CWorldMap g_map;
CWorld* CTCBase::GetWorld() const { return &g_world; }
CWorldMap* CTCBase::GetWorldMap() const { return &g_map; }
void CTCBase::v00() {}
static int g_tc31;
static STCEntry g_entries[2];
static CThing g_thing;
static CTCGraphicAppearance g_appearance;
static bool Run(bool& a, bool& b, float& h)
{
    CTCWaterWader w;
    w.ParentThing = &g_thing;
    a = true; b = true; h = -1.0f;
    w.IsInWater(a, b, h);
    return true;
}
int main()
{
    g_world.PModeObject = &g_mode;
    g_thing.PGraphicAppearance = &g_appearance;
    for (int i = 0; i < 8; ++i) g_thing.m_tcMask[i] = 0;
    g_thing.m_tcMask[1] = 0x20000;
    g_entries[0].m_kind = 0x31; g_entries[0].m_pTC = (CTCBase*)&g_tc31;
    g_entries[1].m_kind = 0x7fffffff; g_entries[1].m_pTC = 0;
    g_thing.m_tcs.m_pFirst = &g_entries[0]; g_thing.m_tcs.m_pLast = &g_entries[1]; g_thing.m_tcs.m_pEnd = &g_entries[1];
    bool a, b; float h; bool ok = true;
    // in range (10 units), rendered: water 6 over ground 5.5 (0.5 > 0.1), 1.0 above the feet (> 0.7)
    Run(a, b, h); ok = ok && a && b && h == 6.0f;
    // too far from the reference point and not forced: outputs cleared only
    g_pos.X = 30.0f; Run(a, b, h); ok = ok && !a && !b && h == 0.0f;
    // forced check ignores the distance
    g_always = true; Run(a, b, h); ok = ok && a && b && h == 6.0f;
    // shallow water: 0.05 over the ground, 0.5 above the feet
    g_water = 5.55f; Run(a, b, h); ok = ok && !a && !b && h == 5.55f;
    // water height at or below 0.05 stops before the comparisons
    g_water = 0.04f; Run(a, b, h); ok = ok && !a && !b && h == 0.04f;
    // kind 0x1A and the parent query both skip the water read
    g_water = 6.0f; g_kind = 0x1a; Run(a, b, h); ok = ok && !a && h == 0.0f;
    g_kind = 3; g_query = 1; Run(a, b, h); ok = ok && !a && h == 0.0f;
    // no 0x31 component
    g_query = 0; g_thing.m_tcMask[1] = 0; Run(a, b, h); ok = ok && !a && h == 0.0f;
    // not rendered and not forced
    g_thing.m_tcMask[1] = 0x20000; g_always = false; g_pos.X = 1.0f; g_rendered = false; Run(a, b, h); ok = ok && !a && h == 0.0f;
    if (!ok) { std::printf("007b5ca0_TEST FAIL\n"); return 1; }
    std::printf("007b5ca0_TEST PASS\n");
    return 0;
}