// Behaviour test for 0x0049DBE0 CWorld::GetGameLODCentre: per-frame cache, free camera vs game camera source.
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
                   C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct CCamera { C3DVector WorldPosition; float rest[11]; };
static int g_cameraCalls = 0;
struct CAIGameCameraBase {
    C3DVector pos;
    virtual ~CAIGameCameraBase() { }
    virtual CCamera GetRenderCamera(float) const { ++g_cameraCalls; CCamera c; c.WorldPosition = pos; return c; }
};
struct CPlayer { CCamera CurrentFreeCamera; bool UsingFreeCamera; };
struct CPlayerManager { CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
struct CMainGameComponent { long frame; long GetFrame() { return frame; } };
struct CWorld {
    CMainGameComponent* Component; CPlayerManager* PlayerManager; CAIGameCameraBase* PCurrentGameCamera;
    mutable long FrameCachedLODCentre; mutable C3DVector CachedLODCentre;
    C3DVector GetGameLODCentre() const
    {
        if (FrameCachedLODCentre == -1 || FrameCachedLODCentre != Component->GetFrame()) {
            if (PlayerManager->GetMainPlayer()->UsingFreeCamera)
                CachedLODCentre = PlayerManager->GetMainPlayer()->CurrentFreeCamera.WorldPosition;
            else
                CachedLODCentre = PCurrentGameCamera->GetRenderCamera(0.0f).WorldPosition;
            FrameCachedLODCentre = Component->GetFrame();
        }
        return CachedLODCentre;
    }
};
int main()
{
    CMainGameComponent comp; comp.frame = 10;
    CPlayerManager pm; pm.player.UsingFreeCamera = false; pm.player.CurrentFreeCamera.WorldPosition = C3DVector(7.0f, 8.0f, 9.0f);
    CAIGameCameraBase cam; cam.pos = C3DVector(1.0f, 2.0f, 3.0f);
    CWorld w; w.Component = &comp; w.PlayerManager = &pm; w.PCurrentGameCamera = &cam; w.FrameCachedLODCentre = -1;
    C3DVector a = w.GetGameLODCentre();
    bool ok = a.X == 1.0f && a.Z == 3.0f && g_cameraCalls == 1 && w.FrameCachedLODCentre == 10;
    cam.pos = C3DVector(4.0f, 5.0f, 6.0f);
    C3DVector b = w.GetGameLODCentre();                 // same frame: cached
    ok = ok && b.X == 1.0f && g_cameraCalls == 1;
    comp.frame = 11; pm.player.UsingFreeCamera = true;
    C3DVector c = w.GetGameLODCentre();
    ok = ok && c.X == 7.0f && c.Y == 8.0f && g_cameraCalls == 1 && w.FrameCachedLODCentre == 11;
    if (!ok) { std::printf("0049dbe0_TEST FAIL\n"); return 1; }
    std::printf("0049dbe0_TEST PASS\n");
    return 0;
}