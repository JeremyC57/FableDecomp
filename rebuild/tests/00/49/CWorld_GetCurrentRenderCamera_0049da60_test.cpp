// Behaviour test for CWorld::GetCurrentRenderCamera() const (0x0049DA60): the free camera while in use, else the game
// camera, both at fraction 1.0.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CCamera {
public:
    C3DVector WorldPosition;
    float     Zoom;
    long      Source;                         // test tag: 1 free camera, 2 game camera
    float     Fraction;                       // test tag: the fraction asked for
    CCamera(long source, float t) : Zoom(1.0f), Source(source), Fraction(t) { WorldPosition.X = WorldPosition.Y = WorldPosition.Z = 0.0f; }
    CCamera(const CCamera& o) : WorldPosition(o.WorldPosition), Zoom(o.Zoom), Source(o.Source), Fraction(o.Fraction) {}
};
struct CInterpolationInfoSet { float GTPredictedTimeSinceLastRenderFrame; float WFInterpolate; };
class CInterpolationInfo {
public:
    CInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent, BulletTimeLast;
    long WFServerCurrentTime;
};
class CPlayer { public:
    bool UsingFreeCamera;
    CCamera GetFreeCamera(float t) const { return CCamera(1, t); }
};
class CPlayerManager { public: CPlayer* Main; CPlayer* GetMainPlayer() { return Main; } };
class CAIGameCameraBase { public:
    virtual ~CAIGameCameraBase() {}
    virtual CCamera GetCamera(float t) const { return CCamera(2, t); }
};
class CWorld {
public:
    virtual ~CWorld() {}
    virtual CCamera GetCurrentRenderCamera() const;
    virtual CCamera GetCurrentRenderCamera(const CInterpolationInfo& info) const;
    CPlayerManager*    PlayerManager;
    CAIGameCameraBase* PCurrentGameCamera;
    bool               Paused;
};

CCamera CWorld::GetCurrentRenderCamera() const
{
    CPlayer* player = PlayerManager->GetMainPlayer();
    if (player->UsingFreeCamera)
        return player->GetFreeCamera(1.0f);
    return PCurrentGameCamera->GetCamera(1.0f);
}
CCamera CWorld::GetCurrentRenderCamera(const CInterpolationInfo&) const { return CCamera(0, 0.0f); }

int main()
{
    CPlayer player; player.UsingFreeCamera = false;
    CPlayerManager pm; pm.Main = &player;
    CAIGameCameraBase game;
    CWorld w; w.PlayerManager = &pm; w.PCurrentGameCamera = &game; w.Paused = false;
    CCamera a = w.GetCurrentRenderCamera();
    player.UsingFreeCamera = true;
    CCamera b = w.GetCurrentRenderCamera();
    if (a.Source != 2 || a.Fraction != 1.0f || b.Source != 1 || b.Fraction != 1.0f)
        { std::printf("0049da60_TEST FAIL\n"); return 1; }
    std::printf("0049da60_TEST PASS\n");
    return 0;
}