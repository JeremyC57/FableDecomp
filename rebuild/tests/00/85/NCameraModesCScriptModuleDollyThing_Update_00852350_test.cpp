// Behaviour test for 0x00852350 NCameraModes::CScriptModuleDollyThing::Update
#include <cstdio>
#include <cmath>

static long g_FramesPerSecond = 30;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { C3DVector r; r.X = X + o.X; r.Y = Y + o.Y; r.Z = Z + o.Z; return r; }
    void Rotate(const C3DVector& axis, float angle)
    {   // rotation about +Z by angle (in turns) -- only the Z axis is used by the caller
        if (axis.X != 0.0f || axis.Y != 0.0f || axis.Z != 1.0f) return;
        float a = angle * 6.2831853f, c = (float)std::cos(a), s = (float)std::sin(a);
        float x = X * c - Y * s, y = X * s + Y * c; X = x; Y = y;
    }
};
class CThing { public: C3DVector pos; int calls; const C3DVector& GetPosition() const { ++const_cast<CThing*>(this)->calls; return pos; } };
struct CCameraState { C3DVector LookAt; C3DVector Position; };
struct CScriptModuleDollyThing {
    CCameraState* POwner; CThing* RotThing; C3DVector DollyVector; float TimeFor360Turn;
    void Update()
    {
        long fps = g_FramesPerSecond;
        POwner->Position = RotThing->GetPosition() + DollyVector;
        CCameraState* owner = POwner;
        owner->LookAt = RotThing->GetPosition();
        DollyVector.Rotate(C3DVector(0.0f, 0.0f, 1.0f), 1.0f / (fps * TimeFor360Turn));
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    CThing t; t.pos = C3DVector(10.0f, 20.0f, 5.0f); t.calls = 0;
    CCameraState cam;
    CScriptModuleDollyThing m; m.POwner = &cam; m.RotThing = &t; m.DollyVector = C3DVector(4.0f, 0.0f, 2.0f); m.TimeFor360Turn = 1.0f / 30.0f * 4.0f;
    m.Update();   // angle = 1 / (30 * 4/30) = quarter turn
    if (!Near(cam.Position.X, 14.0f) || !Near(cam.Position.Y, 20.0f) || !Near(cam.Position.Z, 7.0f) || !Near(cam.LookAt.X, 10.0f)
        || !Near(cam.LookAt.Z, 5.0f) || t.calls != 2 || !Near(m.DollyVector.X, 0.0f) || !Near(m.DollyVector.Y, 4.0f) || !Near(m.DollyVector.Z, 2.0f)) {
        std::printf("00852350_TEST FAIL\n"); return 1;
    }
    std::printf("00852350_TEST PASS\n");
    return 0;
}