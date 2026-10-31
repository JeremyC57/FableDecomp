// Behaviour test for NCameraModes::CBase::UpdateRaiseUp (0x0084D7E0): the camera height above the observed thing is scaled by
// tanh(d2/4) and the thing's focal height (+0.28) faded by exp(-d2/4) is added, d2 being the squared ground distance.
#include <cstdio>
#include <cmath>

class C3DVector { public: float X, Y, Z; };
struct CTCGraphicAppearance {
    C3DVector Focal;
    C3DVector GetFocalPos(float scale) const { C3DVector r = Focal; r.Z *= scale; return r; }
};
class CThing { public:
    C3DVector Pos;
    CTCGraphicAppearance* PTCGraphicAppearance;
    const C3DVector& GetPos() const { return Pos; }
};
class CGameCameraManager { public:
    CThing* Observed;
    const CThing* GetObservedThing() const { return Observed; }
};
struct CBase {
    CGameCameraManager* CameraManager;
    const CThing* GetObservedThing() const { return CameraManager->GetObservedThing(); }
    void UpdateRaiseUp(C3DVector& pos) const
    {
        const C3DVector& thingPos = GetObservedThing()->GetPos();
        float distanceSquared;
        {
            float dx = thingPos.X - pos.X;
            float dy = thingPos.Y - pos.Y;
            distanceSquared = dy * dy + dx * dx;
        }
        const CTCGraphicAppearance* appearance = GetObservedThing()->PTCGraphicAppearance;
        C3DVector offset;
        offset.Z = appearance->GetFocalPos(1.0f).Z - thingPos.Z;
        float raise = (float)tanh(distanceSquared * 0.25f) * (pos.Z - thingPos.Z);
        pos.Z = raise + (float)exp(distanceSquared * -0.25f) * (offset.Z + 0.28f) + thingPos.Z;
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    CTCGraphicAppearance app; app.Focal.X = 0; app.Focal.Y = 0; app.Focal.Z = 2.0f;
    CThing thing; thing.Pos.X = 1.0f; thing.Pos.Y = 2.0f; thing.Pos.Z = 1.0f; thing.PTCGraphicAppearance = &app;
    CGameCameraManager mgr; mgr.Observed = &thing;
    CBase cam; cam.CameraManager = &mgr;
    // camera right above the thing: d2 = 0 -> tanh 0, exp 1 -> Z = focal.Z + 0.28
    C3DVector p; p.X = 1.0f; p.Y = 2.0f; p.Z = 10.0f;
    cam.UpdateRaiseUp(p);
    if (!Near(p.Z, 2.28f) || p.X != 1.0f || p.Y != 2.0f) { std::printf("0084d7e0_TEST FAIL\n"); return 1; }
    // d2 = 3*3 + 4*4 = 25 (dx = -3, dy = -4)
    C3DVector q; q.X = 4.0f; q.Y = 6.0f; q.Z = 5.0f;
    cam.UpdateRaiseUp(q);
    float expect = (float)tanh(6.25f) * 4.0f + (float)exp(-6.25f) * (1.0f + 0.28f) + 1.0f;
    if (!Near(q.Z, expect)) { std::printf("0084d7e0_TEST FAIL\n"); return 1; }
    std::printf("0084d7e0_TEST PASS\n");
    return 0;
}