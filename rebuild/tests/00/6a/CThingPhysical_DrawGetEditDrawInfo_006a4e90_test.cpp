// Behaviour test for 006a4e90: src/006a4e90.cpp copied verbatim; the base draw-info call, the edit-info flag and the
// physics component stubbed so the copy of position-at-time, angle and radius only happens when both gates pass.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingPhysical::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const -- the CThing editor draw info plus the physics component's interpolated position, XY angle and radius
// Retry 10b (accessor sweep, FIXES B-4/B-5): the physics component is read through an inline accessor; the plain member read gives the parked register difference.
// Leaf: ?DrawGetEditDrawInfo@CThingPhysical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A4E90, length 115 bytes (`ret 8`, + int3 padding). CThingPhysical vtable 0x01244E84 slot 25.
// Callees: CThing vtable slot 29 (+0x74, 0x004C77B0: a bool "has an editor draw" query), CThing::DrawGetEditDrawInfo @ 0x004C74B0 (landed),
// CTCPhysicsBase position-at-time getter @ 0x0049D150 (C3DVector by hidden pointer; it lerps the current +0x0c and previous +0x18
// positions by `time` -- the manifest's "CTCPhysicsStandard::GetFacingVector" is a propagated label), CTCPhysicsBase vtable 0x0125D2C4
// slot 63 (+0xfc, 0x006B1230, the XY angle as float).
// Layout: PDB CEditEngineDrawInfo (40 bytes): Pos +0, Radius +0xc, Fullname +0x10, Colour +0x14, PlayerOwner +0x18, EffectRadius +0x1c,
// UsesEffectRadius +0x20, AngleXY +0x24; retail CThing PhysicsTC +0x60; the physics component's radius float at +0x34 (local model).
// Shape: `if (virtual && base) {...; return true;} return false;` (early returns split the exits); Pos assigned from the by-value result (three dword copies through ecx = info),
// AngleXY from the float virtual, Radius as a float member copy (integer move).
struct C3DVector { float X, Y, Z; };
struct CCharString { void* m_pData; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CEditEngineDrawInfo {
    C3DVector   Pos;                            // +0x00
    float       Radius;                         // +0x0c
    CCharString Fullname;                       // +0x10
    CRGBColour  Colour;                         // +0x14
    long        PlayerOwner;                    // +0x18
    float       EffectRadius;                   // +0x1c
    bool        UsesEffectRadius;               // +0x20
    float       AngleXY;                        // +0x24
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual float GetAngleXY() const;           // slot 63 (+0xfc) 0x006B1230
    unsigned char _pad04[0x34 - 0x04];
    float         Radius;                       // +0x34
    C3DVector GetPositionAtTime(float time) const;   // 0x0049D150
};
struct CThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual bool HasEditDrawInfo() const;       // slot 29 (+0x74) 0x004C77B0
    unsigned char   _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;   // 0x004C74B0
};
struct CThingPhysical : CThing {
    bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;
};

bool CThingPhysical::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const
{
    if (HasEditDrawInfo() && CThing::DrawGetEditDrawInfo(time, info)) {
        info->Pos = GetPhysicsTC()->GetPositionAtTime(time);
        info->AngleXY = GetPhysicsTC()->GetAngleXY();
        info->Radius = GetPhysicsTC()->Radius;
        return true;
    }
    return false;
}
// ---- end source ----
void CTCPhysicsBase::v000() {} void CTCPhysicsBase::v001() {} void CTCPhysicsBase::v002() {} void CTCPhysicsBase::v003() {} void CTCPhysicsBase::v004() {} void CTCPhysicsBase::v005() {} void CTCPhysicsBase::v006() {} void CTCPhysicsBase::v007() {} void CTCPhysicsBase::v008() {} void CTCPhysicsBase::v009() {} void CTCPhysicsBase::v010() {} void CTCPhysicsBase::v011() {} void CTCPhysicsBase::v012() {} void CTCPhysicsBase::v013() {} void CTCPhysicsBase::v014() {} void CTCPhysicsBase::v015() {} void CTCPhysicsBase::v016() {} void CTCPhysicsBase::v017() {} void CTCPhysicsBase::v018() {} void CTCPhysicsBase::v019() {} void CTCPhysicsBase::v020() {} void CTCPhysicsBase::v021() {} void CTCPhysicsBase::v022() {} void CTCPhysicsBase::v023() {} void CTCPhysicsBase::v024() {} void CTCPhysicsBase::v025() {} void CTCPhysicsBase::v026() {} void CTCPhysicsBase::v027() {} void CTCPhysicsBase::v028() {} void CTCPhysicsBase::v029() {} void CTCPhysicsBase::v030() {} void CTCPhysicsBase::v031() {} void CTCPhysicsBase::v032() {} void CTCPhysicsBase::v033() {} void CTCPhysicsBase::v034() {} void CTCPhysicsBase::v035() {} void CTCPhysicsBase::v036() {} void CTCPhysicsBase::v037() {} void CTCPhysicsBase::v038() {} void CTCPhysicsBase::v039() {} void CTCPhysicsBase::v040() {} void CTCPhysicsBase::v041() {} void CTCPhysicsBase::v042() {} void CTCPhysicsBase::v043() {} void CTCPhysicsBase::v044() {} void CTCPhysicsBase::v045() {} void CTCPhysicsBase::v046() {} void CTCPhysicsBase::v047() {} void CTCPhysicsBase::v048() {} void CTCPhysicsBase::v049() {} void CTCPhysicsBase::v050() {} void CTCPhysicsBase::v051() {} void CTCPhysicsBase::v052() {} void CTCPhysicsBase::v053() {} void CTCPhysicsBase::v054() {} void CTCPhysicsBase::v055() {} void CTCPhysicsBase::v056() {} void CTCPhysicsBase::v057() {} void CTCPhysicsBase::v058() {} void CTCPhysicsBase::v059() {} void CTCPhysicsBase::v060() {} void CTCPhysicsBase::v061() {} void CTCPhysicsBase::v062() {}
void CThing::v000() {} void CThing::v001() {} void CThing::v002() {} void CThing::v003() {} void CThing::v004() {} void CThing::v005() {} void CThing::v006() {} void CThing::v007() {} void CThing::v008() {} void CThing::v009() {} void CThing::v010() {} void CThing::v011() {} void CThing::v012() {} void CThing::v013() {} void CThing::v014() {} void CThing::v015() {} void CThing::v016() {} void CThing::v017() {} void CThing::v018() {} void CThing::v019() {} void CThing::v020() {} void CThing::v021() {} void CThing::v022() {} void CThing::v023() {} void CThing::v024() {} void CThing::v025() {} void CThing::v026() {} void CThing::v027() {} void CThing::v028() {}
static bool g_has = true, g_base = true; static float g_timeSeen = -1;
bool CThing::HasEditDrawInfo() const { return g_has; }
bool CThing::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const { g_timeSeen = time; info->PlayerOwner = 5; return g_base; }
float CTCPhysicsBase::GetAngleXY() const { return 0.75f; }
C3DVector CTCPhysicsBase::GetPositionAtTime(float time) const { C3DVector v; v.X = time; v.Y = 2 * time; v.Z = 3; return v; }
int main()
{
    static CTCPhysicsBase physics; physics.Radius = 1.5f;
    static CThingPhysical thing; thing.PhysicsTC = &physics;
    CEditEngineDrawInfo info; memset(&info, 0, sizeof(info));
    CHECK(thing.DrawGetEditDrawInfo(2.0f, &info) == true && g_timeSeen == 2.0f && info.PlayerOwner == 5);
    CHECK(info.Pos.X == 2.0f && info.Pos.Y == 4.0f && info.Pos.Z == 3.0f && info.AngleXY == 0.75f && info.Radius == 1.5f);
    memset(&info, 0, sizeof(info)); g_base = false;
    CHECK(thing.DrawGetEditDrawInfo(1.0f, &info) == false && info.Radius == 0.0f);
    g_base = true; g_has = false; g_timeSeen = -1;
    CHECK(thing.DrawGetEditDrawInfo(1.0f, &info) == false && g_timeSeen == -1);
    if (fails == 0) printf("006a4e90_TEST PASS\n"); else printf("006a4e90_TEST FAIL\n");
    return fails;
}