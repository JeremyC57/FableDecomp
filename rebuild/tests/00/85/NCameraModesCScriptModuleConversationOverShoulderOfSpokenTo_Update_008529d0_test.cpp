// Behaviour test for NCameraModes::CScriptModuleConversationOverShoulderOfSpokenTo::Update (0x008529D0): with two
// distinct things the camera goes 2 units behind the spoken-to (along the normalised XY speaker->spoken-to line),
// sideways by 2 x RandomDirection, 4 up, looking at the speaker's focal point; otherwise nothing changes.
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void Normalise() { float l = (float)std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
};
class CThing { public:
    C3DVector Pos;
    C3DVector Focal;
    const C3DVector& GetPosition() const { return Pos; }
    C3DVector GetCameraFocalPos(float) const { return Focal; }
    C3DVector GetXYDirectionTo(const C3DVector& pos) const
    {
        const C3DVector& here = GetPosition();
        return C3DVector(here.X - pos.X, here.Y - pos.Y, 0.0f);
    }
};
struct CPtr {
    CThing* Item;
    CThing* GetPItem() const { return Item; }
    CThing* operator->() const { return GetPItem(); }
};
struct CCameraState { C3DVector LookAt; C3DVector Position; };
struct CModule {
    CCameraState* POwner;
    CPtr PSpeaker;
    CPtr PSpokenTo;
    float RandomDirection;
    bool IsThingsOk() const
    {
        const CThing* speaker = PSpeaker.GetPItem();
        const CThing* spokenTo = PSpokenTo.GetPItem();
        return speaker && spokenTo && speaker != spokenTo;
    }
    void Update();
};
void CModule::Update()
{
    if (IsThingsOk()) {
        C3DVector direction = PSpeaker->GetXYDirectionTo(PSpokenTo->GetPosition());
        direction.Normalise();
        C3DVector side(direction.Y, -direction.X, 0.0f);
        C3DVector spokenToPos = PSpokenTo->GetPosition();
        float scale = RandomDirection * 2.0f;
        C3DVector focal = PSpeaker->GetCameraFocalPos(1.0f);
        POwner->Position = spokenToPos - direction * 2.0f + side * scale + C3DVector(0.0f, 0.0f, 4.0f);
        POwner->LookAt = focal;
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    CThing speaker; speaker.Pos = C3DVector(10.0f, 0.0f, 3.0f); speaker.Focal = C3DVector(10.0f, 0.0f, 5.0f);
    CThing spoken; spoken.Pos = C3DVector(0.0f, 0.0f, 1.0f); spoken.Focal = C3DVector(0, 0, 0);
    CCameraState cam; cam.LookAt = C3DVector(0, 0, 0); cam.Position = C3DVector(0, 0, 0);
    CModule m; m.POwner = &cam; m.PSpeaker.Item = &speaker; m.PSpokenTo.Item = &spoken; m.RandomDirection = 1.0f;
    m.Update();
    // direction (1,0,0); side (0,-1,0) * 2 -> position (-2,-2,5)
    bool ok = Near(cam.Position.X, -2.0f) && Near(cam.Position.Y, -2.0f) && Near(cam.Position.Z, 5.0f)
           && Near(cam.LookAt.X, 10.0f) && Near(cam.LookAt.Z, 5.0f);
    CCameraState cam2; cam2.LookAt = C3DVector(7, 7, 7); cam2.Position = C3DVector(7, 7, 7);
    m.POwner = &cam2; m.PSpokenTo.Item = &speaker;          // same thing twice: no change
    m.Update();
    ok = ok && cam2.Position.X == 7.0f && cam2.LookAt.Y == 7.0f;
    m.PSpokenTo.Item = 0;                                   // missing thing: no change
    m.Update();
    ok = ok && cam2.Position.Z == 7.0f;
    if (!ok) { std::printf("008529d0_TEST FAIL\n"); return 1; }
    std::printf("008529d0_TEST PASS\n");
    return 0;
}