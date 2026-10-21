// CWorld::PrepareForLoad() -- before a saved game is deserialised: reset the main player, the hero-information screens, the player GUI, the music overrides, the environment themes, the atmos processor, the world map, bullet time, the display engine and the message queue, and clear the per-load world flags
// Leaf: ?PrepareForLoad@CWorld@@QAEXXZ (Ego_r.pdb)
// Retail: Fable.exe @ 0x004A0220, 324 bytes, plain `ret`.
// Body: PlayerManager->GetMainPlayer() (0x00449970)->RemoveMode(0x2B) (0x00634190), ->PrepareForLoad() (0x00487550);
// g_pPlayerGui (0x013B8790)->GetPHeroInfoScreensManager() (0x00437CD0)->Close() (0x0066E73D, name unconfirmed) and
// ->PrepareForLoad() (0x00438460); music: ClearUIOverridingMusicSet 0x006BE030, ClearCinematicOverridingMusicSet
// 0x006BDF90, ClearOverridingMusicSet 0x006BDEF0 (the three 0xA0-byte siblings, in Ego_r.pdb order), EnableDangerMusic(false)
// 0x006BDE50, ResetMusicEntriesOffsets 0x006BD870 (unconfirmed); PEnvironment->SetEnvironmentStatic?(true) (0x006B32B0,
// bool); PAtmosProcessor counted pointer cleared inline (release: destroy + operator delete 0x00BFE9BC; Info then Data
// zeroed); PEnvironment->TransitionToDefaultTheme(channel 1/2/3, 0.0f) (0x006B3950); virtual slot 12 (+0x30) world map
// ->PrepareForLoad() (0x004FF560); PBulletTimeManager->PrepareForLoad() (0x004C6080); Component->PDisplayEngine
// ->PrepareForLoad() (0x00434AD0; labelled CPlayerGui::ForceOpen, wrong: CMainGameComponent +0x28 is PDisplayEngine);
// PMessageEventManager's list (+4) clear (0x004A9CF0); flags; WeatherMaskingPrimitiveHandles.erase(begin, end)
// (0x00440D70); PlayerSpawnStatus = 0.
// Layout: CWorld per Ego_r.pdb up to +0xDC, then retail containers are 12 bytes (InitialActiveQuests, RegisteredQuests,
// ActiveTestQuests: -0xC; RegionLoadFollowers: -0x10; WeatherMaskingPrimitiveHandles: -0x14; the atmos-copy list is 4
// bytes): FrameStarted3DRendering +0x8C, DontPopulateNextLoadedRegion +0xD8, ScreenToFadeInOnNextRegionChange +0xDB,
// DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn +0xDC, RegionLoadDisplayRegionName +0xDF,
// WaitingForInventoryTutorialToFinish +0xE2, HeroInformationScreenModeAfterTutorial +0xE4, PreviousRegion +0x124,
// AutoSaveCheckPointExists +0x140, WeatherMaskingPrimitivesSent +0x164, WeatherMaskingPrimitiveHandles +0x168,
// PlayerSpawnStatus +0x178. Names for the stores follow from these offsets (not confirmed against the PDB build's code).
// Shape: the atmos pointer is a CCountedPointer member with an inline Clear() and the handle vector an inline clear()
// over erase(begin(), end()) (open-coded forms schedule the environment load and the vector words differently).
// Opt: /O2 (16-aligned entry).
struct CCPPointerInfo {
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
class CPlayer {
public:
    void RemoveMode(long mode);                         // 0x00634190 (EPlayerMode)
    void PrepareForLoad();                              // 0x00487550
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer();                           // 0x00449970
};
namespace NHeroInformationScreens {
class CManager {
public:
    void Close();                                       // 0x0066E73D (name unconfirmed)
};
}
class CPlayerGui {
public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
    void PrepareForLoad();                              // 0x00438460
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
class CMusicManager {
public:
    void ResetMusicEntriesOffsets();                    // 0x006BD870 (unconfirmed)
    void EnableDangerMusic(bool enable);                // 0x006BDE50
    void ClearOverridingMusicSet();                     // 0x006BDEF0
    void ClearCinematicOverridingMusicSet();            // 0x006BDF90
    void ClearUIOverridingMusicSet();                   // 0x006BE030
};
class CEnvironment {
public:
    void SetStatic(bool on);                            // 0x006B32B0 (bool argument; name unconfirmed)
    void TransitionToDefaultTheme(long channel, float time);   // 0x006B3950
};
class CAtmosProcess;
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Clear()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
class CWorldMap {
public:
    void PrepareForLoad();                              // 0x004FF560
};
class CBulletTimeManager {
public:
    void PrepareForLoad();                              // 0x004C6080
};
class CDisplayEngine {
public:
    void PrepareForLoad();                              // 0x00434AD0
};
struct CMainGameComponent {
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                     // +0x28
};
struct CMessageList {
    void clear();                                       // 0x004A9CF0
};
struct CMessageEventManager {
    void*        _vptr;
    CMessageList Messages;                              // +0x04
};
struct CEnginePrimitiveHandle;
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                        // 0x00440D70 for CEnginePrimitiveHandle
    void clear() { erase(begin(), end()); }
};
}

class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                   // slot 12 (+0x30)
    unsigned char         _base04[0x4];
    CMainGameComponent*   Component;                    // +0x08
    CPlayerManager*       PlayerManager;                // +0x0c
    unsigned char         _pad10[0x8];
    CEnvironment*         PEnvironment;                 // +0x18
    unsigned char         _pad1c[0x8];
    CCountedPointer<CAtmosProcess> PAtmosProcessor;     // +0x24
    unsigned char         _pad2c[0x34];
    CMessageEventManager* PMessageEventManager;         // +0x60
    unsigned char         _pad64[0x4];
    CBulletTimeManager*   PBulletTimeManager;           // +0x68
    CMusicManager*        PMusicManager;                // +0x6c
    unsigned char         _pad70[0x1c];
    long                  FrameStarted3DRendering;      // +0x8c
    unsigned char         _pad90[0x48];
    bool                  DontPopulateNextLoadedRegion; // +0xd8
    unsigned char         _padd9[0x2];
    bool                  ScreenToFadeInOnNextRegionChange;   // +0xdb
    bool                  DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn;   // +0xdc
    unsigned char         _paddd[0x2];
    bool                  RegionLoadDisplayRegionName;  // +0xdf
    unsigned char         _pade0[0x2];
    bool                  WaitingForInventoryTutorialToFinish;   // +0xe2
    unsigned char         _pade3[0x1];
    long                  HeroInformationScreenModeAfterTutorial;   // +0xe4
    unsigned char         _pade8[0x3c];
    long                  PreviousRegion;               // +0x124
    unsigned char         _pad128[0x18];
    bool                  AutoSaveCheckPointExists;     // +0x140
    unsigned char         _pad141[0x23];
    bool                  WeatherMaskingPrimitivesSent; // +0x164
    unsigned char         _pad165[0x3];
    std::vector<CEnginePrimitiveHandle> WeatherMaskingPrimitiveHandles;   // +0x168
    unsigned char         _pad174[0x4];
    long                  PlayerSpawnStatus;            // +0x178
    void PrepareForLoad();
};

void CWorld::PrepareForLoad()
{
    PlayerManager->GetMainPlayer()->RemoveMode(0x2b);
    PlayerManager->GetMainPlayer()->PrepareForLoad();
    g_pPlayerGui->GetPHeroInfoScreensManager()->Close();
    g_pPlayerGui->PrepareForLoad();
    PMusicManager->ClearUIOverridingMusicSet();
    PMusicManager->ClearCinematicOverridingMusicSet();
    PMusicManager->ClearOverridingMusicSet();
    PMusicManager->EnableDangerMusic(false);
    PMusicManager->ResetMusicEntriesOffsets();
    PEnvironment->SetStatic(true);
    PAtmosProcessor.Clear();
    PEnvironment->TransitionToDefaultTheme(1, 0.0f);
    PEnvironment->TransitionToDefaultTheme(2, 0.0f);
    PEnvironment->TransitionToDefaultTheme(3, 0.0f);
    GetWorldMap()->PrepareForLoad();
    PBulletTimeManager->PrepareForLoad();
    Component->PDisplayEngine->PrepareForLoad();
    PMessageEventManager->Messages.clear();
    FrameStarted3DRendering = 0;
    DontPopulateNextLoadedRegion = false;
    ScreenToFadeInOnNextRegionChange = true;
    DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn = false;
    RegionLoadDisplayRegionName = true;
    WaitingForInventoryTutorialToFinish = false;
    HeroInformationScreenModeAfterTutorial = 0;
    PreviousRegion = 0;
    AutoSaveCheckPointExists = false;
    WeatherMaskingPrimitivesSent = false;
    WeatherMaskingPrimitiveHandles.clear();
    PlayerSpawnStatus = 0;
}