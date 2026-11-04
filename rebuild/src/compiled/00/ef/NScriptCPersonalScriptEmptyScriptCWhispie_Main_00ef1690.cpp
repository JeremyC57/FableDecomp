// NScript::CPersonalScript_EmptyScript::CWhispie::Main() -- button-tutorial test entity: sheathe Me's weapons, then loop: wait for each of the ranged-unsheathe / first-person-targeting / lock-target / fire-ranged buttons to be pressed and released and for projectile-weapon mode to switch on and off, posting a screen message at each edge
// Leaf: ?Main@CWhispie@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF1690, 1237 bytes (+ int3 padding), plain `ret`. Literals "You pressed the unsheathe ranged
//   weapon button!" 0x012F60C0, "You let go!" 0x012F60B4, "You pressed the first person targeting button!" 0x012F6084, "You pressed
//   the lock target button!" 0x012F6060, "You pressed the fire ranged weapon button!" 0x012F6034, "You're in projectile weapon mode!"
//   0x012F6010, "Now you're not!" 0x012F6000.
// Identity (two sources): vtable 0x012F5FE4 slot 1 (CEntityScriptBase layout: slot 0 ??_G, slot 1 Main, slot 2 Init); Ego_r.pdb
//   ?Main@CWhispie@CPersonalScript_EmptyScript@NScript@@EAEXXZ (CWhispie 28 bytes: ParentClass +0x14 CPersonalScript_EmptyScript*,
//   PMasterData +0x18 CEmptyData*); sibling of the landed CPersonalScript_EmptyScript::CLeader::Init 0x00EF3ED0.
// Callees: CIsThingAlive (vtable 0x012C32F0, out-of-line dtor 0x00E241B0), CScriptThing copy 0x004ABE90, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7 NewScriptFrame, 506
//   EntitySheatheWeapons, 65..69 IsPlayerHolding{UnsheatheRangedWeapon,LockTarget,FireRangedWeapon,FirstPersonTargeting}Button /
//   IsHeroInProjectileWeaponMode, 112 AddScreenMessage.
// TU: shared headers (engine/CPersonalScript_EmptyScript.h owner class, CEntityScriptBase, CIsThingAlive), as the landed same-script sibling CPersonalScript_EmptyScript::CTrappedMan::Main 0x00EF1E10
//   (CIsThingAlive's destructor left out of line, as retail calls 0x00E241B0 for the predicate temporary).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
using NScript::CIsThingAlive;
#include "engine/CPersonalScript_EmptyScript.h"
using NScript::CPersonalScript_EmptyScript;


namespace NScript {
class CPersonalScript_EmptyScript::CWhispie : public CEntityScriptBase {
public:
    CPersonalScript_EmptyScript* ParentClass;      // +0x14
    CEmptyData*                  PMasterData;      // +0x18
private:
    virtual void Main();   // PDB ?Main@CWhispie@CPersonalScript_EmptyScript@NScript@@EAEXXZ (slot 1)
public:
};
}

void NScript::CPersonalScript_EmptyScript::CWhispie::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySheatheWeapons(Me, false);
    while (!IsActiveThreadTerminating()) {
        while (!Game->IsPlayerHoldingUnsheatheRangedWeaponButton()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString message("You pressed the unsheathe ranged weapon button!");
            Game->AddScreenMessage(message, (ETextGroupSelectionMethod)0);
        }
        while (Game->IsPlayerHoldingUnsheatheRangedWeaponButton()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString message("You let go!");
            Game->AddScreenMessage(message, (ETextGroupSelectionMethod)0);
        }
        while (!Game->IsPlayerHoldingFirstPersonTargetingButton()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString message("You pressed the first person targeting button!");
            Game->AddScreenMessage(message, (ETextGroupSelectionMethod)0);
        }
        while (Game->IsPlayerHoldingFirstPersonTargetingButton()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString message("You let go!");
            Game->AddScreenMessage(message, (ETextGroupSelectionMethod)0);
        }
        while (!Game->IsPlayerHoldingLockTargetButton()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString message("You pressed the lock target button!");
            Game->AddScreenMessage(message, (ETextGroupSelectionMethod)0);
        }
        while (Game->IsPlayerHoldingLockTargetButton()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString message("You let go!");
            Game->AddScreenMessage(message, (ETextGroupSelectionMethod)0);
        }
        while (!Game->IsPlayerHoldingFireRangedWeaponButton()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString message("You pressed the fire ranged weapon button!");
            Game->AddScreenMessage(message, (ETextGroupSelectionMethod)0);
        }
        while (Game->IsPlayerHoldingFireRangedWeaponButton()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString message("You let go!");
            Game->AddScreenMessage(message, (ETextGroupSelectionMethod)0);
        }
        while (!Game->IsHeroInProjectileWeaponMode()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString message("You're in projectile weapon mode!");
            Game->AddScreenMessage(message, (ETextGroupSelectionMethod)0);
        }
        while (Game->IsHeroInProjectileWeaponMode()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString message("Now you're not!");
            Game->AddScreenMessage(message, (ETextGroupSelectionMethod)0);
        }
        Game->NewScriptFrame();
    }
}