// Behaviour test for 0x00852fd0 NCameraModes::CScriptModuleFOVZoomStayRelative::Update
#include <cstdio>
#include <cmath>

static long g_frame = 0;
static long GFGetCurrentFrame() { return g_frame; }
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Rotate(const C3DVector& axis, float angle)
    {   // test stand-in: rotation about +Z by angle radians
        if (axis.Z != 1.0f) return;
        float c = (float)std::cos(angle), s = (float)std::sin(angle), x = X * c - Y * s, y = X * s + Y * c; X = x; Y = y;
    }
    C3DVector& operator*=(float k) { X *= k; Y *= k; Z *= k; return *this; }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};
class CTCPhysicsBase { public: C3DVector facing; C3DVector GetFacing() { return facing; } };
class CThing { public: CTCPhysicsBase* PhysicsTC; CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; } };
struct CIntelligentPointerThing { CThing* item; CThing* GetPItem() { return item; } };
struct CCameraState { C3DVector Position; };
struct CScriptModuleFOVZoom {
    CCameraState* POwner; C3DVector LookAtPos; long EndFrame;
    int baseUpdates; C3DVector incFrom, incTo; long incFrames; int incCalls;
    void Update() { ++baseUpdates; }
    void CalculateIncrementVector(const C3DVector& from, const C3DVector& to, long frames) { incFrom = from; incTo = to; incFrames = frames; ++incCalls; }
};
struct CScriptModuleFOVZoomStayRelative : CScriptModuleFOVZoom {
    CIntelligentPointerThing Thing; C3DVector Axis; float Angle; float Mag;
    void Update()
    {
        C3DVector position = Thing.GetPItem()->GetPhysicsTC()->GetFacing();
        position.Rotate(Axis, Angle);
        position *= Mag;
        position += LookAtPos;
        if (GFGetCurrentFrame() < EndFrame) {
            CCameraState* owner = POwner;
            CalculateIncrementVector(owner->Position, position, EndFrame - GFGetCurrentFrame());
        }
        else
            POwner->Position = position;
        CScriptModuleFOVZoom::Update();
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    CTCPhysicsBase phys; phys.facing = C3DVector(1.0f, 0.0f, 0.0f);
    CThing thing; thing.PhysicsTC = &phys;
    CCameraState cam; cam.Position = C3DVector(0.0f, 0.0f, 0.0f);
    CScriptModuleFOVZoomStayRelative m;
    m.POwner = &cam; m.LookAtPos = C3DVector(10.0f, 10.0f, 1.0f); m.EndFrame = 100; m.baseUpdates = 0; m.incCalls = 0;
    m.Thing.item = &thing; m.Axis = C3DVector(0.0f, 0.0f, 1.0f); m.Angle = 1.5707963f; m.Mag = 5.0f;
    g_frame = 40;
    m.Update();   // facing (1,0,0) -> (0,1,0) -> (0,5,0) -> (10,15,1); glide over 60 frames
    if (m.incCalls != 1 || m.incFrames != 60 || !Near(m.incTo.X, 10.0f) || !Near(m.incTo.Y, 15.0f) || !Near(m.incTo.Z, 1.0f)
        || m.baseUpdates != 1 || !Near(cam.Position.X, 0.0f)) { std::printf("00852fd0_TEST FAIL\n"); return 1; }
    g_frame = 100;
    m.Update();   // at EndFrame: snap the camera
    if (m.incCalls != 1 || m.baseUpdates != 2 || !Near(cam.Position.X, 10.0f) || !Near(cam.Position.Y, 15.0f)) {
        std::printf("00852fd0_TEST FAIL\n"); return 1;
    }
    std::printf("00852fd0_TEST PASS\n");
    return 0;
}
