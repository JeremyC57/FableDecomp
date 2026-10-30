// CStopDrawing::Main() -- snow-troll rock pile: shut the nearest region entrance/exit; unless the troll intro already ran, take the rock pile under scripted control, loop its idle animation and wait until the hero comes within 15 units, hits it, or hits it with a special ability other than ability 14; then play CS_SNOWTROLL_INTRO, unhide Me on the minimap, flag the intro done and release the pile; finally show the ice-troll health bar and idle
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE36A0, 1151 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Literals "REGION_ENTRANCE_POINT" 0x012CA3DC, "REGION_EXIT_POINT" 0x012CA510, "SCRIPT_ROCK_PILE" 0x012F461C,
//   "SCRIPT_NAME_HERO" 0x0125D1C8, "CS_SNOWTROLL_INTRO" 0x012F4608, "HUD_QUEST_ICON_ICE_TROLL" 0x012F45EC.
// Callees: CScriptThing copy 0x004ABE90 (filter member), ~CIsThingAlive 0x00E241B0 (out of line), ~CBaseClass 0x0099A2E0,
//   RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, operator delete 0x00BFE9BC, CCharString 0x0099EBF0 /
//   0x0099EAE0, handle IsValid 0x00CD23B9 / Release 0x00CD2770, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall),
//   parent PlayCutscene(CCharString) 0x00EE2DB0; GameCalls slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero,
//   81 GetNearestWithDefName, 321 DisplayQuestInfo, 325 AddQuestInfoBarHealth, 356 EntitySetAsHiddenOnMiniMap,
//   477 SetThingPersistent, 567 SetRegionExitAsActive, 568 SetRegionEntranceAsActive; CScriptThing slots 21 MsgIsHitBy,
//   41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom; handle slot 20 PlayLoopingAnimation.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (the troll quest script: rock-pile handle +0x48,
//   intro-played flag +0x7d), PMasterData +0x18, EntrancePoint +0x1C, ExitPoint +0x28 (CScriptThing members).
// Shape: the parent's IsValid/Release pair is an inline parent helper (a local handle alias computes the handle with
//   `add` instead of `lea`); CRGBColour is the D3DCOLOR B,G,R,A byte view set R,G,B,A in its body (store order 0x16,0x17,0x15,0x14);
//   the GetNearestWithDefName results are assigned through inline operator= and destroyed by the inline ~CScriptThing;
//   the hero-name temporaries of the wait test are one `||`/`&&` chain (construction flags in ebx).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;
struct CBaseIntelligentPointer {                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
    CBaseIntelligentPointer();                          // 0x0099A380
    virtual ~CBaseIntelligentPointer();                 // 0x0099A430
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseIntelligentPointer { public:   // vtable 0x0127094C, 0x10 bytes
    virtual void h001() const; virtual void h002() const; virtual void h003() const; virtual void h004() const; virtual void h005() const; virtual void h006() const; virtual void h007() const; virtual void h008() const;
    virtual void h009() const; virtual void h010() const; virtual void h011() const; virtual void h012() const; virtual void h013() const; virtual void h014() const; virtual void h015() const; virtual void h016() const;
    virtual void h017() const; virtual void h018() const; virtual void h019() const;
    virtual void PlayLoopingAnimation(const CCharString& anim, long loops, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // slot 20 (+0x50)
    void* PImpData;                                     // +0x08
    void* PImpInfo;                                     // +0x0c
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern bool g_bScriptAnimationFlag;                    // 0x01375748
struct CQ_SnowTrollScript {
    unsigned char _pad00[0x48];
    CScriptGameResourceObjectScriptedThingBase RockPile;   // +0x48
    unsigned char _pad58[0x7d - 0x58];
    bool IntroPlayed;                                   // +0x7d
    void PlayCutscene(CCharString name);
    void ReleaseRockPile() { if (RockPile.IsValid()) RockPile.Release(); }                // 0x00EE2DB0
};
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
struct CStopDrawing : CEntityScriptBase {
    CQ_SnowTrollScript* ParentClass;                    // +0x14
    void*               PMasterData;                    // +0x18
    CScriptThing        EntrancePoint;                  // +0x1c
    CScriptThing        ExitPoint;                      // +0x28
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};

void CStopDrawing::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingPersistent(Me, true);
    EntrancePoint = Game->GetNearestWithDefName(Me, "REGION_ENTRANCE_POINT");
    ExitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    Game->SetRegionEntranceAsActive(EntrancePoint, false);
    Game->SetRegionExitAsActive(ExitPoint, false);
    if (!ParentClass->IntroPlayed) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsHiddenOnMiniMap(Me, true);
        ParentClass->ReleaseRockPile();
        while (!Game->StartScriptingEntity(Me, ParentClass->RockPile, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ParentClass->RockPile.PlayLoopingAnimation("SCRIPT_ROCK_PILE", -1, false, false, false, true, g_bScriptAnimationFlag, false, false);
        for (;;) {
            bool waiting = !(IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)
                || Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO")));
            if (!waiting)
                break;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ParentClass->PlayCutscene("CS_SNOWTROLL_INTRO");
        Game->EntitySetAsHiddenOnMiniMap(Me, false);
        ParentClass->IntroPlayed = true;
        ParentClass->RockPile.Release();
    }
    Game->AddQuestInfoBarHealth(Me, CRGBColour(0xff, 0, 0, 0xff), "HUD_QUEST_ICON_ICE_TROLL", 1.0f);
    Game->DisplayQuestInfo(true);
    do
        Game->NewScriptFrame();
    while (!IsActiveThreadTerminating());
}