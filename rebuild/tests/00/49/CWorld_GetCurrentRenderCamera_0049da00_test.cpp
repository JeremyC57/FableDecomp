// Behaviour test for CWorld::GetCurrentRenderCamera(const CInterpolationInfo&) const (0x0049DA00): free camera at the
// paused or current fraction while in use, else the game camera at the current fraction.
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

CCamera CWorld::GetCurrentRenderCamera(const CInterpolationInfo& info) const
{
    CPlayer* player = PlayerManager->GetMainPlayer();
    if (player->UsingFreeCamera) {
        float t = Paused ? info.PausedCurrent.WFInterpolate : info.Current.WFInterpolate;
        return player->GetFreeCamera(t);
    }
    return PCurrentGameCamera->GetCamera(info.Current.WFInterpolate);
}
CCamera CWorld::GetCurrentRenderCamera() const { return CCamera(0, 0.0f); }

int main()
{
    CPlayer player; player.UsingFreeCamera = false;
    CPlayerManager pm; pm.Main = &player;
    CAIGameCameraBase game;
    CWorld w; w.PlayerManager = &pm; w.PCurrentGameCamera = &game; w.Paused = false;
    CInterpolationInfo info;
    info.Current.WFInterpolate = 0.25f; info.PausedCurrent.WFInterpolate = 0.75f;
    CCamera a = w.GetCurrentRenderCamera(info);
    w.Paused = true;
    CCamera b = w.GetCurrentRenderCamera(info);          /* game camera ignores Paused */
    player.UsingFreeCamera = true;
    CCamera c = w.GetCurrentRenderCamera(info);          /* free camera, paused fraction */
    w.Paused = false;
    CCamera d = w.GetCurrentRenderCamera(info);          /* free camera, current fraction */
    if (a.Source != 2 || a.Fraction != 0.25f || b.Source != 2 || b.Fraction != 0.25f
        || c.Source != 1 || c.Fraction != 0.75f || d.Source != 1 || d.Fraction != 0.25f)
        { std::printf("0049da00_TEST FAIL\n"); return 1; }
    std::printf("0049da00_TEST PASS\n");
    return 0;
}