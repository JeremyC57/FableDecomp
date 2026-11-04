// NScript::CQ_HeroSoulsBriarScript::CBRIAR_ROSE_IMPOSTER::Main() -- a Briar Rose imposter in the Hero Souls quest: alive filter on Me, reset the two taunt timers, then every frame: when the hero hits it (or hits it with any special ability but number 14) and the taunt timer has run out, run a one-line taunt conversation and restart the timers; when the "here" timer has run out, the taunt timer is above 7 and the hero is over 13 units away, say "here" and restart that timer
// Leaf: ?Main@CBRIAR_ROSE_IMPOSTER@CQ_HeroSoulsBriarScript@NScript@@EAEXXZ
// Naming wave 30 (family cst): the local virtual-slot CScriptThing stand-in (non-virtual ??1CScriptThing@@QAE@XZ) and its CScriptThingImpl pointee are
//   replaced by engine/CScriptThing.h's retail class (virtual ~CScriptThing 0x004AA840, Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ;
//   the pointee is CScriptThing); MsgIsHitBySpecialAbilityFrom takes the PDB's EHeroAbility
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D81820, length 822 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy @ 0x004ABE90 / dtor @ 0x004AA840, SetPredicate @ 0x00F35B10, IsDone @ 0x00F35B30,
//          CScriptThing slots 21 MsgIsHitBy (+0x54), 41 MsgIsHitBySpecialAbilityFrom (+0xa4), 42
//          MsgIsHitByAnySpecialAbilityFrom (+0xa8); CGameScriptInterface slots 7, 70 GetHero, 89 SetTimer, 90 GetTimer,
//          364/365/366 AddNewConversation / AddPersonToConversation / AddLineToConversation (Game +4 and the timer
//          interface global 0x0143E8F8); IsDistanceBetweenThingsOver @ 0x00CBE3EA; CCharString @ 0x0099EBF0 / 0x0099EAE0.
//          Strings "SCRIPT_NAME_HERO" 0x0125D1C8, "TEXT_QST_B04_BRIAR_COPY_TAUNT" 0x012D4350,
//          "TEXT_QST_B04_BRIAR_COPY_HERE" 0x012D4330.
// Layout: Ego_r.pdb CBRIAR_ROSE_IMPOSTER (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//         (CQ_HeroSoulsBriarScript: BriarRoseImposterTauntTimer PDB +0x80 / retail +0x68, BriarRoseImposterHereTimer
//         PDB +0x84 / retail +0x6c), PMasterData +0x18.
// Shape: the hit test is one bool whose CCharString temporaries are destroyed under the flag word (spilled to the
//         frame); the timers are CTimer members with inline Get/Set through the interface global.
// Naming wave 35: identity reg 00d80eef "BRIAR_ROSE_IMPOSTER" in 00d80e40; Alloc 00d81750; vtable 012d4314 slot 1; PDB ?Main@CBRIAR_ROSE_IMPOSTER@CQ_HeroSoulsBriarScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CBRIAR_ROSE_IMPOSTER::Main (wave3/labels_alloc.tsv); PDB ?Main@CBRIAR_ROSE_IMPOSTER@CQ_HeroSoulsBriarScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CBRIAR_ROSE_IMPOSTER@@QAEXXZ`; this source now defines the PDB member (outer stand-in CQ_HeroSoulsBriarScript moved into NScript); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator+=(const CCharString& other); // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830

#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                         // : CBaseClass, vtable 0x01238C8C; virtual ~CScriptThing 0x004AA840

bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CCountedPointer.h"
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

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
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;

class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};




extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
namespace NScript {
class CQ_HeroSoulsBriarScript {
public:
    unsigned char _pad[0x68];
    CTimer BriarRoseImposterTauntTimer;                // +0x68
    CTimer BriarRoseImposterHereTimer;                 // +0x6c
public:
    class CBRIAR_ROSE_IMPOSTER;      // the entity class below (naming wave 35)
};
}
using NScript::CQ_HeroSoulsBriarScript;

namespace NScript {
class CQ_HeroSoulsBriarScript::CBRIAR_ROSE_IMPOSTER : public CEntityScriptBase {
public:
    CQ_HeroSoulsBriarScript* ParentClass;              // +0x14
    void* PMasterData;                                 // +0x18
private:
    virtual void Main();                          // PDB ?Main@CBRIAR_ROSE_IMPOSTER@CQ_HeroSoulsBriarScript@NScript@@EAEXXZ
};
}

void NScript::CQ_HeroSoulsBriarScript::CBRIAR_ROSE_IMPOSTER::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    ParentClass->BriarRoseImposterTauntTimer = 0;
    ParentClass->BriarRoseImposterHereTimer = 4;
    while (!IsActiveThreadTerminating()) {
        bool hit = (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                    || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                        && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO")))
                   && ParentClass->BriarRoseImposterTauntTimer.GetTime() == 0;
        if (hit) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_BRIAR_COPY_TAUNT", false, Me, Game->GetHero());
            ParentClass->BriarRoseImposterTauntTimer = 15;
            ParentClass->BriarRoseImposterHereTimer = 4;
        }
        if (ParentClass->BriarRoseImposterHereTimer.GetTime() == 0
            && ParentClass->BriarRoseImposterTauntTimer.GetTime() > 7
            && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 13.0f)) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_BRIAR_COPY_HERE", false, Me, Game->GetHero());
            ParentClass->BriarRoseImposterHereTimer = 15;
        }
        Game->NewScriptFrame();
    }
}