// NScript::CPersonalScript_EmptyScript::CTrappedMan::Main() -- the trapped man on the hill: register an alive filter over Me, run a frame; then, until the script is done, wait for the hero to hit Me, show "You hit the man on the hill!" and, when his health is below 1e-4, "You hit him to the point of death!", once per script frame
// Leaf: Main
// Identity (naming wave 11): ?Main@CTrappedMan@CPersonalScript_EmptyScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F6128 slot 1 is stored by the factory 0x00EF1D30 registered as "TrappedMan"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF1E10, 384 bytes (entity-script vtable slot 1 = Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (filter member; filter vtable 0x012C32F0), the filter registration
//          @ 0x00F35B10, the filter's out-of-line dtor @ 0x00E241B0, IsDone @ 0x00F35B30, CCharString(const char*, long)
//          @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CScriptThing::MsgIsHitBy (slot 21, +0x54), CGameScriptInterface slots
//          7 NewScriptFrame (+0x1c), 112 AddScreenMessage (+0x1c0) and 264 GetHealth (+0x420); float pool 0x0129BA3C = 1e-4f.
// Layout: Ego_r.pdb CTrappedMan (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
//          PMasterData +0x18.
// Shape: the landed CHHUndead::Main filter block; `WAIT_UNTIL(Me.MsgIsHitBy("SCRIPT_NAME_HERO"))` is a `while (!x)` whose
//          neg/sbb/inc bool survives the string temporary's dtor (VC duplicates the test at the loop bottom); the outer
//          loop is a rotated `while (!IsActiveThreadTerminating())`.
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
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;
namespace NScript {
class CPersonalScript_EmptyScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CTrappedMan;                        // the entity class below (naming wave 11)
};
}
using NScript::CPersonalScript_EmptyScript;
struct CEmptyData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CPersonalScript_EmptyScript::CTrappedMan : public CEntityScriptBase {
public:
        CPersonalScript_EmptyScript* ParentClass;      // +0x14
    CEmptyData*                  PMasterData;      // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CPersonalScript_EmptyScript::CTrappedMan::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->AddScreenMessage("You hit the man on the hill!", (ETextGroupSelectionMethod)0);
        if (Game->GetHealth(Me) < 1e-4f) {
            if (IsActiveThreadTerminating())
                return;
            Game->AddScreenMessage("You hit him to the point of death!", (ETextGroupSelectionMethod)0);
        }
        Game->NewScriptFrame();
    }
}