// Behaviour test for 0075daa0: src/0075daa0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCCameraPointBuilding::IsLeafCriteriaMet()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCameraPointBuilding::IsLeafCriteriaMet() -- a building camera point applies when the observed thing stands in this point's building and the camera tools see the hero from the thing's focal point (entry/exit points never apply)
// Leaf: IsLeafCriteriaMet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075DAA0, length 229 bytes (+ int3 padding).
// Callees: CTCBase::GetWorld() const @ 0x00686A80, CGameCameraManager::GetCameraManager(CCameraManager**) (virtual slot 10, +0x28;
// world +0x34), CTCCameraPointBuilding::GetOwnerBuilding @ 0x0075D940, NCameraModes::CBase::GetObservedThing() const @ 0x0084F1B0 (the
// camera manager's current mode, +0x98), CTCGraphicAppearance::GetCameraFocalPos(float) @ 0x004C12D0, CThing::GetPosition
// @ 0x004C73D0, GetPBuildingThingOccupying(CThing*) @ 0x006989D0 (__fastcall), CTools::IsLineOfSightFiltered @ 0x00877C40 (the
// camera manager's tools at +0x78; 8 arguments).
// Layout = Ego_r.pdb CTCCameraPointBuilding: InMyBuilding +0x40, ExitPoint +0x41, StartPoint +0x42.
// Shape: nested tests sharing one false exit; the observed thing missing clears InMyBuilding; the hero position is copied into
// a local; the owner-building out pointer and the cleared line-of-sight hit-thing out are two locals that share one frame
// word. That sharing needs the same-TU GetOwnerBuilding body (0x0075D940 stand-in: writes the out pointer only on success,
// the lookup itself behind an extern), which tells VC7.1 the owner's address does not escape; with only a declaration the
// hit-thing gets its own word, and one shared variable swaps the camera/owner words.
class C3DVector { public: float X, Y, Z; };
class CThing;
class CThingBuilding;
struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float height) const;   // 0x004C12D0
};
class CThing {
public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
const CThing* __fastcall GetPBuildingThingOccupying(CThing* thing);     // 0x006989D0 (the CThingBuilding)
struct CTools {
    bool IsLineOfSightFiltered(const C3DVector* camera, const C3DVector* pos, C3DVector* hitPos, C3DVector* hitNormal, long* hitIndex,
                               const CThing*& hitThing, long filter, bool flag);    // 0x00877C40
};
namespace NCameraModes {
class CBase { public:
    CThing* GetObservedThing() const;           // 0x0084F1B0
};
}
struct CCameraManager {
    unsigned char _pad00[0x78];
    CTools Tools;                               // +0x78
    unsigned char _pad7c[0x98 - 0x7c];
    NCameraModes::CBase* PCurrentMode;          // +0x98
};
class CGameCameraManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool GetCameraManager(CCameraManager** out);   // slot 10 (+0x28) 0x0069E1D0
};
class CWorld { public:
    unsigned char _pad00[0x34];
    CGameCameraManager* PCameraManager;         // +0x34
    CGameCameraManager* GetCameraManager() const { return PCameraManager; }
};
class CTCCameraPointBuilding {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x40 - 0x08];
    bool    InMyBuilding;                       // +0x40
    bool    ExitPoint;                          // +0x41
    bool    StartPoint;                         // +0x42
    CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)
    bool GetOwnerBuilding(const CThing** building);     // 0x0075D940
    virtual bool IsLeafCriteriaMet();
};

const CThing* FindOwnerBuilding_0075d940(const CTCCameraPointBuilding* point);   // stand-in callee of the body below
#pragma auto_inline(off)
__declspec(noinline) bool CTCCameraPointBuilding::GetOwnerBuilding(const CThing** building)
{
    const CThing* owner = FindOwnerBuilding_0075d940(this);
    if (!owner)
        return false;
    *building = owner;
    return true;
}
#pragma auto_inline(on)
bool CTCCameraPointBuilding::IsLeafCriteriaMet()
{
    const CThing* thing;
    CCameraManager* camera;
    if (GetWorld()->GetCameraManager()->GetCameraManager(&camera) && GetOwnerBuilding(&thing)) {
        CThing* observed = camera->PCurrentMode->GetObservedThing();
        if (observed) {
            C3DVector focal = observed->PGraphicAppearance->GetCameraFocalPos(1.0f);
            C3DVector heroPosition = ParentThing->GetPosition();
            if (thing == GetPBuildingThingOccupying(observed)) {
                const CThing* hitThing = 0;
                if (!ExitPoint && !StartPoint) {
                    long hitIndex;
                    C3DVector hitNormal;
                    C3DVector hitPos;
                    if (camera->Tools.IsLineOfSightFiltered(&focal, &heroPosition, &hitPos, &hitNormal, &hitIndex, hitThing, 7, true))
                        return true;
                }
            }
        } else {
            InMyBuilding = false;
        }
    }
    return false;
}

// ---- stubs and driver ----
static CWorld g_world;
static CCameraManager g_camera;
static bool g_haveCamera = true;
static const CThing* g_owner = 0;
static CThing* g_observed = 0;
static const CThing* g_occupying = 0;
static bool g_los = true; static int g_losCalls = 0;
static const CThing* g_losHitIn = (const CThing*)1;
static C3DVector g_focal, g_hero, g_losCam, g_losPos; static float g_focalHeight = 0;
static long g_filter = 0; static bool g_flag = false;
CWorld* CTCCameraPointBuilding::GetWorld() const { return &g_world; }
void CTCCameraPointBuilding::v00() { }
const CThing* FindOwnerBuilding_0075d940(const CTCCameraPointBuilding*) { return g_owner; }
void CGameCameraManager::v00() { } void CGameCameraManager::v01() { } void CGameCameraManager::v02() { } void CGameCameraManager::v03() { }
void CGameCameraManager::v04() { } void CGameCameraManager::v05() { } void CGameCameraManager::v06() { } void CGameCameraManager::v07() { }
void CGameCameraManager::v08() { } void CGameCameraManager::v09() { }
bool CGameCameraManager::GetCameraManager(CCameraManager** out) { if (!g_haveCamera) return false; *out = &g_camera; return true; }
CThing* NCameraModes::CBase::GetObservedThing() const { return g_observed; }
C3DVector CTCGraphicAppearance::GetCameraFocalPos(float height) const { g_focalHeight = height; return g_focal; }
const C3DVector& CThing::GetPosition() const { return g_hero; }
const CThing* __fastcall GetPBuildingThingOccupying(CThing* thing) { CHECK(thing == g_observed); return g_occupying; }
bool CTools::IsLineOfSightFiltered(const C3DVector* camera, const C3DVector* pos, C3DVector* hitPos, C3DVector* hitNormal, long* hitIndex,
                                   const CThing*& hitThing, long filter, bool flag)
{ ++g_losCalls; CHECK(this == &g_camera.Tools); g_losCam = *camera; g_losPos = *pos; g_losHitIn = hitThing; g_filter = filter; g_flag = flag;
  CHECK(hitPos && hitNormal && hitIndex); return g_los; }
int main()
{
    static CGameCameraManager gcm; g_world.PCameraManager = &gcm;
    static NCameraModes::CBase mode; g_camera.PCurrentMode = &mode;
    static CThing parent, observed, building; static CTCGraphicAppearance app;
    observed.PGraphicAppearance = &app;
    static CTCCameraPointBuilding p; p.ParentThing = &parent; p.InMyBuilding = true; p.ExitPoint = false; p.StartPoint = false;
    g_focal.X = 1; g_focal.Y = 2; g_focal.Z = 3; g_hero.X = 4; g_hero.Y = 5; g_hero.Z = 6;
    // 1) all conditions met: line of sight asked from the focal point to the hero, hit-thing cleared, filter 7, flag true
    g_owner = &building; g_observed = &observed; g_occupying = &building;
    CHECK(p.IsLeafCriteriaMet() == true);
    CHECK(g_losCalls == 1 && g_losHitIn == 0 && g_filter == 7 && g_flag == true && g_focalHeight == 1.0f);
    CHECK(g_losCam.X == 1 && g_losCam.Z == 3 && g_losPos.X == 4 && g_losPos.Z == 6);
    // 2) line of sight fails
    g_los = false; CHECK(p.IsLeafCriteriaMet() == false); g_los = true;
    // 3) entry / exit points never apply
    p.ExitPoint = true; CHECK(p.IsLeafCriteriaMet() == false); p.ExitPoint = false;
    p.StartPoint = true; g_losCalls = 0; CHECK(p.IsLeafCriteriaMet() == false && g_losCalls == 0); p.StartPoint = false;
    // 4) observed thing in another building
    g_occupying = &parent; CHECK(p.IsLeafCriteriaMet() == false && g_losCalls == 0); g_occupying = &building;
    // 5) no observed thing: InMyBuilding cleared
    g_observed = 0; p.InMyBuilding = true; CHECK(p.IsLeafCriteriaMet() == false && p.InMyBuilding == false); g_observed = &observed;
    // 6) no owner building / no camera manager: false, InMyBuilding untouched
    p.InMyBuilding = true; g_owner = 0; CHECK(p.IsLeafCriteriaMet() == false && p.InMyBuilding == true); g_owner = &building;
    g_haveCamera = false; CHECK(p.IsLeafCriteriaMet() == false && p.InMyBuilding == true); g_haveCamera = true;
    CHECK(p.IsLeafCriteriaMet() == true);
    if (fails == 0) printf("0075daa0_TEST PASS\n");
    return fails;
}
