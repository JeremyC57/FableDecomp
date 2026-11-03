// CWorld::UpdateFrame() -- one game frame of the world: the DVD crash-marker cleanup, the save-load-failed reset prompt, the pending
//   save-game / region load and player-spawn steps, then (unpaused) every manager's frame update in retail order, the frame counter,
//   the console pause-at-frame check and the atmos / speech-gain updates
// Leaf: ?UpdateFrame@CWorld@@IAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A5A40, 1102 bytes, thiscall, plain `ret` (Ego_r.pdb / FableWin.pdb ?UpdateFrame@CWorld@@IAEXXZ).
// Callees: GFGetTime 0x009E1BC0, CAFile::PathExists 0x00999230, CDriveManager::DeleteFileW 0x009E4BC0 (singleton 0x013CAA28),
//   NProgressDisplay::StartProgress 0x009E9EF0 / StopProgress 0x009E9F20, CGamePlayerInterface::IsGameActionEventInQueue 0x00446B10,
//   CPlayer::GetJoystickDeviceNumber 0x00487510 / UpdateFreeCamera 0x004884D0 / Update 0x00488AB0, CInitBaseClass::IsValid 0x0099A330,
//   CWorld::ResetToFrontEnd 0x0049DCC0 / OpenHeroInformationScreen 0x0049F3F0 / UpdateSaveGameLoad 0x004A2F10 / UpdateRegionLoad
//   0x004A3740 / PrepareAtmosBanksForLoadedRegion 0x0049E5E0 / UpdateEnvironmentCameraPos 0x0049E370 / GetCurrentRenderCamera
//   (vtable +0x18), CPlayerGui::Get / Update 0x0043A080 / UpdateMiniMapMarkers 0x004378C0, CSubtitleRenderer::IsDisplayingText,
//   CWorldMap::GetRegion 0x004FC180 / GetLoadedRegion 0x004FB150, CAtmosProcess::SetAtmosSoundsAsMuted 0x006B1D30 / Update 0x006B2260,
//   CEnvironment::UpdateInterpolation 0x006B32A0 / Update 0x006B3FF0, CTCBase::UpdateGlobalTCs 0x00686DC0,
//   CPhysicsMesh::ResetStartInformationCount 0x00A17600, CInterpolationInfo() 0x00415A60, CNavigatorManager::FrameUpdate 0x00A14E20,
//   CWorldMap::Update, CCombatManager::Update 0x006ED200, CThingManager::WorldUpdateThings 0x0051FCC0 / UpdateDeaths 0x0051F070,
//   CBulletTimeManager::IsBulletTimeActive 0x00419680 / IsTimeForFullUpdate 0x004C6060 / FrameUpdate 0x004C5E90,
//   NParticleEngine::GetParticleEmitterManager 0x00AEAAF0 + Update 0x00AEB4A0, CQuestManager::Update 0x004B4490,
//   CGameScriptInterface::WorldUpdate 0x006E75C0, CMessageEventManager::Update 0x006874B0, CEngineManager singleton 0x00436FB0 +
//   OnUpdate 0x00640320 / OnPausedUpdate 0x00640360, CGameTimeManager::Update 0x006BB990, CScriptConversationManager::FrameUpdate
//   0x006E60F0, CMusicManager::Update 0x006BDC60, NSpeechGainManager::Update 0x006E37D0.
// Data: the function-local statics startTime 0x013B89F0 (guard bit 0x013B89F8) and crashMarkerChecked 0x013B89E8, g_bRunFromDVD 0x013B8615, g_bSaveLoadFailed
//   0x013B89C0, g_bProgressDisplaySuppressed 0x013B89D3 (descriptive names: no PDB data symbols), CWorld::Frame 0x013B89BC.
// Shape: the bullet-time reference is scoped to the else arm of the full-update choice (declared before the || chain VC7.1
//   hoists the PBulletTimeManager load and keeps a zero in edi); SetAsPaused is the inline FableWin body.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CDRIVEMANAGER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CEnvironment.h"
#include "engine/CNavigatorManager.h"
#include "engine/CBulletTimeManager.h"
#include "engine/CThingManager.h"
#include "engine/CQuestManager.h"
#include "engine/CCombatManager.h"
#include "engine/CDriveManager.h"
#include "engine/CAFile.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CTCBase.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CCamera.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CGameScriptInterface.h"
#include "fable_game.h"
#include "fable_string.h"

// Classes without a shared header: member functions only (Ego_r.pdb / FableWin.pdb spellings).
class CAtmosProcess {
public:
    void SetAtmosSoundsAsMuted(bool muted, float fadeTime);      // 0x006B1D30 ?SetAtmosSoundsAsMuted@CAtmosProcess@@QAEX_NM@Z
    void Update();                                               // 0x006B2260 ?Update@CAtmosProcess@@QAEXXZ
};
class CMusicManager {
public:
    void Update();                                               // 0x006BDC60 ?Update@CMusicManager@@QAEXXZ
};
class CMessageEventManager {
public:
    void Update();                                               // 0x006874B0 ?Update@CMessageEventManager@@QAEXXZ
};
class CScriptConversationManager {
public:
    void FrameUpdate();                                          // 0x006E60F0 ?FrameUpdate@CScriptConversationManager@@QAEXXZ
};
class CGameTimeManager {
public:
    void Update(float timeStep);                                 // 0x006BB990 ?Update@CGameTimeManager@@QAEXM@Z
    float GetTime() const { return Time; }                       // FableWin ?GetTime@CGameTimeManager@@QBEMXZ (inline)
private:
    CWorld*             World;                                   // +0x00
    CMainGameComponent* GameComponent;                           // +0x04
    float               Time;                                    // +0x08
};
class CInterpolationInfo {
public:
    CInterpolationInfo();                                        // 0x00415A60 ??0CInterpolationInfo@@QAE@XZ
private:
    unsigned char Sets[0x34];                                    // Current .. WFServerCurrentTime (Ego_r.pdb, 0x34 bytes)
};
class CPhysicsMesh {
public:
    static void __fastcall ResetStartInformationCount();         // 0x00A17600 ?ResetStartInformationCount@CPhysicsMesh@@SIXXZ
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                                  // 0x00436FB0 ?Get@?$CTBaseSingleton@VCEngineManager@@@@SIAAVCEngineManager@@XZ
};
class CEngineManager : public CTBaseSingleton<CEngineManager> {
public:
    void OnUpdate(bool fullUpdate);                              // 0x00640320 ?OnUpdate@CEngineManager@@QAEX_N@Z
    void OnPausedUpdate();                                       // 0x00640360 ?OnPausedUpdate@CEngineManager@@QAEXXZ
};
namespace NParticleEngine {
class CParticleEmitterManager {
public:
    void Update(bool fullUpdate);                                // 0x00AEB4A0 ?Update@CParticleEmitterManager@NParticleEngine@@QAEX_N@Z
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager(); // 0x00AEAAF0 ?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ
}
namespace NSpeechGainManager {
void __fastcall Update(const CWorld& world);                     // 0x006E37D0 ?Update@NSpeechGainManager@@YIXABVCWorld@@@Z
}
namespace NProgressDisplay {
void __fastcall StartProgress(bool, bool, bool, bool, bool, bool);   // 0x009E9EF0 ?StartProgress@NProgressDisplay@@YIX_N00000@Z
void __fastcall StopProgress(bool, bool);                            // 0x009E9F20 ?StopProgress@NProgressDisplay@@YIX_N0@Z
}
double __fastcall GFGetTime();                                   // 0x009E1BC0 ?GFGetTime@@YINXZ
extern bool g_bRunFromDVD;                                       // 0x013B8615 (descriptive; no PDB data symbol)
extern bool g_bSaveLoadFailed;                                   // 0x013B89C0 (descriptive; set by CMainGameComponent::PostInit)
extern bool g_bProgressDisplaySuppressed;                        // 0x013B89D3 (descriptive)
void CWorld::UpdateFrame()
{
    static double startTime = GFGetTime();
    static bool crashMarkerChecked = false;
    if (!crashMarkerChecked && GFGetTime() - startTime > 20.0) {
        if (g_bRunFromDVD) {
            if (CAFile::PathExists(L"Z:\\game_crash_marker.dat"))
                CTBaseSingletonNoDefaultAlloc<CDriveManager>::Get().DeleteFileW(L"Z:\\game_crash_marker.dat");
        }
        crashMarkerChecked = true;
    }
    if (g_bSaveLoadFailed) {
        if (!WaitingForResetToFrontEndConfirmation) {
            g_bProgressDisplaySuppressed = true;
            NProgressDisplay::StartProgress(true, false, false, false, false, false);
            WaitingForResetToFrontEndConfirmation = true;
        }
        if (WaitingForResetToFrontEndConfirmation &&
            PlayerManager->GetMainPlayer().GetPlayerInterface()->IsGameActionEventInQueue(
                (EGameAction)59 /* GAME_ACTION_CONFIRM_RESET_TO_FRONT_END */, (EInputButtonEventType)2,
                PlayerManager->GetMainPlayer().GetJoystickDeviceNumber())) {
            g_bProgressDisplaySuppressed = false;
            NProgressDisplay::StopProgress(true, false);
            ResetToFrontEnd();
        }
        return;
    }
    if (!HasInitialisedStartTime) {
        HasInitialisedStartTime = true;
        StartTime = GFGetTime();
    }
    if (WaitingForInventoryTutorialToFinish && !CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText() && !Paused) {
        WaitingForInventoryTutorialToFinish = false;
        OpenHeroInformationScreen(HeroInformationScreenModeAfterTutorial);
    }
    if (SaveGameLoadStatus != 0) {
        UpdateSaveGameLoad();
        if (SaveGameLoadStatus == 3 || SaveGameLoadStatus == 4)
            return;
    }
    if (RegionLoadStatus != 0) {
        UpdateRegionLoad();
        if (RegionLoadStatus == 6 || RegionLoadStatus == 7 || RegionLoadStatus == 8)
            return;
    }
    if (PlayerSpawnStatus != 0) {
        if (PlayerSpawnStatus == 1) {
            Component->GetDisplayEngine().Engine3D->StartNewScene(0, 20.0f);
            PrepareAtmosBanksForLoadedRegion();
            PlayerSpawnStatus = (EPlayerSpawnStatus)0;
        }
        return;
    }
    if (PAtmosProcessor_Data)
        PAtmosProcessor_Data->SetAtmosSoundsAsMuted(!PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).IsSoundThemesEnabled(), 1000.0f);
    UpdateEnvironmentCameraPos(GetCurrentRenderCamera().PeekWorldPos());
    PEnvironment_ptr->UpdateInterpolation();
    CTCBase::UpdateGlobalTCs(Paused);
    PlayerManager->GetMainPlayer().UpdateFreeCamera();
    if (!Paused) {
        CPhysicsMesh::ResetStartInformationCount();
        CInterpolationInfo info;
        PNavigationManager_ptr->FrameUpdate();
        PWorldMap_ptr->Update();
        if (JustLoaded)
            JustLoaded = false;
        PThingCombatManager_ptr->Update();
        PThingManager_ptr->WorldUpdateThings();
        CPlayerGui::Get().UpdateMiniMapMarkers(true);
        PCurrentGameCamera->Update();
        bool fullUpdate;
        if (PThingManager_ptr->IsNonScriptedEntitesPauseMode())
            fullUpdate = true;
        else {
            CBulletTimeManager& bulletTimeManager = GetBulletTimeManager();
            fullUpdate = !bulletTimeManager.IsBulletTimeActive() || bulletTimeManager.IsTimeForFullUpdate();
        }
        NParticleEngine::GetParticleEmitterManager().Update(fullUpdate);
        if (RegionLoadStatus == 0 || RegionLoadStatus == 9) {
            CQuestManager::Get().Update();
            PGameScriptInterface_ptr->WorldUpdate(fullUpdate);
        }
        PMessageEventManager_Data->Update();
        for (long i = 0; i < 4; ++i) {
            CPlayer& player = PlayerManager->GetPlayer(i);
            if (player.IsValid())
                player.Update();
        }
        CTBaseSingleton<CEngineManager>::Get().OnUpdate(fullUpdate);
        GetGameTimeManager().Update(1.0f / CMainGameComponent::GetConstantFPS());
        PEnvironment_ptr->Update(GetGameTimeManager().GetTime());
        PBulletTimeManager_ptr->FrameUpdate();
        PScriptConversationManager_ptr->FrameUpdate();
        PThingManager_ptr->UpdateDeaths();
        ++Frame;
        if (CountdownTimer > 0)
            --CountdownTimer;
    } else {
        CTBaseSingleton<CEngineManager>::Get().OnPausedUpdate();
    }
    PMusicManager_Data->Update();
    CPlayerGui::Get().Update(Paused);
    if (ConsolePauseAtFrameNumber > 0 && Frame == ConsolePauseAtFrameNumber)
        SetAsPaused(true);
    if (PAtmosProcessor_Data)
        PAtmosProcessor_Data->Update();
    NSpeechGainManager::Update(*this);
}
