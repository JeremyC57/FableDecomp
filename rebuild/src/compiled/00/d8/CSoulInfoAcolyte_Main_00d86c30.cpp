// CSoulInfoAcolyte::Main() -- Hero Souls guild acolyte: once the parent quest flags Me ready (+0x4a), take Me under scripted control; then each frame, if the hero talks to Me speak the acolyte line inside a paused movie sequence, and if the hero hits Me (plainly, or with a special ability other than ability 14) speak the on-hit line and make Me and the hero allies
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D86C30, 1296 bytes, plain `ret`; entity-script vtable slot 1 (Main).
// Literals "SCRIPT_NAME_HERO" 0x0125D1C8, "" 0x0122D70E, "TEXT_QST_B04_GUILD_ACOLYTE" 0x012D4C5C, "TEXT_QST_B04_GUILD_ACOLYTE_HIT" 0x012D4C3C.
// Callees: the landed CEndScythe::Main 0x00D79D50 model: CIsThingAlive (inline dtor) over CScriptThing copy 0x004ABE90 / dtor 0x004AA840,
//   RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle
//   IsValid 0x00CD23B9 / Release 0x00CD2770 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 /
//   dtor 0x007E74D0, movie handle ctor 0x006E7B60 / dtor 0x006E7B80; GameCalls slots 7, 8, 70, 264 GetHealth, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities, 599 EntitySetThingAsAllyOfThing; CScriptThing slots 21, 27, 41, 42.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (ready flag +0x4a), PMasterData +0x18.
// Shape: CEndScythe's body without its prelude; here the handle destructor is always the out-of-line copy.
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
    CCountedPointer() : Data(0), Info(0) {}
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
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    VPAD16(ma) VPAD4(mb)                                // slots 1..20
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    VPAD4(mc) VPAD1(mg)                                 // slots 22..26
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    VPAD8(md) VPAD4(me) VPAD1(mf)                       // slots 28..40
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                                    // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_HeroSoulsGuildScript {
    unsigned char _pad[0x4a];
    bool AcolyteReady;                                  // +0x4a
};
extern const float g_fZero;                            // 0x0122DEDC (0.0f)
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CSoulInfoAcolyte : CEntityScriptBase {
    CQ_HeroSoulsGuildScript* ParentClass;               // +0x14
    void*                    PMasterData;               // +0x18
    void Main();
};

void CSoulInfoAcolyte::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase face;
    while (!ParentClass->AcolyteReady) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (face.IsValid())
        face.Release();
    while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth(0)
            CScriptGameResourceObjectMovieBase movie;
#pragma inline_depth()
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                face.Speak(Game->GetHero(), "TEXT_QST_B04_GUILD_ACOLYTE", (ETextGroupSelectionMethod)2, false, true, false);
                while (face.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
            }
            game->PauseAllNonScriptedEntities(false);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth(0)
            CScriptGameResourceObjectMovieBase movie;
#pragma inline_depth()
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                face.Speak(Game->GetHero(), "TEXT_QST_B04_GUILD_ACOLYTE_HIT", (ETextGroupSelectionMethod)2, false, true, false);
                while (face.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
            }
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            game->PauseAllNonScriptedEntities(false);
        }
        Game->NewScriptFrame();
    }
}