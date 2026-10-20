// Behaviour test for 0x00852c40 NCameraModes::CScriptModuleThingLookAtUpperBody::Update
#include <cstdio>
#include <cmath>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CTCPhysicsBase { public: C3DVector facing; C3DVector GetFacing() { return facing; } };
class CTCGraphicAppearance {
public:
    C3DVector focal; float lastHeight;
    C3DVector GetCameraFocalPos(float h) { lastHeight = h; return focal; }
};
class CThing {
public:
    CTCPhysicsBase* PhysicsTC; CTCGraphicAppearance* PTCGraphicAppearance;
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    CTCGraphicAppearance* GetPTCGraphicAppearance() const { return PTCGraphicAppearance; }
};
struct CIntelligentPointerThing { CThing* item; int calls; CThing* GetPItem() { ++calls; return item; } };
struct CCameraState { C3DVector LookAt; C3DVector Position; };
struct CScriptModuleThingLookAtUpperBody {
    CCameraState* POwner; CIntelligentPointerThing Thing;
    void Update()
    {
        if (Thing.GetPItem()) {
            C3DVector facing = Thing.GetPItem()->GetPhysicsTC()->GetFacing();
            C3DVector focal = Thing.GetPItem()->GetPTCGraphicAppearance()->GetCameraFocalPos(1.0f);
            POwner->Position = focal + facing * 1.25f;
            POwner->LookAt = focal;
        }
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    CTCPhysicsBase phys; phys.facing = C3DVector(4.0f, -8.0f, 2.0f);
    CTCGraphicAppearance look; look.focal = C3DVector(100.0f, 50.0f, 1.8f); look.lastHeight = 0.0f;
    CThing thing; thing.PhysicsTC = &phys; thing.PTCGraphicAppearance = &look;
    CCameraState cam; cam.Position = C3DVector(0, 0, 0); cam.LookAt = C3DVector(0, 0, 0);
    CScriptModuleThingLookAtUpperBody m; m.POwner = &cam; m.Thing.item = &thing; m.Thing.calls = 0;
    m.Update();
    float k = 1.25f;
    if (!Near(cam.Position.X, 100.0f + 4.0f * k) || !Near(cam.Position.Y, 50.0f - 8.0f * k) || !Near(cam.Position.Z, 1.8f + 2.0f * k)
        || !Near(cam.LookAt.X, 100.0f) || !Near(cam.LookAt.Z, 1.8f) || look.lastHeight != 1.0f || m.Thing.calls != 3) {
        std::printf("00852c40_TEST FAIL\n"); return 1;
    }
    // no thing: nothing touched
    CCameraState cam2; cam2.Position = C3DVector(7, 7, 7); m.POwner = &cam2; m.Thing.item = 0; m.Thing.calls = 0;
    m.Update();
    if (cam2.Position.X != 7.0f || m.Thing.calls != 1) { std::printf("00852c40_TEST FAIL\n"); return 1; }
    std::printf("00852c40_TEST PASS\n");
    return 0;
}
