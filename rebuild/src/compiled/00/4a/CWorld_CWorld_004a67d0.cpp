// CWorld::CWorld(const CWorldInit& init) -- construct the game world: take the component, player manager and definition manager from the init block, null every manager pointer, set the debug/feature flags to their defaults, build the empty strings, vectors and atmos-copy list and stamp the start time
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004A67D0, length 736 bytes (`ret 4`, returns this).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, CCharString::CCharString 0x0099E4B0,
//   CWideString::CWideString 0x0099AED0, CRT malloc thunk 0x00BFEA0E (STLport list sentinel), GFGetTime 0x009E1BC0.
// Vtables: CIDrawWorld 0x01238B78 (second base at +4), CWorld 0x012390F0 / 0x012390DC.
// Layout: rebuild/include/engine/CWorld.h is the Ego_r.pdb view (0x1b8, 16-byte vectors/CArray); retail is smaller:
//   CArray InitialActiveQuests and the three STLport vectors are 12 bytes, so CreatureGenerationDisabledGroups sits at
//   +0xd0 and the 15 feature flags at +0xd4..+0xe2 (the retail stores), HeroInformationScreenModeAfterTutorial +0xe4,
//   FrameCachedLODCentre +0xe8 (-1), SaveGameLoadStatus +0xf8, SaveGamePathName +0xfc, AtmosBanksWaitingToCopy +0x174,
//   StartTime +0x180. SaveGameMarkerPos is a byte-aligned vector (PDB +0x151, retail +0x141) -- modelled as a pack(1) type.
//   A local model is used because the header's flattened members cannot run the member constructors.
// Shape: the init block is read through one reference (+0 / +8 / +0xc); the list ctor is STLport's
//   `_M_node(0)` then a malloc'd sentinel linked to itself.
#include <stdlib.h>

struct CMainGameComponent;
struct CPlayerManager;
struct CGameDefinitionManager;
struct CWorldMap;
struct CEnvironment;
struct CGameTimeManager;
struct CThingSearchTools;
struct CAtmosProcess;
struct CAIGameCameraBase;
struct CGameScriptInterface;
struct CMeshDataBank;
struct C3DAnimationManager;
struct CNavigatorManager;
struct CCombatManager;
struct CThingManager;
struct CFactionManager;
struct CScriptInfoManager;
struct CMessageEventManager;
struct CBulletTimeManager;
struct CMusicManager;
struct COpinionReactionManager;
struct CScriptConversationManager;
struct CCPPointerInfo;

template <class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) {}
};
template <class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};

struct CWorldInit {
    CMainGameComponent* Component;                 // +0x00
    void* Reserved;                                // +0x04
    CPlayerManager* PlayerManager;                 // +0x08
    CGameDefinitionManager* DefinitionManager;     // +0x0c
};

struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                       // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

struct CIDrawWorld {
    virtual void DrawWorld() = 0;
};

struct CCharString { void* PStringData; CCharString(); /* 0x0099E4B0 */ };
struct CWideString { void* PStringData; CWideString(); /* 0x0099AED0 */ };

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
#pragma pack(push, 1)
struct CPackedVector {
    float X, Y, Z;
    CPackedVector() : X(0), Y(0), Z(0) {}
};
#pragma pack(pop)

template <class T> struct CPtrVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CPtrVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

struct CAtmosCopyInfo { long A, B, C; };
struct SAtmosNode { SAtmosNode* _M_next; SAtmosNode* _M_prev; CAtmosCopyInfo _M_data; };
struct CAtmosCopyList {
    SAtmosNode* _M_node;
    CAtmosCopyList() : _M_node(0)
    {
        SAtmosNode* node = (SAtmosNode*)malloc(sizeof(SAtmosNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

double GFGetTime();                                // 0x009E1BC0

struct CWorld : CBaseClassNonCopyable, CIDrawWorld {
    CMainGameComponent* Component;                 // +0x08
    CPlayerManager* PlayerManager;                 // +0x0c
    CGameDefinitionManager* DefinitionManager;     // +0x10
    scoped_ptr<CWorldMap> PWorldMap;              // +0x14
    scoped_ptr<CEnvironment> PEnvironment;        // +0x18
    scoped_ptr<CGameTimeManager> PGameTimeManager; // +0x1c
    scoped_ptr<CThingSearchTools> PThingSearchTools; // +0x20
    CCountedPointer<CAtmosProcess> PAtmosProcessor; // +0x24
    scoped_ptr<CAIGameCameraBase> PGameCamera;    // +0x2c
    scoped_ptr<CAIGameCameraBase> PGameCameraManager; // +0x30
    CAIGameCameraBase* PCurrentGameCamera;        // +0x34
    scoped_ptr<CGameScriptInterface> PGameScriptInterface; // +0x38
    CCountedPointer<CMeshDataBank> PMainMeshBank; // +0x3c
    C3DAnimationManager* PAnimationManager;       // +0x44
    scoped_ptr<CNavigatorManager> PNavigationManager; // +0x48
    scoped_ptr<CCombatManager> PThingCombatManager; // +0x4c
    scoped_ptr<CThingManager> PThingManager;      // +0x50
    scoped_ptr<CFactionManager> PFactionManager;  // +0x54
    CCountedPointer<CScriptInfoManager> PScriptInfoManager; // +0x58
    CCountedPointer<CMessageEventManager> PMessageEventManager; // +0x60
    scoped_ptr<CBulletTimeManager> PBulletTimeManager; // +0x68
    CCountedPointer<CMusicManager> PMusicManager; // +0x6c
    CCountedPointer<COpinionReactionManager> POpinionReactionManager; // +0x74
    scoped_ptr<CScriptConversationManager> PScriptConversationManager; // +0x7c
    bool JustLoaded;                               // +0x80
    CCharString CurrentWorldName;                  // +0x84
    long ConsolePauseAtFrameNumber;                // +0x88
    long FrameStarted3DRendering;                  // +0x8c
    double LastUpdateTimeLength;                   // +0x90
    double LastUpdateTime;                         // +0x98
    long CountdownTimer;                           // +0xa0
    bool Paused;                                   // +0xa4
    bool SlowMotion;                               // +0xa5
    bool ShowDebugText;                            // +0xa6
    bool ShowFPSText;                              // +0xa7
    bool ShowProfileText;                          // +0xa8
    CPtrVector<CCharString> InitialActiveQuests;   // +0xac CArray<CCharString>
    CPtrVector<CCharString> RegisteredQuests;      // +0xb8
    CPtrVector<void> ActiveTestQuests;             // +0xc4
    unsigned long CreatureGenerationDisabledGroups; // +0xd0
    bool CreatureGenerationEnabled;                // +0xd4
    bool TeleportingEnabled;                       // +0xd5
    bool ExperienceSpendingEnabled;                // +0xd6
    bool SavingEnabled;                            // +0xd7
    bool DontPopulateNextLoadedRegion;             // +0xd8
    bool HeroSleepingEnabled;                      // +0xd9
    bool MapTableShowQuestCardsOnUsed;             // +0xda
    bool ScreenToFadeInOnNextRegionChange;         // +0xdb
    bool DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn; // +0xdc
    bool MiniMapEnabled;                           // +0xdd
    bool MiniMapActiveBeforeDisabled;              // +0xde
    bool RegionLoadDisplayRegionName;              // +0xdf
    bool GuildMasterMessagesEnabled;               // +0xe0
    bool SummonerDeathExplosionAffectsHero;        // +0xe1
    bool WaitingForInventoryTutorialToFinish;      // +0xe2
    long HeroInformationScreenModeAfterTutorial;   // +0xe4 EPlayerMode
    long FrameCachedLODCentre;                     // +0xe8
    C3DVector CachedLODCentre;                     // +0xec
    long SaveGameLoadStatus;                       // +0xf8 ESaveGameLoadStatus
    CWideString SaveGamePathName;                  // +0xfc
    bool AutoSaveLocked;                           // +0x100
    bool SerialisingAboutToLoadHeroState;          // +0x101
    bool SerialisingHeroState;                     // +0x102
    bool SerialisingNonPersistentQuestItems;       // +0x103
    long RegionLoadStatus;                         // +0x104 ERegionLoadStatus
    C3DVector RegionLoadStartPos;                  // +0x108
    float RegionLoadStartAngleXY;                  // +0x114
    CPtrVector<void> RegionLoadFollowers;          // +0x118
    long PreviousRegion;                           // +0x124
    long NumberOfTimesFreezeControlsModeAddedDuringRegionLoad; // +0x128
    bool RegionLoadViaTeleporter;                  // +0x12c
    bool RegionLoadViaDoor;                        // +0x12d
    bool PutIntoPauseModeOnRegionChange;           // +0x12e
    bool DisplayMiniMapOnRegionLoad;               // +0x12f
    bool RegionLoadWaitingForConfirmation;         // +0x130
    bool RegionLoadScreenFullyFaded;               // +0x131
    bool RegionLoadScreenWasFadedOut;              // +0x132
    bool WaitingForResetToFrontEndConfirmation;    // +0x133
    long MostRecentSaveType;                       // +0x134 ESaveType
    long MostRecentSaveTypeBeforeManualSave;       // +0x138 ESaveType
    CWideString MostRecentManualSaveName;          // +0x13c
    bool AutoSaveCheckPointExists;                 // +0x140
    CPackedVector SaveGameMarkerPos;               // +0x141
    float SaveGameMarkerAngleXY;                   // +0x150
    C3DVector GuildSealRecallPos;                  // +0x154
    float GuildSealRecallAngleXY;                  // +0x160
    bool WeatherMaskingPrimitivesSent;             // +0x164
    CPtrVector<void> WeatherMaskingPrimitiveHandles; // +0x168
    CAtmosCopyList AtmosBanksWaitingToCopy;        // +0x174
    long PlayerSpawnStatus;                        // +0x178 EPlayerSpawnStatus
    long VillagerReactionDebug;                    // +0x17c
    double StartTime;                              // +0x180
    double TimePlayed;                             // +0x188
    bool HasInitialisedStartTime;                  // +0x190

    CWorld(const CWorldInit& init);
    virtual ~CWorld();
    virtual void DrawWorld();
};

CWorld::CWorld(const CWorldInit& init)
    : Component(init.Component), PlayerManager(init.PlayerManager), DefinitionManager(init.DefinitionManager),
      PCurrentGameCamera(0), PAnimationManager(0),
      JustLoaded(false), ConsolePauseAtFrameNumber(0), FrameStarted3DRendering(0),
      LastUpdateTimeLength(0.05f), LastUpdateTime(0.0), CountdownTimer(0),
      Paused(false), SlowMotion(false), ShowDebugText(false), ShowFPSText(false), ShowProfileText(false),
      CreatureGenerationDisabledGroups(0),
      CreatureGenerationEnabled(true), TeleportingEnabled(true), ExperienceSpendingEnabled(true), SavingEnabled(true),
      DontPopulateNextLoadedRegion(false), HeroSleepingEnabled(true), MapTableShowQuestCardsOnUsed(true),
      ScreenToFadeInOnNextRegionChange(true), DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn(false),
      MiniMapEnabled(true), MiniMapActiveBeforeDisabled(true), RegionLoadDisplayRegionName(true),
      GuildMasterMessagesEnabled(true), SummonerDeathExplosionAffectsHero(true), WaitingForInventoryTutorialToFinish(false),
      HeroInformationScreenModeAfterTutorial(0), FrameCachedLODCentre(-1), SaveGameLoadStatus(0),
      AutoSaveLocked(false), SerialisingAboutToLoadHeroState(false), SerialisingHeroState(false),
      SerialisingNonPersistentQuestItems(true), RegionLoadStatus(0), RegionLoadStartPos(0.0f, 0.0f, 0.0f),
      RegionLoadStartAngleXY(0.0f), PreviousRegion(0), NumberOfTimesFreezeControlsModeAddedDuringRegionLoad(0),
      RegionLoadViaTeleporter(false), RegionLoadViaDoor(false), PutIntoPauseModeOnRegionChange(false),
      DisplayMiniMapOnRegionLoad(true), RegionLoadWaitingForConfirmation(false), RegionLoadScreenFullyFaded(false),
      RegionLoadScreenWasFadedOut(false), WaitingForResetToFrontEndConfirmation(false),
      MostRecentSaveType(0), MostRecentSaveTypeBeforeManualSave(0), AutoSaveCheckPointExists(false),
      SaveGameMarkerAngleXY(0.0f), GuildSealRecallPos(0.0f, 0.0f, 0.0f), GuildSealRecallAngleXY(0.0f),
      WeatherMaskingPrimitivesSent(false), PlayerSpawnStatus(0), VillagerReactionDebug(0),
      HasInitialisedStartTime(false)
{
    StartTime = GFGetTime();
    TimePlayed = 0.0;
}