// Behaviour test for NCameraModes::CRotateAround::Update (0x0087BD70): the row body verbatim over models of its callees. Kills: the
// inversion ignored / applied unconditionally, the wrong delta source, sin / cos or X / Y axes swapped, the radius or height offset
// dropped, the focal Z used instead of the thing's Z, the clamp applied when occluded / not limited, min / max swapped, the else
// dropped, the position or the look-at target not stored.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static void check(bool v, int line) { if (!v) { printf("FAIL line %d\n", line); exit(1); } }
#define CHECK(c) check((c), __LINE__)
static bool same(float a, float b) { return memcmp(&a, &b, 4) == 0; }
static const double TWO_PI = 6.2831854820251465;
static double GFSin(float a) { return sin(a * TWO_PI); }
static double GFCos(float a) { return cos(a * TWO_PI); }
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
static bool inverted;
struct CUserProfileManager { static CUserProfileManager& Get() { static CUserProfileManager m; return m; } bool IsCameraXYInverted() const { return inverted; } };
struct CTCGraphicAppearance { C3DVector Focal; C3DVector GetStaticCameraFocalPos() const { return Focal; } };
struct CThingGameObject {
    C3DVector Pos; CTCGraphicAppearance* App;
    const CTCGraphicAppearance* GetTCGraphicAppearance() const { return App; }
    const C3DVector& GetPos() const { return Pos; }
};
struct CGameCameraManager {
    float CamRSLookAroundDeltaTheta; bool LimitMinMaxWorldHeight; float MinWorldHeight, MaxWorldHeight;
    float GetMinWorldHeight() const { return MinWorldHeight; }
    float GetMaxWorldHeight() const { return MaxWorldHeight; }
};
namespace NCameraModes {
class CRotateAround {
public:
    C2DVector RightStick; CThingGameObject* Thing; CGameCameraManager* Mgr; bool ViewOccluded;
    C3DVector GoToPos, LookAtPos;
    float StringLengthXY, HeightOffset; C3DVector XAxis, YAxis; float Theta;
    const C2DVector& GetRightStickVector() const { return RightStick; }
    const CThingGameObject* GetObservedThing() const { return Thing; }
    CGameCameraManager& GetCameraManager() { return *Mgr; }
    bool IsCameraViewPosOccluded() const { return ViewOccluded; }
    void SetPosition(const C3DVector& p) { GoToPos = p; }
    void SetObservedPos(const C3DVector& p) { LookAtPos = p; }
    void Update();
};
// ---- row body (verbatim) ----
void CRotateAround::Update()
{
    const C2DVector& cam_stick = GetRightStickVector();
    float cam_stick_x = cam_stick.X;
    if (CUserProfileManager::Get().IsCameraXYInverted())
        cam_stick_x *= -1.0f;
    C3DVector foc_pos = GetObservedThing()->GetTCGraphicAppearance()->GetStaticCameraFocalPos();
    C3DVector thing_base(GetObservedThing()->GetPos());
    float d_thet = GetCameraManager().CamRSLookAroundDeltaTheta * cam_stick_x;
    Theta += d_thet;
    C3DVector line_out = XAxis * (float)GFCos(Theta) + YAxis * (float)GFSin(Theta);
    C3DVector new_pos = C3DVector(foc_pos.X, foc_pos.Y, thing_base.Z) + line_out * StringLengthXY + C3DVector(0.0f, 0.0f, HeightOffset);
    if (GetCameraManager().LimitMinMaxWorldHeight && !IsCameraViewPosOccluded()) {
        if (new_pos.Z < GetCameraManager().GetMinWorldHeight())
            new_pos.Z = GetCameraManager().GetMinWorldHeight();
        else if (new_pos.Z > GetCameraManager().GetMaxWorldHeight())
            new_pos.Z = GetCameraManager().GetMaxWorldHeight();
    }
    SetPosition(new_pos);
    SetObservedPos(foc_pos);
}
// ---- end row body ----
}
using namespace NCameraModes;
int main()
{
    CTCGraphicAppearance app; CThingGameObject thing; CGameCameraManager mgr;
    const float sticks[] = { 0.5f, -0.25f, 1.0f }, zs[] = { -5.0f, 2.0f, 40.0f };
    long cases = 0;
    for (int inv = 0; inv != 2; ++inv) for (int lim = 0; lim != 2; ++lim) for (int occ = 0; occ != 2; ++occ)
    for (int s = 0; s != 3; ++s) for (int z = 0; z != 3; ++z) {
        inverted = inv != 0;
        app.Focal = C3DVector(3.0f, -2.0f, 99.0f + z); thing.Pos = C3DVector(7.0f, 8.0f, zs[z]); thing.App = &app;
        mgr.CamRSLookAroundDeltaTheta = 0.125f; mgr.LimitMinMaxWorldHeight = lim != 0; mgr.MinWorldHeight = 0.0f; mgr.MaxWorldHeight = 30.0f;
        CRotateAround r; r.RightStick.X = sticks[s]; r.RightStick.Y = 9.0f; r.Thing = &thing; r.Mgr = &mgr; r.ViewOccluded = occ != 0;
        r.StringLengthXY = 4.0f; r.HeightOffset = 1.5f; r.XAxis = C3DVector(1.0f, 0.0f, 0.0f); r.YAxis = C3DVector(0.0f, 1.0f, 0.0f); r.Theta = 0.1f;
        r.Update();
        float sx = inv ? sticks[s] * -1.0f : sticks[s];
        float theta = 0.1f + 0.125f * sx;
        CHECK(same(r.Theta, theta));
        float c = (float)GFCos(theta), sn = (float)GFSin(theta);
        float ex = 3.0f + (1.0f * c + 0.0f * sn) * 4.0f + 0.0f;
        float ey = -2.0f + (0.0f * c + 1.0f * sn) * 4.0f + 0.0f;
        float ez = zs[z] + (0.0f * c + 0.0f * sn) * 4.0f + 1.5f;
        if (lim && !occ) { if (ez < 0.0f) ez = 0.0f; else if (ez > 30.0f) ez = 30.0f; }
        CHECK(fabs(r.GoToPos.X - ex) < 1e-5f && fabs(r.GoToPos.Y - ey) < 1e-5f && fabs(r.GoToPos.Z - ez) < 1e-5f);
        CHECK(same(r.LookAtPos.X, 3.0f) && same(r.LookAtPos.Y, -2.0f) && same(r.LookAtPos.Z, 99.0f + z));
        ++cases;
    }
    CHECK(cases == 72);
    puts("0087bd70_TEST PASS");
    return 0;
}