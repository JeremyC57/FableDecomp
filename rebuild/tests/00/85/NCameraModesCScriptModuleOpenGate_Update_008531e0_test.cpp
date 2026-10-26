// Behaviour test for 008531e0: body copied from src/008531e0.cpp (NCameraModes::CScriptModuleOpenGate::Update), GetPItem and the helper-point getter stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool feq(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-5f; }
class CBaseObject;
class C3DVector { public: float X, Y, Z; };
static int g_helper = 0; static float g_helperArg = 0.0f;
struct CTCGraphicAppearance {
    C3DVector Helper;
    C3DVector GetHelperPointOrCentre(float t) const { ++g_helper; g_helperArg = t; return Helper; }
};
class CThing { public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* m_pGraphicAppearance;
};
struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;
    unsigned char _pad1c[0x38];
    C3DVector     Position;
};
static int g_getItem = 0; static CThing g_thing;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CBaseObject* GetPItem() const { ++g_getItem; return (CBaseObject*)m_pItem; }
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {
    virtual void v00() {}
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
};
static inline void Add(C3DVector& a, const C3DVector& b) { a.X += b.X; a.Y += b.Y; a.Z += b.Z; }
namespace NCameraModes {
struct CScriptModuleOpenGate {
    virtual void v00() {}
    unsigned char                   _pad04[0x34];
    CCameraState*                   POwner;
    unsigned char                   _pad3c[0x18];
    CIntelligentPointer_CThingConst PGate;
    C3DVector                       Pos;
    C3DVector                       DeltaPos;
    void Update();
};
void CScriptModuleOpenGate::Update()
{
    POwner->Position = Pos;
    CTCGraphicAppearance* appearance = PGate->m_pGraphicAppearance;
    CCameraState* owner = POwner;
    owner->LookAt = appearance->GetHelperPointOrCentre(1.0f);
    Add(Pos, DeltaPos);
}
}
int main()
{
    CTCGraphicAppearance app; app.Helper.X = 7.0f; app.Helper.Y = 8.0f; app.Helper.Z = 9.0f;
    g_thing.m_pGraphicAppearance = &app;
    CCameraState cam; memset(&cam, 0, sizeof(cam));
    NCameraModes::CScriptModuleOpenGate m;
    m.POwner = &cam; m.PGate.m_pItem = &g_thing;
    m.Pos.X = 1.0f; m.Pos.Y = 2.0f; m.Pos.Z = 3.0f;
    m.DeltaPos.X = 0.5f; m.DeltaPos.Y = -1.0f; m.DeltaPos.Z = 2.0f;
    m.Update();
    CHECK(feq(cam.Position.X, 1.0f) && feq(cam.Position.Y, 2.0f) && feq(cam.Position.Z, 3.0f));   // position = Pos before the advance
    CHECK(g_getItem == 1); CHECK(g_helper == 1); CHECK(feq(g_helperArg, 1.0f));
    CHECK(feq(cam.LookAt.X, 7.0f) && feq(cam.LookAt.Y, 8.0f) && feq(cam.LookAt.Z, 9.0f));
    CHECK(feq(m.Pos.X, 1.5f) && feq(m.Pos.Y, 1.0f) && feq(m.Pos.Z, 5.0f));
    m.Update();
    CHECK(feq(cam.Position.X, 1.5f) && feq(cam.Position.Y, 1.0f) && feq(cam.Position.Z, 5.0f));
    CHECK(feq(m.Pos.X, 2.0f) && feq(m.Pos.Y, 0.0f) && feq(m.Pos.Z, 7.0f)); CHECK(g_helper == 2);
    if (fails == 0) printf("008531e0_TEST PASS\n"); else printf("008531e0_TEST FAIL\n");
    return fails;
}