// Behaviour test for 0x008735D0 NCameraModes::CFirstPerson::OnInit: base, flattened normalised axis, zeroed angles,
// stick copy, constants and the camera cut.
#include <cstdio>
#include <cmath>
#include <cstring>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    void Normalise() { float l = (float)std::sqrt(X * X + Y * Y + Z * Z); if (l > 0) { X /= l; Y /= l; Z /= l; } }
};
class C2DVector { public: float X, Y; };
class CCamera { public: unsigned char _body[0x38]; };
struct CThingPhysics { C3DVector facing; C3DVector GetFacingVector() const { return facing; } };
class CThing { public:
    CThingPhysics* m_pPhysics; C3DVector pos;
    CThingPhysics* GetPhysics() const { return m_pPhysics; }
    const C3DVector& GetPosition() const { return pos; }
};
class CThingGameObject : public CThing { public:};
class CGameCameraManager {
public:
    CCamera Camera; CCamera OldCamera; bool SingleCut;
    void CutCamera() { OldCamera = Camera; SingleCut = true; }
};
namespace NCameraModes {
class CBase {
public:
    CGameCameraManager* CameraManager; C2DVector LeftStickVector; const CThingGameObject* observed;
    const CThingGameObject* GetObservedThing() const { return observed; }
};
class CFirstPerson : public CBase {
public:
    float OffsetHeight, CylinderRadius; C3DVector Base, PrincipleAxis; float Theta, Phi; C2DVector RSVector;
    void OnInit();
};
}
void NCameraModes::CFirstPerson::OnInit()
{
    Base = GetObservedThing()->GetPosition();
    PrincipleAxis = GetObservedThing()->GetPhysics()->GetFacingVector();
    PrincipleAxis.Z = 0.0f;
    PrincipleAxis.Normalise();
    Theta = 0.0f;
    Phi = 0.0f;
    CylinderRadius = 0.015f;
    OffsetHeight = 1.45f;
    RSVector = LeftStickVector;
    CameraManager->CutCamera();
}
int main()
{
    CThingPhysics phys; phys.facing = C3DVector(3.0f, 4.0f, 9.0f);
    CThingGameObject thing; thing.m_pPhysics = &phys; thing.pos = C3DVector(1.0f, 2.0f, 3.0f);
    CGameCameraManager mgr; std::memset(&mgr, 0, sizeof(mgr)); mgr.Camera._body[5] = 42;
    NCameraModes::CFirstPerson fp; fp.CameraManager = &mgr; fp.observed = &thing; fp.LeftStickVector.X = 0.5f; fp.LeftStickVector.Y = -0.25f;
    fp.Theta = fp.Phi = 7.0f;
    fp.OnInit();
    int fail = 0;
    if (fp.Base.X != 1.0f || fp.Base.Y != 2.0f || fp.Base.Z != 3.0f) fail |= 1;
    if (std::fabs(fp.PrincipleAxis.X - 0.6f) > 1e-5f || std::fabs(fp.PrincipleAxis.Y - 0.8f) > 1e-5f || fp.PrincipleAxis.Z != 0.0f) fail |= 2;
    if (fp.Theta != 0.0f || fp.Phi != 0.0f || fp.RSVector.X != 0.5f || fp.RSVector.Y != -0.25f) fail |= 4;
    if (fp.CylinderRadius != 0.015f || fp.OffsetHeight != 1.45f) fail |= 8;
    if (mgr.OldCamera._body[5] != 42 || !mgr.SingleCut) fail |= 16;
    if (fail) { std::printf("008735d0_TEST FAIL\n"); return 1; }
    std::printf("008735d0_TEST PASS\n");
    return 0;
}