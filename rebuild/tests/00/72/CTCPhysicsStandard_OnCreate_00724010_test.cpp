// 00724010 behaviour test: CTCPhysicsStandard::OnCreate (control flow + data of the final.cpp body)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Clear() { X = Y = Z = 0.0f; }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct C2DBoxI { long Left; long Top; long Right; long Bottom; };
struct CThing;
struct CThingPtrVector {
    CThing** first; CThing** last; CThing** endOfStorage;
    CThingPtrVector() : first(0), last(0), endOfStorage(0) { }
    ~CThingPtrVector() { if (first) free(first); }
};
const float g_FloatEpsilon = 0.0001f;
const float g_OnGroundTolerance = 0.5f;
struct C3DMeshInfo { long PhysicsMeshIndex; bool HasPhysicsMesh() const { return PhysicsMeshIndex != -1; } };
static C3DMeshInfo g_meshes[4], g_anims[4];
static int g_meshCalls = 0, g_animCalls = 0;
struct CGraphicBank {
    C3DMeshInfo* GetMesh(unsigned long index) { ++g_meshCalls; return &g_meshes[index]; }
    C3DMeshInfo* GetAnimatedMesh(unsigned long index) { ++g_animCalls; return &g_anims[index]; }
};
struct CWorld { CGraphicBank* PGraphicBank; };
static float g_ground = 0.0f;
struct CWorldMap { float GetGroundSizeZAt(const C3DVector&) const { return g_ground; } };
struct CGraphicDataRef {
    unsigned long Index;
    unsigned char Type;
    bool IsValid() const { return Type != 0; }
};
struct CTCGraphicAppearance { CGraphicDataRef GraphicData; };
struct CThing { CTCGraphicAppearance* PGraphicAppearance; };
struct CPhysicsDef { float Mass; long InteractionFlags; bool PushableByHero; float Air; };
static CWorld g_world; static CWorldMap g_map;
static const CGraphicDataRef* g_created = 0;
static int g_base = 0, g_interactCalls = 0, g_surface = 0;
struct CTCPhysicsStandard {
    CThing*         ParentThing;
    C3DVector       Position;
    const CPhysicsDef* PhysicsDef;
    C3DVector       OffsetVector3D;
    CRightHandedSet RHSet;
    CRightHandedSet OldRHSet;
    unsigned char   AlwaysFacingMovementDirection : 1;
    unsigned char   AngledWithGround : 1;
    unsigned char   FeelsGravity : 1;
    unsigned char   FeelsAirResistance : 1;
    unsigned char   OnGround : 1;
    unsigned char   PushableByHero : 1;
    long Flags; int Unknown36; bool Interacts; float SurfaceZ;
    virtual void OnCreate();
    virtual void SetUnknown36(bool value) { Unknown36 = value ? 1 : 2; }
    virtual void SetInteractionFlags(long flags) { Flags = flags; }
    virtual bool InteractsWithThingsCheck() { return Interacts; }
    virtual void UpdateOnSurface(CThingPtrVector&, C3DVector& position) { ++g_surface; SurfaceZ = position.Z; }
    virtual C2DBoxI GetPossibleCollidedWithArea() { C2DBoxI b = { 1, 2, 3, 4 }; return b; }
    const CWorld& GetWorld() const { return g_world; }
    CWorldMap* GetWorldMap() { return &g_map; }
    void CreatePPhysicsMeshInfo(const CGraphicDataRef& graphic) { g_created = &graphic; }
    void BaseOnCreate() { ++g_base; }
    float GetMass() { return PhysicsDef->Mass; }
    float GetAirResistance() { return PhysicsDef->Air; }
    void GetPossibleInteractedWithList(const C2DBoxI& area, CThingPtrVector&, float speed) { if (area.Right == 3 && speed == 1.0f) ++g_interactCalls; }
};
void CTCPhysicsStandard::OnCreate()
{
    OldRHSet = RHSet;
    OffsetVector3D.Clear();
    CTCGraphicAppearance* graphic = ParentThing->PGraphicAppearance;
    if (graphic && graphic->GraphicData.IsValid()) {
        const CGraphicDataRef& data = graphic->GraphicData;
        const CWorld& world = GetWorld();
        CGraphicBank* bank = world.PGraphicBank;
        unsigned char type = data.Type;
        if (type == 5) {
            if (data.Index && bank->GetMesh(data.Index)->HasPhysicsMesh())
                CreatePPhysicsMeshInfo(data);
        } else if (type == 4) {
            if (data.Index && bank->GetAnimatedMesh(data.Index)->HasPhysicsMesh())
                CreatePPhysicsMeshInfo(data);
        }
    }
    BaseOnCreate();
    if (PhysicsDef) {
        SetInteractionFlags(PhysicsDef->InteractionFlags);
        FeelsGravity = GetMass() > g_FloatEpsilon;
        FeelsAirResistance = GetAirResistance() > g_FloatEpsilon;
        PushableByHero = PhysicsDef->PushableByHero;
    } else {
        SetInteractionFlags(0);
        FeelsGravity = false;
        FeelsAirResistance = false;
        PushableByHero = true;
    }
    C3DVector position(Position);
    OnGround = fabs(position.Z - GetWorldMap()->GetGroundSizeZAt(position)) < g_OnGroundTolerance;
    if (InteractsWithThingsCheck()) {
        CThingPtrVector things;
        GetPossibleInteractedWithList(GetPossibleCollidedWithArea(), things, 1.0f);
        C3DVector here(Position);
        UpdateOnSurface(things, here);
        SetUnknown36(false);
    }
}
static void Reset(CTCPhysicsStandard& p)
{
    p.OffsetVector3D.X = p.OffsetVector3D.Y = p.OffsetVector3D.Z = 9.0f;
    p.RHSet.Up.X = 1; p.RHSet.Up.Y = 2; p.RHSet.Up.Z = 3; p.RHSet.Forward.X = 4; p.RHSet.Forward.Y = 5; p.RHSet.Forward.Z = 6;
    p.OldRHSet.Up.X = 0; p.OldRHSet.Forward.Z = 0;
    p.FeelsGravity = p.FeelsAirResistance = p.OnGround = p.PushableByHero = 0;
    p.Flags = -1; p.Unknown36 = 0; p.Interacts = false; p.SurfaceZ = 0;
    g_created = 0; g_base = 0; g_interactCalls = 0; g_surface = 0; g_meshCalls = g_animCalls = 0;
}
int main()
{
    CGraphicBank bank; g_world.PGraphicBank = &bank;
    g_meshes[1].PhysicsMeshIndex = 3; g_meshes[2].PhysicsMeshIndex = -1; g_anims[1].PhysicsMeshIndex = 0;
    CTCGraphicAppearance app; CThing thing; thing.PGraphicAppearance = &app;
    CPhysicsDef def; def.Mass = 2.0f; def.InteractionFlags = 0x15; def.PushableByHero = false; def.Air = 0.0f;
    CTCPhysicsStandard p; p.ParentThing = &thing; p.Position.X = 1; p.Position.Y = 2; p.Position.Z = 0.25f;

    // static mesh with physics, def present, on ground, interacts
    Reset(p); app.GraphicData.Index = 1; app.GraphicData.Type = 5; p.PhysicsDef = &def; p.Interacts = true; g_ground = 0.0f;
    p.OnCreate();
    CHECK(p.OldRHSet.Up.X == 1 && p.OldRHSet.Forward.Z == 6 && p.OffsetVector3D.X == 0 && p.OffsetVector3D.Z == 0);
    CHECK(g_created == &app.GraphicData && g_meshCalls == 1 && g_animCalls == 0 && g_base == 1);
    CHECK(p.Flags == 0x15 && p.FeelsGravity && !p.FeelsAirResistance && !p.PushableByHero && p.OnGround);
    CHECK(g_interactCalls == 1 && g_surface == 1 && p.SurfaceZ == 0.25f && p.Unknown36 == 2);

    // static mesh without physics; no def; off ground; no interaction
    Reset(p); app.GraphicData.Index = 2; p.PhysicsDef = 0; p.Interacts = false; g_ground = 5.0f;
    p.OnCreate();
    CHECK(g_created == 0 && g_meshCalls == 1 && p.Flags == 0 && !p.FeelsGravity && !p.FeelsAirResistance && p.PushableByHero);
    CHECK(!p.OnGround && g_surface == 0 && p.Unknown36 == 0);

    // animated mesh with physics; index 0 skips lookup; other type ignored; no appearance
    Reset(p); app.GraphicData.Index = 1; app.GraphicData.Type = 4; def.Air = 1.0f; def.PushableByHero = true; p.PhysicsDef = &def;
    p.OnCreate();
    CHECK(g_created == &app.GraphicData && g_animCalls == 1 && p.FeelsAirResistance && p.PushableByHero);
    Reset(p); app.GraphicData.Index = 0; p.OnCreate(); CHECK(g_created == 0 && g_animCalls == 0 && g_base == 1);
    Reset(p); app.GraphicData.Index = 1; app.GraphicData.Type = 3; p.OnCreate(); CHECK(g_created == 0 && g_meshCalls == 0 && g_animCalls == 0);
    Reset(p); app.GraphicData.Type = 0; p.OnCreate(); CHECK(g_created == 0 && g_meshCalls == 0);
    Reset(p); thing.PGraphicAppearance = 0; p.OnCreate(); CHECK(g_created == 0 && g_base == 1);

    if (!g_fail) printf("00724010_TEST PASS\n");
    return g_fail;
}