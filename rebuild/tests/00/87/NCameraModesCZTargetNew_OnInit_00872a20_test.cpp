// Behaviour test for NCameraModes::CZTargetNew::OnInit 0x00872A20: Z offset inherited, axes from the reversed facing.
#include <cstdio>
#include <cmath>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    void Normalise() { float l = (float)std::sqrt(X * X + Y * Y + Z * Z); if (l > 0) { X /= l; Y /= l; Z /= l; } }
    C3DVector GetPerpendicularXY() const { return C3DVector(Y, -X, 0.0f); }
};
struct CThingPhysics { C3DVector Facing; C3DVector GetFacingVector() const { return Facing; } };
struct CThingGameObject { CThingPhysics* m_pPhysics; CThingPhysics* GetPhysics() const { return m_pPhysics; } };
struct CBase;
static int g_resets = 0;
struct CGameCameraManager { CBase* CurrentModeData; void ResetRightStickLookAroundVector() { ++g_resets; } };
static int g_transitions = 0, g_coords = 0;
struct CBase {
    CGameCameraManager* CameraManager; float ObservedPositionZOffset; const CThingGameObject* Observed;
    const CThingGameObject* GetObservedThing() const { return Observed; }
    virtual void UpdateCoordSystem() { }
    virtual ~CBase() { }
};
struct CZTargetNew : CBase {
    C3DVector XAxis, YAxis;
    void InitialiseTransitionVariables() { ++g_transitions; }
    virtual void UpdateCoordSystem() { ++g_coords; }
    void OnInit();
};

void CZTargetNew::OnInit()
{
    ObservedPositionZOffset = CameraManager->CurrentModeData->ObservedPositionZOffset;
    CameraManager->ResetRightStickLookAroundVector();
    XAxis = GetObservedThing()->GetPhysics()->GetFacingVector() * -1.0f;
    XAxis.Z = 0.0f;
    XAxis.Normalise();
    YAxis = XAxis.GetPerpendicularXY();
    InitialiseTransitionVariables();
    UpdateCoordSystem();
}

int main()
{
    CBase previous; previous.ObservedPositionZOffset = 1.25f;
    CGameCameraManager mgr; mgr.CurrentModeData = &previous;
    CThingPhysics phys; phys.Facing = C3DVector(0.0f, 3.0f, 4.0f);
    CThingGameObject thing; thing.m_pPhysics = &phys;
    CZTargetNew mode; mode.CameraManager = &mgr; mode.ObservedPositionZOffset = 0.0f; mode.Observed = &thing;
    mode.OnInit();
    bool ok = mode.ObservedPositionZOffset == 1.25f && g_resets == 1 && g_transitions == 1 && g_coords == 1;
    if (mode.XAxis.X != 0.0f || mode.XAxis.Y != -1.0f || mode.XAxis.Z != 0.0f) ok = false;
    if (mode.YAxis.X != -1.0f || mode.YAxis.Y != 0.0f || mode.YAxis.Z != 0.0f) ok = false;
    if (!ok) { std::printf("00872a20_TEST FAIL\n"); return 1; }
    std::printf("00872a20_TEST PASS\n");
    return 0;
}