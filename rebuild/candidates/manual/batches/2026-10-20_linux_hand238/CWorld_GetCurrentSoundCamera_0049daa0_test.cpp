// Behaviour test for CWorld::GetCurrentSoundCamera (0x0049DAA0): each source of the sound camera in priority order.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CCamera { C3DVector WorldPosition; int Source; };
struct CInterpolationInfo { long Frame; float Fraction; };
static float g_tSeen;
struct CTCPhysicsBase { C3DVector Pos; C3DVector GetInterpolatedPosition(float t) const { g_tSeen = t; return Pos; } };
struct CThing { CTCPhysicsBase* PPhysics; };
struct CPlayer {
    bool UsingFreeCamera; CThing* Hero;
    CCamera GetFreeCamera(float t) const { CCamera c = { { t, 0, 0 }, 3 }; return c; }
    CThing* GetHeroThing() { return Hero; }
};
struct CPlayerManager { CPlayer* P; CPlayer* GetMainPlayer() { return P; } };
struct CGameCameraManager { bool Scripted; bool IsUsingScriptedCameraMode() const { return Scripted; } };
struct CAIGameCameraBase {
    CGameCameraManager* M;
    CCamera GetCamera(float t) const { CCamera c = { { 7.0f, 8.0f, 9.0f }, 4 }; c.WorldPosition.Y += t; return c; }
    bool GetCameraManager(CGameCameraManager*& m) { m = M; return M != 0; }
};
struct CMainGameComponent { bool Editor; bool IsEditorActive() { return Editor; } };
static CMainGameComponent g_mgc; static CMainGameComponent* g_pMainGameComponent = &g_mgc; static void* g_pSoundManager;
struct CWorld {
    CPlayerManager* PlayerManager; CAIGameCameraBase* PCurrentGameCamera;
    CCamera GetCurrentRenderCamera() const { CCamera c = { { 0, 0, 0 }, 2 }; return c; }
    CCamera GetCurrentRenderCamera(const CInterpolationInfo&) const { CCamera c = { { 0, 0, 0 }, 1 }; return c; }
    CCamera GetCurrentSoundCamera(const CInterpolationInfo& info) const
    {
        float t = info.Fraction;
        if (!g_pSoundManager)
            return GetCurrentRenderCamera(info);
        if (g_pMainGameComponent->IsEditorActive())
            return GetCurrentRenderCamera();
        CPlayer* player = PlayerManager->GetMainPlayer();
        if (player->UsingFreeCamera)
            return player->GetFreeCamera(t);
        CGameCameraManager* manager;
        if (PCurrentGameCamera->GetCameraManager(manager) && !manager->IsUsingScriptedCameraMode() && player->GetHeroThing()) {
            CCamera camera = PCurrentGameCamera->GetCamera(t);
            const CTCPhysicsBase* physics = player->GetHeroThing()->PPhysics;
            camera.WorldPosition = physics->GetInterpolatedPosition(t);
            return camera;
        }
        return PCurrentGameCamera->GetCamera(t);
    }
};
int main()
{
    CTCPhysicsBase phys = { { 1.0f, 2.0f, 3.0f } }; CThing hero = { &phys };
    CPlayer p = { false, &hero }; CPlayerManager pm = { &p }; CGameCameraManager mgr = { false };
    CAIGameCameraBase cam = { &mgr }; CWorld w = { &pm, &cam }; CInterpolationInfo info = { 0, 0.5f };
    int dummy;
    g_pSoundManager = 0; bool ok = w.GetCurrentSoundCamera(info).Source == 1;
    g_pSoundManager = &dummy; g_mgc.Editor = true; ok = ok && w.GetCurrentSoundCamera(info).Source == 2;
    g_mgc.Editor = false; p.UsingFreeCamera = true; CCamera f = w.GetCurrentSoundCamera(info); ok = ok && f.Source == 3 && f.WorldPosition.X == info.Fraction;
    p.UsingFreeCamera = false; CCamera h = w.GetCurrentSoundCamera(info);
    ok = ok && h.Source == 4 && h.WorldPosition.X == phys.Pos.X && h.WorldPosition.Y == phys.Pos.Y && g_tSeen == info.Fraction;
    mgr.Scripted = true; CCamera s = w.GetCurrentSoundCamera(info); float y = 8.5f; ok = ok && s.Source == 4 && s.WorldPosition.Y == y;
    mgr.Scripted = false; p.Hero = 0; CCamera n = w.GetCurrentSoundCamera(info); ok = ok && n.WorldPosition.Y == y;
    cam.M = 0; p.Hero = &hero; CCamera m = w.GetCurrentSoundCamera(info); ok = ok && m.WorldPosition.Y == y;
    if (!ok) { std::printf("0049daa0_TEST FAIL\n"); return 1; }
    std::printf("0049daa0_TEST PASS\n");
    return 0;
}
