class CGameCameraManager
{
public:
    void ResetCamera();
    void ReleaseScriptedControl();
};

struct CameraTarget
{
    unsigned char unused00[0x91];
    unsigned char flags91;
};
class CThingPlayerCreature : public CameraTarget { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
struct CameraProvider
{
    virtual void unused00();
    virtual void unused04();
    virtual void unused08();
    virtual void unused0C();
    virtual void unused10();
    virtual void unused14();
    virtual void unused18();
    virtual void unused1C();
    virtual void unused20();
    virtual void GetCameraManager(CGameCameraManager** manager);
};
struct WorldOverlay
{
    unsigned char unused00[0x34];
    CameraProvider* cameraProvider;
};
struct GlobalOverlay
{
    unsigned char unused00[0x1C];
    void* cameraOwner;
};

extern GlobalOverlay* gCameraGlobal;
class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */

class CGameScriptInterface
{
public:
    virtual void unused00();
    virtual void unused04();
    virtual void unused08();
    virtual void unused0C();
    virtual void unused10();
    virtual void unused14();
    virtual void unused18();
    virtual void ReleaseOneCameraLayer();
    WorldOverlay* world;
    unsigned char padding08[0x40];
    long cameraLayerCount;
    void CameraDefault();
};

void CGameScriptInterface::CameraDefault()
{
    CPlayer* context = (&((CPlayerManager*)gCameraGlobal->cameraOwner)->GetMainPlayer());
    CameraTarget* target = context->GetPControlledCreature();
    if (target != 0 && (target->flags91 & 1) == 0)
    {
        CGameCameraManager* cameraManager;
        world->cameraProvider->GetCameraManager(&cameraManager);
        if (cameraLayerCount >= 0)
        {
            cameraManager->ResetCamera();
            do
            {
                ReleaseOneCameraLayer();
                --cameraLayerCount;
            }
            while (cameraLayerCount != -1);
            cameraLayerCount = -1;
            return;
        }
        cameraManager->ReleaseScriptedControl();
        cameraLayerCount = -1;
    }
}