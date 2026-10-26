#include <cstdio>
#include <cmath>
#include <cstring>
// Behaviour model of NCameraModes::CScriptedControl::OnDie @ 0x0087BBF0 (copy of the source logic, stand-in callees).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};
class CRightHandedSet { public: C3DVector Up, Forward; };
class CCamera { public:
    C3DVector WorldPosition; CRightHandedSet RHSet; float HorizontalFOV;
    float GetHorizontalFOV() const { return HorizontalFOV; }
};
class CThing { public: C3DVector pos; unsigned char m_flags91; const C3DVector& GetPosition() const { return pos; } };
static bool IS_THING_ALIVE(const CThing* t) { return t && !(t->m_flags91 & 1); }
class CGameCameraManager { public: CCamera Camera, OldCamera; bool SingleCut; void CutCamera() { OldCamera = Camera; SingleCut = true; } };
struct CScriptModuleBase { bool finished; bool IsFinished() { return finished; } };
struct CScriptedControl {
    C3DVector LookAtPosition, GoToPosition; float CameraFOV;
    CGameCameraManager* CameraManager; CScriptModuleBase* PScriptModule; CCamera InitCamera; const CThing* observed;
    const CThing* GetObservedThing() const { return observed; }
    void OnDie()
    {
        if (PScriptModule->IsFinished()) {
            C3DVector position = InitCamera.WorldPosition;
            float distance = 1.0f;
            if (IS_THING_ALIVE(GetObservedThing()))
                distance = (position - GetObservedThing()->GetPosition()).GetLength();
            GoToPosition = position;
            LookAtPosition = InitCamera.RHSet.Forward * distance + position;
            CameraFOV = InitCamera.GetHorizontalFOV();
            CameraManager->CutCamera();
        }
    }
};
int main()
{
    static CGameCameraManager mgr; std::memset(&mgr, 0, sizeof(mgr)); mgr.Camera.HorizontalFOV = 9.0f;
    CScriptModuleBase module = { true };
    CThing thing; thing.pos = C3DVector(3, 4, 0); thing.m_flags91 = 0;
    static CScriptedControl c; std::memset(&c, 0, sizeof(c));
    c.CameraManager = &mgr; c.PScriptModule = &module; c.observed = &thing;
    c.InitCamera.WorldPosition = C3DVector(0, 0, 0); c.InitCamera.RHSet.Forward = C3DVector(0, 1, 0); c.InitCamera.HorizontalFOV = 1.5f;
    c.OnDie();                                               // distance 5
    int ok = c.GoToPosition.X == 0.0f && c.LookAtPosition.Y == 5.0f && c.CameraFOV == 1.5f && mgr.SingleCut && mgr.OldCamera.HorizontalFOV == 9.0f;
    thing.m_flags91 = 1; mgr.SingleCut = false; c.OnDie();   // dead: distance 1
    ok = ok && c.LookAtPosition.Y == 1.0f && mgr.SingleCut;
    module.finished = false; mgr.SingleCut = false; c.CameraFOV = 0; c.OnDie();
    ok = ok && !mgr.SingleCut && c.CameraFOV == 0;
    if (!ok) { std::printf("0087bbf0_TEST FAIL\n"); return 1; }
    std::printf("0087bbf0_TEST PASS\n");
    return 0;
}