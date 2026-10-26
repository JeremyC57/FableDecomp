#include <cstdio>
class CCPPointerInfo { public:
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
template <class T> class CCountedPointer { public:
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
class CMainGameComponent { public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                     // +0x28
};
struct CMessageList {
    void clear();                                       // 0x004A9CF0
};
class CMessageEventManager { public:
    void*        _vptr;
    CMessageList Messages;                              // +0x04
};
class CEnginePrimitiveHandle;
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

#include <string.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { printf("FAIL %s\n", w); g_fail = 1; } }
static char g_log[1024];
static void L(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static char tmp[64];
static CPlayer g_player;
void CPlayer::RemoveMode(long mode) { sprintf(tmp, "RemoveMode%ld", mode); L(tmp); }
void CPlayer::PrepareForLoad() { L("Player"); }
CPlayer* CPlayerManager::GetMainPlayer() { L("Main"); return &g_player; }
static NHeroInformationScreens::CManager g_his;
void NHeroInformationScreens::CManager::Close() { L("HISClose"); }
NHeroInformationScreens::CManager* CPlayerGui::GetPHeroInfoScreensManager() const { return &g_his; }
void CPlayerGui::PrepareForLoad() { L("Gui"); }
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;
void CMusicManager::ResetMusicEntriesOffsets() { L("MusicReset"); }
void CMusicManager::EnableDangerMusic(bool e) { L(e ? "Danger1" : "Danger0"); }
void CMusicManager::ClearOverridingMusicSet() { L("ClrOver"); }
void CMusicManager::ClearCinematicOverridingMusicSet() { L("ClrCine"); }
void CMusicManager::ClearUIOverridingMusicSet() { L("ClrUI"); }
void CEnvironment::SetStatic(bool on) { L(on ? "Static1" : "Static0"); }
void CEnvironment::TransitionToDefaultTheme(long channel, float time) { sprintf(tmp, "Theme%ld:%g", channel, time); L(tmp); }
void CWorldMap::PrepareForLoad() { L("Map"); }
void CBulletTimeManager::PrepareForLoad() { L("Bullet"); }
void CDisplayEngine::PrepareForLoad() { L("Display"); }
void CMessageList::clear() { L("Msgs"); }
static int g_deleted = 0;
static void __fastcall DeleteAtmos(void* d) { ++g_deleted; L("AtmosDelete"); }
namespace std { template <class T> T* vector<T>::erase(T* first, T* last) { sprintf(tmp, "Erase%d", (int)(last - first)); L(tmp); _M_finish = first; return first; } }
class CEnginePrimitiveHandle { public: long h; };
static CWorldMap g_map;
void CWorld::v00() {} void CWorld::v01() {} void CWorld::v02() {} void CWorld::v03() {} void CWorld::v04() {} void CWorld::v05() {}
void CWorld::v06() {} void CWorld::v07() {} void CWorld::v08() {} void CWorld::v09() {} void CWorld::v10() {} void CWorld::v11() {}
CWorldMap* CWorld::GetWorldMap() { return &g_map; }
int main()
{
    static CWorld w; static CPlayerManager pm; static CMusicManager mm; static CEnvironment env; static CBulletTimeManager bt;
    static CDisplayEngine de; static CMainGameComponent mgc; static CMessageEventManager mem;
    static CEnginePrimitiveHandle handles[3];
    w.PlayerManager = &pm; w.PMusicManager = &mm; w.PEnvironment = &env; w.PBulletTimeManager = &bt;
    mgc.PDisplayEngine = &de; w.Component = &mgc; w.PMessageEventManager = &mem;
    w.WeatherMaskingPrimitiveHandles._M_start = handles; w.WeatherMaskingPrimitiveHandles._M_finish = handles + 3;
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 1; info->DeleteFunc = DeleteAtmos; info->Data = 0;
    w.PAtmosProcessor.Data = (CAtmosProcess*)0x10; w.PAtmosProcessor.Info = info;
    w.FrameStarted3DRendering = 9; w.DontPopulateNextLoadedRegion = true; w.ScreenToFadeInOnNextRegionChange = false;
    w.DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn = true; w.RegionLoadDisplayRegionName = false;
    w.WaitingForInventoryTutorialToFinish = true; w.HeroInformationScreenModeAfterTutorial = 4; w.PreviousRegion = 5;
    w.AutoSaveCheckPointExists = true; w.WeatherMaskingPrimitivesSent = true; w.PlayerSpawnStatus = 2;
    w.PrepareForLoad();
    check(strcmp(g_log, "Main;RemoveMode43;Main;Player;HISClose;Gui;ClrUI;ClrCine;ClrOver;Danger0;MusicReset;Static1;AtmosDelete;"
                        "Theme1:0;Theme2:0;Theme3:0;Map;Bullet;Display;Msgs;Erase3;") == 0, "call order");
    check(w.PAtmosProcessor.Data == 0 && w.PAtmosProcessor.Info == 0 && g_deleted == 1, "atmos released");
    check(w.FrameStarted3DRendering == 0 && !w.DontPopulateNextLoadedRegion && w.ScreenToFadeInOnNextRegionChange
          && !w.DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn && w.RegionLoadDisplayRegionName
          && !w.WaitingForInventoryTutorialToFinish && w.HeroInformationScreenModeAfterTutorial == 0 && w.PreviousRegion == 0
          && !w.AutoSaveCheckPointExists && !w.WeatherMaskingPrimitivesSent && w.PlayerSpawnStatus == 0, "flags reset");
    // shared atmos pointer: only the count drops
    g_log[0] = 0;
    info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = DeleteAtmos; info->Data = 0;
    w.PAtmosProcessor.Info = info; w.PAtmosProcessor.Data = (CAtmosProcess*)0x10;
    w.PrepareForLoad();
    check(info->RefCount == 1 && g_deleted == 1 && w.PAtmosProcessor.Info == 0, "shared atmos kept alive");
    // no atmos pointer
    w.PrepareForLoad();
    check(g_deleted == 1, "null atmos");
    if (!g_fail) printf("004a0220_TEST PASS\n");
    else printf("%s\n", g_log);
    return g_fail;
}