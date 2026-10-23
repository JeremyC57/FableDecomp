// Behaviour test for 00703590: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CThingObject::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&)
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static char g_log[2048];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CThingObject::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- after the CThingPhysical preparation: prepare the weapon trail (interface 0x4C, when trails are shown) and the two draw-preparing components (0x54, 0xCF); take the render position and orientation interpolators from the carry pose when the object is being carried (pushing them into the physics component and updating attached particles, 0xD8) or else from the physics component; then let the graphic appearance prepare itself with them
// Leaf: DrawPreparePrimitivesForRendering
// Callees: CThingPhysical::DrawPreparePrimitivesForRendering @ 0x006A64D0 (non-virtual), CVectorMap::LowerBound const @ 0x004365B0 /
//          non-const @ 0x0040F020, CTCWeaponTrail @ 0x0076E6A0, the components' slot 11 (+0x2c), CBase ctor @ 0x0099A2D0 /
//          dtor @ 0x0099A2E0, CTCCarryable::GetCarriedPositionInfo @ 0x00705B00, physics slots 31/32 (+0x7c/+0x80),
//          CTCParticleAttachmentTracker::UpdateAttachedParticles @ 0x0071A9C0, CTCPhysicsBase::GetPositionInterpolator @ 0x006B0250 /
//          GetOrientationInterpolator @ 0x006B0460, CTCGraphicAppearance::PrepareForRenderIfNeeded @ 0x004BE490; global 0x0137548E.
// Layout: retail CThing interface bitset +0x20, CVectorMap +0x44, PhysicsTC +0x60, PTCGraphicAppearance +0x64;
//          CEnginePrimitivePrepareInfo time +0x04; interpolators per Ego_r.pdb (vtables 0x0123981C / 0x01239824).
// Shape: the const lookups go through `const CThingObject* const& self = this;` -- a reference to the this pointer (the
//        pointer itself homed, then forwarded): VC7.1 then treats the 0x47 bit load as a may-alias of the frame-local
//        interpolators and keeps the orientation vtable store ahead of it (a plain `self = this` or `this` hoists the load).
struct CEnginePrimitivePrepareInfo {
    long  _f00;
    float Time;                                         // +0x04
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
namespace N_0099a2d0 { struct CBase {
    virtual ~CBase();                                   // 0x0099A2E0
    CBase();                                            // 0x0099A2D0
}; } using N_0099a2d0::CBase;
struct C3DPositionInterpolator : CBase {                // vtable 0x0123981C
    C3DVector StartPosition;                            // +0x04
    C3DVector EndPosition;                              // +0x10
};
struct C3DOrientationInterpolator : CBase {             // vtable 0x01239824
    CRightHandedSet StartOrientation;                   // +0x04
    CRightHandedSet EndOrientation;                     // +0x1c
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#define VF(n) virtual void v##n();
struct CTCDrawPreparer {
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10)
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // slot 11 (+0x2c)
};
struct CTCWeaponTrail {
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);           // 0x0076E6A0
};
struct CTCCarryable {
    unsigned char _pad00[0x10];
    void* Carrier;                                      // +0x10
    bool GetCarriedPositionInfo(C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation, float time);   // 0x00705B00
};
struct CTCParticleAttachmentTracker {
    void UpdateAttachedParticles(float time);           // 0x0071A9C0
};
struct CTCPhysicsBase {
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15)
    VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30)
    virtual void SetRenderPosition(const C3DVector* position);              // slot 31 (+0x7c)
    virtual void SetRenderOrientation(const CRightHandedSet* orientation);  // slot 32 (+0x80)
    void GetPositionInterpolator(C3DPositionInterpolator& out) const;       // 0x006B0250
    void GetOrientationInterpolator(C3DOrientationInterpolator& out) const; // 0x006B0460
};
struct CTCGraphicAppearance {
    void PrepareForRenderIfNeeded(const CEnginePrimitivePrepareInfo& info, C3DPositionInterpolator* position,
                                  C3DOrientationInterpolator* orientation);  // 0x004BE490
};
extern bool g_ShowWeaponTrails;                         // 0x0137548E

struct CThingPhysical {
    VF(00)
    unsigned char         _pad04[0x1c];
    unsigned long         UsingComponents[9];           // +0x20
    CInterfaceMap         Components;                   // +0x44
    unsigned char         _pad4c[0x60 - 0x4c];
    CTCPhysicsBase*       PhysicsTC;                    // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006A64D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PickInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PickInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingObject : CThingPhysical {
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

bool CThingObject::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
        const CThingObject* const& self = this;
        CTCWeaponTrail* trail;
        if (g_ShowWeaponTrails && self->GetInterface(&trail, 0x4c))
            trail->DrawPreparePrimitivesForRendering(info);
        CTCDrawPreparer* preparer;
        if (PickInterface(&preparer, 0x54))
            preparer->DrawPreparePrimitivesForRendering(info);
        CTCDrawPreparer* preparer2;
        if (PickInterface(&preparer2, 0xcf))
            preparer2->DrawPreparePrimitivesForRendering(info);
        C3DPositionInterpolator position;
        C3DOrientationInterpolator orientation;
        CTCCarryable* carryable;
        if (self->PickInterface(&carryable, 0x47) && carryable->Carrier) {
            carryable->GetCarriedPositionInfo(&position, &orientation, info.Time);
            PhysicsTC->SetRenderPosition(&position.EndPosition);
            PhysicsTC->SetRenderOrientation(&orientation.EndOrientation);
            CTCParticleAttachmentTracker* tracker;
            if (GetInterface(&tracker, 0xd8))
                tracker->UpdateAttachedParticles(info.Time);
        } else {
            PhysicsTC->GetPositionInterpolator(position);
            PhysicsTC->GetOrientationInterpolator(orientation);
        }
        if (PTCGraphicAppearance)
            PTCGraphicAppearance->PrepareForRenderIfNeeded(info, &position, &orientation);
        return true;
    }
    return false;
}

// ---- stubs ----
bool g_ShowWeaponTrails = false;
static bool g_baseResult = true;
static C3DPositionInterpolator* g_lastPos = 0;
static C3DOrientationInterpolator* g_lastOri = 0;
static float g_lastTime = 0.0f;
static const C3DVector* g_renderPos = 0;
static const CRightHandedSet* g_renderOri = 0;
static int g_ctor = 0, g_dtor = 0;
CBase::CBase() { ++g_ctor; }
CBase::~CBase() { ++g_dtor; }
bool CThingPhysical::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { note("base"); (void)info; return g_baseResult; }
void CThingPhysical::v00() {}
// one linear search serves both map overloads
static CInterfaceMap::SEntry* Find(CInterfaceMap* m, long type)
{
    CInterfaceMap::SEntry* it = m->first;
    while (it != m->last && it->type < type) ++it;
    return it;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { note("lb"); return Find(this, *type); }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { note("lbc"); return Find((CInterfaceMap*)this, *type); }
void CTCWeaponTrail::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { char b[64]; _snprintf(b, sizeof b, "trail%.1f", info.Time); note(b); }
#define VD(C,n) void C::v##n() {}
VD(CTCDrawPreparer,00) VD(CTCDrawPreparer,01) VD(CTCDrawPreparer,02) VD(CTCDrawPreparer,03) VD(CTCDrawPreparer,04) VD(CTCDrawPreparer,05)
VD(CTCDrawPreparer,06) VD(CTCDrawPreparer,07) VD(CTCDrawPreparer,08) VD(CTCDrawPreparer,09) VD(CTCDrawPreparer,10)
void CTCDrawPreparer::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { char b[64]; _snprintf(b, sizeof b, "prep%p", (void*)this); (void)info; note(b); }
bool CTCCarryable::GetCarriedPositionInfo(C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation, float time)
{
    note("carried"); g_lastPos = position; g_lastOri = orientation; g_lastTime = time;
    position->EndPosition.X = 7.0f; orientation->EndOrientation.Up.Y = 9.0f;
    return true;
}
void CTCParticleAttachmentTracker::UpdateAttachedParticles(float time) { char b[64]; _snprintf(b, sizeof b, "particles%.1f", time); note(b); }
VD(CTCPhysicsBase,00) VD(CTCPhysicsBase,01) VD(CTCPhysicsBase,02) VD(CTCPhysicsBase,03) VD(CTCPhysicsBase,04) VD(CTCPhysicsBase,05)
VD(CTCPhysicsBase,06) VD(CTCPhysicsBase,07) VD(CTCPhysicsBase,08) VD(CTCPhysicsBase,09) VD(CTCPhysicsBase,10) VD(CTCPhysicsBase,11)
VD(CTCPhysicsBase,12) VD(CTCPhysicsBase,13) VD(CTCPhysicsBase,14) VD(CTCPhysicsBase,15) VD(CTCPhysicsBase,16) VD(CTCPhysicsBase,17)
VD(CTCPhysicsBase,18) VD(CTCPhysicsBase,19) VD(CTCPhysicsBase,20) VD(CTCPhysicsBase,21) VD(CTCPhysicsBase,22) VD(CTCPhysicsBase,23)
VD(CTCPhysicsBase,24) VD(CTCPhysicsBase,25) VD(CTCPhysicsBase,26) VD(CTCPhysicsBase,27) VD(CTCPhysicsBase,28) VD(CTCPhysicsBase,29)
VD(CTCPhysicsBase,30)
void CTCPhysicsBase::SetRenderPosition(const C3DVector* position) { note("setpos"); g_renderPos = position; }
void CTCPhysicsBase::SetRenderOrientation(const CRightHandedSet* orientation) { note("setori"); g_renderOri = orientation; }
void CTCPhysicsBase::GetPositionInterpolator(C3DPositionInterpolator& out) const { note("getpos"); out.StartPosition.Z = 3.0f; g_lastPos = &out; }
void CTCPhysicsBase::GetOrientationInterpolator(C3DOrientationInterpolator& out) const { note("getori"); out.StartOrientation.Forward.X = 4.0f; g_lastOri = &out; }
void CTCGraphicAppearance::PrepareForRenderIfNeeded(const CEnginePrimitivePrepareInfo& info, C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation)
{
    char b[96]; _snprintf(b, sizeof b, "render%.1f/%s/%s", info.Time, position == g_lastPos ? "pos" : "?", orientation == g_lastOri ? "ori" : "?"); note(b);
}

struct TestThing : CThingObject {
    CInterfaceMap::SEntry entries[8];
    TestThing() { memset(UsingComponents, 0, sizeof UsingComponents); Components.first = entries; Components.last = entries; PhysicsTC = 0; PTCGraphicAppearance = 0; }
    void Add(long type, void* comp)
    {
        CInterfaceMap::SEntry* it = Components.first;
        while (it != Components.last && it->type < type) ++it;
        for (CInterfaceMap::SEntry* p = Components.last; p != it; --p) *p = *(p - 1);
        it->type = type; it->component = (CTCBase*)comp; ++Components.last;
        UsingComponents[type >> 5] |= 1UL << (type & 31);
    }
};
static void reset() { g_log[0] = 0; g_lastPos = 0; g_lastOri = 0; g_renderPos = 0; g_renderOri = 0; g_ctor = g_dtor = 0; }

int main()
{
    CEnginePrimitivePrepareInfo info; info._f00 = 0; info.Time = 2.5f;
    CTCWeaponTrail trail; CTCDrawPreparer prep1, prep2; CTCCarryable carry; memset(&carry, 0, sizeof carry);
    CTCParticleAttachmentTracker tracker; CTCPhysicsBase physics; CTCGraphicAppearance appearance;
    char expect[256];

    // 1. base preparation fails: nothing else happens
    { TestThing t; reset(); g_baseResult = false; CHECK(t.DrawPreparePrimitivesForRendering(info) == false); CHECK(strcmp(g_log, "base;") == 0); CHECK(g_ctor == 0 && g_dtor == 0); }
    g_baseResult = true;

    // 2. no components, no appearance: both interpolators come from the physics component
    { TestThing t; reset(); t.PhysicsTC = &physics;
      CHECK(t.DrawPreparePrimitivesForRendering(info) == true);
      CHECK(strcmp(g_log, "base;getpos;getori;") == 0); CHECK(g_ctor == 2 && g_dtor == 2); }

    // 3. trails shown but no trail component; preparers 0x54 and 0xcf present; carryable present but not carried; appearance
    { TestThing t; reset(); g_ShowWeaponTrails = true; t.PhysicsTC = &physics; t.PTCGraphicAppearance = &appearance;
      t.Add(0x54, &prep1); t.Add(0xcf, &prep2); t.Add(0x47, &carry);
      CHECK(t.DrawPreparePrimitivesForRendering(info) == true);
      _snprintf(expect, sizeof expect, "base;lb;prep%p;lb;prep%p;lbc;getpos;getori;render2.5/pos/ori;", (void*)&prep1, (void*)&prep2);
      CHECK(strcmp(g_log, expect) == 0); CHECK(g_ctor == 2 && g_dtor == 2); }

    // 4. trail component with trails shown, carried by someone, particle tracker attached
    { TestThing t; reset(); g_ShowWeaponTrails = true; t.PhysicsTC = &physics; t.PTCGraphicAppearance = &appearance;
      carry.Carrier = &t; t.Add(0x4c, &trail); t.Add(0x47, &carry); t.Add(0xd8, &tracker);
      CHECK(t.DrawPreparePrimitivesForRendering(info) == true);
      CHECK(strcmp(g_log, "base;lbc;trail2.5;lbc;carried;setpos;setori;lb;particles2.5;render2.5/pos/ori;") == 0);
      CHECK(g_lastTime == 2.5f); CHECK(g_renderPos == &g_lastPos->EndPosition); CHECK(g_renderOri == &g_lastOri->EndOrientation);
      CHECK(g_ctor == 2 && g_dtor == 2); }

    // 5. trails hidden: the trail component is not consulted; carried without a particle tracker
    { TestThing t; reset(); g_ShowWeaponTrails = false; t.PhysicsTC = &physics;
      carry.Carrier = &t; t.Add(0x4c, &trail); t.Add(0x47, &carry);
      CHECK(t.DrawPreparePrimitivesForRendering(info) == true);
      CHECK(strcmp(g_log, "base;lbc;carried;setpos;setori;") == 0); }

    if (fails == 0) printf("00703590_TEST PASS\n"); else printf("00703590_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}