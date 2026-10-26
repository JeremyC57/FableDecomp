#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CCamera { public: float v; };
class CGameCameraManager { public:
    CCamera Camera; CCamera OldCamera; bool SingleCut;
    void SetSingleCut() { SingleCut = true; OldCamera = Camera; }
};
struct CPhysicsState { C3DVector Position; C3DVector Velocity; };
struct CBase {
    C3DVector LookAt; CGameCameraManager* CameraManager; CPhysicsState GoTo;
    const C3DVector& GetPosition() const { return GoTo.Position; }
    void SetPosition(const C3DVector& p) { GoTo.Position = p; }
    void SetObservedPos(const C3DVector& p) { LookAt = p; }
    CGameCameraManager& GetCameraManager() { return *CameraManager; }
};
static C3DVector V(float x, float y, float z) { C3DVector r; r.X = x; r.Y = y; r.Z = z; return r; }
struct CSimplePointBase : CBase {
    C3DVector StartPos; C3DVector Position; C3DVector LookAtPos; bool AllowLookAround;
    void Update()
    {
        SetObservedPos(LookAtPos);
        SetPosition(Position);
    }
};
int main()
{
    CGameCameraManager mgr; mgr.Camera.v = 5.0f; mgr.OldCamera.v = 0.0f; mgr.SingleCut = false;
    CSimplePointBase m; m.CameraManager = &mgr;
    m.GoTo.Position = V(1.0f, 2.0f, 3.0f); m.LookAt = V(0.0f, 0.0f, 0.0f);
    m.StartPos = V(-1.0f, -2.0f, -3.0f); m.Position = V(4.0f, 5.0f, 6.0f); m.LookAtPos = V(7.0f, 8.0f, 9.0f);
    m.Update();
    if (m.LookAt.X != 7.0f || m.GoTo.Position.X != 4.0f || m.GoTo.Position.Z != 6.0f || mgr.SingleCut) { std::printf("00873530_TEST FAIL\n"); return 1; }
    std::printf("00873530_TEST PASS\n");
    return 0;
}