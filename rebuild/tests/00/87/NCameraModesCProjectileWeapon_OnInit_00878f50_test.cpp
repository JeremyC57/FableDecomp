// Behaviour test for NCameraModes::CProjectileWeapon::OnInit (0x00878F50): single cut (old camera = camera), carry the orbit
// over from a current projectile-weapon mode, else set up params with the zoom FOV at the camera FOV; one Update either way.
#include <stdio.h>
static int g_setUp = 0, g_updates = 0;
class C3DVector { public: float X, Y, Z; };
class CCamera { public: C3DVector WorldPosition; float HorizontalFOV; };
namespace NCameraModes {
enum EMode { EMode_Other = 3, EMode_ProjectileWeapon = 15 };
class CBase;
}
class CGameCameraManager { public:
    CCamera Camera, OldCamera;
    NCameraModes::CBase* CurrentMode;
    NCameraModes::EMode CurrentType;
    bool SingleCut;
    bool HasCameraMode(NCameraModes::EMode mode) const { return CurrentMode != 0 && CurrentType == mode; }
    template <class T> bool GetCameraMode(T** mode)
    {
        if (HasCameraMode(T::StatGetModeType())) { *mode = (T*)CurrentMode; return true; }
        return false;
    }
    void SetSingleCut() { OldCamera = Camera; SingleCut = true; }
};
namespace NCameraModes {
class CBase { public:
    virtual ~CBase() { }
    virtual void Update() { g_updates++; }
    CGameCameraManager* CameraManager;
    float CameraFOV;
};
class CProjectileWeapon : public CBase { public:
    C3DVector XAxis, YAxis, ZAxis;
    float Phi, HeightParam, DeltaPhi, DeltaHeightParam, LockDeltaPhi, LockDeltaHeightParam, DeltaLockDeltaPhi, DeltaLockDeltaHeightParam;
    float ZoomInFOV;
    static EMode StatGetModeType() { return EMode_ProjectileWeapon; }
    void SetUpParams() { g_setUp++; Phi = 1.0f; }
    void OnInit()
    {
        CameraManager->SetSingleCut();
        CProjectileWeapon* previous;
        if (CameraManager->GetCameraMode(&previous)) {
            LockDeltaPhi = previous->LockDeltaPhi;
            LockDeltaHeightParam = previous->LockDeltaHeightParam;
            DeltaLockDeltaPhi = previous->DeltaLockDeltaPhi;
            DeltaLockDeltaHeightParam = previous->DeltaLockDeltaHeightParam;
            Phi = previous->Phi;
            HeightParam = previous->HeightParam;
            DeltaPhi = previous->DeltaPhi;
            DeltaHeightParam = previous->DeltaHeightParam;
            ZoomInFOV = previous->ZoomInFOV;
            XAxis = previous->XAxis;
            YAxis = previous->YAxis;
            ZAxis = previous->ZAxis;
        } else {
            SetUpParams();
            ZoomInFOV = CameraFOV;
        }
        Update();
    }
};
}
using namespace NCameraModes;
int main()
{
    CGameCameraManager mgr;
    mgr.Camera.WorldPosition.X = 7.0f; mgr.Camera.HorizontalFOV = 0.9f; mgr.OldCamera.WorldPosition.X = 0; mgr.SingleCut = false;
    CProjectileWeapon prev;
    prev.Phi = 2.0f; prev.HeightParam = 3.0f; prev.DeltaPhi = 4.0f; prev.DeltaHeightParam = 5.0f; prev.LockDeltaPhi = 6.0f;
    prev.LockDeltaHeightParam = 7.0f; prev.DeltaLockDeltaPhi = 8.0f; prev.DeltaLockDeltaHeightParam = 9.0f; prev.ZoomInFOV = 0.5f;
    prev.XAxis.X = 1; prev.YAxis.Y = 1; prev.ZAxis.Z = 1;
    mgr.CurrentMode = &prev; mgr.CurrentType = EMode_ProjectileWeapon;
    CProjectileWeapon mode; mode.CameraManager = &mgr; mode.CameraFOV = 1.2f;
    mode.OnInit();
    if (!mgr.SingleCut || mgr.OldCamera.WorldPosition.X != 7.0f || g_setUp != 0 || g_updates != 1 || mode.Phi != 2.0f
        || mode.DeltaLockDeltaHeightParam != 9.0f || mode.ZoomInFOV != 0.5f || mode.XAxis.X != 1 || mode.ZAxis.Z != 1)
        { printf("00878f50_TEST FAIL\n"); return 1; }
    mgr.CurrentType = EMode_Other;                       // a different camera: set up afresh
    CProjectileWeapon fresh; fresh.CameraManager = &mgr; fresh.CameraFOV = 1.2f;
    fresh.OnInit();
    if (g_setUp != 1 || g_updates != 2 || fresh.ZoomInFOV != 1.2f || fresh.Phi != 1.0f) { printf("00878f50_TEST FAIL\n"); return 1; }
    printf("00878f50_TEST PASS\n");
    return 0;
}