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

// CMainGameComponent::InitDisplayEngine() -- construct display engine from current graphics/world/surface settings.
// Native00417418 length217. Actual flattened headers retained; native CMainGameComponent header absent.
// CMain late PMainGraphicBank native161c8 vs donor161d0. Native new size100 matches donor DisplayEngine.
// Calls419270 init ctor;415bf0 height;9f2f90 dimensions;9f2f60 pixel;44c6b0 definition;
// BFEA1A scalar new;434e10 display ctor;41940c scoped reset;99ebf0/99eae0 string;9e9f40 progress;4350d0 Init.
#pragma optimize("s", on)
#pragma optimize("y", off)
#include <new>
#define CMainGameComponent CMainGameComponentDonor
#define CGameComponent CGameComponentDonor
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
#undef CGameComponent
#undef CMainGameComponent
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
#define CSurface CSurfaceDonor
#ifndef FABLE_ENGINE_CSURFACE_H
#define FABLE_ENGINE_CSURFACE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CSurface = 0x10.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


struct IDirect3DSurface9;

#pragma pack(push, 1)
class CSurface { public:
    void*              __vftable;                            // +0x00 vptr, or first dword of a flattened base subobject
    IDirect3DSurface9* PD3DSurface;                          // +0x04
    unsigned long      AllocationSource;                     // +0x08
    void*              PAllocatedMemory;                     // +0x0c
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CSurface) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CSurface, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CSurface, PD3DSurface) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CSurface, AllocationSource) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CSurface, PAllocatedMemory) == 0xc);

#endif // FABLE_ENGINE_CSURFACE_H

#undef CSurface
#ifndef FABLE_ENGINE_CPIXELFORMAT_H
#define FABLE_ENGINE_CPIXELFORMAT_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CPixelFormat = 0x4.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class CPixelFormat { public:
    long FormatIndex;                                        // +0x00
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CPixelFormat) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CPixelFormat, FormatIndex) == 0x0);

#endif // FABLE_ENGINE_CPIXELFORMAT_H

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

#define CGameDefinitionManager CGameDefinitionManagerDonor
#pragma push_macro("FABLE_STATIC_ASSERT")
#undef FABLE_STATIC_ASSERT
#define FABLE_STATIC_ASSERT(x)
// Quarantined donor reports sizeof1 despite emitting void*; preserve its declaration, suppress invalid donor assertions only.
#ifndef FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#define FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CGameDefinitionManager = 0x1.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class CGameDefinitionManager { public:
    void* __vftable;                                         // +0x00 vptr, or first dword of a flattened base subobject
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CGameDefinitionManager) == 0x1);
FABLE_STATIC_ASSERT(offsetof(CGameDefinitionManager, __vftable) == 0x0);

#endif // FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H

#pragma pop_macro("FABLE_STATIC_ASSERT")
#undef CGameDefinitionManager
class CCharString { public: CCharStringData* Data; CCharString(const char*,long); CCharString(const CCharString&); ~CCharString(); };
class C2DExtentsI { public: long Width,Height; };
class CSurface : public CSurfaceDonor { public:
 unsigned long GetHeight()const; //00415bf0
 C2DExtentsI GetDimensions()const; //009f2f90
 unsigned long GetWidth()const {return GetDimensions().Width;}
 CPixelFormat GetPixelFormat()const; //009f2f60
};
struct CDisplayDevice { unsigned char DisplayState[0x2c]; CSurface BackSurface; CSurface& GetBackSurface(){return BackSurface;} };
extern CDisplayDevice* g_DisplayDevice; //013b8390
class CBaseClass { public: CBaseClass(); virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass { CBaseClassNonCopyable(); private: CBaseClassNonCopyable(const CBaseClassNonCopyable&); CBaseClassNonCopyable&operator=(const CBaseClassNonCopyable&); };
class CMainGameComponent; class CGamePlayerInterface;
class CDisplayEngineInit { public:
 CMainGameComponent* Component; CGameDefinitionManager* DefinitionManager; CPlayerManager* PlayerManager; CWorld* World;
 CGraphicDataBank* MainGraphicBank; unsigned long Width,Height; CPixelFormat PixelFormat; CMeshDataBank* MeshBank;
 CDisplayEngineInit(); //00419270 signature confirmed PDB
};
class CDisplayEngine : public CBaseClassNonCopyable { public:
 virtual ~CDisplayEngine(); //native slot0 00437070
 virtual bool InitEngine(); //slot1 00435190
 virtual void UninitEngine(); //slot2 004354a0
 unsigned char DisplayState[0xfc];
 CDisplayEngine(const CDisplayEngineInit&); //00434e10 signature confirmed PDB
 void Init(); //004350d0
};
template<class T> struct scoped_ptr { T* ptr; void reset(T*); }; //0041940c
class CMainGameComponent { public:
 unsigned char BaseAndBanks[0x1c]; CPlayerManager* PPlayerManager; CGamePlayerInterface* PPlayerInterface;
 CWorld* PWorld; scoped_ptr<CDisplayEngine> PDisplayEngine;
 unsigned char LUAAndWorldState[0x1619c]; CGraphicDataBank* PMainGraphicBank; void* PMainGraphicBankInfo;
 void InitDisplayEngine();
};
CGameDefinitionManager* __fastcall GetDefinitionManager(); //0044c6b0
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&,float,bool,bool); } //009e9f40
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDisplayEngine)==0x100);
FABLE_STATIC_ASSERT(sizeof(CDisplayEngine)==sizeof(CDisplayEngineDonor));
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,PWorld)==0x24);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,PMainGraphicBank)==0x161c8);
FABLE_STATIC_ASSERT(sizeof(CDisplayEngineInit)==0x24);
#endif
void CMainGameComponent::InitDisplayEngine(){
 CDisplayEngineInit init;
 init.World=PWorld;
 init.Height=g_DisplayDevice->BackSurface.GetHeight();
 init.Width=g_DisplayDevice->BackSurface.GetWidth();
 init.PixelFormat=g_DisplayDevice->BackSurface.GetPixelFormat();
 init.DefinitionManager=GetDefinitionManager();
 init.PlayerManager=PPlayerManager;
 init.MainGraphicBank=PMainGraphicBank;
 init.Component=this;
 init.MeshBank=PWorld->PMainMeshBank_Data;
 PDisplayEngine.reset(new CDisplayEngine(init));
 {CCharString text("Init Display Engine: Calling Init",-1);NProgressDisplay::DisplayProgress(text,-1.0f,false,false);}
 PDisplayEngine.ptr->Init();
}


#include <stdio.h>
#include <string.h>
static int good=1,stage=0,aliveStrings=0,displayDestructions=0;
static CDisplayDevice device1,device2,device3;
CDisplayDevice* g_DisplayDevice=&device1;
static CDisplayEngine* constructed=0;
static CMainGameComponent* component=0;
static CWorld world;
static int opaquePlayer,opaqueBank,opaqueMesh,opaqueDef;
static void checkStage(int wanted){if(stage!=wanted)good=0;++stage;}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CDisplayEngineInit::CDisplayEngineInit(){
 checkStage(0);Component=0;DefinitionManager=0;PlayerManager=0;World=0;MainGraphicBank=0;Width=Height=0;PixelFormat.FormatIndex=-1;MeshBank=0;
}
unsigned long CSurface::GetHeight()const {checkStage(1);if(this!=&device1.BackSurface)good=0;g_DisplayDevice=&device2;return 720;}
C2DExtentsI CSurface::GetDimensions()const {checkStage(2);if(this!=&device2.BackSurface)good=0;g_DisplayDevice=&device3;C2DExtentsI e;e.Width=1280;e.Height=999;return e;}
CPixelFormat CSurface::GetPixelFormat()const {checkStage(3);if(this!=&device3.BackSurface)good=0;CPixelFormat p;p.FormatIndex=37;return p;}
CGameDefinitionManager* __fastcall GetDefinitionManager(){checkStage(4);return reinterpret_cast<CGameDefinitionManager*>(&opaqueDef);}
CDisplayEngine::CDisplayEngine(const CDisplayEngineInit& init){
 checkStage(5);constructed=this;
 if(init.Component!=component||init.DefinitionManager!=reinterpret_cast<CGameDefinitionManager*>(&opaqueDef)||init.PlayerManager!=component->PPlayerManager||init.World!=&world||init.MainGraphicBank!=component->PMainGraphicBank||init.Width!=1280||init.Height!=720||init.PixelFormat.FormatIndex!=37||init.MeshBank!=world.PMainMeshBank_Data)good=0;
}
CDisplayEngine::~CDisplayEngine(){++displayDestructions;}
bool CDisplayEngine::InitEngine(){return true;}
void CDisplayEngine::UninitEngine(){}
void CDisplayEngine::Init(){checkStage(10);if(this!=constructed||component->PDisplayEngine.ptr!=this||aliveStrings)good=0;}
template<class T> void scoped_ptr<T>::reset(T* p){checkStage(6);if(p!=constructed)good=0;T* old=ptr;ptr=p;delete old;}
CCharString::CCharString(const char* text,long length){checkStage(7);if(strcmp(text,"Init Display Engine: Calling Init")||length!=-1)good=0;Data=new CCharStringData;Data->text=new char[strlen(text)+1];strcpy(Data->text,text);Data->owners=1;++aliveStrings;}
CCharString::CCharString(const CCharString& r):Data(r.Data){++Data->owners;}
CCharString::~CCharString(){checkStage(9);if(--Data->owners==0){delete[] Data->text;delete Data;--aliveStrings;}}
namespace NProgressDisplay {void __fastcall DisplayProgress(const CCharString& text,float progress,bool first,bool second){checkStage(8);if(strcmp(text.Data->text,"Init Display Engine: Calling Init")||progress!=-1.0f||first||second||aliveStrings!=1||component->PDisplayEngine.ptr!=constructed)good=0;}}
int main(){
 CMainGameComponent mainComponent;component=&mainComponent;
 component->PPlayerManager=reinterpret_cast<CPlayerManager*>(&opaquePlayer);
 component->PWorld=&world;component->PMainGraphicBank=reinterpret_cast<CGraphicDataBank*>(&opaqueBank);
 world.PMainMeshBank_Data=reinterpret_cast<CMeshDataBank*>(&opaqueMesh);
 component->PDisplayEngine.ptr=0;
 component->InitDisplayEngine();
 if(stage!=11||aliveStrings||!constructed||component->PDisplayEngine.ptr!=constructed)good=0;
 delete component->PDisplayEngine.ptr;component->PDisplayEngine.ptr=0;
 if(displayDestructions!=1)good=0;
 printf("00417418_TEST %s\n",good?"PASS":"FAIL");return good?0:1;
}