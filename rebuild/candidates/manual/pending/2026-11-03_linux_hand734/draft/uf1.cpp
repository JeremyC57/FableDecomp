// draft UpdateFrame (shape probe, local decls)
typedef unsigned short wchar16;
double __fastcall GFGetTime();
class CWideString { public: void* P; CWideString(const wchar16*); ~CWideString(); };
class CAFile { public: static bool __fastcall PathExists(const CWideString&); };
class CDriveManager { public: long DeleteFileW(const CWideString&); };
template<class T> class CCountedPointer { public: T* Data; void* Info; T& operator*() const { return *Data; } T* operator->() const { return Data; } };
template<class T> class CTBaseSingletonNoDefaultAlloc { static CCountedPointer<T> PClass; public: static T& __fastcall Get() { return *PClass; } };
template<class T> class CTBaseSingleton { public: static T& __fastcall Get(); };
extern bool g_bRunFromDVD; extern bool g_bSaveLoadFailed; extern bool g_bProgressDisplaySuppressed;
extern long g_GameFramesPerSecond;
namespace NProgressDisplay { void __fastcall StartProgress(bool,bool,bool,bool,bool,bool); void __fastcall StopProgress(bool,bool); }
enum EGameAction { GAME_ACTION_CONFIRM_RESET_TO_FRONT_END = 59 };
enum EInputButtonEventType { IBE2 = 2 };
enum EPlayerMode { PM };
class CGamePlayerInterface { public: bool IsGameActionEventInQueue(EGameAction, EInputButtonEventType, long) const; };
class CPlayer { public: void* v; bool Valid; char p[7]; CGamePlayerInterface* PlayerInterface;
  CGamePlayerInterface* GetPlayerInterface() { return PlayerInterface; }
  long GetJoystickDeviceNumber() const; void UpdateFreeCamera(); void Update(); bool IsValid() const; };
class CPlayerManager { public: CPlayer& GetMainPlayer(); CPlayer& GetPlayer(long); };
class CPlayerGui { public: void Update(bool); void UpdateMiniMapMarkers(bool); char pad[0xfd]; bool DisplayingText; };
class CRegion { public: char p[0x56]; bool SoundThemesEnabled; bool IsSoundThemesEnabled() const { return SoundThemesEnabled; } };
class CWorldMap { public: long GetLoadedRegion() const; const CRegion& GetRegion(long) const;
 virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9();
 virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
 virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void Update(); };
class CAtmosProcess { public: void SetAtmosSoundsAsMuted(bool, float); void Update(); };
class C3DVector { public: float X,Y,Z; };
class CCamera { public: C3DVector WorldPosition; char r[0x2c]; const C3DVector& PeekWorldPos() const { return WorldPosition; } };
class CEnvironment { public: void UpdateInterpolation(); void Update(float); };
class CTCBase { public: static void __fastcall UpdateGlobalTCs(bool); };
class CPhysicsMesh { public: static void __fastcall ResetStartInformationCount(); };
class CInterpolationInfo { public: char d[0x34]; CInterpolationInfo(); };
class CNavigatorManager { public: void FrameUpdate(); };
class CCombatManager { public: void Update(); };
class CThingManager { public: char p[0x54]; bool NonScriptedEntitesPauseMode; bool IsNonScriptedEntitesPauseMode() const { return NonScriptedEntitesPauseMode; } void WorldUpdateThings(); void UpdateDeaths(); };
class CAIGameCameraBase { public: virtual void Update() = 0; };
class CBulletTimeManager { public: bool IsBulletTimeActive() const; bool IsTimeForFullUpdate() const; void FrameUpdate(); };
namespace NParticleEngine { class CParticleEmitterManager { public: void Update(bool); }; CParticleEmitterManager& __fastcall GetParticleEmitterManager(); }
class CQuestManager { public: void Update(); };
class CGameScriptInterface { public: void WorldUpdate(bool); };
class CMessageEventManager { public: void Update(); };
class CEngineManager { public: void OnUpdate(bool); void OnPausedUpdate(); };
class CGameTimeManager { public: char p[8]; float Time; float GetTime() const { return Time; } void Update(float); };
class CScriptConversationManager { public: void FrameUpdate(); };
class CMusicManager { public: void Update(); };
class CIEngine { public: virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7(); virtual void s8(); virtual void s9();
 virtual void s10(); virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
 virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29();
 virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39();
 virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47(); virtual void s48(); virtual void s49();
 virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54(); virtual void StartNewScene(unsigned long, float); };
class CDisplayEngine { public: char p[0x2c]; CIEngine* Engine3D; };
class CMainGameComponent { public: char p[0x28]; CDisplayEngine* PDisplayEngine; CDisplayEngine& GetDisplayEngine() { return *PDisplayEngine; } };
class CWorld;
namespace NSpeechGainManager { void __fastcall Update(const CWorld&); }
class CWorld { public:
 virtual void w0(); virtual void w1(); virtual void w2(); virtual void w3(); virtual void w4(); virtual void w5();
 virtual CCamera GetCurrentRenderCamera() const; virtual void w7(); virtual void w8(); virtual void w9(); virtual void w10(); virtual void w11(); virtual void w12();
 virtual const CWorldMap& PeekWorldMap() const;
 char b4[4];
 CMainGameComponent* Component; CPlayerManager* PlayerManager; void* DefinitionManager; CWorldMap* PWorldMap; CEnvironment* PEnvironment; CGameTimeManager* PGameTimeManager; void* PThingSearchTools;
 CAtmosProcess* PAtmosProcessor; void* PAtmosInfo; void* PGameCamera; void* PGameCameraManager; CAIGameCameraBase* PCurrentGameCamera; CGameScriptInterface* PGameScriptInterface;
 void* MeshBank; void* MeshInfo; void* PAnimationManager; CNavigatorManager* PNavigationManager; CCombatManager* PThingCombatManager; CThingManager* PThingManager; void* PFactionManager;
 void* SI; void* SII; CMessageEventManager* PMessageEventManager; void* MEI; CBulletTimeManager* PBulletTimeManager; CMusicManager* PMusicManager; void* MMI; void* ORM; void* ORMI; CScriptConversationManager* PScriptConversationManager;
 bool JustLoaded; char p81[3]; void* CurrentWorldName; long ConsolePauseAtFrameNumber; long FrameStarted3DRendering; double LastUpdateTimeLength; double LastUpdateTime; long CountdownTimer;
 bool Paused; char pa5[0xe2-0xa5]; bool WaitingForInventoryTutorialToFinish; char pe3; EPlayerMode HeroInformationScreenModeAfterTutorial; char pe8[0xf8-0xe8];
 long SaveGameLoadStatus; char pfc[0x104-0xfc]; long RegionLoadStatus; char p108[0x133-0x108]; bool WaitingForResetToFrontEndConfirmation; char p134[0x178-0x134]; long PlayerSpawnStatus; long VRD; double StartTime; double TimePlayed; bool HasInitialisedStartTime;
 static long Frame;
 void ResetToFrontEnd(); void OpenHeroInformationScreen(EPlayerMode); void UpdateSaveGameLoad(); void UpdateRegionLoad(); void PrepareAtmosBanksForLoadedRegion(); void UpdateEnvironmentCameraPos(const C3DVector&);
 void SetAsPaused(bool p) { if (Paused != p) Paused = p; }
 void UpdateFrame();
};
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
            PlayerManager->GetMainPlayer().GetPlayerInterface()->IsGameActionEventInQueue(GAME_ACTION_CONFIRM_RESET_TO_FRONT_END, IBE2, PlayerManager->GetMainPlayer().GetJoystickDeviceNumber())) {
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
    if (WaitingForInventoryTutorialToFinish && !CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().DisplayingText && !Paused) {
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
            PlayerSpawnStatus = 0;
        }
        return;
    }
    if (PAtmosProcessor)
        PAtmosProcessor->SetAtmosSoundsAsMuted(!PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).IsSoundThemesEnabled(), 1000.0f);
    UpdateEnvironmentCameraPos(GetCurrentRenderCamera().PeekWorldPos());
    PEnvironment->UpdateInterpolation();
    CTCBase::UpdateGlobalTCs(Paused);
    PlayerManager->GetMainPlayer().UpdateFreeCamera();
    if (!Paused) {
        CPhysicsMesh::ResetStartInformationCount();
        CInterpolationInfo info;
        PNavigationManager->FrameUpdate();
        PWorldMap->Update();
        if (JustLoaded)
            JustLoaded = false;
        PThingCombatManager->Update();
        PThingManager->WorldUpdateThings();
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(true);
        PCurrentGameCamera->Update();
        bool fullUpdate = PThingManager->IsNonScriptedEntitesPauseMode() || !PBulletTimeManager->IsBulletTimeActive() || PBulletTimeManager->IsTimeForFullUpdate();
        NParticleEngine::GetParticleEmitterManager().Update(fullUpdate);
        if (RegionLoadStatus == 0 || RegionLoadStatus == 9) {
            CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().Update();
            PGameScriptInterface->WorldUpdate(fullUpdate);
        }
        PMessageEventManager->Update();
        for (long i = 0; i < 4; ++i) {
            CPlayer& player = PlayerManager->GetPlayer(i);
            if (player.IsValid())
                player.Update();
        }
        CTBaseSingleton<CEngineManager>::Get().OnUpdate(fullUpdate);
        PGameTimeManager->Update(1.0f / g_GameFramesPerSecond);
        PEnvironment->Update(PGameTimeManager->GetTime());
        PBulletTimeManager->FrameUpdate();
        PScriptConversationManager->FrameUpdate();
        PThingManager->UpdateDeaths();
        ++Frame;
        if (CountdownTimer > 0)
            --CountdownTimer;
    } else {
        CTBaseSingleton<CEngineManager>::Get().OnPausedUpdate();
    }
    PMusicManager->Update();
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Update(Paused);
    if (ConsolePauseAtFrameNumber > 0 && Frame == ConsolePauseAtFrameNumber)
        SetAsPaused(true);
    if (PAtmosProcessor)
        PAtmosProcessor->Update();
    NSpeechGainManager::Update(*this);
}
