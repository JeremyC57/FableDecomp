#include <stdio.h>
#include <string.h>
// CTCLightningOrb::FrameUpdate() -- move the orb along its spline (eased); when it reaches the end, outlives LifeTime or enters the target's bounding sphere it explodes (when the def names an explosion) and the orb thing is killed
// Shape: early `if (!PDef) return;` and the explode/kill and spline-overrun paths each `Kill(true); return;` (VC7.1 cross-jumps
//   the two Kill calls; the early-out joins the final SetAsMoving epilogue as retail); ONE function-scope C3DVector serves as
//   the hit-test sphere centre and the new spline position (same frame slot as retail), and the CVectorMap LowerBound
//   instantiation is defined in this TU (noinline).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
    void SetAsMoving(bool moving);                      // 0x006B0130
};
class CThing {
public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                   // +0x20 interface-present bitset
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCHitLocations {                                // interface 0x7F
    void GetBoundingSphere(C3DVector* centre, float* radius, bool flag);   // 0x00815420
};
CThing* __fastcall CreateExplosion(long defIndex, const C3DVector* position);   // 0x0080C460

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* PItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
namespace NBBBMaths {
struct CLinearSpline {
    C3DVector GetPointWithParam(float t);               // 0x00A57F70
};
}
struct CLightningOrbDef {
    unsigned char _pad00[0x28];
    long  LightningDefIndex;                            // +0x28
    float LifeTime;                                     // +0x2c
    float Speed;                                        // +0x30
    float ExplosionRange;                               // +0x34
    long  ExplosionDefIndex;                            // +0x38
};
struct CTCLightningOrb {
    void*                    __vftable;                 // +0x00
    CThing*                  ParentThing;               // +0x04
    unsigned char            _pad08[0x0c - 0x08];
    CBaseIntelligentPointer  PParent;                   // +0x0c
    CBaseIntelligentPointer  PTarget;                   // +0x14
    const CLightningOrbDef*  PDef;                      // +0x1c
    long                     Timer;                     // +0x20
    long                     LifeTime;                  // +0x24
    long                     NumFramesForSplineTraversal;   // +0x28
    NBBBMaths::CLinearSpline* PSpline;                  // +0x2c (CCountedPointer data)
    void*                    PSplineBlock;              // +0x30
    void FrameUpdate();
};

void CTCLightningOrb::FrameUpdate()
{
    if (!PDef)
        return;
    bool hit = false;
    CTCHitLocations* hitLocations;
    C3DVector centre;
    float radius;
    if (IS_THING_ALIVE(PTarget) && PTarget.GetPItem()->GetInterface(&hitLocations, 0x7f)) {
        float range = PDef->ExplosionRange;
        hitLocations->GetBoundingSphere(&centre, &radius, true);
        if ((centre - ParentThing->GetPosition()).GetLengthSquared() < range * range)
            hit = true;
    }
    if ((LifeTime >= 0 && Timer > LifeTime) || hit) {
        const CLightningOrbDef* def = PDef;
        if (def->ExplosionDefIndex > 0)
            CreateExplosion(def->ExplosionDefIndex, &ParentThing->GetPosition());
        ParentThing->Kill(true);
        return;
    }
    ++Timer;
    float t = (float)Timer / NumFramesForSplineTraversal;
    t = (t * t + (t + t)) * 0.33333334f;
    if (t > 1.0f) {
        ParentThing->Kill(true);
        return;
    }
    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;
    centre = PSpline->GetPointWithParam(t);
    ParentThing->PhysicsTC->SetPosition(centre);
    ParentThing->PhysicsTC->SetAsMoving(true);
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static C3DVector g_parentPos(0.0f, 0.0f, 0.0f), g_centre(0.0f, 0.0f, 0.0f), g_setPos(0.0f, 0.0f, 0.0f);
static int g_kills, g_killImm, g_explosions, g_explIdx, g_spheres, g_setPosCount, g_moving, g_movingArg;
static const C3DVector* g_explPos; static CThing* g_killThis; static float g_t;
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
const C3DVector& CThing::GetPosition() const { return g_parentPos; }
void CThing::Kill(bool immediately) { g_kills++; g_killImm = immediately; g_killThis = this; }
void CTCHitLocations::GetBoundingSphere(C3DVector* centre, float* radius, bool flag) { g_spheres++; *centre = g_centre; *radius = 1.0f; (void)flag; }
CThing* __fastcall CreateExplosion(long defIndex, const C3DVector* position) { g_explosions++; g_explIdx = defIndex; g_explPos = position; return 0; }
C3DVector NBBBMaths::CLinearSpline::GetPointWithParam(float t) { g_t = t; return C3DVector(t, 2.0f * t, 3.0f * t); }
void CTCPhysicsBase::v00() {} void CTCPhysicsBase::v01() {} void CTCPhysicsBase::v02() {} void CTCPhysicsBase::v03() {} void CTCPhysicsBase::v04() {}
void CTCPhysicsBase::v05() {} void CTCPhysicsBase::v06() {} void CTCPhysicsBase::v07() {} void CTCPhysicsBase::v08() {} void CTCPhysicsBase::v09() {}
void CTCPhysicsBase::v10() {} void CTCPhysicsBase::v11() {} void CTCPhysicsBase::v12() {} void CTCPhysicsBase::v13() {} void CTCPhysicsBase::v14() {}
void CTCPhysicsBase::v15() {} void CTCPhysicsBase::v16() {} void CTCPhysicsBase::v17() {} void CTCPhysicsBase::v18() {} void CTCPhysicsBase::v19() {}
void CTCPhysicsBase::v20() {} void CTCPhysicsBase::v21() {} void CTCPhysicsBase::v22() {} void CTCPhysicsBase::v23() {} void CTCPhysicsBase::v24() {}
void CTCPhysicsBase::v25() {} void CTCPhysicsBase::v26() {} void CTCPhysicsBase::v27() {} void CTCPhysicsBase::v28() {} void CTCPhysicsBase::v29() {}
void CTCPhysicsBase::SetPosition(const C3DVector& position) { g_setPosCount++; g_setPos = position; }
void CTCPhysicsBase::SetAsMoving(bool moving) { g_moving++; g_movingArg = moving; }
static void Reset() { g_kills = g_killImm = g_explosions = g_explIdx = g_spheres = g_setPosCount = g_moving = g_movingArg = 0; g_explPos = 0; g_killThis = 0; g_t = -99.0f; }
static unsigned char g_parentBuf[0x100], g_targetBuf[0x100], g_orbBuf[sizeof(CTCLightningOrb)];
static int Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
int main()
{
    CThing* parent = (CThing*)g_parentBuf; CThing* target = (CThing*)g_targetBuf;
    static CTCPhysicsBase physics; parent->PhysicsTC = &physics;
    static CTCHitLocations hitLocs;
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0x7f, (CTCBase*)&hitLocs } };
    target->m_presentBits[0x7f >> 5] |= 1UL << (0x7f & 31); target->m_interfaces.first = e; target->m_interfaces.last = e + 2;
    static CLightningOrbDef def; def.ExplosionRange = 5.0f; def.ExplosionDefIndex = 7;
    static NBBBMaths::CLinearSpline spline;
    CTCLightningOrb* orb = (CTCLightningOrb*)g_orbBuf;
    orb->ParentThing = parent; orb->PTarget.PItem = target; orb->PDef = &def; orb->LifeTime = -1;
    orb->NumFramesForSplineTraversal = 4; orb->PSpline = &spline;
    g_parentPos = C3DVector(0.0f, 0.0f, 0.0f);
    // 1: no def -> nothing
    orb->PDef = 0; orb->Timer = 0; Reset(); orb->FrameUpdate();
    CHECK(g_kills == 0 && g_spheres == 0 && g_setPosCount == 0 && orb->Timer == 0);
    orb->PDef = &def;
    // 2: target far away -> advance along the spline: Timer 0 -> 1, t = 1/4 eased (t*t + 2t)/3
    g_centre = C3DVector(10.0f, 0.0f, 0.0f); orb->Timer = 0; Reset(); orb->FrameUpdate();
    CHECK(orb->Timer == 1 && g_spheres == 1 && g_kills == 0 && g_explosions == 0);
    CHECK(Near(g_t, (0.0625f + 0.5f) * 0.33333334f) && g_setPosCount == 1 && Near(g_setPos.Y, 2.0f * g_t) && Near(g_setPos.Z, 3.0f * g_t));
    CHECK(g_moving == 1 && g_movingArg == 1);
    // 3: target inside the range -> explode at the parent's position and kill
    g_centre = C3DVector(3.0f, 0.0f, 0.0f); orb->Timer = 1; Reset(); orb->FrameUpdate();
    CHECK(g_explosions == 1 && g_explIdx == 7 && g_explPos == &g_parentPos && g_kills == 1 && g_killImm == 1 && g_killThis == parent);
    CHECK(orb->Timer == 1 && g_setPosCount == 0 && g_moving == 0);
    // 4: hit but the def names no explosion -> kill only
    def.ExplosionDefIndex = 0; Reset(); orb->FrameUpdate();
    CHECK(g_explosions == 0 && g_kills == 1);
    def.ExplosionDefIndex = 7;
    // 5: outlived LifeTime (target dead -> no sphere test)
    target->m_flags91 = 1; orb->LifeTime = 3; orb->Timer = 4; Reset(); orb->FrameUpdate();
    CHECK(g_spheres == 0 && g_explosions == 1 && g_kills == 1 && orb->Timer == 4);
    // 6: Timer == LifeTime is not over; spline overrun (t > 1) kills without exploding
    orb->LifeTime = 4; orb->Timer = 4; Reset(); orb->FrameUpdate();
    CHECK(orb->Timer == 5 && g_kills == 1 && g_explosions == 0 && g_setPosCount == 0);
    // 7: exactly at the end (t == 1) still moves
    orb->LifeTime = -1; orb->Timer = 3; Reset(); orb->FrameUpdate();
    CHECK(orb->Timer == 4 && g_kills == 0 && Near(g_t, 1.0f) && g_setPosCount == 1 && g_moving == 1);
    // 8: no target at all
    orb->PTarget.PItem = 0; orb->Timer = 0; Reset(); orb->FrameUpdate();
    CHECK(g_spheres == 0 && g_setPosCount == 1 && orb->Timer == 1);
    if (fails == 0) printf("007f24e0_TEST PASS\n"); else printf("007f24e0_TEST FAIL\n");
    return fails;
}