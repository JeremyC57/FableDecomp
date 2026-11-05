// NScript::CQ_RansomVictimScript::CRansomRevenge::Main() -- the ransom victim's avenger: register an alive-filter over Me, wait until the hero is within a random 10..29 units, then turn on "Victim" (when alive, at even odds) or else on the hero, one time in three calling out who it is going for; wait until Me is hit by the hero (plainly, or by any of his special abilities other than ability 14), then clear the target and turn on the hero
// Leaf: ?Main@CRansomRevenge@CQ_RansomVictimScript@NScript@@EAEXXZ
// Plan phase 4 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line, as retail calls them here). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE3830, length 972 bytes, plain `ret`. Vtable 0x012DD3D8, factory 0x00DE3760
// registered as "RansomRevenge".
// Callees: CIsThingAlive filter (vtable 0x012C32F0, inline dtor) over the CScriptThing copy ctor 0x004ABE90 /
// ~CScriptThing 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30;
// IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); rand 0x00BFEB16; CCharString 0x0099EBF0 / 0x0099EAE0;
// CScriptThing slots 21 MsgIsHitBy, 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom, 75 IsAlive;
// CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName, 364 AddNewConversation,
// 365 AddPersonToConversation, 366 AddLineToConversation, 451 GiveThingBestEnemyTarget, 452 ClearThingBestEnemyTarget.
// Literals "Victim" 0x012CA958, "TEXT_QST_B06_ASSASSIN_ON_GOING_FOR_VICTIM" 0x012DD41C,
// "TEXT_QST_B06_ASSASSIN_ON_GOING_FOR_HERO" 0x012DD3F4, "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: Ego_r.pdb CRansomRevenge (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
// PMasterData +0x18 (neither used here).
// Shape: the sibling CRandomAttacker::Main 0x00DE21D0; the two "going for the hero" arms are identical and VC7.1
// cross-jumps them (and the AddLineToConversation tails of all three); the hit test is a break-out `if (a || (b && !c))` chain whose
// conditional string temporaries carry ebx flag bits 1, 2, 4.
// Naming wave 35: identity reg 00de0bc4 "RansomRevenge" in 00de0850; Alloc 00de3760; vtable 012dd3d8 slot 1; PDB ?Main@CRansomRevenge@CQ_RansomVictimScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CRansomRevenge::Main (wave3/labels_alloc.tsv); PDB ?Main@CRansomRevenge@CQ_RansomVictimScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CRansomRevenge@@QAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const char* text) const;           // 0x0099E960
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
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern "C" int __cdecl rand();                         // 0x00BFEB16
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
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
namespace NScript {
class CQ_RansomVictimScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CRansomRevenge;
};
}
using NScript::CQ_RansomVictimScript;
namespace NScript {
class CQ_RansomVictimScript::CRansomRevenge : public CEntityScriptBase {
public:
    void* ParentClass;                                 // +0x14 CQ_RansomVictimScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
private:
    virtual void Main();                          // PDB ?Main@CRansomRevenge@CQ_RansomVictimScript@NScript@@EAEXXZ
};
}

void NScript::CQ_RansomVictimScript::CRansomRevenge::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    float distance = (float)(rand() % 20 + 10);
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), distance)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->GetThingWithScriptName("Victim").IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        if (rand() % 2 == 0) {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveThingBestEnemyTarget(Me, Game->GetThingWithScriptName("Victim"));
            if (rand() % 3 == 0) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B06_ASSASSIN_ON_GOING_FOR_VICTIM", false, Me, Game->GetHero());
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            if (rand() % 3 == 0) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B06_ASSASSIN_ON_GOING_FOR_HERO", false, Me, Game->GetHero());
            }
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        if (rand() % 3 == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B06_ASSASSIN_ON_GOING_FOR_HERO", false, Me, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_14, "SCRIPT_NAME_HERO")))
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ClearThingBestEnemyTarget(Me);
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (IsActiveThreadTerminating())
        return;
}