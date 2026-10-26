// Behaviour test for 0075b2d0: body copied from src/0075b2d0.cpp (CTCPhysicsRigidBody::OnCreate); CreatePPhysicsMeshInfo, the
// slot-93 GetPhysicsMeshInfo virtual, CTCPhysicsBase::OnCreate and InitialiseBody stubbed to record their calls.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool feq(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-5f; }
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    void Set(float x, float y) { X = x; Y = y; }
};
class CThingPhysicsMeshInfo { public:
    unsigned short BankIndex;
    float          RenderSizeX;
    C3DVector      ToCentre;
};
struct CGraphicDataRef;
struct CTCGraphicAppearance {
    unsigned char _pad00[0x44];
    unsigned char GraphicData[0x10];
};
class CThing { public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;
};
static int g_create = 0, g_baseOnCreate = 0, g_init = 0, g_getInfo = 0; static const void* g_createArg = 0;
static CThingPhysicsMeshInfo g_info;
static int g_order[8]; static int g_n = 0;
class CTCPhysicsBase { public:
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {}
    virtual void OnCreate() { ++g_baseOnCreate; g_order[g_n++] = 3; }
    virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {} virtual void v010() {}
    virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {}
    virtual void v017() {} virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {}
    virtual void v023() {} virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {}
    virtual void v029() {} virtual void v030() {} virtual void v031() {} virtual void v032() {} virtual void v033() {} virtual void v034() {}
    virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {} virtual void v040() {}
    virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {}
    virtual void v047() {} virtual void v048() {} virtual void v049() {} virtual void v050() {} virtual void v051() {} virtual void v052() {}
    virtual void v053() {} virtual void v054() {} virtual void v055() {} virtual void v056() {} virtual void v057() {} virtual void v058() {}
    virtual void v059() {} virtual void v060() {} virtual void v061() {} virtual void v062() {} virtual void v063() {} virtual void v064() {}
    virtual void v065() {} virtual void v066() {} virtual void v067() {} virtual void v068() {} virtual void v069() {} virtual void v070() {}
    virtual void v071() {} virtual void v072() {} virtual void v073() {} virtual void v074() {} virtual void v075() {} virtual void v076() {}
    virtual void v077() {} virtual void v078() {} virtual void v079() {} virtual void v080() {} virtual void v081() {} virtual void v082() {}
    virtual void v083() {} virtual void v084() {} virtual void v085() {} virtual void v086() {} virtual void v087() {} virtual void v088() {}
    virtual void v089() {} virtual void v090() {} virtual void v091() {} virtual void v092() {}
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo() { ++g_getInfo; g_order[g_n++] = 2; return &g_info; }
    CThing*   ParentThing;
    unsigned char _pad08[0x38];
    void CreatePPhysicsMeshInfo(const CGraphicDataRef& graphic) { ++g_create; g_createArg = &graphic; g_order[g_n++] = 1; }
};
struct CTCPhysicsRigidBody : CTCPhysicsBase {
    void*           CollisionNormal[3];
    void*           CollidingVertexIndex[3];
    void*           Collidors[3];
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;
    CRightHandedSet RHSet;
    CRightHandedSet OldRHSet;
    C2DVector       OffsetVector;
    void InitialiseBody() { ++g_init; g_order[g_n++] = 4; }
    virtual void OnCreate();
};
void CTCPhysicsRigidBody::OnCreate()
{
    OldRHSet = RHSet;
    OffsetVector.Set(0.0f, 0.0f);
    CTCGraphicAppearance* graphic = ParentThing->PGraphicAppearance;
    if (graphic)
        CreatePPhysicsMeshInfo(*(const CGraphicDataRef*)graphic->GraphicData);
    const C3DVector& centre = GetPhysicsMeshInfo()->ToCentre;
    OffsetVector.X = centre.X;
    OffsetVector.Y = centre.Y;
    CTCPhysicsBase::OnCreate();
    InitialiseBody();
}
int main()
{
    CTCPhysicsRigidBody body; memset(&body.ParentThing, 0, sizeof(body) - sizeof(void*));
    CThing thing; memset(&thing, 0, sizeof(thing)); CTCGraphicAppearance app; memset(&app, 0, sizeof(app));
    body.ParentThing = &thing;
    body.RHSet.Up.X = 1; body.RHSet.Up.Y = 2; body.RHSet.Up.Z = 3; body.RHSet.Forward.X = 4; body.RHSet.Forward.Y = 5; body.RHSet.Forward.Z = 6;
    body.OffsetVector.X = 77; body.OffsetVector.Y = 88;
    g_info.ToCentre.X = 1.5f; g_info.ToCentre.Y = -2.5f; g_info.ToCentre.Z = 9.0f;
    body.OnCreate();                                  // no graphic appearance: no mesh-info build
    CHECK(g_create == 0); CHECK(g_getInfo == 1); CHECK(g_baseOnCreate == 1); CHECK(g_init == 1);
    CHECK(feq(body.OldRHSet.Up.X, 1) && feq(body.OldRHSet.Up.Z, 3) && feq(body.OldRHSet.Forward.X, 4) && feq(body.OldRHSet.Forward.Z, 6));
    CHECK(feq(body.OffsetVector.X, 1.5f) && feq(body.OffsetVector.Y, -2.5f));
    CHECK(g_n == 3 && g_order[0] == 2 && g_order[1] == 3 && g_order[2] == 4);
    g_n = 0; thing.PGraphicAppearance = &app;
    body.OnCreate();                                  // with a graphic appearance: mesh info built from its graphic data
    CHECK(g_create == 1); CHECK(g_createArg == (const void*)app.GraphicData);
    CHECK(g_n == 4 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3 && g_order[3] == 4);
    CHECK(feq(body.OffsetVector.X, 1.5f) && feq(body.OffsetVector.Y, -2.5f));
    if (fails == 0) printf("0075b2d0_TEST PASS\n"); else printf("0075b2d0_TEST FAIL\n");
    return fails;
}