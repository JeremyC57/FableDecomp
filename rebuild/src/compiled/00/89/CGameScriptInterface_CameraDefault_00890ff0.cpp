// Leaf: ?CameraDefault@CGameScriptInterface@@UAEXXZ
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
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its PPlayerManager scoped_ptr +0x1C through the inline GetPlayerManager() (FableWin.pdb ?GetPlayerManager@CMainGameComponent@@QAEAAVCPlayerManager@@XZ); was the stand-in GlobalOverlay* gCameraGlobal.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#include "engine/CMainGameComponent.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */

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
    virtual void CameraDefault();
};

void CGameScriptInterface::CameraDefault()
{
    CPlayer* context = (&CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer());
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