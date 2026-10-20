// Behaviour test for CTCLook::UpdateHeadPos (0x00752000).
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float MagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CWorld {
    C3DVector Camera;
    C3DVector GetCameraPosition() { return Camera; }
};
struct CThing {
    C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
};
struct CThingCreatureBase {
    C3DVector Head; int Queries;
    C3DVector GetHeadPos(bool) { ++Queries; return Head; }
};
struct CLookDef {
    bool AlwaysUpdateHeadPos;                  // +0x54
};
template<class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
static float g_HeadPosCacheTolerance = 0.0316f;
static long  g_HeadPosCacheFrames = 15;
static CWorld g_world;

struct CTCLook {
    CThing*             ParentThing;            // +0x04
    CThingCreatureBase* PParentCreature;        // +0x10
    CDefPointer<const CLookDef> PDef;           // +0x14
    bool                TargetAcquired;         // +0xa0 (PDB +0xa4)
    C3DVector           CachedHeadPos;          // +0xa1 (PDB +0xa5)
    long                HeadCacheTimer;         // +0xb0 (PDB +0xb4)
    CWorld* GetWorld() { return &g_world; }
    bool ShouldGetExactHeadPos() { return true; }
    void UpdateHeadPos();
};

void CTCLook::UpdateHeadPos()
{
    if (!PDef->AlwaysUpdateHeadPos) {
        CWorld* world = GetWorld();
        const C3DVector& thingPos = ParentThing->GetPosition();
        C3DVector toCamera = world->GetCameraPosition() - thingPos;
        if (HeadCacheTimer > 0 && toCamera.MagnitudeSquared() > 64.0f) {
            --HeadCacheTimer;
            return;
        }
        if (PParentCreature == 0)
            return;
    }
    C3DVector head = PParentCreature->GetHeadPos(ShouldGetExactHeadPos());
    if ((CachedHeadPos - head).MagnitudeSquared() > g_HeadPosCacheTolerance)
        HeadCacheTimer = 0;
    else
        HeadCacheTimer = g_HeadPosCacheFrames / 2;
    CachedHeadPos = head;
}

int main()
{
    CLookDef def; def.AlwaysUpdateHeadPos = false;
    CThing thing; thing.Pos = C3DVector(0, 0, 0);
    CThingCreatureBase creature; creature.Head = C3DVector(1, 2, 3); creature.Queries = 0;
    CTCLook look; look.ParentThing = &thing; look.PParentCreature = &creature; look.PDef.Object = &def;
    look.CachedHeadPos = C3DVector(0, 0, 0); look.HeadCacheTimer = 0;
    bool ok = true;
    g_world.Camera = C3DVector(100, 0, 0);
    look.UpdateHeadPos();                           // timer 0: refresh; head moved a lot -> timer 0
    ok = ok && creature.Queries == 1 && look.CachedHeadPos.Z == 3.0f && look.HeadCacheTimer == 0;
    look.UpdateHeadPos();                           // same head -> timer 15/2 = 7
    ok = ok && creature.Queries == 2 && look.HeadCacheTimer == 7;
    look.UpdateHeadPos();                           // far from camera with timer running: count down only
    ok = ok && creature.Queries == 2 && look.HeadCacheTimer == 6;
    g_world.Camera = C3DVector(1, 1, 1);           // near the camera: refresh anyway
    look.UpdateHeadPos();
    ok = ok && creature.Queries == 3 && look.HeadCacheTimer == 7;
    look.PParentCreature = 0; look.HeadCacheTimer = 0;
    look.UpdateHeadPos();                           // no creature: nothing
    ok = ok && creature.Queries == 3;
    if (!ok) { std::printf("00752000_TEST FAIL\n"); return 1; }
    std::printf("00752000_TEST PASS\n");
    return 0;
}