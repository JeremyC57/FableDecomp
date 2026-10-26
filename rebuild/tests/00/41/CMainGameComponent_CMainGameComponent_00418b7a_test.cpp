#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif

// CMainGameComponent::~CMainGameComponent() -- conditionally shut down, then destroy owned game resources.
// Native00418b7a length193. RTTI: CGameComponent->CBaseClassNonCopyable->CBaseClass, secondary CDeviceResetCallback at4.
// Actual fable_game.h included as donor; full native vtable signatures below, no fabricated slots.
#pragma optimize("s", on)
#define CMainGameComponent CMainGameComponentDonor
#define CGameComponent CGameComponentDonor
#define CMainGameComponentInit CMainGameComponentInitDonor
#define CGame CGameDonor
#define CCharString CCharStringDonor
#pragma once

#pragma once

#include <stddef.h>

#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifndef _MSC_VER
#define FABLE_STATIC_ASSERT(expression)
#else
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_abi_assert_, __LINE__)[(expression) ? 1 : -1]
#endif


class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#pragma pack(push, 1)
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#pragma pack(pop)

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    long GetLength() const;
    CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();
    CWideStringData* AllocStringData(
        const wchar_t* text,
        long length);

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    ~CCharString();
    operator const char*() const;
    CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

private:
    void UnassignString();
    CCharStringData* AllocStringData(
        const char* text,
        long length);

    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);



class CGame;

class CGameComponent
{
public:
    virtual ~CGameComponent();
    virtual void Init();
    virtual bool Run(CGameComponent** nextComponent);
};

class CMainGameComponentInit
{
public:
    CWideString firstWideString;
    CWideString secondWideString;
    CCharString narrowString;
    CWideString fourthWideString;
};

class CFrontendGameComponentInit
{ public:
    fable_u32 value;
};

typedef CFrontendGameComponentInit CNewFrontendGameComponentInit;

class CMainGameComponent : public CGameComponent
{
public:
    static void FABLE_FASTCALL GenerateMetFilesFromLugFiles();

    CMainGameComponent(
        CGame& game,
        const CMainGameComponentInit& init);

private:
    fable_u8 unknown04_[0x161E4];
};

class CFrontendGameComponent : public CGameComponent
{
public:
    CFrontendGameComponent(
        CGame& game,
        const CFrontendGameComponentInit& init);

private:
    fable_u8 unknown04_[0x1E5C];
};

class CNewFrontendGameComponent : public CGameComponent
{
public:
    CNewFrontendGameComponent(
        CGame& game,
        const CNewFrontendGameComponentInit& init);

    static void CompileDefs();

private:
    fable_u8 unknown04_[0x144];
};

class CGame
{
public:
    void Play();

private:
    fable_u8 unknown00_[8];
    CGameComponent* component_;
    fable_u8 unknown0C_[0x200];
    bool quit_;
};

extern fable_u8 g_FableCompileFrontendDefinitions_013B8648;
extern fable_u8 g_FableStartMainGame_013B8605;
extern fable_u8 g_FableUseLegacyFrontend_013B8642;
extern CGameComponent* g_FableRetiredGameComponent_013B7D58;
extern CWideString g_FableMainGameStartupPath_013B7D5C;

FABLE_STATIC_ASSERT(sizeof(CMainGameComponentInit) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CFrontendGameComponentInit) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent) == 0x161E8);
FABLE_STATIC_ASSERT(sizeof(CFrontendGameComponent) == 0x1E60);
FABLE_STATIC_ASSERT(sizeof(CNewFrontendGameComponent) == 0x148);
FABLE_STATIC_ASSERT(sizeof(CGame) == 0x210);

#undef CCharString
#undef CGame
#undef CMainGameComponentInit
#undef CGameComponent
#undef CMainGameComponent
#define CWorld CWorldDonor
#ifndef FABLE_ENGINE_CWORLD_H
#define FABLE_ENGINE_CWORLD_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CWorld = 0x1b8.
// Retail evidence: 3 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class C3DAnimationManager;
class CAIGameCameraBase;
class CAtmosProcess;
class CBulletTimeManager;
class CCPPointerInfo;
class CCharStringData;
class CCombatManager;
class CEnvironment;
class CFactionManager;
class CGameDefinitionManager;
class CGameScriptInterface;
class CGameTimeManager;
class CMainGameComponent;
class CMeshDataBank;
class CMessageEventManager;
class CMusicManager;
class CNavigatorManager;
class COpinionReactionManager;
class CPlayerManager;
class CScriptConversationManager;
class CScriptInfoManager;
class CThingManager;
class CThingSearchTools;
class CWideStringData;
class CWorldMap;

#pragma pack(push, 1)
class CWorld { public:
    void*                       __vftable;                   // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char               _base_0x04[0x4];             // +0x04 flattened base-class subobject(s)
    CMainGameComponent*         Component;                   // +0x08
    CPlayerManager*             PlayerManager;               // +0x0c
    CGameDefinitionManager*     DefinitionManager;           // +0x10
    CWorldMap*                  PWorldMap_ptr;               // +0x14 scoped_ptr<CWorldMap>::ptr
    CEnvironment*               PEnvironment_ptr;            // +0x18 scoped_ptr<CEnvironment>::ptr
    CGameTimeManager*           PGameTimeManager_ptr;        // +0x1c scoped_ptr<CGameTimeManager>::ptr
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
    unsigned char               InitialActiveQuests[0x10];   // +0xac CArray<CCharString>
    unsigned char               RegisteredQuests[0x10];      // +0xbc vector<CCharString,std::allocator<CCharString>_>
    unsigned char               ActiveTestQuests[0x10];      // +0xcc vector<CTestQuest,std::allocator<CTestQuest>_>
    unsigned long               CreatureGenerationDisabledGroups; // +0xdc
    bool                        CreatureGenerationEnabled;   // +0xe0
    bool                        TeleportingEnabled;          // +0xe1
    bool                        ExperienceSpendingEnabled;   // +0xe2
    bool                        SavingEnabled;               // +0xe3
    bool                        DontPopulateNextLoadedRegion; // +0xe4
    bool                        HeroSleepingEnabled;         // +0xe5
    bool                        MapTableShowQuestCardsOnUsed; // +0xe6
    bool                        ScreenToFadeInOnNextRegionChange; // +0xe7
    bool                        DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn; // +0xe8
    bool                        MiniMapEnabled;              // +0xe9
    bool                        MiniMapActiveBeforeDisabled; // +0xea
    bool                        RegionLoadDisplayRegionName; // +0xeb
    bool                        GuildMasterMessagesEnabled;  // +0xec
    bool                        SummonerDeathExplosionAffectsHero; // +0xed
    bool                        WaitingForInventoryTutorialToFinish; // +0xee
    unsigned char               _pad_0xef[0x1];              // +0xef
    unsigned char               HeroInformationScreenModeAfterTutorial[0x4]; // +0xf0 EPlayerMode
    long                        FrameCachedLODCentre;        // +0xf4
    float                       CachedLODCentre_X;           // +0xf8 C3DVector::X
    float                       CachedLODCentre_Y;           // +0xfc C3DVector::Y
    float                       CachedLODCentre_Z;           // +0x100 C3DVector::Z
    unsigned char               SaveGameLoadStatus[0x4];     // +0x104 ESaveGameLoadStatus
    CWideStringData*            SaveGamePathName_PStringData; // +0x108 CWideString::PStringData
    bool                        AutoSaveLocked;              // +0x10c
    bool                        SerialisingAboutToLoadHeroState; // +0x10d
    bool                        SerialisingHeroState;        // +0x10e
    bool                        SerialisingNonPersistentQuestItems; // +0x10f
    unsigned char               RegionLoadStatus[0x4];       // +0x110 ERegionLoadStatus
    float                       RegionLoadStartPos_X;        // +0x114 C3DVector::X
    float                       RegionLoadStartPos_Y;        // +0x118 C3DVector::Y
    float                       RegionLoadStartPos_Z;        // +0x11c C3DVector::Z
    float                       RegionLoadStartAngleXY;      // +0x120
    unsigned char               RegionLoadFollowers[0x10];   // +0x124 vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    long                        PreviousRegion;              // +0x134
    long                        NumberOfTimesFreezeControlsModeAddedDuringRegionLoad; // +0x138
    bool                        RegionLoadViaTeleporter;     // +0x13c
    bool                        RegionLoadViaDoor;           // +0x13d
    bool                        PutIntoPauseModeOnRegionChange; // +0x13e
    bool                        DisplayMiniMapOnRegionLoad;  // +0x13f
    bool                        RegionLoadWaitingForConfirmation; // +0x140
    bool                        RegionLoadScreenFullyFaded;  // +0x141
    bool                        RegionLoadScreenWasFadedOut; // +0x142
    bool                        WaitingForResetToFrontEndConfirmation; // +0x143
    unsigned char               MostRecentSaveType[0x4];     // +0x144 ESaveType
    unsigned char               MostRecentSaveTypeBeforeManualSave[0x4]; // +0x148 ESaveType
    CWideStringData*            MostRecentManualSaveName_PStringData; // +0x14c CWideString::PStringData
    bool                        AutoSaveCheckPointExists;    // +0x150
    unsigned char               SaveGameMarkerPos[0xf];      // +0x151 C3DVector
    float                       SaveGameMarkerAngleXY;       // +0x160
    float                       GuildSealRecallPos_X;        // +0x164 C3DVector::X
    float                       GuildSealRecallPos_Y;        // +0x168 C3DVector::Y
    float                       GuildSealRecallPos_Z;        // +0x16c C3DVector::Z
    float                       GuildSealRecallAngleXY;      // +0x170
    bool                        WeatherMaskingPrimitivesSent; // +0x174
    unsigned char               _pad_0x175[0x3];             // +0x175
    unsigned char               WeatherMaskingPrimitiveHandles[0x10]; // +0x178 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char               AtmosBanksWaitingToCopy[0xc]; // +0x188 list<CAtmosCopyInfo,std::allocator<CAtmosCopyInfo>_>
    unsigned char               PlayerSpawnStatus[0x4];      // +0x194 EPlayerSpawnStatus
    long                        VillagerReactionDebug;       // +0x198
    unsigned char               _pad_0x19c[0x4];             // +0x19c
    double                      StartTime;                   // +0x1a0
    double                      TimePlayed;                  // +0x1a8
    bool                        HasInitialisedStartTime;     // +0x1b0
    unsigned char               _pad_0x1b1[0x7];             // +0x1b1
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CWorld) == 0x1b8);
FABLE_STATIC_ASSERT(offsetof(CWorld, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CWorld, Component) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CWorld, PlayerManager) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CWorld, DefinitionManager) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CWorld, PWorldMap_ptr) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CWorld, PEnvironment_ptr) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameTimeManager_ptr) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PThingSearchTools_ptr) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CWorld, PAtmosProcessor_Data) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CWorld, PAtmosProcessor_Info) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameCamera_ptr) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameCameraManager_ptr) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CWorld, PCurrentGameCamera) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameScriptInterface_ptr) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMainMeshBank_Data) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMainMeshBank_Info) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CWorld, PAnimationManager) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CWorld, PNavigationManager_ptr) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CWorld, PThingCombatManager_ptr) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PThingManager_ptr) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CWorld, PFactionManager_ptr) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CWorld, PScriptInfoManager_Data) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CWorld, PScriptInfoManager_Info) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMessageEventManager_Data) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMessageEventManager_Info) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CWorld, PBulletTimeManager_ptr) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMusicManager_Data) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CWorld, PMusicManager_Info) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CWorld, POpinionReactionManager_Data) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CWorld, POpinionReactionManager_Info) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CWorld, PScriptConversationManager_ptr) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CWorld, JustLoaded) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CWorld, CurrentWorldName_PStringData) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CWorld, ConsolePauseAtFrameNumber) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CWorld, FrameStarted3DRendering) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CWorld, LastUpdateTimeLength) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CWorld, LastUpdateTime) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CWorld, CountdownTimer) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CWorld, Paused) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CWorld, SlowMotion) == 0xa5);
FABLE_STATIC_ASSERT(offsetof(CWorld, ShowDebugText) == 0xa6);
FABLE_STATIC_ASSERT(offsetof(CWorld, ShowFPSText) == 0xa7);
FABLE_STATIC_ASSERT(offsetof(CWorld, ShowProfileText) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CWorld, InitialActiveQuests) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegisteredQuests) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CWorld, ActiveTestQuests) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CWorld, CreatureGenerationDisabledGroups) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CWorld, CreatureGenerationEnabled) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CWorld, TeleportingEnabled) == 0xe1);
FABLE_STATIC_ASSERT(offsetof(CWorld, ExperienceSpendingEnabled) == 0xe2);
FABLE_STATIC_ASSERT(offsetof(CWorld, SavingEnabled) == 0xe3);
FABLE_STATIC_ASSERT(offsetof(CWorld, DontPopulateNextLoadedRegion) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CWorld, HeroSleepingEnabled) == 0xe5);
FABLE_STATIC_ASSERT(offsetof(CWorld, MapTableShowQuestCardsOnUsed) == 0xe6);
FABLE_STATIC_ASSERT(offsetof(CWorld, ScreenToFadeInOnNextRegionChange) == 0xe7);
FABLE_STATIC_ASSERT(offsetof(CWorld, DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CWorld, MiniMapEnabled) == 0xe9);
FABLE_STATIC_ASSERT(offsetof(CWorld, MiniMapActiveBeforeDisabled) == 0xea);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadDisplayRegionName) == 0xeb);
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildMasterMessagesEnabled) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CWorld, SummonerDeathExplosionAffectsHero) == 0xed);
FABLE_STATIC_ASSERT(offsetof(CWorld, WaitingForInventoryTutorialToFinish) == 0xee);
FABLE_STATIC_ASSERT(offsetof(CWorld, HeroInformationScreenModeAfterTutorial) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CWorld, FrameCachedLODCentre) == 0xf4);
FABLE_STATIC_ASSERT(offsetof(CWorld, CachedLODCentre_X) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CWorld, CachedLODCentre_Y) == 0xfc);
FABLE_STATIC_ASSERT(offsetof(CWorld, CachedLODCentre_Z) == 0x100);
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGameLoadStatus) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGamePathName_PStringData) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CWorld, AutoSaveLocked) == 0x10c);
FABLE_STATIC_ASSERT(offsetof(CWorld, SerialisingAboutToLoadHeroState) == 0x10d);
FABLE_STATIC_ASSERT(offsetof(CWorld, SerialisingHeroState) == 0x10e);
FABLE_STATIC_ASSERT(offsetof(CWorld, SerialisingNonPersistentQuestItems) == 0x10f);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStatus) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartPos_X) == 0x114);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartPos_Y) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartPos_Z) == 0x11c);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartAngleXY) == 0x120);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadFollowers) == 0x124);
FABLE_STATIC_ASSERT(offsetof(CWorld, PreviousRegion) == 0x134);
FABLE_STATIC_ASSERT(offsetof(CWorld, NumberOfTimesFreezeControlsModeAddedDuringRegionLoad) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadViaTeleporter) == 0x13c);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadViaDoor) == 0x13d);
FABLE_STATIC_ASSERT(offsetof(CWorld, PutIntoPauseModeOnRegionChange) == 0x13e);
FABLE_STATIC_ASSERT(offsetof(CWorld, DisplayMiniMapOnRegionLoad) == 0x13f);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadWaitingForConfirmation) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadScreenFullyFaded) == 0x141);
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadScreenWasFadedOut) == 0x142);
FABLE_STATIC_ASSERT(offsetof(CWorld, WaitingForResetToFrontEndConfirmation) == 0x143);
FABLE_STATIC_ASSERT(offsetof(CWorld, MostRecentSaveType) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CWorld, MostRecentSaveTypeBeforeManualSave) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CWorld, MostRecentManualSaveName_PStringData) == 0x14c);
FABLE_STATIC_ASSERT(offsetof(CWorld, AutoSaveCheckPointExists) == 0x150);
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGameMarkerPos) == 0x151);
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGameMarkerAngleXY) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallPos_X) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallPos_Y) == 0x168);
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallPos_Z) == 0x16c);
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallAngleXY) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CWorld, WeatherMaskingPrimitivesSent) == 0x174);
FABLE_STATIC_ASSERT(offsetof(CWorld, WeatherMaskingPrimitiveHandles) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CWorld, AtmosBanksWaitingToCopy) == 0x188);
FABLE_STATIC_ASSERT(offsetof(CWorld, PlayerSpawnStatus) == 0x194);
FABLE_STATIC_ASSERT(offsetof(CWorld, VillagerReactionDebug) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CWorld, StartTime) == 0x1a0);
FABLE_STATIC_ASSERT(offsetof(CWorld, TimePlayed) == 0x1a8);
FABLE_STATIC_ASSERT(offsetof(CWorld, HasInitialisedStartTime) == 0x1b0);

#endif // FABLE_ENGINE_CWORLD_H

#undef CWorld
#define CPlayerManager CPlayerManagerDonor
#ifndef FABLE_ENGINE_CPLAYERMANAGER_H
#define FABLE_ENGINE_CPLAYERMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CPlayerManager = 0x34.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CGameDefinitionManager;
class CMainGameComponent;

#pragma pack(push, 1)
class CPlayerManager { public:
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    CMainGameComponent*     Component;                       // +0x04
    CGameDefinitionManager* DefinitionManager;               // +0x08
    unsigned char           Players[0x10];                   // +0x0c vector<CPlayer_*,std::allocator<CPlayer_*>_>
    long                    PlayerNeutral;                   // +0x1c
    long                    MainPlayer;                      // +0x20
    unsigned char           HeroSwapPlayerScriptNames[0x10]; // +0x24 vector<CCharString,std::allocator<CCharString>_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CPlayerManager) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, Component) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, DefinitionManager) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, Players) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, PlayerNeutral) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, MainPlayer) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CPlayerManager, HeroSwapPlayerScriptNames) == 0x24);

#endif // FABLE_ENGINE_CPLAYERMANAGER_H

#undef CPlayerManager
#define CGamePlayerInterface CGamePlayerInterfaceDonor
#ifndef FABLE_ENGINE_CGAMEPLAYERINTERFACE_H
#define FABLE_ENGINE_CGAMEPLAYERINTERFACE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CGamePlayerInterface = 0x8a8.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CDefPointeeBase;
class CMainGameComponent;
class CPlayerManager;

#pragma pack(push, 1)
class CGamePlayerInterface { public:
    void*               __vftable;                           // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char       _pad_0x04[0x4];                      // +0x04
    unsigned char       _pad_0x08[0x4];                      // +0x08
    unsigned char       _pad_0x0c[0x4];                      // +0x0c
    unsigned char       _pad_0x10[0x4];                      // +0x10
    unsigned char       _pad_0x14[0x4];                      // +0x14
    unsigned char       _pad_0x18[0x4];                      // +0x18
    unsigned char       _pad_0x1c[0x4];                      // +0x1c
    unsigned char       _pad_0x20[0x4];                      // +0x20
    unsigned char       _pad_0x24[0x4];                      // +0x24
    unsigned char       _pad_0x28[0x4];                      // +0x28
    unsigned char       _pad_0x2c[0x4];                      // +0x2c
    unsigned char       _pad_0x30[0x4];                      // +0x30
    unsigned char       _pad_0x34[0x4];                      // +0x34
    unsigned char       _pad_0x38[0x4];                      // +0x38
    unsigned char       _pad_0x3c[0x4];                      // +0x3c
    unsigned char       _pad_0x40[0x4];                      // +0x40
    unsigned char       _pad_0x44[0x4];                      // +0x44
    unsigned char       _pad_0x48[0x4];                      // +0x48
    unsigned char       _pad_0x4c[0x4];                      // +0x4c
    unsigned char       _pad_0x50[0x4];                      // +0x50
    unsigned char       _pad_0x54[0x4];                      // +0x54
    unsigned char       _pad_0x58[0x4];                      // +0x58
    unsigned char       _pad_0x5c[0x4];                      // +0x5c
    unsigned char       _pad_0x60[0x4];                      // +0x60
    unsigned char       _pad_0x64[0x4];                      // +0x64
    unsigned char       _pad_0x68[0x4];                      // +0x68
    unsigned char       _pad_0x6c[0x4];                      // +0x6c
    unsigned char       _pad_0x70[0x4];                      // +0x70
    unsigned char       _pad_0x74[0x4];                      // +0x74
    unsigned char       _pad_0x78[0x4];                      // +0x78
    unsigned char       _pad_0x7c[0x4];                      // +0x7c
    unsigned char       _pad_0x80[0x4];                      // +0x80
    unsigned char       _pad_0x84[0x4];                      // +0x84
    unsigned char       _pad_0x88[0x4];                      // +0x88
    unsigned char       _pad_0x8c[0x4];                      // +0x8c
    unsigned char       _pad_0x90[0x4];                      // +0x90
    unsigned char       _pad_0x94[0x4];                      // +0x94
    unsigned char       _pad_0x98[0x4];                      // +0x98
    unsigned char       _pad_0x9c[0x4];                      // +0x9c
    unsigned char       _pad_0xa0[0x4];                      // +0xa0
    unsigned char       _pad_0xa4[0x4];                      // +0xa4
    unsigned char       _pad_0xa8[0x4];                      // +0xa8
    unsigned char       _pad_0xac[0x4];                      // +0xac
    unsigned char       _pad_0xb0[0x4];                      // +0xb0
    unsigned char       _pad_0xb4[0x4];                      // +0xb4
    unsigned char       _pad_0xb8[0x4];                      // +0xb8
    unsigned char       _pad_0xbc[0x4];                      // +0xbc
    unsigned char       _pad_0xc0[0x4];                      // +0xc0
    unsigned char       _pad_0xc4[0x4];                      // +0xc4
    unsigned char       _pad_0xc8[0x4];                      // +0xc8
    unsigned char       _pad_0xcc[0x4];                      // +0xcc
    unsigned char       _pad_0xd0[0x4];                      // +0xd0
    unsigned char       _pad_0xd4[0x4];                      // +0xd4
    unsigned char       _pad_0xd8[0x4];                      // +0xd8
    unsigned char       _pad_0xdc[0x4];                      // +0xdc
    unsigned char       _pad_0xe0[0x4];                      // +0xe0
    unsigned char       _pad_0xe4[0x4];                      // +0xe4
    unsigned char       _pad_0xe8[0x4];                      // +0xe8
    unsigned char       _pad_0xec[0x4];                      // +0xec
    unsigned char       _pad_0xf0[0x4];                      // +0xf0
    unsigned char       _pad_0xf4[0x4];                      // +0xf4
    unsigned char       _pad_0xf8[0x4];                      // +0xf8
    unsigned char       _pad_0xfc[0x4];                      // +0xfc
    unsigned char       _pad_0x100[0x4];                     // +0x100
    unsigned char       _pad_0x104[0x4];                     // +0x104
    unsigned char       _pad_0x108[0x4];                     // +0x108
    unsigned char       _pad_0x10c[0x4];                     // +0x10c
    unsigned char       _pad_0x110[0x4];                     // +0x110
    unsigned char       _pad_0x114[0x4];                     // +0x114
    unsigned char       _pad_0x118[0x4];                     // +0x118
    unsigned char       _pad_0x11c[0x4];                     // +0x11c
    unsigned char       _pad_0x120[0x4];                     // +0x120
    unsigned char       _pad_0x124[0x4];                     // +0x124
    unsigned char       _pad_0x128[0x4];                     // +0x128
    unsigned char       _pad_0x12c[0x4];                     // +0x12c
    unsigned char       _pad_0x130[0x4];                     // +0x130
    unsigned char       _pad_0x134[0x4];                     // +0x134
    unsigned char       _pad_0x138[0x4];                     // +0x138
    unsigned char       _pad_0x13c[0x4];                     // +0x13c
    unsigned char       _pad_0x140[0x4];                     // +0x140
    unsigned char       _pad_0x144[0x4];                     // +0x144
    unsigned char       _pad_0x148[0x4];                     // +0x148
    unsigned char       _pad_0x14c[0x4];                     // +0x14c
    unsigned char       _pad_0x150[0x4];                     // +0x150
    unsigned char       _pad_0x154[0x4];                     // +0x154
    unsigned char       _pad_0x158[0x4];                     // +0x158
    unsigned char       _pad_0x15c[0x4];                     // +0x15c
    unsigned char       _pad_0x160[0x4];                     // +0x160
    unsigned char       _pad_0x164[0x4];                     // +0x164
    unsigned char       _pad_0x168[0x4];                     // +0x168
    unsigned char       _pad_0x16c[0x4];                     // +0x16c
    unsigned char       _pad_0x170[0x4];                     // +0x170
    unsigned char       _pad_0x174[0x4];                     // +0x174
    unsigned char       _pad_0x178[0x4];                     // +0x178
    unsigned char       _pad_0x17c[0x4];                     // +0x17c
    unsigned char       _pad_0x180[0x4];                     // +0x180
    unsigned char       _pad_0x184[0x4];                     // +0x184
    unsigned char       _pad_0x188[0x4];                     // +0x188
    unsigned char       _pad_0x18c[0x4];                     // +0x18c
    unsigned char       _pad_0x190[0x4];                     // +0x190
    unsigned char       _pad_0x194[0x4];                     // +0x194
    unsigned char       _pad_0x198[0x4];                     // +0x198
    unsigned char       _pad_0x19c[0x4];                     // +0x19c
    unsigned char       _pad_0x1a0[0x4];                     // +0x1a0
    unsigned char       _pad_0x1a4[0x4];                     // +0x1a4
    unsigned char       _pad_0x1a8[0x4];                     // +0x1a8
    unsigned char       _pad_0x1ac[0x4];                     // +0x1ac
    unsigned char       _pad_0x1b0[0x4];                     // +0x1b0
    unsigned char       _pad_0x1b4[0x4];                     // +0x1b4
    unsigned char       _pad_0x1b8[0x4];                     // +0x1b8
    unsigned char       _pad_0x1bc[0x4];                     // +0x1bc
    unsigned char       _pad_0x1c0[0x4];                     // +0x1c0
    unsigned char       _pad_0x1c4[0x4];                     // +0x1c4
    unsigned char       _pad_0x1c8[0x4];                     // +0x1c8
    unsigned char       _pad_0x1cc[0x4];                     // +0x1cc
    unsigned char       _pad_0x1d0[0x4];                     // +0x1d0
    unsigned char       _pad_0x1d4[0x4];                     // +0x1d4
    unsigned char       _pad_0x1d8[0x4];                     // +0x1d8
    unsigned char       _pad_0x1dc[0x4];                     // +0x1dc
    unsigned char       _pad_0x1e0[0x4];                     // +0x1e0
    unsigned char       _pad_0x1e4[0x4];                     // +0x1e4
    unsigned char       _pad_0x1e8[0x4];                     // +0x1e8
    unsigned char       _pad_0x1ec[0x4];                     // +0x1ec
    unsigned char       _pad_0x1f0[0x4];                     // +0x1f0
    unsigned char       _pad_0x1f4[0x4];                     // +0x1f4
    unsigned char       _pad_0x1f8[0x4];                     // +0x1f8
    unsigned char       _pad_0x1fc[0x4];                     // +0x1fc
    unsigned char       _pad_0x200[0x4];                     // +0x200
    unsigned char       _pad_0x204[0x4];                     // +0x204
    unsigned char       _pad_0x208[0x4];                     // +0x208
    unsigned char       _pad_0x20c[0x4];                     // +0x20c
    unsigned char       _pad_0x210[0x4];                     // +0x210
    unsigned char       _pad_0x214[0x4];                     // +0x214
    unsigned char       _pad_0x218[0x4];                     // +0x218
    unsigned char       _pad_0x21c[0x4];                     // +0x21c
    unsigned char       _pad_0x220[0x4];                     // +0x220
    unsigned char       _pad_0x224[0x4];                     // +0x224
    unsigned char       _pad_0x228[0x4];                     // +0x228
    unsigned char       _pad_0x22c[0x4];                     // +0x22c
    unsigned char       _pad_0x230[0x4];                     // +0x230
    unsigned char       _pad_0x234[0x4];                     // +0x234
    unsigned char       _pad_0x238[0x4];                     // +0x238
    unsigned char       _pad_0x23c[0x4];                     // +0x23c
    unsigned char       _pad_0x240[0x4];                     // +0x240
    unsigned char       _pad_0x244[0x4];                     // +0x244
    unsigned char       _pad_0x248[0x4];                     // +0x248
    unsigned char       _pad_0x24c[0x4];                     // +0x24c
    unsigned char       _pad_0x250[0x4];                     // +0x250
    unsigned char       _pad_0x254[0x4];                     // +0x254
    unsigned char       _pad_0x258[0x4];                     // +0x258
    unsigned char       _pad_0x25c[0x4];                     // +0x25c
    unsigned char       _pad_0x260[0x4];                     // +0x260
    unsigned char       _pad_0x264[0x4];                     // +0x264
    unsigned char       _pad_0x268[0x4];                     // +0x268
    unsigned char       _pad_0x26c[0x4];                     // +0x26c
    unsigned char       _pad_0x270[0x4];                     // +0x270
    unsigned char       _pad_0x274[0x4];                     // +0x274
    unsigned char       _pad_0x278[0x4];                     // +0x278
    unsigned char       _pad_0x27c[0x4];                     // +0x27c
    unsigned char       _pad_0x280[0x4];                     // +0x280
    unsigned char       _pad_0x284[0x4];                     // +0x284
    unsigned char       _pad_0x288[0x4];                     // +0x288
    unsigned char       _pad_0x28c[0x4];                     // +0x28c
    unsigned char       _pad_0x290[0x4];                     // +0x290
    unsigned char       _pad_0x294[0x4];                     // +0x294
    unsigned char       _pad_0x298[0x4];                     // +0x298
    unsigned char       _pad_0x29c[0x4];                     // +0x29c
    unsigned char       _pad_0x2a0[0x4];                     // +0x2a0
    unsigned char       _pad_0x2a4[0x4];                     // +0x2a4
    unsigned char       _pad_0x2a8[0x4];                     // +0x2a8
    unsigned char       _pad_0x2ac[0x4];                     // +0x2ac
    unsigned char       _pad_0x2b0[0x4];                     // +0x2b0
    unsigned char       _pad_0x2b4[0x4];                     // +0x2b4
    unsigned char       _pad_0x2b8[0x4];                     // +0x2b8
    unsigned char       _pad_0x2bc[0x4];                     // +0x2bc
    unsigned char       _pad_0x2c0[0x4];                     // +0x2c0
    unsigned char       _pad_0x2c4[0x4];                     // +0x2c4
    unsigned char       _pad_0x2c8[0x4];                     // +0x2c8
    unsigned char       _pad_0x2cc[0x4];                     // +0x2cc
    unsigned char       _pad_0x2d0[0x4];                     // +0x2d0
    unsigned char       _pad_0x2d4[0x4];                     // +0x2d4
    unsigned char       _pad_0x2d8[0x4];                     // +0x2d8
    unsigned char       _pad_0x2dc[0x4];                     // +0x2dc
    unsigned char       _pad_0x2e0[0x4];                     // +0x2e0
    unsigned char       _pad_0x2e4[0x4];                     // +0x2e4
    unsigned char       _pad_0x2e8[0x4];                     // +0x2e8
    unsigned char       _pad_0x2ec[0x4];                     // +0x2ec
    unsigned char       _pad_0x2f0[0x4];                     // +0x2f0
    unsigned char       _pad_0x2f4[0x4];                     // +0x2f4
    unsigned char       _pad_0x2f8[0x4];                     // +0x2f8
    unsigned char       _pad_0x2fc[0x4];                     // +0x2fc
    unsigned char       _pad_0x300[0x4];                     // +0x300
    unsigned char       _pad_0x304[0x4];                     // +0x304
    unsigned char       _pad_0x308[0x4];                     // +0x308
    unsigned char       _pad_0x30c[0x4];                     // +0x30c
    unsigned char       _pad_0x310[0x4];                     // +0x310
    unsigned char       _pad_0x314[0x4];                     // +0x314
    unsigned char       _pad_0x318[0x4];                     // +0x318
    unsigned char       _pad_0x31c[0x4];                     // +0x31c
    unsigned char       _pad_0x320[0x4];                     // +0x320
    unsigned char       _pad_0x324[0x4];                     // +0x324
    unsigned char       _pad_0x328[0x4];                     // +0x328
    unsigned char       _pad_0x32c[0x4];                     // +0x32c
    unsigned char       _pad_0x330[0x4];                     // +0x330
    unsigned char       _pad_0x334[0x4];                     // +0x334
    unsigned char       _pad_0x338[0x4];                     // +0x338
    unsigned char       _pad_0x33c[0x4];                     // +0x33c
    unsigned char       _pad_0x340[0x4];                     // +0x340
    unsigned char       _pad_0x344[0x4];                     // +0x344
    unsigned char       _pad_0x348[0x4];                     // +0x348
    unsigned char       _pad_0x34c[0x4];                     // +0x34c
    unsigned char       _pad_0x350[0x4];                     // +0x350
    unsigned char       _pad_0x354[0x4];                     // +0x354
    unsigned char       _pad_0x358[0x4];                     // +0x358
    unsigned char       _pad_0x35c[0x4];                     // +0x35c
    unsigned char       _pad_0x360[0x4];                     // +0x360
    unsigned char       _pad_0x364[0x4];                     // +0x364
    unsigned char       _pad_0x368[0x4];                     // +0x368
    unsigned char       _pad_0x36c[0x4];                     // +0x36c
    unsigned char       _pad_0x370[0x4];                     // +0x370
    unsigned char       _pad_0x374[0x4];                     // +0x374
    unsigned char       _pad_0x378[0x4];                     // +0x378
    unsigned char       _pad_0x37c[0x4];                     // +0x37c
    unsigned char       _pad_0x380[0x4];                     // +0x380
    unsigned char       _pad_0x384[0x4];                     // +0x384
    unsigned char       _pad_0x388[0x4];                     // +0x388
    unsigned char       _pad_0x38c[0x4];                     // +0x38c
    unsigned char       _pad_0x390[0x4];                     // +0x390
    unsigned char       _pad_0x394[0x4];                     // +0x394
    unsigned char       _pad_0x398[0x4];                     // +0x398
    unsigned char       _pad_0x39c[0x4];                     // +0x39c
    unsigned char       _pad_0x3a0[0x4];                     // +0x3a0
    unsigned char       _pad_0x3a4[0x4];                     // +0x3a4
    unsigned char       _pad_0x3a8[0x4];                     // +0x3a8
    unsigned char       _pad_0x3ac[0x4];                     // +0x3ac
    unsigned char       _pad_0x3b0[0x4];                     // +0x3b0
    unsigned char       _pad_0x3b4[0x4];                     // +0x3b4
    unsigned char       _pad_0x3b8[0x4];                     // +0x3b8
    unsigned char       _pad_0x3bc[0x4];                     // +0x3bc
    unsigned char       _pad_0x3c0[0x4];                     // +0x3c0
    unsigned char       _pad_0x3c4[0x4];                     // +0x3c4
    unsigned char       _pad_0x3c8[0x4];                     // +0x3c8
    unsigned char       _pad_0x3cc[0x4];                     // +0x3cc
    unsigned char       _pad_0x3d0[0x4];                     // +0x3d0
    unsigned char       _pad_0x3d4[0x4];                     // +0x3d4
    unsigned char       _pad_0x3d8[0x4];                     // +0x3d8
    unsigned char       _pad_0x3dc[0x4];                     // +0x3dc
    unsigned char       _pad_0x3e0[0x4];                     // +0x3e0
    unsigned char       _pad_0x3e4[0x4];                     // +0x3e4
    unsigned char       _pad_0x3e8[0x4];                     // +0x3e8
    unsigned char       _pad_0x3ec[0x4];                     // +0x3ec
    unsigned char       _pad_0x3f0[0x4];                     // +0x3f0
    unsigned char       _pad_0x3f4[0x4];                     // +0x3f4
    unsigned char       _pad_0x3f8[0x4];                     // +0x3f8
    unsigned char       _pad_0x3fc[0x4];                     // +0x3fc
    unsigned char       _pad_0x400[0x4];                     // +0x400
    unsigned char       _pad_0x404[0x4];                     // +0x404
    unsigned char       _pad_0x408[0x4];                     // +0x408
    unsigned char       _pad_0x40c[0x4];                     // +0x40c
    unsigned char       _pad_0x410[0x4];                     // +0x410
    unsigned char       _pad_0x414[0x4];                     // +0x414
    unsigned char       _pad_0x418[0x4];                     // +0x418
    unsigned char       _pad_0x41c[0x4];                     // +0x41c
    unsigned char       _pad_0x420[0x4];                     // +0x420
    unsigned char       _pad_0x424[0x4];                     // +0x424
    unsigned char       _pad_0x428[0x4];                     // +0x428
    unsigned char       _pad_0x42c[0x4];                     // +0x42c
    unsigned char       _pad_0x430[0x4];                     // +0x430
    unsigned char       _pad_0x434[0x4];                     // +0x434
    unsigned char       _pad_0x438[0x4];                     // +0x438
    unsigned char       _pad_0x43c[0x4];                     // +0x43c
    unsigned char       _pad_0x440[0x4];                     // +0x440
    unsigned char       _pad_0x444[0x4];                     // +0x444
    unsigned char       _pad_0x448[0x4];                     // +0x448
    unsigned char       _pad_0x44c[0x4];                     // +0x44c
    unsigned char       _pad_0x450[0x4];                     // +0x450
    unsigned char       _pad_0x454[0x4];                     // +0x454
    unsigned char       _pad_0x458[0x4];                     // +0x458
    unsigned char       _pad_0x45c[0x4];                     // +0x45c
    unsigned char       _pad_0x460[0x4];                     // +0x460
    unsigned char       _pad_0x464[0x4];                     // +0x464
    unsigned char       _pad_0x468[0x4];                     // +0x468
    unsigned char       _pad_0x46c[0x4];                     // +0x46c
    unsigned char       _pad_0x470[0x4];                     // +0x470
    unsigned char       _pad_0x474[0x4];                     // +0x474
    unsigned char       _pad_0x478[0x4];                     // +0x478
    unsigned char       _pad_0x47c[0x4];                     // +0x47c
    unsigned char       _pad_0x480[0x4];                     // +0x480
    unsigned char       _pad_0x484[0x4];                     // +0x484
    unsigned char       _pad_0x488[0x4];                     // +0x488
    unsigned char       _pad_0x48c[0x4];                     // +0x48c
    unsigned char       _pad_0x490[0x4];                     // +0x490
    unsigned char       _pad_0x494[0x4];                     // +0x494
    unsigned char       _pad_0x498[0x4];                     // +0x498
    unsigned char       _pad_0x49c[0x4];                     // +0x49c
    unsigned char       _pad_0x4a0[0x4];                     // +0x4a0
    unsigned char       _pad_0x4a4[0x4];                     // +0x4a4
    unsigned char       _pad_0x4a8[0x4];                     // +0x4a8
    unsigned char       _pad_0x4ac[0x4];                     // +0x4ac
    unsigned char       _pad_0x4b0[0x4];                     // +0x4b0
    unsigned char       _pad_0x4b4[0x4];                     // +0x4b4
    unsigned char       _pad_0x4b8[0x4];                     // +0x4b8
    unsigned char       _pad_0x4bc[0x4];                     // +0x4bc
    unsigned char       _pad_0x4c0[0x4];                     // +0x4c0
    unsigned char       _pad_0x4c4[0x4];                     // +0x4c4
    unsigned char       _pad_0x4c8[0x4];                     // +0x4c8
    unsigned char       _pad_0x4cc[0x4];                     // +0x4cc
    unsigned char       _pad_0x4d0[0x4];                     // +0x4d0
    unsigned char       _pad_0x4d4[0x4];                     // +0x4d4
    unsigned char       _pad_0x4d8[0x4];                     // +0x4d8
    unsigned char       _pad_0x4dc[0x4];                     // +0x4dc
    unsigned char       _pad_0x4e0[0x4];                     // +0x4e0
    unsigned char       _pad_0x4e4[0x4];                     // +0x4e4
    unsigned char       _pad_0x4e8[0x4];                     // +0x4e8
    unsigned char       _pad_0x4ec[0x4];                     // +0x4ec
    unsigned char       _pad_0x4f0[0x4];                     // +0x4f0
    unsigned char       _pad_0x4f4[0x4];                     // +0x4f4
    unsigned char       _pad_0x4f8[0x4];                     // +0x4f8
    unsigned char       _pad_0x4fc[0x4];                     // +0x4fc
    unsigned char       _pad_0x500[0x4];                     // +0x500
    unsigned char       _pad_0x504[0x4];                     // +0x504
    unsigned char       _pad_0x508[0x4];                     // +0x508
    unsigned char       _pad_0x50c[0x4];                     // +0x50c
    unsigned char       _pad_0x510[0x4];                     // +0x510
    unsigned char       _pad_0x514[0x4];                     // +0x514
    unsigned char       _pad_0x518[0x4];                     // +0x518
    unsigned char       _pad_0x51c[0x4];                     // +0x51c
    unsigned char       _pad_0x520[0x4];                     // +0x520
    unsigned char       _pad_0x524[0x4];                     // +0x524
    unsigned char       _pad_0x528[0x4];                     // +0x528
    unsigned char       _pad_0x52c[0x4];                     // +0x52c
    unsigned char       _pad_0x530[0x4];                     // +0x530
    unsigned char       _pad_0x534[0x4];                     // +0x534
    unsigned char       _pad_0x538[0x4];                     // +0x538
    unsigned char       _pad_0x53c[0x4];                     // +0x53c
    unsigned char       _pad_0x540[0x4];                     // +0x540
    unsigned char       _pad_0x544[0x4];                     // +0x544
    unsigned char       _pad_0x548[0x4];                     // +0x548
    unsigned char       _pad_0x54c[0x4];                     // +0x54c
    unsigned char       _pad_0x550[0x4];                     // +0x550
    unsigned char       _pad_0x554[0x4];                     // +0x554
    unsigned char       _pad_0x558[0x4];                     // +0x558
    unsigned char       _pad_0x55c[0x4];                     // +0x55c
    unsigned char       _pad_0x560[0x4];                     // +0x560
    unsigned char       _pad_0x564[0x4];                     // +0x564
    unsigned char       _pad_0x568[0x4];                     // +0x568
    unsigned char       _pad_0x56c[0x4];                     // +0x56c
    unsigned char       _pad_0x570[0x4];                     // +0x570
    unsigned char       _pad_0x574[0x4];                     // +0x574
    unsigned char       _pad_0x578[0x4];                     // +0x578
    unsigned char       _pad_0x57c[0x4];                     // +0x57c
    unsigned char       _pad_0x580[0x4];                     // +0x580
    unsigned char       _pad_0x584[0x4];                     // +0x584
    unsigned char       _pad_0x588[0x4];                     // +0x588
    unsigned char       _pad_0x58c[0x4];                     // +0x58c
    unsigned char       _pad_0x590[0x4];                     // +0x590
    unsigned char       _pad_0x594[0x4];                     // +0x594
    unsigned char       _pad_0x598[0x4];                     // +0x598
    unsigned char       _pad_0x59c[0x4];                     // +0x59c
    unsigned char       _pad_0x5a0[0x4];                     // +0x5a0
    unsigned char       _pad_0x5a4[0x4];                     // +0x5a4
    unsigned char       _pad_0x5a8[0x4];                     // +0x5a8
    unsigned char       _pad_0x5ac[0x4];                     // +0x5ac
    unsigned char       _pad_0x5b0[0x4];                     // +0x5b0
    unsigned char       _pad_0x5b4[0x4];                     // +0x5b4
    unsigned char       _pad_0x5b8[0x4];                     // +0x5b8
    unsigned char       _pad_0x5bc[0x4];                     // +0x5bc
    unsigned char       _pad_0x5c0[0x4];                     // +0x5c0
    unsigned char       _pad_0x5c4[0x4];                     // +0x5c4
    unsigned char       _pad_0x5c8[0x4];                     // +0x5c8
    unsigned char       _pad_0x5cc[0x4];                     // +0x5cc
    unsigned char       _pad_0x5d0[0x4];                     // +0x5d0
    unsigned char       _pad_0x5d4[0x4];                     // +0x5d4
    unsigned char       _pad_0x5d8[0x4];                     // +0x5d8
    unsigned char       _pad_0x5dc[0x4];                     // +0x5dc
    unsigned char       _pad_0x5e0[0x4];                     // +0x5e0
    unsigned char       _pad_0x5e4[0x4];                     // +0x5e4
    unsigned char       _pad_0x5e8[0x4];                     // +0x5e8
    unsigned char       _pad_0x5ec[0x4];                     // +0x5ec
    unsigned char       _pad_0x5f0[0x4];                     // +0x5f0
    unsigned char       _pad_0x5f4[0x4];                     // +0x5f4
    unsigned char       _pad_0x5f8[0x4];                     // +0x5f8
    unsigned char       _pad_0x5fc[0x4];                     // +0x5fc
    unsigned char       _pad_0x600[0x4];                     // +0x600
    unsigned char       _pad_0x604[0x4];                     // +0x604
    unsigned char       _pad_0x608[0x4];                     // +0x608
    unsigned char       _pad_0x60c[0x4];                     // +0x60c
    unsigned char       _pad_0x610[0x4];                     // +0x610
    unsigned char       _pad_0x614[0x4];                     // +0x614
    unsigned char       _pad_0x618[0x4];                     // +0x618
    unsigned char       _pad_0x61c[0x4];                     // +0x61c
    unsigned char       _pad_0x620[0x4];                     // +0x620
    unsigned char       _pad_0x624[0x4];                     // +0x624
    unsigned char       _pad_0x628[0x4];                     // +0x628
    unsigned char       _pad_0x62c[0x4];                     // +0x62c
    unsigned char       _pad_0x630[0x4];                     // +0x630
    unsigned char       _pad_0x634[0x4];                     // +0x634
    unsigned char       _pad_0x638[0x4];                     // +0x638
    unsigned char       _pad_0x63c[0x4];                     // +0x63c
    unsigned char       _pad_0x640[0x4];                     // +0x640
    unsigned char       _pad_0x644[0x4];                     // +0x644
    unsigned char       _pad_0x648[0x4];                     // +0x648
    unsigned char       _pad_0x64c[0x4];                     // +0x64c
    unsigned char       _pad_0x650[0x4];                     // +0x650
    unsigned char       _pad_0x654[0x4];                     // +0x654
    unsigned char       _pad_0x658[0x4];                     // +0x658
    unsigned char       _pad_0x65c[0x4];                     // +0x65c
    unsigned char       _pad_0x660[0x4];                     // +0x660
    unsigned char       _pad_0x664[0x4];                     // +0x664
    unsigned char       _pad_0x668[0x4];                     // +0x668
    unsigned char       _pad_0x66c[0x4];                     // +0x66c
    unsigned char       _pad_0x670[0x4];                     // +0x670
    unsigned char       _pad_0x674[0x4];                     // +0x674
    unsigned char       _pad_0x678[0x4];                     // +0x678
    unsigned char       _pad_0x67c[0x4];                     // +0x67c
    unsigned char       _pad_0x680[0x4];                     // +0x680
    unsigned char       _pad_0x684[0x4];                     // +0x684
    unsigned char       _pad_0x688[0x4];                     // +0x688
    unsigned char       _pad_0x68c[0x4];                     // +0x68c
    unsigned char       _pad_0x690[0x4];                     // +0x690
    unsigned char       _pad_0x694[0x4];                     // +0x694
    unsigned char       _pad_0x698[0x4];                     // +0x698
    unsigned char       _pad_0x69c[0x4];                     // +0x69c
    unsigned char       _pad_0x6a0[0x4];                     // +0x6a0
    unsigned char       _pad_0x6a4[0x4];                     // +0x6a4
    unsigned char       _pad_0x6a8[0x4];                     // +0x6a8
    unsigned char       _pad_0x6ac[0x4];                     // +0x6ac
    unsigned char       _pad_0x6b0[0x4];                     // +0x6b0
    unsigned char       _pad_0x6b4[0x4];                     // +0x6b4
    unsigned char       _pad_0x6b8[0x4];                     // +0x6b8
    unsigned char       _pad_0x6bc[0x4];                     // +0x6bc
    unsigned char       _pad_0x6c0[0x4];                     // +0x6c0
    unsigned char       _pad_0x6c4[0x4];                     // +0x6c4
    unsigned char       _pad_0x6c8[0x4];                     // +0x6c8
    unsigned char       _pad_0x6cc[0x4];                     // +0x6cc
    unsigned char       _pad_0x6d0[0x4];                     // +0x6d0
    unsigned char       _pad_0x6d4[0x4];                     // +0x6d4
    unsigned char       _pad_0x6d8[0x4];                     // +0x6d8
    unsigned char       _pad_0x6dc[0x4];                     // +0x6dc
    unsigned char       _pad_0x6e0[0x4];                     // +0x6e0
    unsigned char       _pad_0x6e4[0x4];                     // +0x6e4
    unsigned char       _pad_0x6e8[0x4];                     // +0x6e8
    unsigned char       _pad_0x6ec[0x4];                     // +0x6ec
    unsigned char       _pad_0x6f0[0x4];                     // +0x6f0
    unsigned char       _pad_0x6f4[0x4];                     // +0x6f4
    CMainGameComponent* Component;                           // +0x6f8
    CPlayerManager*     PlayerManager;                       // +0x6fc
    bool                PacketSave;                          // +0x700
    bool                PacketLoad;                          // +0x701
    unsigned char       _pad_0x702[0x2];                     // +0x702
    unsigned char       PacketFile[0x1c];                    // +0x704 CDiskFileWin32
    bool                InputEventIsLoaded;                  // +0x720
    unsigned char       _pad_0x721[0x3];                     // +0x721
    unsigned char       LoadedInputEvent[0x78];              // +0x724 CSavedInputEvent
    bool                Suspended;                           // +0x79c
    unsigned char       _pad_0x79d[0x3];                     // +0x79d
    CDefPointeeBase*    PControlsDef_Object;                 // +0x7a0 CDefPointer<CControlsDef_const_>::Object
    long                ControlsDefIndex;                    // +0x7a4
    unsigned char       Controls[0x14];                      // +0x7a8 CVectorMap<EGameAction,CCountedPointer<CInputTypeBase_const_>,CKeyPairCompareLess<EGameAction,CCountedPointer<CInputTypeBase_const_>_>_>
    unsigned char       AssignableSpecialMoveButtons[0x10];  // +0x7bc vector<CCountedPointer<CInputTypeBase_const_>,std::allocator<CCountedPointer<CInputTypeBase_const_>_>_>
    unsigned char       QuickAccessItemButtons[0x10];        // +0x7cc vector<CCountedPointer<CInputTypeBase_const_>,std::allocator<CCountedPointer<CInputTypeBase_const_>_>_>
    unsigned char       ContextSensitiveItemButtons[0x10];   // +0x7dc vector<CCountedPointer<CInputTypeBase_const_>,std::allocator<CCountedPointer<CInputTypeBase_const_>_>_>
    long                CurrentQueuedProcessedInputNo;       // +0x7ec
    unsigned char       CurrentProcessedInput[0xb0];         // +0x7f0 CProcessedInput
    long                CurrentEventInProcessedInput;        // +0x8a0
    bool                HasCalledInputUpdateThisFrame;       // +0x8a4
    unsigned char       _pad_0x8a5[0x3];                     // +0x8a5
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CGamePlayerInterface) == 0x8a8);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, Component) == 0x6f8);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, PlayerManager) == 0x6fc);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, PacketSave) == 0x700);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, PacketLoad) == 0x701);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, PacketFile) == 0x704);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, InputEventIsLoaded) == 0x720);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, LoadedInputEvent) == 0x724);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, Suspended) == 0x79c);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, PControlsDef_Object) == 0x7a0);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, ControlsDefIndex) == 0x7a4);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, Controls) == 0x7a8);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, AssignableSpecialMoveButtons) == 0x7bc);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, QuickAccessItemButtons) == 0x7cc);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, ContextSensitiveItemButtons) == 0x7dc);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, CurrentQueuedProcessedInputNo) == 0x7ec);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, CurrentProcessedInput) == 0x7f0);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, CurrentEventInProcessedInput) == 0x8a0);
FABLE_STATIC_ASSERT(offsetof(CGamePlayerInterface, HasCalledInputUpdateThisFrame) == 0x8a4);

#endif // FABLE_ENGINE_CGAMEPLAYERINTERFACE_H

#undef CGamePlayerInterface
#define CDisplayEngine CDisplayEngineDonor
#ifndef FABLE_ENGINE_CDISPLAYENGINE_H
#define FABLE_ENGINE_CDISPLAYENGINE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDisplayEngine = 0x100.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CCPPointerInfo;
class CDisplayViewManager;
class CGameDefinitionManager;
class CGraphicDataBank;
class CIEngine;
class CMainGameComponent;
class CMeshDataBank;
class CPlayerManager;
class CWorld;

#pragma pack(push, 1)
class CDisplayEngine { public:
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long           EnginePreloadFlags;              // +0x04
    CMainGameComponent*     Component;                       // +0x08
    CPlayerManager*         PlayerManager;                   // +0x0c
    CGameDefinitionManager* DefinitionManager;               // +0x10
    CGraphicDataBank*       EngineGraphicBank;               // +0x14
    CMeshDataBank*          MeshBank;                        // +0x18
    float                   MainWindow_TLX;                  // +0x1c C2DBoxF::TLX
    float                   MainWindow_TLY;                  // +0x20 C2DBoxF::TLY
    float                   MainWindow_BRX;                  // +0x24 C2DBoxF::BRX
    float                   MainWindow_BRY;                  // +0x28 C2DBoxF::BRY
    CIEngine*               Engine3D;                        // +0x2c
    unsigned char           Camera[0x38];                    // +0x30 CCamera
    long                    Frame;                           // +0x68
    bool                    DrawGame;                        // +0x6c
    unsigned char           _pad_0x6d[0x3];                  // +0x6d
    unsigned char           LetterBox[0x20];                 // +0x70 CLetterBoxModeInfo
    bool                    CameraInputsOn;                  // +0x90
    unsigned char           _pad_0x91[0x7];                  // +0x91
    double                  LastRefreshTime;                 // +0x98
    double                  LastRenderTimeLength;            // +0xa0
    double                  LastWorldUpdateRenderTime;       // +0xa8
    bool                    PreparePrimitives;               // +0xb0
    unsigned char           _pad_0xb1[0x3];                  // +0xb1
    float                   GammaRamp;                       // +0xb4
    float                   OldGammaRamp;                    // +0xb8
    unsigned char           ScreenFadeOutInfo[0x1c];         // +0xbc CFadeInFadeOutBase
    bool                    ScreenFadeOutLocked;             // +0xd8
    bool                    DrawMemoryUse;                   // +0xd9
    unsigned char           _pad_0xda[0x2];                  // +0xda
    long                    DrawDebugPage;                   // +0xdc
    CDisplayViewManager*    PViewManager_Data;               // +0xe0 CCountedPointer<CDisplayViewManager>::Data
    CCPPointerInfo*         PViewManager_Info;               // +0xe4 CCountedPointer<CDisplayViewManager>::Info
    long                    InitialFadeReadynessCount;       // +0xe8
    bool                    InitialFadeActive;               // +0xec
    unsigned char           _pad_0xed[0x3];                  // +0xed
    float                   ScreenFadeDuration;              // +0xf0
    float                   TimeSinceFadeStarted;            // +0xf4
    CWorld*                 World;                           // +0xf8
    unsigned char           _pad_0xfc[0x4];                  // +0xfc
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CDisplayEngine) == 0x100);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, EnginePreloadFlags) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, Component) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, PlayerManager) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, DefinitionManager) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, EngineGraphicBank) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, MeshBank) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, MainWindow_TLX) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, MainWindow_TLY) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, MainWindow_BRX) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, MainWindow_BRY) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, Engine3D) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, Camera) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, Frame) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, DrawGame) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, LetterBox) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, CameraInputsOn) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, LastRefreshTime) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, LastRenderTimeLength) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, LastWorldUpdateRenderTime) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, PreparePrimitives) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, GammaRamp) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, OldGammaRamp) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, ScreenFadeOutInfo) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, ScreenFadeOutLocked) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, DrawMemoryUse) == 0xd9);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, DrawDebugPage) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, PViewManager_Data) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, PViewManager_Info) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, InitialFadeReadynessCount) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, InitialFadeActive) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, ScreenFadeDuration) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, TimeSinceFadeStarted) == 0xf4);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, World) == 0xf8);

#endif // FABLE_ENGINE_CDISPLAYENGINE_H

#undef CDisplayEngine
#define CNetworkClient CNetworkClientDonor
#ifndef FABLE_ENGINE_CNETWORKCLIENT_H
#define FABLE_ENGINE_CNETWORKCLIENT_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CNetworkClient = 0x2680.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CMainGameComponent;
struct CNetworkPlayer;

#pragma pack(push, 1)
struct CNetworkClient {
    void*               __vftable;                           // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char       _pad_0x04[0x4];                      // +0x04
    unsigned char       Server[0x8];                         // +0x08 CNetworkServer
    unsigned char       ReceiveBuffer[8192];                 // +0x10
    unsigned char       LocalEventPackage[0x648];            // +0x2010 CGameEventPackage
    double              LastUpdateTime;                      // +0x2658
    bool                FirstTime;                           // +0x2660
    bool                Host;                                // +0x2661
    bool                LocalGame;                           // +0x2662
    unsigned char       _pad_0x2663[0x1];                    // +0x2663
    CNetworkPlayer*     LocalPlayer;                         // +0x2664
    CNetworkPlayer*     HostPlayer;                          // +0x2668
    long                LocalFrame;                          // +0x266c
    unsigned long       Checksum1;                           // +0x2670
    unsigned long       Checksum2;                           // +0x2674
    CMainGameComponent* GameComponent;                       // +0x2678
    unsigned char       _pad_0x267c[0x4];                    // +0x267c
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CNetworkClient) == 0x2680);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, Server) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, ReceiveBuffer) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, LocalEventPackage) == 0x2010);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, LastUpdateTime) == 0x2658);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, FirstTime) == 0x2660);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, Host) == 0x2661);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, LocalGame) == 0x2662);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, LocalPlayer) == 0x2664);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, HostPlayer) == 0x2668);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, LocalFrame) == 0x266c);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, Checksum1) == 0x2670);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, Checksum2) == 0x2674);
FABLE_STATIC_ASSERT(offsetof(CNetworkClient, GameComponent) == 0x2678);

#endif // FABLE_ENGINE_CNETWORKCLIENT_H

#undef CNetworkClient
#define CDiskFileWin32 CDiskFileWin32Donor
#ifndef FABLE_ENGINE_CDISKFILEWIN32_H
#define FABLE_ENGINE_CDISKFILEWIN32_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDiskFileWin32 = 0x1c.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CWideStringData;

#pragma pack(push, 1)
class CDiskFileWin32 { public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    AccessType[0x4];                        // +0x04 FileAccessType
    long             Position;                               // +0x08
    long             Length;                                 // +0x0c
    void*            WinFileHandle;                          // +0x10
    bool             Opened;                                 // +0x14
    unsigned char    _pad_0x15[0x3];                         // +0x15
    CWideStringData* Pathname_PStringData;                   // +0x18 CWideString::PStringData
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CDiskFileWin32) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, AccessType) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, Position) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, Length) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, WinFileHandle) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, Opened) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, Pathname_PStringData) == 0x18);

#endif // FABLE_ENGINE_CDISKFILEWIN32_H

#undef CDiskFileWin32
#define CFontBank CFontBankDonor
#ifndef FABLE_ENGINE_CFONTBANK_H
#define FABLE_ENGINE_CFONTBANK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CFontBank = 0x10.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CCharStringData;
class CFontManager;

#pragma pack(push, 1)
class CFontBank { public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long    FontHeight;                             // +0x04
    CCharStringData* Name_PStringData;                       // +0x08 CCharString::PStringData
    CFontManager*    PManager;                               // +0x0c
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CFontBank) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CFontBank, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CFontBank, FontHeight) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CFontBank, Name_PStringData) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CFontBank, PManager) == 0xc);

#endif // FABLE_ENGINE_CFONTBANK_H

#undef CFontBank
class CCharString { public: CCharStringData* Data; CCharString(); CCharString(const CCharString&); ~CCharString(); };
class CGame;class CFontBank;class CLUA;class CGraphicDataBank;class CCPPointerInfo;
namespace NGameText {class CDataBank;}
class CBaseClass { public: CBaseClass(); virtual ~CBaseClass(); }; //0099a300 destructor
struct CBaseClassNonCopyable : CBaseClass { CBaseClassNonCopyable(); private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable&operator=(const CBaseClassNonCopyable&); };
class CDeviceResetCallback { public:CDeviceResetCallback();virtual void OnPreDeviceReset()=0;virtual bool OnPostDeviceReset()=0;};
class CGameComponent : public CBaseClassNonCopyable,public CDeviceResetCallback { public:
 bool Quit,Running; CGame* Game;
 CGameComponent(CGame&);
 virtual ~CGameComponent(){}
 virtual void Init();
 virtual bool Run(CGameComponent**)=0;
 virtual void ChangeTextureColourDepth(long)=0;
 virtual void SetQuit();
};
// Deleted-pointee interface views: only destructor prefix is used; fixtures supply pre-existing interface doubles, not native game constructors.
class CWorld { public: virtual ~CWorld(); };
class CPlayerManager { public: virtual ~CPlayerManager(); };
class CGamePlayerInterface { public: virtual ~CGamePlayerInterface(); };
class CDisplayEngine : public CBaseClassNonCopyable { public: virtual ~CDisplayEngine(); virtual bool InitEngine(); virtual void UninitEngine(); };
template<class T>struct scoped_ptr {
 T* ptr;
 scoped_ptr():ptr(0){}
 ~scoped_ptr(){delete ptr;}
};
template<class T>class CCountedPointer { public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}~CCountedPointer();};
class CInitBaseClass : public CBaseClass { public:bool Valid;CInitBaseClass();virtual void Validate()const;};
struct CNetworkClient : CInitBaseClass {unsigned char NetworkState[0x2678];CNetworkClient();virtual ~CNetworkClient();}; //00419352
enum FileAccessType {FileRead=0};class CASuspendableProcess;
class CAFile : public CBaseClassNonCopyable { public:
 CAFile();virtual ~CAFile();
 virtual void Open(const CWideString&,FileAccessType,unsigned long)=0;
 virtual void Close()=0;
 virtual void Read(void*,long,CASuspendableProcess*)=0;
 virtual void Write(const void*,long,CASuspendableProcess*)=0;
 virtual void SetPosition(long)=0;
 virtual void MovePosition(long)=0;
 virtual long GetPosition()const=0;
 virtual void SetLength(long)=0;
 virtual long GetLength()const=0;
 virtual bool IsOpen()const=0;
 virtual CWideString GetPathname()const=0;
 virtual bool IsIOFinished()const;
 virtual long GetAlignment()const;
 virtual CAFile* Clone()const=0;
};
class CDiskFileWin32 : public CAFile { public:
 long AccessType,Position,Length;void* WinFileHandle;bool Opened;CWideString Pathname;
 CDiskFileWin32();virtual ~CDiskFileWin32(); //00401630
 virtual void Open(const CWideString&,FileAccessType,unsigned long);
 virtual void Close();virtual void Read(void*,long,CASuspendableProcess*);virtual void Write(const void*,long,CASuspendableProcess*);
 virtual void SetPosition(long);virtual void MovePosition(long);virtual long GetPosition()const;
 virtual void SetLength(long);virtual long GetLength()const;virtual bool IsOpen()const;
 virtual CWideString GetPathname()const;virtual CAFile* Clone()const;
};
struct CFrameRateSmoother : CInitBaseClass {
 unsigned char Times[0x10];long MaxNoTimes,NoTimes,NoFramesToChangeFrameRateOver,FirstTime,LastTime;double SmoothedTime;
 CFrameRateSmoother();virtual ~CFrameRateSmoother();}; //00419764
class CMainGameComponentInit { public:CWideString InitialWorldName,InitialWorldHolySiteName;CCharString InitialQuestName;CWideString SaveGameName;CMainGameComponentInit();CMainGameComponentInit(const CMainGameComponentInit&);~CMainGameComponentInit();}; //004131d0
class CMainGameComponent : public CGameComponent { public:
 void* PSampleBank;
 CCountedPointer<NGameText::CDataBank> PTextBank;
 scoped_ptr<CPlayerManager> PPlayerManager;
 scoped_ptr<CGamePlayerInterface> PPlayerInterface;
 scoped_ptr<CWorld> PWorld;
 scoped_ptr<CDisplayEngine> PDisplayEngine;
 CCountedPointer<CLUA> PLUA;
 unsigned char TimingAndPackageSet[0x13a84];
 CNetworkClient Client;
 unsigned long NoRenderFramesSinceLastGameUpdate,WorldSeed,LocalSeed;
 CCountedPointer<CFontBank> PDebugFont,PCutSceneMainFont;
 CDiskFileWin32 EventPackageFile;
 bool LoadingEventPackages,SavingEventPackages;
 unsigned long EventPackageFileHeader;
 CFrameRateSmoother FrameRateSmoother;
 double LastRenderFrameStartTime;
 float TimePassedSinceLastUpdate,LastUpdateTime;
 bool WorldUpdateTurn;
 CGameComponent* NextComponentToRun;
 CCountedPointer<CGraphicDataBank> PMainGraphicBank;
 CMainGameComponentInit InitStructure;
 bool Initialised,AllowRender,Rendered;
 long DebugNoFramesUnableToRender;
 CMainGameComponent(CGame&,const CMainGameComponentInit&);
 virtual ~CMainGameComponent();
 virtual void Init(); //slot1 004184bd
 virtual bool Run(CGameComponent**); //slot2 004189c2
 virtual void ChangeTextureColourDepth(long); //slot3 0041799b
 virtual void SetQuit(); //slot4 004197a0
 virtual void Update(); //slot5 00418289
 virtual void GetInputs(); //slot6 00416e78
 virtual void Render(); //slot7 00417001
 virtual void PostInit(); //slot8 00416953
 virtual bool IsEditorActive()const; //slot9 004197b0
 virtual void OnPreDeviceReset(); //secondary0 00415e83
 virtual bool OnPostDeviceReset(); //secondary1 004175da
 void Shutdown(); //004175e5
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDiskFileWin32)==sizeof(CDiskFileWin32Donor));
FABLE_STATIC_ASSERT(sizeof(CNetworkClient)==sizeof(CNetworkClientDonor));
FABLE_STATIC_ASSERT(sizeof(CFrameRateSmoother)==0x38);
FABLE_STATIC_ASSERT(sizeof(CInitBaseClass)==8);
FABLE_STATIC_ASSERT(sizeof(CGameComponent)==16);
FABLE_STATIC_ASSERT(offsetof(CGameComponent,Quit)==8);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,Client)==0x13ab8);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,PDebugFont)==0x16144);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,EventPackageFile)==0x16154);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,FrameRateSmoother)==0x16178);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,PMainGraphicBank)==0x161c8);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,InitStructure)==0x161d0);
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent)==0x161e8);
#endif
CMainGameComponent::~CMainGameComponent(){if(!Quit)Shutdown();}


#include <stdio.h>
#include <string.h>
static int good=1,events[64],nEvents=0,active=0,callbackCount=0,liveStrings=0;
static CMainGameComponent* owner=0;
static void event(int n){if(active)events[nEvents++]=n;}
struct CountedPayload {int id;CountedPayload(int n):id(n){}};
class CCPPointerInfo { public:unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void* Data;};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCPPointerInfo,RefCount)==0);
FABLE_STATIC_ASSERT(offsetof(CCPPointerInfo,DeleteFunc)==4);
FABLE_STATIC_ASSERT(offsetof(CCPPointerInfo,Data)==8);
#endif
static void __fastcall countedDelete(void* p){++callbackCount;delete static_cast<CountedPayload*>(p);}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){if(active&&this==static_cast<CBaseClass*>(owner))event(14);}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CInitBaseClass::CInitBaseClass():Valid(false){}
void CInitBaseClass::Validate()const{}
CAFile::CAFile(){}
CAFile::~CAFile(){}
bool CAFile::IsIOFinished()const{return true;}
long CAFile::GetAlignment()const{return 1;}
CDeviceResetCallback::CDeviceResetCallback(){}
CGameComponent::CGameComponent(CGame& game):Quit(false),Running(false),Game(&game){}
void CGameComponent::Init(){}
void CGameComponent::SetQuit(){Quit=true;}
CWorld::~CWorld(){event(10);}
CPlayerManager::~CPlayerManager(){event(12);}
CGamePlayerInterface::~CGamePlayerInterface(){event(11);}
CDisplayEngine::~CDisplayEngine(){event(9);}
bool CDisplayEngine::InitEngine(){return true;}
void CDisplayEngine::UninitEngine(){}
// Genuine polymorphic fixture doubles for the pre-existing destructor interfaces.
struct WorldDouble : CWorld {};
struct PlayerDouble : CPlayerManager {};
struct InterfaceDouble : CGamePlayerInterface {};
struct DisplayDouble : CDisplayEngine {};
CNetworkClient::CNetworkClient(){}
CNetworkClient::~CNetworkClient(){event(7);}
CDiskFileWin32::CDiskFileWin32():AccessType(0),Position(0),Length(0),WinFileHandle(0),Opened(false){}
CDiskFileWin32::~CDiskFileWin32(){event(4);}
void CDiskFileWin32::Open(const CWideString&,FileAccessType,unsigned long){}
void CDiskFileWin32::Close(){}
void CDiskFileWin32::Read(void*,long,CASuspendableProcess*){}
void CDiskFileWin32::Write(const void*,long,CASuspendableProcess*){}
void CDiskFileWin32::SetPosition(long n){Position=n;}
void CDiskFileWin32::MovePosition(long n){Position+=n;}
long CDiskFileWin32::GetPosition()const{return Position;}
void CDiskFileWin32::SetLength(long n){Length=n;}
long CDiskFileWin32::GetLength()const{return Length;}
bool CDiskFileWin32::IsOpen()const{return Opened;}
CWideString CDiskFileWin32::GetPathname()const{return Pathname;}
CAFile* CDiskFileWin32::Clone()const{return 0;}
CFrameRateSmoother::CFrameRateSmoother(){}
CFrameRateSmoother::~CFrameRateSmoother(){event(3);}
CCharString::CCharString(){Data=new CCharStringData;Data->text=new char[1];Data->text[0]=0;Data->owners=1;++liveStrings;}
CCharString::CCharString(const CCharString& r):Data(r.Data){++Data->owners;}
CCharString::~CCharString(){if(--Data->owners==0){delete[] Data->text;delete Data;--liveStrings;}}
CWideString::CWideString(){storage_=new CWideStringData;storage_->text=new wchar_t[1];storage_->text[0]=0;storage_->owners=1;++liveStrings;}
CWideString::CWideString(const CWideString& r):storage_(r.storage_){++storage_->owners;}
CWideString::~CWideString(){if(--storage_->owners==0){delete[] storage_->text;delete storage_;--liveStrings;}}
CMainGameComponentInit::CMainGameComponentInit(){}
CMainGameComponentInit::CMainGameComponentInit(const CMainGameComponentInit& r):InitialWorldName(r.InitialWorldName),InitialWorldHolySiteName(r.InitialWorldHolySiteName),InitialQuestName(r.InitialQuestName),SaveGameName(r.SaveGameName){}
CMainGameComponentInit::~CMainGameComponentInit(){event(1);}
CMainGameComponent::CMainGameComponent(CGame& game,const CMainGameComponentInit& init):CGameComponent(game),InitStructure(init){Quit=false;Running=false;Game=&game;PSampleBank=0;}
void CMainGameComponent::Init(){}
bool CMainGameComponent::Run(CGameComponent**){return false;}
void CMainGameComponent::ChangeTextureColourDepth(long){}
void CMainGameComponent::SetQuit(){Quit=true;}
void CMainGameComponent::Update(){}
void CMainGameComponent::GetInputs(){}
void CMainGameComponent::Render(){}
void CMainGameComponent::PostInit(){}
bool CMainGameComponent::IsEditorActive()const{return false;}
void CMainGameComponent::OnPreDeviceReset(){}
bool CMainGameComponent::OnPostDeviceReset(){return true;}
void CMainGameComponent::Shutdown(){event(0);if(!owner||this!=owner)good=0;}
template<class T>CCountedPointer<T>::~CCountedPointer(){
 if(active){if(this==static_cast<void*>(&owner->PMainGraphicBank))event(2);else if(this==static_cast<void*>(&owner->PCutSceneMainFont))event(5);else if(this==static_cast<void*>(&owner->PDebugFont))event(6);else if(this==static_cast<void*>(&owner->PLUA))event(8);else if(this==static_cast<void*>(&owner->PTextBank))event(13);else good=0;}
 if(Info&&--Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}Data=0;Info=0;
}
template<class T>static void assignCounted(CCountedPointer<T>& p,int id,int refs){CCPPointerInfo* c=new CCPPointerInfo;c->RefCount=refs;c->DeleteFunc=countedDelete;c->Data=new CountedPayload(id);p.Data=reinterpret_cast<T*>(c->Data);p.Info=c;}
class CGame : public CInitBaseClass { public:CGameComponent* CurrentGameComponent;unsigned char ParameterBuffer[512];bool Quit;CGame();virtual ~CGame();};
CGame::CGame():CurrentGameComponent(0),Quit(false){}
CGame::~CGame(){}
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CGame)==sizeof(CGameDonor));
#endif
int main(){
 CGame game;
 {CMainGameComponentInit init;
 for(int quit=0;quit<2;++quit)for(int mask=0;mask<16;++mask)for(int countedMode=0;countedMode<3;++countedMode){
 active=0;nEvents=0;callbackCount=0;CCPPointerInfo* retained[5];
 {
 CMainGameComponent instance(game,init);owner=&instance;instance.Quit=quit!=0;
 if(mask&1)instance.PPlayerManager.ptr=new PlayerDouble;
 if(mask&2)instance.PPlayerInterface.ptr=new InterfaceDouble;
 if(mask&4)instance.PWorld.ptr=new WorldDouble;
 if(mask&8)instance.PDisplayEngine.ptr=new DisplayDouble;
 if(countedMode){int refs=countedMode==2?2:1;
 assignCounted(instance.PMainGraphicBank,2,refs);assignCounted(instance.PDebugFont,6,refs);
 assignCounted(instance.PCutSceneMainFont,5,refs);assignCounted(instance.PLUA,8,refs);assignCounted(instance.PTextBank,13,refs);
 retained[0]=instance.PMainGraphicBank.Info;retained[1]=instance.PDebugFont.Info;retained[2]=instance.PCutSceneMainFont.Info;retained[3]=instance.PLUA.Info;retained[4]=instance.PTextBank.Info;
 }
 // Exercise native virtual dispatch on both callback and primary interfaces.
 static_cast<CDeviceResetCallback*>(&instance)->OnPreDeviceReset();if(!static_cast<CDeviceResetCallback*>(&instance)->OnPostDeviceReset())good=0;
 active=1;
 }
 active=0;
 int expected[15],count=0;if(!quit)expected[count++]=0;
 for(int n=1;n<=8;++n)expected[count++]=n;
 if(mask&8)expected[count++]=9;if(mask&4)expected[count++]=10;if(mask&2)expected[count++]=11;if(mask&1)expected[count++]=12;
 expected[count++]=13;expected[count++]=14;
 if(count!=nEvents||callbackCount!=(countedMode==1?5:0)||liveStrings!=4)good=0;
 if(countedMode==2){for(int r=0;r<5;++r){if(retained[r]->RefCount!=1)good=0;if(--retained[r]->RefCount==0){retained[r]->DeleteFunc(retained[r]->Data);delete retained[r];}}if(callbackCount!=5)good=0;}
 for(int i=0;i<count;++i)if(expected[i]!=events[i])good=0;
 }
 }
 if(liveStrings)good=0;
 printf("00418b7a_TEST %s\n",good?"PASS":"FAIL");return good?0:1;
}