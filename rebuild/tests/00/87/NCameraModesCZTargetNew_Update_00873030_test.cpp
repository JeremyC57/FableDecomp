// Behaviour test for retail 0x00873030: candidate source embedded verbatim, then stubs and a main.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
// NCameraModes::CZTargetNew::Update() -- refresh the coordinate system, place the camera, look at the observed thing's focal point and pull the look-at towards a live target's firing point
// Leaf: ?Update@CZTargetNew
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873030, 293 bytes (+ int3 padding), plain `ret`.
// Callees: virtual slot 24 UpdateCoordSystem (+0x60, 0x00872EC0), CZTargetNew::GetCameraPosition() 0x00872AF0 (C3DVector
//   by hidden pointer), NCameraModes::CBase::GetObservedThing() const 0x0084F1B0, CTCGraphicAppearance::GetCameraFocalPos(float)
//   0x004C12D0 (hidden pointer), the const CVectorMap::LowerBound 0x004365B0 (interface lookup, key 8 = the targeting TC),
//   the targeting component's virtual slot 36 (+0x90) GetTargetedThing, NCameraModes::CBase::GetWorld() const 0x0084D050,
//   NTargeting::GetTargetingPositionOnThing 0x007EA2F0 (`__fastcall`, hidden result in ecx, world in edx, as the landed
//   CProjectileWeapon::GetPhiForTargetedThing 0x008798C0).
// Layout (retail NCameraModes::CBase is 0xA4 bytes, PDB 0xC8; see the landed CZTargetNew::OnInit 0x00872A20): LookAt
//   CPhysicsState +0x10 (position first), CameraManager +0x40, PCameraModeDef +0x44 (the def's float at +0x68 is the
//   look-at blend towards the target), GoTo CPhysicsState +0x54 (position first). CThing: TC bit mask +0x20, interface
//   map +0x44, graphic appearance TC +0x64, dead bit +0x91 bit 0.
// Shape: the appearance is reached through an inline CThing getter in the call chain (a named pointer local or the bare
//   member chain moves the load to eax or below the pushes); HasTC is a forced-inline `unsigned long` mask test (`test ch,1`); the component is the one-ternary entry pick over a template constant key
//   loaded straight into ecx; the blend is a named float read first, then `lookAt += (aim - lookAt) * blend` through inline C3DVector operators; the two
//   positions are stored through the implicit operator= (`lea ecx,[esi+54h]` copy, then `add esi,10h`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};

class CThing;
class CWorld;

class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual const CThing* GetTargetedThing() const;     // slot 36 (+0x90)
};

struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float heightFraction) const;    // 0x004C12D0
};

struct SInterfaceEntry { long type; CTCBase* component; };
struct CInterfaceMap {                                  // CVectorMap<long, CTCBase*>, 12 bytes
    SInterfaceEntry* _M_start; SInterfaceEntry* _M_finish; SInterfaceEntry* _M_end_of_storage;
    const SInterfaceEntry* LowerBound(const long& key) const;   // 0x004365B0
    const SInterfaceEntry* end() const { return _M_finish; }
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long TCFlags;                              // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad50[0x14];
    CTCGraphicAppearance* PGraphicAppearance;           // +0x64
    unsigned char _pad68[0x29];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const CTCGraphicAppearance* GetGraphicAppearance() const { return PGraphicAppearance; }
    __forceinline unsigned long HasTC(long type) const { return TCFlags & (1 << type); }
    template<long TYPE> __forceinline CTCBase* GetTC() const
    {
        long type = TYPE;
        const CInterfaceMap* map = &Interfaces;
        const SInterfaceEntry* it = map->LowerBound(type);
        const SInterfaceEntry* last = map->end();
        return (it == last || TYPE < it->type ? last : it)->component;
    }
};

template<class T> inline bool IS_THING_ALIVE(T* thing) { return thing != 0 && !thing->Dead; }

namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}

class CCameraModeDef { public:
    unsigned char _pad[0x68];
    float ZTargetLookAtTargetOffset;                    // +0x68 (Ego_r.pdb +0x6C; retail defs sit 4 below)
};
struct CPhysicsState {                                  // retail 0x30 bytes
    C3DVector Position;                                 // +0x00
    unsigned char _rest[0x24];
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void UpdateCoordSystem();                   // slot 24 (+0x60)
    float DefaultFOV;                                   // +0x04
    float DefaultRollAngle;                             // +0x08
    float DefaultObservedPositionZOffset;               // +0x0c
    CPhysicsState LookAt;                               // +0x10
    void* CameraManager;                                // +0x40
    const CCameraModeDef* PCameraModeDef;               // +0x44 (CDefPointer data)
    void* PCameraModeDefInfo_SubComponents[3];          // +0x48 SubComponents vector
    CPhysicsState GoTo;                                 // +0x54
    unsigned char _pad84[0x20];
    const CThing* GetObservedThing() const;             // 0x0084F1B0
    const CWorld& GetWorld() const;                     // 0x0084D050
};
class CZTargetNew : public CBase {
public:
    C3DVector GetCameraPosition();                      // 0x00872AF0
    void Update();
};
}

void NCameraModes::CZTargetNew::Update()
{
    UpdateCoordSystem();
    C3DVector position = GetCameraPosition();
    C3DVector lookAt = GetObservedThing()->GetGraphicAppearance()->GetCameraFocalPos(1.0f);
    const CThing* thing = GetObservedThing();
    if (thing->HasTC(8)) {
        const CThing* target = thing->GetTC<8>()->GetTargetedThing();
        if (IS_THING_ALIVE(target)) {
            C3DVector aim = NTargeting::GetTargetingPositionOnThing(GetWorld(), *target, 1.0f);
            float blend = PCameraModeDef->ZTargetLookAtTargetOffset;
            lookAt += (aim - lookAt) * blend;
        }
    }
    GoTo.Position = position;
    LookAt.Position = lookAt;
}

// ===== harness =====
class CWorld { public: int id; };
static CWorld g_world;
static int g_coordUpdates;
#define V(n) void CTCBase::v##n() {}
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17)
V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35)
#undef V
const CThing* CTCBase::GetTargetedThing() const { return 0; }
#define V(n) void NCameraModes::CBase::v##n() {}
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17)
V(18) V(19) V(20) V(21) V(22) V(23)
#undef V
void NCameraModes::CBase::UpdateCoordSystem() { ++g_coordUpdates; }
struct CTargetingTC : CTCBase { const CThing* Target; const CThing* GetTargetedThing() const { return Target; } };
static const CThing* g_observed;
const CThing* NCameraModes::CBase::GetObservedThing() const { return g_observed; }
const CWorld& NCameraModes::CBase::GetWorld() const { return g_world; }
C3DVector NCameraModes::CZTargetNew::GetCameraPosition() { return C3DVector(5.0f, 6.0f, 7.0f); }
C3DVector CTCGraphicAppearance::GetCameraFocalPos(float h) const { return C3DVector(1.0f * h, 2.0f * h, 3.0f * h); }
const SInterfaceEntry* CInterfaceMap::LowerBound(const long& key) const
{ const SInterfaceEntry* p = _M_start; while (p != _M_finish && p->type < key) ++p; return p; }
static const CThing* g_aimedAt; static const CWorld* g_aimWorld;
C3DVector __fastcall NTargeting::GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height)
{ g_aimWorld = &world; g_aimedAt = &thing; return C3DVector(10.0f * height, 20.0f, 30.0f); }
static int Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
static int Is(const C3DVector& v, float x, float y, float z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }
int main()
{
    static CThing observed, target; static CTCGraphicAppearance appearance; static CCameraModeDef def;
    CTargetingTC tc; tc.Target = &target;
    SInterfaceEntry entries[2]; entries[0].type = 3; entries[0].component = 0; entries[1].type = 8; entries[1].component = &tc;
    observed.PGraphicAppearance = &appearance; observed.Interfaces._M_start = entries; observed.Interfaces._M_finish = entries + 2;
    def.ZTargetLookAtTargetOffset = 0.5f;
    NCameraModes::CZTargetNew mode; memset((char*)&mode.LookAt, 0, sizeof mode.LookAt); mode.PCameraModeDef = &def;
    g_observed = &observed;
    int ok = 1;
    observed.TCFlags = 0;                                  /* no targeting TC: plain focal point */
    mode.Update();
    ok = ok && g_coordUpdates == 1 && Is(mode.GoTo.Position, 5, 6, 7) && Is(mode.LookAt.Position, 1, 2, 3) && g_aimedAt == 0;
    observed.TCFlags = 1 << 8; target.Dead = 1;            /* dead target: ignored */
    mode.Update();
    ok = ok && Is(mode.LookAt.Position, 1, 2, 3) && g_aimedAt == 0;
    target.Dead = 0;                                       /* live target: halfway to its firing point */
    mode.Update();
    ok = ok && Is(mode.LookAt.Position, 5.5f, 11.0f, 16.5f) && g_aimedAt == &target && g_aimWorld == &g_world;
    tc.Target = 0; g_aimedAt = 0;                          /* nothing targeted */
    mode.Update();
    ok = ok && Is(mode.LookAt.Position, 1, 2, 3) && g_aimedAt == 0 && g_coordUpdates == 4;
    if (!ok) { printf("00873030_TEST FAIL\n"); return 1; }
    printf("00873030_TEST PASS\n");
    return 0;
}