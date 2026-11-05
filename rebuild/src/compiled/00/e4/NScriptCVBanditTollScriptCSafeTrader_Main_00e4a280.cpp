// NScript::CV_BanditTollScript::CSafeTrader::Main() -- the bandit-toll quest's safe trader: register an alive-filter over Me, take Me under scripted control, then until Me reaches the "SafeTraderEscape" marker keep walking there, warn the hero once when he comes near (a hostile line if he already attacked), and when the hero hits Me turn scared and run the precalculated route; finally fade out and kill Me
// Leaf: ?Main@CSafeTrader@CV_BanditTollScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4A280, length 992 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, out-of-line dtor 0x00E241B0) over the CScriptThing copy ctor
// 0x004ABE90 / ~CScriptThing 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30;
// scripted-thing handle (vtable 0x0127094C) inline ctor over CBaseObject 0x0099A380, IsValid 0x00CD23B9, Release
// 0x00CD2770, MoveToThing 0x007E7300, FollowPreCalculatedRoute 0x007E7310, IsPerformingScriptTask 0x007E7450, dtor
// 0x007E74D0; IsDistanceBetweenThingsOver 0x00CBE3EA / IsDistanceBetweenThingsUnder 0x00CBE2FF / FadeOutAndKillEntity
// 0x00CBE716 (all __fastcall); CCharString 0x0099EBF0 / 0x0099EAE0; CScriptThing slots 21 MsgIsHitBy, 41
// MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom; CGameScriptInterface slots 7 NewScriptFrame, 8
// StartScriptingEntity, 70 GetHero, 72 GetThingWithScriptName, 364 AddNewConversation, 365 AddPersonToConversation,
// 366 AddLineToConversation, 496 EntitySetAsScared. pGlobalScriptDef 0x0143E90C (+0x42c long warning distance, +0x43c
// float escape distance, +0x440 float fade time; the retail CScriptDef differs from the Ego_r.pdb one here).
// Literals "SafeTraderEscape" 0x012E4F80, "TEXT_QST_011_SAFE_TRADER_WARNING_10" 0x012E4F5C,
// "TEXT_QST_011_SAFE_TRADER_WARNING_10_HATE" 0x012E4F30, "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: Ego_r.pdb CSafeTrader (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
// (CV_BanditTollScript*, byte +0x4d set on the warning), PMasterData +0x18, WarnedHero +0x1c, AttackedByHero +0x1d.
// Naming wave 35: identity reg 00e47ce9 "SafeTrader" in 00e479b0; Alloc 00e4a190; vtable 012e4f14 slot 1; PDB ?Main@CSafeTrader@CV_BanditTollScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CSafeTrader::Main (wave3/labels_alloc.tsv); PDB ?Main@CSafeTrader@CV_BanditTollScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CSafeTrader@@QAEXXZ`; this source now defines the PDB member (outer stand-in CV_BanditTollScript moved into NScript); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
enum EHeroAbility { HERO_ABILITY_14 = 14 };
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                  // 0x00E241B0
};
}
using NScript::CIsThingAlive;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_0 };
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);  // 0x00CBE716
class CScriptDef { public:
    unsigned char _pad0[0x42c];
    long  SafeTraderWarningDistance;                   // +0x42C
    unsigned char _pad1[0x43c - 0x430];
    float SafeTraderEscapeDistance;                    // +0x43C
    float SafeTraderFadeTime;                          // +0x440
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CV_BanditTollScript {
public:
    unsigned char _pad[0x4d];
    bool SafeTraderWarnedHero;                         // +0x4D
public:
    class CSafeTrader;               // the entity class below (naming wave 35)
};
}
using NScript::CV_BanditTollScript;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
namespace NScript {
class CV_BanditTollScript::CSafeTrader : public CEntityScriptBase {
public:
    CV_BanditTollScript* ParentClass;                  // +0x14
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    bool  WarnedHero;                                  // +0x1C
    bool  AttackedByHero;                              // +0x1D
private:
    virtual void Main();                          // PDB ?Main@CSafeTrader@CV_BanditTollScript@NScript@@EAEXXZ
};
}

void NScript::CV_BanditTollScript::CSafeTrader::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing escape = Game->GetThingWithScriptName("SafeTraderEscape");
    long warningDistance = pGlobalScriptDef->SafeTraderWarningDistance;
    while (IsDistanceBetweenThingsOver(Me, escape, pGlobalScriptDef->SafeTraderEscapeDistance)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!handle.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return;
            handle.MoveToThing(escape, 1.0f, (EScriptEntityMoveType)0, false, true, false, true);
        }
        if (!WarnedHero && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, (float)warningDistance)) {
            if (IsActiveThreadTerminating())
                return;
            WarnedHero = true;
            ParentClass->SafeTraderWarnedHero = true;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (!AttackedByHero) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_QST_011_SAFE_TRADER_WARNING_10", false, Me, Game->GetHero());
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_QST_011_SAFE_TRADER_WARNING_10_HATE", false, Me, Game->GetHero());
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            AttackedByHero = true;
            Game->EntitySetAsScared(Me, true);
            handle.FollowPreCalculatedRoute(escape, (EScriptEntityMoveType)1, false, true);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    FadeOutAndKillEntity(Me, true, pGlobalScriptDef->SafeTraderFadeTime, true);
}