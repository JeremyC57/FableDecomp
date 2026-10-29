#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
// 00658db3 CTCVillage::IsCurfewTime
// Leaf: ?IsCurfewTime@CTCVillage@@QBE_NXZ
// Flags: /Os
#include <stddef.h>
#include "rebuild_abi.h"
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
#define CTCVillage CTCVillageFields
#include "engine/CTCVillage.h"
#undef CTCVillage
#define CVillageDef CVillageDefFields
#include "engine/_quarantine/CVillageDef.h"
#undef CVillageDef
#ifdef _MSC_VER
#define FABLE_GNU_ALIGN8
#else
#define FABLE_GNU_ALIGN8 __attribute__((aligned(8)))
#endif
// Shared-header composites not used here retain their explicit data projections.
// Their flattened identifiers are implementation projections, not retail names.
class CBaseClass {public:virtual~CBaseClass();};class CBaseClassNonCopyable:public CBaseClass {protected:virtual~CBaseClassNonCopyable();};
class CIVCountedPointeeBase {public:virtual~CIVCountedPointeeBase();virtual void Delete()const;mutable unsigned long IVRefCount;__forceinline void AddRef()const{++IVRefCount;}__forceinline long Release()const{if(--IVRefCount==0){Delete();return 0;}return IVRefCount;}};
class CResourceList;class CResource:public CIVCountedPointeeBase {public:CResourceList*ResourceList;CResource*PrevResource,*NextResource;unsigned long ResourceSize,LastUsedFrame;};class CDefPointeeBase:public CResource {};
class CDefinitionManager;
#pragma pack(push,1)
class CDefClassBase:public CDefPointeeBase {public:CDefinitionManager*PDefManager;unsigned long GlobalIndex;unsigned char Setup:1,Template:1,DefaultValsApplied:1;};
#pragma pack(pop)
class CSubDefClassBase:public CDefClassBase {};
class CVillageDef:public CSubDefClassBase {public:
    unsigned char    _pad_0x25[0x3];                         // +0x25
    unsigned char    Tasks[0xc];                             // +0x28 vector<EVillageTaskType,std::allocator<EVillageTaskType>_>
    long             PatrolTrackName_TablePos;               // +0x34 CDefString::TablePos
    unsigned char    DefaultStanceToHero[0x4];               // +0x38 EVillageStanceToHero
    long             SecondsSecuritySweepLasts;              // +0x3c
    long             WarehouseCrateDef;                      // +0x40
    unsigned char    StoryStartTimes[0xc];                   // +0x44 vector<long,std::allocator<long>_>
    long             StoryDuration;                          // +0x50
    unsigned char    SchoolStartTimes[0xc];                  // +0x54 vector<long,std::allocator<long>_>
    long             SchoolDuration;                         // +0x60
    unsigned char    WorkStartTimes[0xc];                    // +0x64 vector<long,std::allocator<long>_>
    long             WorkDuration;                           // +0x70
    unsigned char    CookStartTimes[0xc];                    // +0x74 vector<long,std::allocator<long>_>
    long             CookDuration;                           // +0x80
    unsigned char    EatStartTimes[0xc];                     // +0x84 vector<long,std::allocator<long>_>
    long             EatDuration;                            // +0x90
    unsigned char    TavernStartTimes[0xc];                  // +0x94 vector<long,std::allocator<long>_>
    long             TavernDuration;                         // +0xa0
    unsigned char    GamingStartTimes[0xc];                  // +0xa4 vector<long,std::allocator<long>_>
    long             GamingDuration;                         // +0xb0
    long             SleepAdultStartTime;                    // +0xb4
    long             SleepAdultEndTime;                      // +0xb8
    long             SleepChildStartTime;                    // +0xbc
    long             SleepChildEndTime;                      // +0xc0
    long             SleepElderlyStartTime;                  // +0xc4
    long             SleepElderlyEndTime;                    // +0xc8
    long             HouseLightsOnTime;                      // +0xcc
    long             HouseLightsOffTime;                     // +0xd0
    long             SociableStartTime;                      // +0xd4
    long             SociableFinishTime;                     // +0xd8
    long             CurfewStartTime;                        // +0xdc
    long             CurfewFinishTime;                       // +0xe0
    bool             EnableAutoPlacement;                    // +0xe4
    bool             AllowHouseBuying;                       // +0xe5
    bool             HearTownCrier;                          // +0xe6
    unsigned char    _pad_0xe7[0x1];                         // +0xe7
    CCharStringData* CurfewStartSound_PStringData;           // +0xe8 CCharString::PStringData
    CCharStringData* CurfewEndSound_PStringData;             // +0xec CCharString::PStringData
    CCharStringData* WorkStartSound_PStringData;             // +0xf0 CCharString::PStringData
    CCharStringData* WorkEndSound_PStringData;               // +0xf4 CCharString::PStringData
    long             BribeCost;                              // +0xf8
    long             FramesBetweenGuardSnoop;                // +0xfc
    long             FramesBetweenGuardSnoopNightShops;      // +0x100
    long             FramesBeforeGuardSnoop;                 // +0x104
    long             FramesBeforeGuardSnoopNightShops;       // +0x108
};
template<class T>class CDefPointer {public:CDefPointeeBase*Object;CDefPointer():Object(0){}__forceinline CDefPointer(const CDefPointer&x):Object(x.Object){if(Object)Object->AddRef();}__forceinline void Release(){if(Object){Object->Release();Object=0;}}__forceinline~CDefPointer(){Release();}__forceinline T*operator->()const{return static_cast<T*>(Object);}};
class CWorld;
class CTCBase:public CBaseClassNonCopyable {public:virtual~CTCBase();CThing*ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;protected:const CWorld&PeekWorld()const;};
class CGameTimeManager;
namespace boost {template<class T>class scoped_ptr {T*ptr;scoped_ptr(const scoped_ptr&);scoped_ptr&operator=(const scoped_ptr&);public:explicit scoped_ptr(T*p=0):ptr(p){}~scoped_ptr();T*operator->()const{return ptr;}};}
class CCamera;class C3DVector;class CProceduralTexture;class CIDrawWorldMap;class CIDrawEnvironment;class CInterpolationInfo;class CAFile;class CDisplayEngine;class CEnginePrimitivePrepareInfo;
class CIDrawWorld {public:virtual const CProceduralTexture*DrawGetProceduralTexture()const=0;virtual const CIDrawWorldMap*DrawGetWorldMap()const=0;virtual const CIDrawEnvironment*DrawGetEnvironment()const=0;virtual~CIDrawWorld();};
class FABLE_GNU_ALIGN8 CWorld:public CBaseClassNonCopyable,public CIDrawWorld {protected:virtual void PostLoadInit();public:virtual~CWorld();virtual void LoadWorldFromFile(CAFile&,long);virtual CThingManager&GetThingManager();virtual const CThingManager&PeekThingManager()const;virtual CCamera GetCurrentRenderCamera(const CInterpolationInfo&)const;virtual CCamera GetCurrentRenderCamera()const;virtual void PostInit(CDisplayEngine&);virtual C3DVector GetGameLODCentre()const;virtual void Init();virtual void Uninitialise();virtual void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&);virtual CWorldMap&GetWorldMap();virtual const CWorldMap&PeekWorldMap()const;virtual const CProceduralTexture*DrawGetProceduralTexture()const;virtual const CIDrawWorldMap*DrawGetWorldMap()const;virtual const CIDrawEnvironment*DrawGetEnvironment()const;
    CMainGameComponent*         Component;                   // +0x08
    CPlayerManager*             PlayerManager;               // +0x0c
    CGameDefinitionManager*     DefinitionManager;           // +0x10
    CWorldMap*                  PWorldMap_ptr;               // +0x14 scoped_ptr<CWorldMap>::ptr
    CEnvironment*               PEnvironment_ptr;            // +0x18 scoped_ptr<CEnvironment>::ptr
    boost::scoped_ptr<CGameTimeManager> PGameTimeManager;        // +0x1c scoped_ptr<CGameTimeManager>::ptr
    CThingSearchTools*          PThingSearchTools_ptr;       // +0x20 scoped_ptr<CThingSearchTools>::ptr
    CAtmosProcess*              PAtmosProcessor_Data;        // +0x24 CCountedPointer<CAtmosProcess>::Data
    CCPPointerInfo*             PAtmosProcessor_Info;        // +0x28 CCountedPointer<CAtmosProcess>::Info
    CAIGameCameraBase*          PGameCamera_ptr;             // +0x2c scoped_ptr<CAIGameCameraBase>::ptr
    CAIGameCameraBase*          PGameCameraManager_ptr;      // +0x30 scoped_ptr<CAIGameCameraBase>::ptr
    CAIGameCameraBase*          PCurrentGameCamera;          // +0x34
    CGameScriptInterface*       PGameScriptInterface_ptr;    // +0x38 scoped_ptr<CGameScriptInterface>::ptr
    CMeshDataBank*              PMainMeshBank_Data;          // +0x3c CCountedPointer<CMeshDataBank>::Data
    CCPPointerInfo*             PMainMeshBank_Info;          // +0x40 CCountedPointer<CMeshDataBank>::Info
    C3DAnimationManager*        PAnimationManager;           // +0x44
    CNavigatorManager*          PNavigationManager_ptr;      // +0x48 scoped_ptr<CNavigatorManager>::ptr
    CCombatManager*             PThingCombatManager_ptr;     // +0x4c scoped_ptr<CCombatManager>::ptr
    CThingManager*              PThingManager_ptr;           // +0x50 scoped_ptr<CThingManager>::ptr
    CFactionManager*            PFactionManager_ptr;         // +0x54 scoped_ptr<CFactionManager>::ptr
    CScriptInfoManager*         PScriptInfoManager_Data;     // +0x58 CCountedPointer<CScriptInfoManager>::Data
    CCPPointerInfo*             PScriptInfoManager_Info;     // +0x5c CCountedPointer<CScriptInfoManager>::Info
    CMessageEventManager*       PMessageEventManager_Data;   // +0x60 CCountedPointer<CMessageEventManager>::Data
    CCPPointerInfo*             PMessageEventManager_Info;   // +0x64 CCountedPointer<CMessageEventManager>::Info
    CBulletTimeManager*         PBulletTimeManager_ptr;      // +0x68 scoped_ptr<CBulletTimeManager>::ptr
    CMusicManager*              PMusicManager_Data;          // +0x6c CCountedPointer<CMusicManager>::Data
    CCPPointerInfo*             PMusicManager_Info;          // +0x70 CCountedPointer<CMusicManager>::Info
    COpinionReactionManager*    POpinionReactionManager_Data; // +0x74 CCountedPointer<COpinionReactionManager>::Data
    CCPPointerInfo*             POpinionReactionManager_Info; // +0x78 CCountedPointer<COpinionReactionManager>::Info
    CScriptConversationManager* PScriptConversationManager_ptr; // +0x7c scoped_ptr<CScriptConversationManager>::ptr
    bool                        JustLoaded;                  // +0x80
    unsigned char               _pad_0x81[0x3];              // +0x81
    CCharStringData*            CurrentWorldName_PStringData; // +0x84 CCharString::PStringData
    long                        ConsolePauseAtFrameNumber;   // +0x88
    long                        FrameStarted3DRendering;     // +0x8c
    double                      LastUpdateTimeLength;        // +0x90
    double                      LastUpdateTime;              // +0x98
    long                        CountdownTimer;              // +0xa0
    bool                        Paused;                      // +0xa4
    bool                        SlowMotion;                  // +0xa5
    bool                        ShowDebugText;               // +0xa6
    bool                        ShowFPSText;                 // +0xa7
    bool                        ShowProfileText;             // +0xa8
    unsigned char               _pad_0xa9[0x3];              // +0xa9
    unsigned char               InitialActiveQuests[0xc];    // +0xac CArray<CCharString>
    unsigned char               RegisteredQuests[0xc];       // +0xb8 vector<CCharString,std::allocator<CCharString>_>
    unsigned char               ActiveTestQuests[0xc];       // +0xc4 vector<CTestQuest,std::allocator<CTestQuest>_>
    unsigned long               CreatureGenerationDisabledGroups; // +0xd0
    bool                        CreatureGenerationEnabled;   // +0xd4
    bool                        TeleportingEnabled;          // +0xd5
    bool                        ExperienceSpendingEnabled;   // +0xd6
    bool                        SavingEnabled;               // +0xd7
    bool                        DontPopulateNextLoadedRegion; // +0xd8
    bool                        HeroSleepingEnabled;         // +0xd9
    bool                        MapTableShowQuestCardsOnUsed; // +0xda
    bool                        ScreenToFadeInOnNextRegionChange; // +0xdb
    bool                        DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn; // +0xdc
    bool                        MiniMapEnabled;              // +0xdd
    bool                        MiniMapActiveBeforeDisabled; // +0xde
    bool                        RegionLoadDisplayRegionName; // +0xdf
    bool                        GuildMasterMessagesEnabled;  // +0xe0
    bool                        SummonerDeathExplosionAffectsHero; // +0xe1
    bool                        WaitingForInventoryTutorialToFinish; // +0xe2
    unsigned char               _pad_0xe3[0x1];              // +0xe3
    unsigned char               HeroInformationScreenModeAfterTutorial[0x4]; // +0xe4 EPlayerMode
    long                        FrameCachedLODCentre;        // +0xe8
    float                       CachedLODCentre_X;           // +0xec C3DVector::X
    float                       CachedLODCentre_Y;           // +0xf0 C3DVector::Y
    float                       CachedLODCentre_Z;           // +0xf4 C3DVector::Z
    unsigned char               SaveGameLoadStatus[0x4];     // +0xf8 ESaveGameLoadStatus
    CWideStringData*            SaveGamePathName_PStringData; // +0xfc CWideString::PStringData
    bool                        AutoSaveLocked;              // +0x100
    bool                        SerialisingAboutToLoadHeroState; // +0x101
    bool                        SerialisingHeroState;        // +0x102
    bool                        SerialisingNonPersistentQuestItems; // +0x103
    unsigned char               RegionLoadStatus[0x4];       // +0x104 ERegionLoadStatus
    float                       RegionLoadStartPos_X;        // +0x108 C3DVector::X
    float                       RegionLoadStartPos_Y;        // +0x10c C3DVector::Y
    float                       RegionLoadStartPos_Z;        // +0x110 C3DVector::Z
    float                       RegionLoadStartAngleXY;      // +0x114
    unsigned char               RegionLoadFollowers[0xc];    // +0x118 vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    long                        PreviousRegion;              // +0x124
    long                        NumberOfTimesFreezeControlsModeAddedDuringRegionLoad; // +0x128
    bool                        RegionLoadViaTeleporter;     // +0x12c
    bool                        RegionLoadViaDoor;           // +0x12d
    bool                        PutIntoPauseModeOnRegionChange; // +0x12e
    bool                        DisplayMiniMapOnRegionLoad;  // +0x12f
    bool                        RegionLoadWaitingForConfirmation; // +0x130
    bool                        RegionLoadScreenFullyFaded;  // +0x131
    bool                        RegionLoadScreenWasFadedOut; // +0x132
    bool                        WaitingForResetToFrontEndConfirmation; // +0x133
    unsigned char               MostRecentSaveType[0x4];     // +0x134 ESaveType
    unsigned char               MostRecentSaveTypeBeforeManualSave[0x4]; // +0x138 ESaveType
    CWideStringData*            MostRecentManualSaveName_PStringData; // +0x13c CWideString::PStringData
    bool                        AutoSaveCheckPointExists;    // +0x140
    unsigned char               SaveGameMarkerPos[0xf];      // +0x141 C3DVector
    float                       SaveGameMarkerAngleXY;       // +0x150
    float                       GuildSealRecallPos_X;        // +0x154 C3DVector::X
    float                       GuildSealRecallPos_Y;        // +0x158 C3DVector::Y
    float                       GuildSealRecallPos_Z;        // +0x15c C3DVector::Z
    float                       GuildSealRecallAngleXY;      // +0x160
    bool                        WeatherMaskingPrimitivesSent; // +0x164
    unsigned char               _pad_0x165[0x3];             // +0x165
    unsigned char               WeatherMaskingPrimitiveHandles[0xc];  // +0x168 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char               AtmosBanksWaitingToCopy[0x4]; // +0x174 list<CAtmosCopyInfo,std::allocator<CAtmosCopyInfo>_>
    unsigned char               PlayerSpawnStatus[0x4];      // +0x178 EPlayerSpawnStatus
    long                        VillagerReactionDebug;       // +0x17c
    double                      StartTime;                   // +0x180
    double                      TimePlayed;                  // +0x188
    bool                        HasInitialisedStartTime;     // +0x190
    unsigned char               _pad_0x191[0x7];             // +0x191
};
namespace std {
template<class T>class allocator {};template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};template<class T>struct less:public binary_function<T,T,bool>{};template<class T>struct _Identity:public unary_function<T,T>{};
struct _Rb_tree_node_base {bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};template<class T>struct _Rb_tree_node:public _Rb_tree_node_base {T _M_value_field;};
template<class T,class A>struct _Rb_tree_base {protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > >_M_header;};template<class K,class V,class S,class C,class A>class _Rb_tree:public _Rb_tree_base<V,A> {protected:size_t _M_node_count;C _M_key_compare;};template<class K,class C=less<K>,class A=allocator<K> >class set {private:_Rb_tree<K,K,_Identity<K>,C,A>_M_t;};
}
class CGameTimeManager {public:CWorld*World;CMainGameComponent*GameComponent;float Time,PrevTime,TimeStep;long DayCount;bool UsingVillageTime;float VillageTime;bool VillageDay,FastForwardingToTime;float TimeFastForwardingTo,SecondsLeftToFastForwardTime;std::set<long>StopTimeIndexes;long NextStopTimeIndex;bool IsTimeBetween(long,long)const;};
class CTCVillage:public CTCBase {public:
    long             TorchTasksUpdateFrame;                  // +0x0c
    long             FrameStartedSecuritySweep;              // +0x10
    float            PositionPlayerWasInWhenSecuritySweepCalculated_X; // +0x14 C3DVector::X
    float            PositionPlayerWasInWhenSecuritySweepCalculated_Y; // +0x18 C3DVector::Y
    float            PositionPlayerWasInWhenSecuritySweepCalculated_Z; // +0x1c C3DVector::Z
    long             LastAweCrowdEventFrame;                 // +0x20
    long             LastLynchCrowdEventFrame;               // +0x24
    unsigned char    VillageBuildings[0x4];                  // +0x28 list<CIntelligentPointer<CThingBuilding>,std::allocator<CIntelligentPointer<CThingBuilding>_>_>
    unsigned char    VillagePeople[0x4];                     // +0x2c list<CIntelligentPointer<CThingAICreature>,std::allocator<CIntelligentPointer<CThingAICreature>_>_>
    unsigned char    VillageTorches[0x4];                    // +0x30 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageHorns[0x4];                      // +0x34 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageGates[0x4];                      // +0x38 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageCrops[0x4];                      // +0x3c list<CIntelligentPointer<CThingObject>,std::allocator<CIntelligentPointer<CThingObject>_>_>
    unsigned char    VillageCages[0x4];                      // +0x40 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageCampfires[0x4];                  // +0x44 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillagePatrols[0x4];                    // +0x48 list<CIntelligentPointer<CThingTrackNode>,std::allocator<CIntelligentPointer<CThingTrackNode>_>_>
    unsigned char    TorchTasks[0x4];                        // +0x4c list<CCountedPointer<CVillageTaskTorch>,std::allocator<CCountedPointer<CVillageTaskTorch>_>_>
    unsigned char    HornTasks[0x4];                         // +0x50 list<CCountedPointer<CVillageTaskHorn>,std::allocator<CCountedPointer<CVillageTaskHorn>_>_>
    unsigned char    PatrolTasks[0x4];                       // +0x54 list<CCountedPointer<CVillageTaskPatrol>,std::allocator<CCountedPointer<CVillageTaskPatrol>_>_>
    unsigned char    CropTasks[0x4];                         // +0x58 list<CCountedPointer<CVillageTaskPickCrops>,std::allocator<CCountedPointer<CVillageTaskPickCrops>_>_>
    unsigned char    SecuritySweepTasks[0x4];                // +0x5c list<CCountedPointer<CVillageTaskSecuritySweep>,std::allocator<CCountedPointer<CVillageTaskSecuritySweep>_>_>
    unsigned char    CrateTasks[0x4];                        // +0x60 list<CCountedPointer<CVillageTaskCarryCrate>,std::allocator<CCountedPointer<CVillageTaskCarryCrate>_>_>
    unsigned char    PlayersCrimes[0xc];                     // +0x64 set<long,std::less<long>,std::allocator<long>_>
    unsigned char    VillageMarkers[0x4];                    // +0x70 list<CIntelligentPointer<CThingMarker>,std::allocator<CIntelligentPointer<CThingMarker>_>_>
    unsigned char    VillageDummies[0x4];                    // +0x74 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    bool             Initialised;                            // +0x78
    bool             InitiallyPopulated;                     // +0x79
    bool             HasBeenInitiallyPopulated;              // +0x7a
    bool             HornTasksAdded;                         // +0x7b
    bool             IsEnemyBecauseOfCrime;                  // +0x7c
    unsigned char    _pad_0x7d[0x3];                         // +0x7d
    long             FramePlayerLastSeenByGuard;             // +0x80
    bool             InCurfew;                               // +0x84
    unsigned char    _pad_0x85[0x3];                         // +0x85
    long             NumGuards;                              // +0x88
    long             HouseCurrentlyChecking;                 // +0x8c
    bool             TimeIsPaused;                           // +0x90
    unsigned char    _pad_0x91[0x3];                         // +0x91
    CDefPointer<const CVillageDef> PDef;                            // +0x94 CDefPointer<CVillageDef_const_>::Object
    unsigned char    StanceToHero[0x4];                      // +0x98 EVillageStanceToHero
    long             TimeStopIndex;                          // +0x9c
    long             AmountOfFineAlreadyPaid;                // +0xa0
    long             NextGuardRespawn;                       // +0xa4
    long             FrameBribeEnds;                         // +0xa8
    long             FrameForNextGuardGreeting;              // +0xac
    bool             WorkTimeAccordingToCrier;               // +0xb0
    bool             CurfewTimeAccordingToCrier;             // +0xb1
    bool             Limbo;                                  // +0xb2
    bool             LaggyLimbo;                             // +0xb3
    bool             SecuritySweepUrgent;                    // +0xb4
    unsigned char    _pad_0xb5[0x3];                         // +0xb5
    float            InitialSecuritySweepRadius;             // +0xb8
    bool             TeacherTeaching;                        // +0xbc
    unsigned char    GuardFollowingPlayer[0xb];              // +0xbd CIntelligentPointer<CThingAICreature>
    long             NextFrameToFollowPlayer;                // +0xc8
    bool             LightActiveStateToChange;               // +0xcc
    unsigned char    _pad_0xcd[0x3];                         // +0xcd
    long             MaxFrameToAssignTask;                   // +0xd0
    long             HeroEnemyOfGuardsUntil;                 // +0xd4
    long             LastFrameApologised;                    // +0xd8
    unsigned char    GuardWarningHero[0x8];                  // +0xdc CIntelligentPointer<CThingAICreature>
    bool             CurrentIsHeroCriminal;                  // +0xe4
    unsigned char    _pad_0xe5[0x3];                         // +0xe5
__forceinline CDefPointer<const CVillageDef>GetPDef()const{return PDef;}
bool IsNightTime()const;bool IsSociableTime()const;bool IsCurfewTime()const;
};
FABLE_STATIC_ASSERT(sizeof(CResource)==28);FABLE_STATIC_ASSERT(sizeof(CDefClassBase)==37);FABLE_STATIC_ASSERT(sizeof(CSubDefClassBase)==37);FABLE_STATIC_ASSERT(sizeof(CVillageDef)==sizeof(CVillageDefFields));FABLE_STATIC_ASSERT(sizeof(CTCBase)==sizeof(CTCBaseFields));FABLE_STATIC_ASSERT(sizeof(CTCVillage)==sizeof(CTCVillageFields));FABLE_STATIC_ASSERT(sizeof(CWorld)==sizeof(CWorldFields));FABLE_STATIC_ASSERT(sizeof(CGameTimeManager)==60);
bool CTCVillage::IsCurfewTime()const{const CWorld&world=PeekWorld();const CGameTimeManager*time=world.PGameTimeManager.operator->();return time->IsTimeBetween(GetPDef()->CurfewStartTime,GetPDef()->CurfewFinishTime);}

CIVCountedPointeeBase::~CIVCountedPointeeBase(){}void CIVCountedPointeeBase::Delete()const{abort();}
static const CTCBase*expectedOwner;static const CWorld*expectedWorld;static const CGameTimeManager*expectedClock;static long expectedStart,expectedEnd;static unsigned long expectedRefs;static int worldCalls,clockCalls,deletes;static bool expectedReturn,bad;
class TestDef:public CVillageDef {public:virtual void Delete()const{++deletes;}};static TestDef*expectedDef;
const CWorld&CTCBase::PeekWorld()const{if(this!=expectedOwner)bad=true;++worldCalls;return *expectedWorld;}
bool CGameTimeManager::IsTimeBetween(long start,long end)const{if(this!=expectedClock||start!=expectedStart||end!=expectedEnd||expectedDef->IVRefCount!=expectedRefs)bad=true;++clockCalls;return expectedReturn;}
int main(){
union OwnerStorage{double align;unsigned char bytes[sizeof(CTCVillage)];}ownerStorage;
union WorldStorage{double align;unsigned char bytes[sizeof(CWorld)];}worldStorage;
union ClockStorage{double align;unsigned char bytes[sizeof(CGameTimeManager)];}clockStorage;
memset(&ownerStorage,0,sizeof ownerStorage);memset(&worldStorage,0,sizeof worldStorage);memset(&clockStorage,0,sizeof clockStorage);
CTCVillage*owner=reinterpret_cast<CTCVillage*>(ownerStorage.bytes);CWorld*world=reinterpret_cast<CWorld*>(worldStorage.bytes);CGameTimeManager*clock=reinterpret_cast<CGameTimeManager*>(clockStorage.bytes);
expectedOwner=owner;expectedWorld=world;expectedClock=clock;memcpy(&world->PGameTimeManager,&clock,sizeof clock);TestDef def;expectedDef=&def;owner->PDef.Object=&def;
unsigned char ownerBefore[sizeof(CTCVillage)],worldBefore[sizeof(CWorld)],clockBefore[sizeof(CGameTimeManager)];memcpy(ownerBefore,owner,sizeof ownerBefore);memcpy(worldBefore,world,sizeof worldBefore);memcpy(clockBefore,clock,sizeof clockBefore);
const long values[]={-1,0,1,1234,2345,1000000};const unsigned long refs[]={0,1,2,7,100};unsigned cases=0;
for(int s=0;s<6;++s)for(int e=0;e<6;++e)for(int r=0;r<5;++r)for(int answer=0;answer<2;++answer){
 expectedStart=values[s];expectedEnd=values[e];def.CurfewStartTime=expectedStart;def.CurfewFinishTime=expectedEnd;
 worldCalls=clockCalls=deletes=0;bad=false;def.IVRefCount=refs[r];expectedRefs=refs[r]+2;expectedReturn=answer!=0;
 bool result=owner->IsCurfewTime();
 if(bad||result!=expectedReturn||worldCalls!=1||clockCalls!=1||def.IVRefCount!=refs[r]||deletes!=(refs[r]==0?1:0))return 1;
 if(memcmp(ownerBefore,owner,sizeof ownerBefore)||memcmp(worldBefore,world,sizeof worldBefore)||memcmp(clockBefore,clock,sizeof clockBefore))return 2;
 ++cases;
}
def.IVRefCount=1;if(cases!=360)return 3;puts("00658db3_TEST PASS");return 0;}