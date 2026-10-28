#ifndef _MSC_VER
#define __int64 long long
#endif
#include <cstdio>
#include <cstring>
#include <new>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __fastcall
#define __cdecl
#endif
// BEGIN EXACT CANDIDATE 00717d40
// CActionTalkToThing::CActionTalkToThing(CThing&, const char*, const char*, const CCharStringValue&, const CCharStringValue&, const CCharStringValue&, bool, bool) -- initialize a question-talk action, retain its three button strings, and load its subtitle messages
// Retail: Fable.exe 0x00717d40, length 267; `ret 20h`.
// Callees: CActionTalkBase::CActionTalkBase @ 0x00717950; CCharString copy construction @ 0x0099ec30; GetMessagesFromTextName @ 0x004cebb0; CArray<CSubtitledMessage>::operator= @ 0x0071a160; CSubtitledMessage copy construction @ 0x004cdb70; CSubtitledMessage destruction @ 0x004d1db0; global CPlayerGui @ 0x013b8790.
// Native vtable slots are base [dtor, Update, OnActivate, OnAdd, pure GetActionName() const, OnCleanup] and derived [dtor, Update, OnActivate, OnAdd, GetActionName() const, OnCleanup, DisplayText]. Retail CSubtitledMessage is 0x44 bytes: the three donor 0x14-byte vector maps are 0x10-byte native maps. The base message array is at +0x30; button strings are at +0x94/+0x98/+0x9c.
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
#define FABLE_STATIC_ASSERT(expression) \


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
    const CWideString& operator=(const CWideString& other);
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
    const CCharString& operator=(const CCharString& other);
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

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);
#endif

#define CPlayerGui CPlayerGui_DonorPDB
#ifndef FABLE_ENGINE_CPLAYERGUI_H
#define FABLE_ENGINE_CPLAYERGUI_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CPlayerGui = 0x360.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


struct CAutoPickupBar;
class CCPPointerInfo;
struct CCrimeStatBar;
struct CDraw2DText;
struct CDrawAbilityButtons;
struct CDrawAutoPickups;
struct CDrawBettingHUD;
struct CDrawCombatMultiplier;
struct CDrawCrime;
struct CDrawDamage;
struct CDrawExperience;
struct CDrawFishingGame;
struct CDrawGuildSeal;
struct CDrawHealth;
struct CDrawHighlightingTargetingCursor;
struct CDrawLetterBoxBlackBars;
struct CDrawMeleeActionButtonEffects;
struct CDrawMiniMap;
struct CDrawMoney;
struct CDrawMorality;
struct CDrawMouseCursor;
struct CDrawNPCName;
struct CDrawPerceivers;
struct CDrawProjectileTargetingCentre;
struct CDrawQuestInfo;
struct CDrawQuickAccessIcons;
struct CDrawReceivedItems;
struct CDrawRenown;
struct CDrawScreenFade;
struct CDrawScriptScreenText;
struct CDrawSpiritScores;
struct CDrawSpiritTargetingCursor;
struct CDrawSpiritsHealth;
struct CDrawStamina;
struct CDrawStatUpdates;
struct CDrawTargetInfo;
struct CDrawWorldTime;
class CFontBank;
class CGamePlayerInterface;
class CGraphicDataBank;
class CMainGameComponent;
struct CManager;
struct CMultiStatBar;
class CPlayerManager;
struct CStatBar;
class CWorld;
struct IDirect3DTexture9;

#pragma pack(push, 1)
class CPlayerGui { public:
    void*                             __vftable;             // +0x00 vptr, or first dword of a flattened base subobject
    CWorld*                           World;                 // +0x04
    CPlayerManager*                   PlayerManager;         // +0x08
    CGamePlayerInterface*             InputInterface;        // +0x0c
    CMainGameComponent*               Component;             // +0x10
    CGraphicDataBank*                 Graphics;              // +0x14
    unsigned char                     SubtitleRenderer[0x128]; // +0x18 CSubtitleRenderer
    CFontBank*                        PFont_Data;            // +0x140 CCountedPointer<CFontBank>::Data
    CCPPointerInfo*                   PFont_Info;            // +0x144 CCountedPointer<CFontBank>::Info
    CDrawHealth*                      PDrawHealth_Data;      // +0x148 CCountedPointer<NPlayerGui::CDrawHealth>::Data
    CCPPointerInfo*                   PDrawHealth_Info;      // +0x14c CCountedPointer<NPlayerGui::CDrawHealth>::Info
    CDrawStamina*                     PDrawStamina_Data;     // +0x150 CCountedPointer<NPlayerGui::CDrawStamina>::Data
    CCPPointerInfo*                   PDrawStamina_Info;     // +0x154 CCountedPointer<NPlayerGui::CDrawStamina>::Info
    CDrawCombatMultiplier*            PDrawCombatMultiplier_Data; // +0x158 CCountedPointer<NPlayerGui::CDrawCombatMultiplier>::Data
    CCPPointerInfo*                   PDrawCombatMultiplier_Info; // +0x15c CCountedPointer<NPlayerGui::CDrawCombatMultiplier>::Info
    CDrawSpiritsHealth*               PDrawSpiritsHealth_Data; // +0x160 CCountedPointer<NPlayerGui::CDrawSpiritsHealth>::Data
    CCPPointerInfo*                   PDrawSpiritsHealth_Info; // +0x164 CCountedPointer<NPlayerGui::CDrawSpiritsHealth>::Info
    CDrawMiniMap*                     PDrawMiniMap_Data;     // +0x168 CCountedPointer<NPlayerGui::CDrawMiniMap>::Data
    CCPPointerInfo*                   PDrawMiniMap_Info;     // +0x16c CCountedPointer<NPlayerGui::CDrawMiniMap>::Info
    CDrawScreenFade*                  PDrawScreenFade_Data;  // +0x170 CCountedPointer<NPlayerGui::CDrawScreenFade>::Data
    CCPPointerInfo*                   PDrawScreenFade_Info;  // +0x174 CCountedPointer<NPlayerGui::CDrawScreenFade>::Info
    CDrawMoney*                       PDrawMoney_Data;       // +0x178 CCountedPointer<NPlayerGui::CDrawMoney>::Data
    CCPPointerInfo*                   PDrawMoney_Info;       // +0x17c CCountedPointer<NPlayerGui::CDrawMoney>::Info
    CDrawRenown*                      PDrawRenown_Data;      // +0x180 CCountedPointer<NPlayerGui::CDrawRenown>::Data
    CCPPointerInfo*                   PDrawRenown_Info;      // +0x184 CCountedPointer<NPlayerGui::CDrawRenown>::Info
    CDrawMorality*                    PDrawMorality_Data;    // +0x188 CCountedPointer<NPlayerGui::CDrawMorality>::Data
    CCPPointerInfo*                   PDrawMorality_Info;    // +0x18c CCountedPointer<NPlayerGui::CDrawMorality>::Info
    CDrawExperience*                  PDrawExperience_Data;  // +0x190 CCountedPointer<NPlayerGui::CDrawExperience>::Data
    CCPPointerInfo*                   PDrawExperience_Info;  // +0x194 CCountedPointer<NPlayerGui::CDrawExperience>::Info
    CDrawCrime*                       PDrawCrime_Data;       // +0x198 CCountedPointer<NPlayerGui::CDrawCrime>::Data
    CCPPointerInfo*                   PDrawCrime_Info;       // +0x19c CCountedPointer<NPlayerGui::CDrawCrime>::Info
    CDrawReceivedItems*               PDrawReceivedItems_Data; // +0x1a0 CCountedPointer<NPlayerGui::CDrawReceivedItems>::Data
    CCPPointerInfo*                   PDrawReceivedItems_Info; // +0x1a4 CCountedPointer<NPlayerGui::CDrawReceivedItems>::Info
    CDrawFishingGame*                 PDrawFishingGame_Data; // +0x1a8 CCountedPointer<NPlayerGui::CDrawFishingGame>::Data
    CCPPointerInfo*                   PDrawFishingGame_Info; // +0x1ac CCountedPointer<NPlayerGui::CDrawFishingGame>::Info
    CDrawStatUpdates*                 PDrawStatUpdates_Data; // +0x1b0 CCountedPointer<NPlayerGui::CDrawStatUpdates>::Data
    CCPPointerInfo*                   PDrawStatUpdates_Info; // +0x1b4 CCountedPointer<NPlayerGui::CDrawStatUpdates>::Info
    CDrawAutoPickups*                 PDrawAutoPickups_Data; // +0x1b8 CCountedPointer<NPlayerGui::CDrawAutoPickups>::Data
    CCPPointerInfo*                   PDrawAutoPickups_Info; // +0x1bc CCountedPointer<NPlayerGui::CDrawAutoPickups>::Info
    CDrawPerceivers*                  PDrawPerceivers_Data;  // +0x1c0 CCountedPointer<NPlayerGui::CDrawPerceivers>::Data
    CCPPointerInfo*                   PDrawPerceivers_Info;  // +0x1c4 CCountedPointer<NPlayerGui::CDrawPerceivers>::Info
    CDrawDamage*                      PDrawDamage_Data;      // +0x1c8 CCountedPointer<NPlayerGui::CDrawDamage>::Data
    CCPPointerInfo*                   PDrawDamage_Info;      // +0x1cc CCountedPointer<NPlayerGui::CDrawDamage>::Info
    CDrawQuestInfo*                   PDrawQuestInfo_Data;   // +0x1d0 CCountedPointer<NPlayerGui::CDrawQuestInfo>::Data
    CCPPointerInfo*                   PDrawQuestInfo_Info;   // +0x1d4 CCountedPointer<NPlayerGui::CDrawQuestInfo>::Info
    CDrawGuildSeal*                   PDrawGuildSeal_Data;   // +0x1d8 CCountedPointer<NPlayerGui::CDrawGuildSeal>::Data
    CCPPointerInfo*                   PDrawGuildSeal_Info;   // +0x1dc CCountedPointer<NPlayerGui::CDrawGuildSeal>::Info
    CDrawQuickAccessIcons*            PDrawQuickAccessIcons_Data; // +0x1e0 CCountedPointer<NPlayerGui::CDrawQuickAccessIcons>::Data
    CCPPointerInfo*                   PDrawQuickAccessIcons_Info; // +0x1e4 CCountedPointer<NPlayerGui::CDrawQuickAccessIcons>::Info
    CDrawScriptScreenText*            PDrawScriptScreenText_Data; // +0x1e8 CCountedPointer<NPlayerGui::CDrawScriptScreenText>::Data
    CCPPointerInfo*                   PDrawScriptScreenText_Info; // +0x1ec CCountedPointer<NPlayerGui::CDrawScriptScreenText>::Info
    CDrawTargetInfo*                  PDrawTargetInfo_Data;  // +0x1f0 CCountedPointer<NPlayerGui::CDrawTargetInfo>::Data
    CCPPointerInfo*                   PDrawTargetInfo_Info;  // +0x1f4 CCountedPointer<NPlayerGui::CDrawTargetInfo>::Info
    CDrawHighlightingTargetingCursor* PDrawHighlightingTargetingCursor_Data; // +0x1f8 CCountedPointer<NPlayerGui::CDrawHighlightingTargetingCursor>::Data
    CCPPointerInfo*                   PDrawHighlightingTargetingCursor_Info; // +0x1fc CCountedPointer<NPlayerGui::CDrawHighlightingTargetingCursor>::Info
    CDrawWorldTime*                   PDrawWorldTime_Data;   // +0x200 CCountedPointer<NPlayerGui::CDrawWorldTime>::Data
    CCPPointerInfo*                   PDrawWorldTime_Info;   // +0x204 CCountedPointer<NPlayerGui::CDrawWorldTime>::Info
    CDrawAbilityButtons*              PDrawAbilityButtons_Data; // +0x208 CCountedPointer<NPlayerGui::CDrawAbilityButtons>::Data
    CCPPointerInfo*                   PDrawAbilityButtons_Info; // +0x20c CCountedPointer<NPlayerGui::CDrawAbilityButtons>::Info
    CDrawLetterBoxBlackBars*          PDrawLetterBoxBlackBars_Data; // +0x210 CCountedPointer<NPlayerGui::CDrawLetterBoxBlackBars>::Data
    CCPPointerInfo*                   PDrawLetterBoxBlackBars_Info; // +0x214 CCountedPointer<NPlayerGui::CDrawLetterBoxBlackBars>::Info
    CDrawBettingHUD*                  PDrawBettingHUD_Data;  // +0x218 CCountedPointer<NPlayerGui::CDrawBettingHUD>::Data
    CCPPointerInfo*                   PDrawBettingHUD_Info;  // +0x21c CCountedPointer<NPlayerGui::CDrawBettingHUD>::Info
    CDrawProjectileTargetingCentre*   PDrawProjectileTargetingCentre_Data; // +0x220 CCountedPointer<NPlayerGui::CDrawProjectileTargetingCentre>::Data
    CCPPointerInfo*                   PDrawProjectileTargetingCentre_Info; // +0x224 CCountedPointer<NPlayerGui::CDrawProjectileTargetingCentre>::Info
    CDrawMeleeActionButtonEffects*    PDrawMeleeActionButtonEffects_Data; // +0x228 CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>::Data
    CCPPointerInfo*                   PDrawMeleeActionButtonEffects_Info; // +0x22c CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>::Info
    CDrawSpiritScores*                PDrawSpiritScores_Data; // +0x230 CCountedPointer<NPlayerGui::CDrawSpiritScores>::Data
    CCPPointerInfo*                   PDrawSpiritScores_Info; // +0x234 CCountedPointer<NPlayerGui::CDrawSpiritScores>::Info
    CDrawSpiritTargetingCursor*       PDrawSpiritTargetingCursor_Data; // +0x238 CCountedPointer<NPlayerGui::CDrawSpiritTargetingCursor>::Data
    CCPPointerInfo*                   PDrawSpiritTargetingCursor_Info; // +0x23c CCountedPointer<NPlayerGui::CDrawSpiritTargetingCursor>::Info
    CDraw2DText*                      PDraw2DText_Data;      // +0x240 CCountedPointer<NPlayerGui::CDraw2DText>::Data
    CCPPointerInfo*                   PDraw2DText_Info;      // +0x244 CCountedPointer<NPlayerGui::CDraw2DText>::Info
    CDrawMouseCursor*                 PDrawMouseCursor_Data; // +0x248 CCountedPointer<NPlayerGui::CDrawMouseCursor>::Data
    CCPPointerInfo*                   PDrawMouseCursor_Info; // +0x24c CCountedPointer<NPlayerGui::CDrawMouseCursor>::Info
    CDrawNPCName*                     PDrawNPCName_Data;     // +0x250 CCountedPointer<NPlayerGui::CDrawNPCName>::Data
    CCPPointerInfo*                   PDrawNPCName_Info;     // +0x254 CCountedPointer<NPlayerGui::CDrawNPCName>::Info
    bool                              ShowSpecialAbilityButtons; // +0x258
    bool                              ShowSpecialAbilityButtonsAbilities; // +0x259
    bool                              SpecialSelected;       // +0x25a
    bool                              SpecialDeselected;     // +0x25b
    unsigned char                     AbilityButtons[0x10];  // +0x25c vector<long,std::allocator<long>_>
    unsigned char                     AbilityButtonsAlpha[0x10]; // +0x26c vector<float,std::allocator<float>_>
    unsigned char                     QuickAccessButtonsAlpha[0x10]; // +0x27c vector<float,std::allocator<float>_>
    float                             AbilityButtonScale;    // +0x28c
    bool                              NormalToSpecial;       // +0x290
    bool                              SpecialToNormal;       // +0x291
    unsigned char                     _pad_0x292[0x2];       // +0x292
    float                             VWindow_TLX;           // +0x294 C2DBoxF::TLX
    float                             VWindow_TLY;           // +0x298 C2DBoxF::TLY
    float                             VWindow_BRX;           // +0x29c C2DBoxF::BRX
    float                             VWindow_BRY;           // +0x2a0 C2DBoxF::BRY
    bool                              DrawGui;               // +0x2a4
    bool                              OldDrawGui;            // +0x2a5
    unsigned char                     _pad_0x2a6[0x2];       // +0x2a6
    unsigned char                     GuiMode[0x4];          // +0x2a8 EPlayerGuiMode
    float                             FadeInTimer;           // +0x2ac
    bool                              ForcedClosed;          // +0x2b0
    bool                              ShowTime;              // +0x2b1
    bool                              ShowMoneyBag;          // +0x2b2
    bool                              ShowStatBars;          // +0x2b3
    float                             Alpha;                 // +0x2b4
    unsigned __int64                  HeroCentreUID;         // +0x2b8
    bool                              DrawingSafeAreas;      // +0x2c0
    bool                              DrawingDebugTGA;       // +0x2c1
    unsigned char                     _pad_0x2c2[0x2];       // +0x2c2
    unsigned char                     DebugTGATexture[0x8];  // +0x2c4 CTexture
    bool                              ScreenMessagingEnabled; // +0x2cc
    bool                              ForceAllowSubtitleToRender; // +0x2cd
    bool                              LiveGuiMode;           // +0x2ce
    unsigned char                     _pad_0x2cf[0x1];       // +0x2cf
    unsigned char                     Illustrators[0x10];    // +0x2d0 vector<NPlayerGui::CDrawBase_*,std::allocator<NPlayerGui::CDrawBase_*>_>
    CStatBar*                         PMoney_Data;           // +0x2e0 CCountedPointer<NPlayerGui::CStatBar>::Data
    CCPPointerInfo*                   PMoney_Info;           // +0x2e4 CCountedPointer<NPlayerGui::CStatBar>::Info
    CStatBar*                         PRenown_Data;          // +0x2e8 CCountedPointer<NPlayerGui::CStatBar>::Data
    CCPPointerInfo*                   PRenown_Info;          // +0x2ec CCountedPointer<NPlayerGui::CStatBar>::Info
    CStatBar*                         PMorality_Data;        // +0x2f0 CCountedPointer<NPlayerGui::CStatBar>::Data
    CCPPointerInfo*                   PMorality_Info;        // +0x2f4 CCountedPointer<NPlayerGui::CStatBar>::Info
    CStatBar*                         PExperience_Data;      // +0x2f8 CCountedPointer<NPlayerGui::CStatBar>::Data
    CCPPointerInfo*                   PExperience_Info;      // +0x2fc CCountedPointer<NPlayerGui::CStatBar>::Info
    CCrimeStatBar*                    PCrime_Data;           // +0x300 CCountedPointer<NPlayerGui::CCrimeStatBar>::Data
    CCPPointerInfo*                   PCrime_Info;           // +0x304 CCountedPointer<NPlayerGui::CCrimeStatBar>::Info
    CMultiStatBar*                    PStatUpdates_Data;     // +0x308 CCountedPointer<NPlayerGui::CMultiStatBar>::Data
    CCPPointerInfo*                   PStatUpdates_Info;     // +0x30c CCountedPointer<NPlayerGui::CMultiStatBar>::Info
    CAutoPickupBar*                   PAutoPickups_Data;     // +0x310 CCountedPointer<NPlayerGui::CAutoPickupBar>::Data
    CCPPointerInfo*                   PAutoPickups_Info;     // +0x314 CCountedPointer<NPlayerGui::CAutoPickupBar>::Info
    unsigned char                     TextPrimitiveHandles[0x10]; // +0x318 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char                     SafeAreaPrimitiveHandles[0x10]; // +0x328 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char                     DebugTGAPrimitiveHandle[0x8]; // +0x338 CEnginePrimitiveHandle
    unsigned char                     ScriptScreenText[0xc]; // +0x340 list<NPlayerGui::CScriptScreenText,std::allocator<NPlayerGui::CScriptScreenText>_>
    unsigned char                     ScreenMessages[0xc];   // +0x34c list<NPlayerGui::CScreenMessage,std::allocator<NPlayerGui::CScreenMessage>_>
    CManager*                         PHeroInfoScreensManager_ptr; // +0x358 scoped_ptr<NHeroInformationScreens::CManager>::ptr
    bool                              DisplayBottomHalfGUI;  // +0x35c
    unsigned char                     _pad_0x35d[0x3];       // +0x35d
};
#pragma pack(pop)

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CPlayerGui) == 0x360);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, __vftable) == 0x0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, World) == 0x4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PlayerManager) == 0x8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, InputInterface) == 0xc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Component) == 0x10);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Graphics) == 0x14);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SubtitleRenderer) == 0x18);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PFont_Data) == 0x140);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PFont_Info) == 0x144);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHealth_Data) == 0x148);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHealth_Info) == 0x14c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStamina_Data) == 0x150);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStamina_Info) == 0x154);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCombatMultiplier_Data) == 0x158);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCombatMultiplier_Info) == 0x15c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritsHealth_Data) == 0x160);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritsHealth_Info) == 0x164);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMiniMap_Data) == 0x168);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMiniMap_Info) == 0x16c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScreenFade_Data) == 0x170);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScreenFade_Info) == 0x174);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMoney_Data) == 0x178);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMoney_Info) == 0x17c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawRenown_Data) == 0x180);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawRenown_Info) == 0x184);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMorality_Data) == 0x188);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMorality_Info) == 0x18c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawExperience_Data) == 0x190);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawExperience_Info) == 0x194);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCrime_Data) == 0x198);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCrime_Info) == 0x19c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawReceivedItems_Data) == 0x1a0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawReceivedItems_Info) == 0x1a4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawFishingGame_Data) == 0x1a8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawFishingGame_Info) == 0x1ac);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStatUpdates_Data) == 0x1b0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStatUpdates_Info) == 0x1b4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAutoPickups_Data) == 0x1b8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAutoPickups_Info) == 0x1bc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawPerceivers_Data) == 0x1c0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawPerceivers_Info) == 0x1c4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawDamage_Data) == 0x1c8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawDamage_Info) == 0x1cc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuestInfo_Data) == 0x1d0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuestInfo_Info) == 0x1d4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawGuildSeal_Data) == 0x1d8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawGuildSeal_Info) == 0x1dc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuickAccessIcons_Data) == 0x1e0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuickAccessIcons_Info) == 0x1e4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScriptScreenText_Data) == 0x1e8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScriptScreenText_Info) == 0x1ec);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawTargetInfo_Data) == 0x1f0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawTargetInfo_Info) == 0x1f4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHighlightingTargetingCursor_Data) == 0x1f8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHighlightingTargetingCursor_Info) == 0x1fc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawWorldTime_Data) == 0x200);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawWorldTime_Info) == 0x204);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAbilityButtons_Data) == 0x208);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAbilityButtons_Info) == 0x20c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawLetterBoxBlackBars_Data) == 0x210);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawLetterBoxBlackBars_Info) == 0x214);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawBettingHUD_Data) == 0x218);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawBettingHUD_Info) == 0x21c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawProjectileTargetingCentre_Data) == 0x220);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawProjectileTargetingCentre_Info) == 0x224);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMeleeActionButtonEffects_Data) == 0x228);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMeleeActionButtonEffects_Info) == 0x22c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritScores_Data) == 0x230);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritScores_Info) == 0x234);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritTargetingCursor_Data) == 0x238);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritTargetingCursor_Info) == 0x23c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDraw2DText_Data) == 0x240);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDraw2DText_Info) == 0x244);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMouseCursor_Data) == 0x248);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMouseCursor_Info) == 0x24c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawNPCName_Data) == 0x250);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawNPCName_Info) == 0x254);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowSpecialAbilityButtons) == 0x258);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowSpecialAbilityButtonsAbilities) == 0x259);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SpecialSelected) == 0x25a);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SpecialDeselected) == 0x25b);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, AbilityButtons) == 0x25c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, AbilityButtonsAlpha) == 0x26c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, QuickAccessButtonsAlpha) == 0x27c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, AbilityButtonScale) == 0x28c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, NormalToSpecial) == 0x290);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SpecialToNormal) == 0x291);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_TLX) == 0x294);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_TLY) == 0x298);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_BRX) == 0x29c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_BRY) == 0x2a0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DrawGui) == 0x2a4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, OldDrawGui) == 0x2a5);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, GuiMode) == 0x2a8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, FadeInTimer) == 0x2ac);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ForcedClosed) == 0x2b0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowTime) == 0x2b1);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowMoneyBag) == 0x2b2);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowStatBars) == 0x2b3);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Alpha) == 0x2b4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, HeroCentreUID) == 0x2b8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DrawingSafeAreas) == 0x2c0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DrawingDebugTGA) == 0x2c1);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DebugTGATexture) == 0x2c4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ScreenMessagingEnabled) == 0x2cc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ForceAllowSubtitleToRender) == 0x2cd);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, LiveGuiMode) == 0x2ce);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Illustrators) == 0x2d0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMoney_Data) == 0x2e0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMoney_Info) == 0x2e4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PRenown_Data) == 0x2e8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PRenown_Info) == 0x2ec);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMorality_Data) == 0x2f0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMorality_Info) == 0x2f4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PExperience_Data) == 0x2f8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PExperience_Info) == 0x2fc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PCrime_Data) == 0x300);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PCrime_Info) == 0x304);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PStatUpdates_Data) == 0x308);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PStatUpdates_Info) == 0x30c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PAutoPickups_Data) == 0x310);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PAutoPickups_Info) == 0x314);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, TextPrimitiveHandles) == 0x318);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SafeAreaPrimitiveHandles) == 0x328);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DebugTGAPrimitiveHandle) == 0x338);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ScriptScreenText) == 0x340);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ScreenMessages) == 0x34c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PHeroInfoScreensManager_ptr) == 0x358);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DisplayBottomHalfGUI) == 0x35c);
#endif

#endif // FABLE_ENGINE_CPLAYERGUI_H

#undef CPlayerGui
#ifndef FABLE_ENGINE_CSUBTITLERENDERER_H
#define FABLE_ENGINE_CSUBTITLERENDERER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CSubtitleRenderer = 0x128.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CFontBank;
class CGraphicDataBank;
class CWideStringData;

#pragma pack(push, 1)
class CSubtitleRenderer { public:
    void*             __vftable;                             // +0x00 vptr, or first dword of a flattened base subobject
    float             VWindow_TLX;                           // +0x04 C2DBoxF::TLX
    float             VWindow_TLY;                           // +0x08 C2DBoxF::TLY
    float             VWindow_BRX;                           // +0x0c C2DBoxF::BRX
    float             VWindow_BRY;                           // +0x10 C2DBoxF::BRY
    float             VTelevisionWindow_TLX;                 // +0x14 C2DBoxF::TLX
    float             VTelevisionWindow_TLY;                 // +0x18 C2DBoxF::TLY
    float             VTelevisionWindow_BRX;                 // +0x1c C2DBoxF::BRX
    float             VTelevisionWindow_BRY;                 // +0x20 C2DBoxF::BRY
    long              TextCharactersShowing;                 // +0x24
    bool              TextGroupHasEntriesRemaining;          // +0x28
    unsigned char     _pad_0x29[0x3];                        // +0x29
    long              TextGroupIndex;                        // +0x2c
    CWideStringData*  TextGroupTag_PStringData;              // +0x30 CWideString::PStringData
    unsigned char     TextInfoQueue[0x18];                   // +0x34 queue<CSubtitleRenderer::CSetTextInfo,std::deque<CSubtitleRenderer::CSetTextInfo,std::allocator<CSubtitleRenderer::CSetTextInfo>_>_>
    CWideStringData*  Text_PStringData;                      // +0x4c CWideString::PStringData
    CWideStringData*  NameText_PStringData;                  // +0x50 CWideString::PStringData
    CWideStringData*  Button1Text_PStringData;               // +0x54 CWideString::PStringData
    CWideStringData*  Button2Text_PStringData;               // +0x58 CWideString::PStringData
    CWideStringData*  Button3Text_PStringData;               // +0x5c CWideString::PStringData
    CWideStringData*  Button1OverrideText_PStringData;       // +0x60 CWideString::PStringData
    CWideStringData*  Button2OverrideText_PStringData;       // +0x64 CWideString::PStringData
    unsigned char     Button1Alpha;                          // +0x68
    unsigned char     Button2Alpha;                          // +0x69
    unsigned char     _pad_0x6a[0x2];                        // +0x6a
    unsigned long     DisplayFlags;                          // +0x6c
    CFontBank*        PFont;                                 // +0x70
    CGraphicDataBank* Graphics;                              // +0x74
    float             MaxDisplayTime;                        // +0x78
    float             TextTimer;                             // +0x7c
    unsigned char     PlayerModeAdded[0x4];                  // +0x80 EPlayerMode
    unsigned char     PrimitiveHandles[0x10];                // +0x84 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char     ButtonPrimitiveHandles[0x10];          // +0x94 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char     EmbeddedPrimitiveHandles[0x10];        // +0xa4 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char     EmbeddedGraphics[0x10];                // +0xb4 vector<CSubtitleRenderer::CEmbeddedGraphic,std::allocator<CSubtitleRenderer::CEmbeddedGraphic>_>
    unsigned char     EmbeddedHighlights[0x10];              // +0xc4 vector<CSubtitleRenderer::CEmbeddedHighlight,std::allocator<CSubtitleRenderer::CEmbeddedHighlight>_>
    unsigned char     EmbeddedHighlightActionNames[0x10];    // +0xd4 vector<CWideString,std::allocator<CWideString>_>
    bool              PausedGame;                            // +0xe4
    bool              AllCharactersDisplayed;                // +0xe5
    bool              InCutscene;                            // +0xe6
    bool              CutsceneSkipped;                       // +0xe7
    bool              CutsceneSkippable;                     // +0xe8
    bool              CutsceneSkippableWhilePaused;          // +0xe9
    bool              CutsceneActionMode;                    // +0xea
    bool              Button1TextOverridden;                 // +0xeb
    bool              Button2TextOverridden;                 // +0xec
    bool              DisplayingText;                        // +0xed
    bool              UpdateWhilePaused;                     // +0xee
    bool              UpdateTimer;                           // +0xef
    unsigned char     SkipCutsceneTimer[0x1c];               // +0xf0 CFadeInFadeOutBase
    unsigned char     ButtonPulseTimer[0x1c];                // +0x10c CFadeInFadeOutBase
};
#pragma pack(pop)

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CSubtitleRenderer) == 0x128);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, __vftable) == 0x0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, VWindow_TLX) == 0x4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, VWindow_TLY) == 0x8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, VWindow_BRX) == 0xc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, VWindow_BRY) == 0x10);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, VTelevisionWindow_TLX) == 0x14);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, VTelevisionWindow_TLY) == 0x18);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, VTelevisionWindow_BRX) == 0x1c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, VTelevisionWindow_BRY) == 0x20);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, TextCharactersShowing) == 0x24);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, TextGroupHasEntriesRemaining) == 0x28);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, TextGroupIndex) == 0x2c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, TextGroupTag_PStringData) == 0x30);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, TextInfoQueue) == 0x34);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Text_PStringData) == 0x4c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, NameText_PStringData) == 0x50);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Button1Text_PStringData) == 0x54);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Button2Text_PStringData) == 0x58);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Button3Text_PStringData) == 0x5c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Button1OverrideText_PStringData) == 0x60);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Button2OverrideText_PStringData) == 0x64);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Button1Alpha) == 0x68);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Button2Alpha) == 0x69);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, DisplayFlags) == 0x6c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, PFont) == 0x70);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Graphics) == 0x74);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, MaxDisplayTime) == 0x78);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, TextTimer) == 0x7c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, PlayerModeAdded) == 0x80);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, PrimitiveHandles) == 0x84);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, ButtonPrimitiveHandles) == 0x94);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, EmbeddedPrimitiveHandles) == 0xa4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, EmbeddedGraphics) == 0xb4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, EmbeddedHighlights) == 0xc4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, EmbeddedHighlightActionNames) == 0xd4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, PausedGame) == 0xe4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, AllCharactersDisplayed) == 0xe5);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, InCutscene) == 0xe6);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, CutsceneSkipped) == 0xe7);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, CutsceneSkippable) == 0xe8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, CutsceneSkippableWhilePaused) == 0xe9);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, CutsceneActionMode) == 0xea);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Button1TextOverridden) == 0xeb);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, Button2TextOverridden) == 0xec);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, DisplayingText) == 0xed);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, UpdateWhilePaused) == 0xee);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, UpdateTimer) == 0xef);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, SkipCutsceneTimer) == 0xf0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSubtitleRenderer, ButtonPulseTimer) == 0x10c);
#endif

#endif // FABLE_ENGINE_CSUBTITLERENDERER_H


namespace NTCScriptedControl {
struct CThing;
void __cdecl free(void* p); // 0x00bfea14 is the MSVCR71 free import thunk, not operator delete
struct CSubtitledMessage { unsigned char nativeFields[0x44]; CSubtitledMessage(); CSubtitledMessage(const CSubtitledMessage&); ~CSubtitledMessage(); };
struct CSubtitledMessageArray {
    CSubtitledMessage* first; CSubtitledMessage* last; CSubtitledMessage* end;
    CSubtitledMessageArray() : first(0), last(0), end(0) {}
    ~CSubtitledMessageArray() { CSubtitledMessage* e=last; CSubtitledMessage* p=first; for (; p!=e; ++p) p->~CSubtitledMessage(); if (first) free(first); }
    CSubtitledMessageArray& operator=(const CSubtitledMessageArray& other);
    void resize(unsigned long, const CSubtitledMessage&); // 0x0071a5c0
};

// One-dword retail ABI view over the real fable_string.h CCharString. The derived copy
// constructor is out-of-line in retail and preserves the native counted-string lifetime.
struct CCharStringValue : ::CCharString {
    CCharStringValue() : ::CCharString() {}
    CCharStringValue(const CCharStringValue& other); // retail copy ctor @ 0x0099ec30
};

struct CActionTalkBase {
    virtual ~CActionTalkBase(); // scalar deleting dtor slot 0, thunk 0x0071a530
    virtual void Update();      // slot 1, 0x00716de0
    virtual void OnActivate(); // slot 2, 0x00717710
    virtual void OnAdd();      // slot 3, inherited by derived
    virtual ::CCharString GetActionName() const = 0; // slot 4, pure in base
    virtual void OnCleanup();  // slot 5, inherited by derived
    unsigned char pad[0x2c];
    CSubtitledMessageArray Messages;
    unsigned char tail[0x54];
    CActionTalkBase(CThing*, bool, bool, bool);
};
struct CActionTalkToThing : CActionTalkBase {
    bool AddedDuringCutscene;
    bool AskingHeroQuestion;
    bool DisplayOverScreenFade;
    CCharStringValue QuestionButton1;
    CCharStringValue QuestionButton2;
    CCharStringValue QuestionButton3;
    virtual void Update();              // override slot 1
    virtual void OnActivate();          // override slot 2
    virtual ::CCharString GetActionName() const; // override slot 4
    virtual void DisplayText();         // new slot 6, 0x007132c0
    CActionTalkToThing(CThing&, const char*, const char*, const CCharStringValue&, const CCharStringValue&, const CCharStringValue&, bool, bool);
};
void __fastcall GetMessagesFromTextName(CSubtitledMessageArray*, const char*, const char*, long); // 0x004cebb0
// The PDB headers place CPlayerGui::SubtitleRenderer at +0x18 and
// CSubtitleRenderer::InCutscene at +0xe6, or composite donor offset +0xfe.
// Native storage is eight bytes smaller before that flag, so this field-free
// byte view reads retail +0xf6 without inventing a direct PDB member.
enum {
    NATIVE_PLAYERGUI_INCUTSCENE_OFFSET =
        offsetof(CPlayerGui_DonorPDB, SubtitleRenderer) + offsetof(CSubtitleRenderer, InCutscene) - 8
};
#if defined(_MSC_VER)
typedef char check_playergui_subtitle_renderer_offset[(offsetof(CPlayerGui_DonorPDB, SubtitleRenderer) == 0x18) ? 1 : -1];
typedef char check_subtitle_renderer_cutscene_offset[(offsetof(CSubtitleRenderer, InCutscene) == 0xe6) ? 1 : -1];
typedef char check_native_playergui_cutscene_offset[(NATIVE_PLAYERGUI_INCUTSCENE_OFFSET == 0xf6) ? 1 : -1];
#endif
extern CPlayerGui_DonorPDB* g_pPlayerGui; // native global type CPlayerGui @ 0x013b8790

CActionTalkToThing::CActionTalkToThing(CThing& target, const char* textName, const char* groupName,
                                       const CCharStringValue& button1, const CCharStringValue& button2,
                                       const CCharStringValue& button3, bool makeTargetListen, bool soundIn2D)
    : CActionTalkBase(&target, makeTargetListen, true, soundIn2D),
      AskingHeroQuestion(true), DisplayOverScreenFade(false),
      QuestionButton1(button1), QuestionButton2(button2), QuestionButton3(button3)
{
    CSubtitledMessageArray messages;
    GetMessagesFromTextName(&messages, textName, groupName, 0);
    if (messages.first != messages.last) {
        messages.resize(1, CSubtitledMessage());
    }
    Messages = messages;
    CPlayerGui_DonorPDB* playerGui = g_pPlayerGui;
    AddedDuringCutscene = *(const bool*)((const unsigned char*)playerGui + NATIVE_PLAYERGUI_INCUTSCENE_OFFSET);
}

}
// END EXACT CANDIDATE 00717d40

typedef char check_subtitled_message_size[(sizeof(NTCScriptedControl::CSubtitledMessage) == 0x44) ? 1 : -1];
#if defined(_MSC_VER)
typedef char check_talk_base_size[(sizeof(NTCScriptedControl::CActionTalkBase) == 0x90) ? 1 : -1];
typedef char check_messages_offset[(offsetof(NTCScriptedControl::CActionTalkBase, Messages) == 0x30) ? 1 : -1];
typedef char check_button1_offset[(offsetof(NTCScriptedControl::CActionTalkToThing, QuestionButton1) == 0x94) ? 1 : -1];
typedef char check_button2_offset[(offsetof(NTCScriptedControl::CActionTalkToThing, QuestionButton2) == 0x98) ? 1 : -1];
typedef char check_button3_offset[(offsetof(NTCScriptedControl::CActionTalkToThing, QuestionButton3) == 0x9c) ? 1 : -1];
#endif

static int s_baseCalled;
static int s_messageDestroyed;
static int s_messagesRead;
static int s_messagesAssigned;
static int s_messageCopies;
static int s_derivedUpdates;
static int s_derivedActivations;
static int s_baseAdds;
static int s_baseCleanups;
static int s_actionNames;
static int s_displayTexts;
static int s_requestedMessageCount;
static int s_bad;
static const char* s_expectedTextName;
static const char* s_expectedGroupName;
static NTCScriptedControl::CThing* s_baseTarget;
static bool s_baseListen;
static bool s_baseCameraTags;
static bool s_baseSound2D;

namespace NTCScriptedControl {

struct CThing {};
CPlayerGui_DonorPDB playerGui;
CPlayerGui_DonorPDB* g_pPlayerGui = &playerGui;

CActionTalkBase::~CActionTalkBase() {}
void CActionTalkBase::Update() {}
void CActionTalkBase::OnActivate() {}
void CActionTalkBase::OnAdd() { ++s_baseAdds; }
::CCharString CActionTalkBase::GetActionName() const { return ::CCharString(); }
void CActionTalkBase::OnCleanup() { ++s_baseCleanups; }
void CActionTalkToThing::Update() { ++s_derivedUpdates; }
void CActionTalkToThing::OnActivate() { ++s_derivedActivations; }
::CCharString CActionTalkToThing::GetActionName() const { ++s_actionNames; return ::CCharString(); }
void CActionTalkToThing::DisplayText() { ++s_displayTexts; }

CActionTalkBase::CActionTalkBase(CThing* target, bool listen, bool cameraTags, bool sound2D)
    : Messages()
{
    s_baseCalled = 1;
    s_baseTarget = target;
    s_baseListen = listen;
    s_baseCameraTags = cameraTags;
    s_baseSound2D = sound2D;
}

 CCharStringValue::CCharStringValue(const CCharStringValue& other)
    : ::CCharString()
{
    void* data = *(void* const*)&other;
    *(void**)this = data;
    if (data)
        ++((CCharStringData*)data)->owners;
}


CSubtitledMessage::CSubtitledMessage()
{
    std::memset(nativeFields, 0, sizeof(nativeFields));
}
CSubtitledMessage::CSubtitledMessage(const CSubtitledMessage& other)
{
    std::memcpy(nativeFields, other.nativeFields, sizeof(nativeFields));
    ++s_messageCopies;
}
CSubtitledMessage::~CSubtitledMessage()
{
    ++s_messageDestroyed;
}

CSubtitledMessageArray& CSubtitledMessageArray::operator=(const CSubtitledMessageArray& other)
{
    if (first) {
        for (CSubtitledMessage* p = first; p != last; ++p)
            p->~CSubtitledMessage();
        free(first);
    }
    unsigned long count = other.last ? (unsigned long)(other.last - other.first) : 0;
    first = count ? (CSubtitledMessage*)::operator new(sizeof(CSubtitledMessage) * count) : 0;
    last = first;
    end = first ? first + count : 0;
    for (unsigned long i = 0; i < count; ++i) {
        new (last) CSubtitledMessage(other.first[i]);
        ++last;
    }
    ++s_messagesAssigned;
    return *this;
}

void CSubtitledMessageArray::resize(unsigned long count, const CSubtitledMessage& fill)
{
    unsigned long size = first ? (unsigned long)(last - first) : 0;
    if (count < size) {
        for (CSubtitledMessage* p = first + count; p != last; ++p)
            p->~CSubtitledMessage();
        last = first + count;
    } else if (count > size) {
        CSubtitledMessage* newFirst = (CSubtitledMessage*)::operator new(sizeof(CSubtitledMessage) * count);
        CSubtitledMessage* p = newFirst;
        for (unsigned long i = 0; i < size; ++i, ++p)
            new (p) CSubtitledMessage(first[i]);
        while ((unsigned long)(p - newFirst) < count) {
            new (p) CSubtitledMessage(fill);
            ++p;
        }
        if (first) {
            for (CSubtitledMessage* old = first; old != last; ++old)
                old->~CSubtitledMessage();
            free(first);
        }
        first = newFirst;
        last = p;
        end = p;
    }
}

void __cdecl free(void* p)
{
    ::operator delete(p);
}

void __fastcall GetMessagesFromTextName(CSubtitledMessageArray* out, const char* textName,
                                        const char* groupName, long zero)
{
    ++s_messagesRead;
    if (!out || !textName || !groupName || std::strcmp(textName, s_expectedTextName) != 0 ||
        std::strcmp(groupName, s_expectedGroupName) != 0 || zero != 0)
        ++s_bad;
    if (s_requestedMessageCount == 0) {
        out->first = out->last = out->end = 0;
        return;
    }
    out->first = (CSubtitledMessage*)::operator new(sizeof(CSubtitledMessage) * s_requestedMessageCount);
    out->last = out->first;
    out->end = out->first + s_requestedMessageCount;
    for (int i = 0; i < s_requestedMessageCount; ++i) {
        out->last->nativeFields[0] = (unsigned char)(i + 1);
        new (out->last) CSubtitledMessage();
        out->last->nativeFields[0] = (unsigned char)(i + 1);
        ++out->last;
    }
}

}

::CCharString::CCharString() : storage_(0) {}
::CCharString::~CCharString()
{
    if (storage_) --storage_->owners;
}

void __cdecl free(void* p);

static void setNativeCutsceneFlag(bool value)
{
    unsigned char* raw = (unsigned char*)&NTCScriptedControl::playerGui;
    raw[NTCScriptedControl::NATIVE_PLAYERGUI_INCUTSCENE_OFFSET] = value ? 1 : 0;
}

static bool runCase(int sequence, int suppliedMessages, bool cutscene, bool listen, bool sound2D)
{
    CCharStringData a = {0, 0, 0, 0, 1};
    CCharStringData b = {0, 0, 0, 0, 1};
    CCharStringData c = {0, 0, 0, 0, 1};
    NTCScriptedControl::CCharStringValue buttons[3];
    NTCScriptedControl::CThing target;
    char text[16];
    char group[16];
    std::sprintf(text, "text%d", sequence);
    std::sprintf(group, "group%d", sequence);
    *(void**)&buttons[0] = &a;
    *(void**)&buttons[1] = &b;
    *(void**)&buttons[2] = &c;

    s_baseCalled = s_messageDestroyed = s_messagesRead = s_messagesAssigned = 0;
    s_messageCopies = s_derivedUpdates = s_derivedActivations = s_baseAdds = 0;
    s_baseCleanups = s_actionNames = s_displayTexts = s_bad = 0;
    s_requestedMessageCount = suppliedMessages;
    s_expectedTextName = text;
    s_expectedGroupName = group;
    s_baseTarget = 0;
    setNativeCutsceneFlag(cutscene);
    {
        NTCScriptedControl::CActionTalkToThing action(target, text, group,
            buttons[0], buttons[1], buttons[2], listen, sound2D);
        if (s_bad || !s_baseCalled || s_baseTarget != &target || s_baseListen != listen ||
            !s_baseCameraTags || s_baseSound2D != sound2D)
            return false;
        if (!action.AskingHeroQuestion || action.DisplayOverScreenFade ||
            action.AddedDuringCutscene != cutscene)
            return false;
        if (*(void**)&action.QuestionButton1 != &a || *(void**)&action.QuestionButton2 != &b ||
            *(void**)&action.QuestionButton3 != &c)
            return false;
        if (a.owners != 2 || b.owners != 2 || c.owners != 2)
            return false;
        if (s_messagesRead != 1 || s_messagesAssigned != 1 || s_bad)
            return false;
        if (suppliedMessages == 0) {
            if (action.Messages.first || action.Messages.last || action.Messages.end)
                return false;
        } else {
            if (!action.Messages.first || action.Messages.last - action.Messages.first != 1 ||
                action.Messages.first[0].nativeFields[0] != 1)
                return false;
        }
#if defined(_MSC_VER)
        if ((char*)&action.Messages - (char*)&action != 0x30 ||
            (char*)&action.QuestionButton1 - (char*)&action != 0x94 ||
            (char*)&action.QuestionButton2 - (char*)&action != 0x98 ||
            (char*)&action.QuestionButton3 - (char*)&action != 0x9c)
            return false;
#endif
    }
    if (a.owners != 1 || b.owners != 1 || c.owners != 1)
        return false;
    if (s_messagesRead != 1 || s_messagesAssigned != 1 || s_bad)
        return false;
    if (s_messageCopies != (suppliedMessages ? 1 : 0) ||
        s_messageDestroyed != (suppliedMessages ? 5 : 0))
        return false;
    return true;
}

int main()
{
    if (!runCase(1, 3, true, true, false) || !runCase(2, 0, false, false, true)) {
        std::printf("00717d40_TEST FAIL\n");
        return 1;
    }
    std::printf("00717d40_TEST PASS\n");
    return 0;
}