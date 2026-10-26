#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static bool g_queued = false; static long g_action = 0, g_type = -1;
class CGameCameraManager { public: void PushCurrentManualMode() { std::strcat(g_seq, "P"); } };
static CGameCameraManager g_manager;
class CAIGameCameraBase { public: virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { }
    virtual void GetCameraManager(CGameCameraManager** ppManager) { std::strcat(g_seq, "G"); *ppManager = &g_manager; } };
class CWorld { public: unsigned char _pad[0x34]; CAIGameCameraBase* PCurrentGameCamera; };
struct CInputProcessGameBase { virtual void v00() { } unsigned char _pad04[0x10]; CWorld* World; void* MyPlayer; void* DefinitionManager; void* Component; void* PGamePlayerInterface;
    bool IsGameActionEventInQueue(long action, long buttonEventType) { std::strcat(g_seq, "Q"); g_action = action; g_type = buttonEventType; return g_queued; } };
class CInputProcessControlCreatureRightStick : public CInputProcessGameBase { public:
    bool PushingCameraModeUp; bool PushingCameraModeDown; bool LookingAround;
    void Update(float)
    {
        if (!IsGameActionEventInQueue(0x3d, 0)) {
            CGameCameraManager* cameraManager = 0;
            World->PCurrentGameCamera->GetCameraManager(&cameraManager);
            if (LookingAround) cameraManager->PushCurrentManualMode();
            PushingCameraModeUp = false; PushingCameraModeDown = false; LookingAround = false;
        }
    }
};
int main()
{
    CWorld w; CAIGameCameraBase cam; w.PCurrentGameCamera = &cam; CInputProcessControlCreatureRightStick s; s.World = &w; s.PushingCameraModeUp = true; s.PushingCameraModeDown = true; s.LookingAround = true;
    s.Update(0.1f); if (std::strcmp(g_seq, "QGP") || g_action != 0x3d || g_type != 0 || s.PushingCameraModeUp || s.PushingCameraModeDown || s.LookingAround) { std::printf("0068d840_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; s.PushingCameraModeUp = true; s.Update(0.1f); if (std::strcmp(g_seq, "QG") || s.PushingCameraModeUp) { std::printf("0068d840_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_queued = true; s.PushingCameraModeUp = true; s.LookingAround = true; s.Update(0.1f); if (std::strcmp(g_seq, "Q") || !s.PushingCameraModeUp || !s.LookingAround) { std::printf("0068d840_TEST FAIL\n"); return 1; }
    std::printf("0068d840_TEST PASS\n");
    return 0;
}