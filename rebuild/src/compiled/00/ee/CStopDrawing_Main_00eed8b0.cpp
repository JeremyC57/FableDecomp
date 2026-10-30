// NScript::CPersonalScript_alextestScript::CStopDrawing::Main() -- alextest script: until done, hide Me (EntitySetAsDrawable false) once the hero talks to it, hits it, or hits it with any special ability other than ability 14
// Leaf: Main
// Identity (naming wave 11): ?Main@CStopDrawing@CPersonalScript_alextestScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F58AC slot 1 is stored by the factory 0x00EED7D0 registered as "StopDrawing"
// Retail 0x00EED8B0, 391 bytes. Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (filter member), RegisterFilter
// 0x00F35B10, ~CIsThingAlive 0x00E241B0 (out of line in this module), IsDone 0x00F35B30, CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0 ("SCRIPT_NAME_HERO" 0x0125D1C8); CScriptThing (vtable 0x01238C8C) slots 21
// MsgIsHitBy, 27 MsgIsTalkedToBy, 41 MsgIsHitBySpecialAbilityFrom(long, name), 42 MsgIsHitByAnySpecialAbilityFrom
// (names from the slot targets 0x004AAB30/0x004AABD0/0x004AAD20/0x004AAD40); CGameScriptInterface slots 7
// NewScriptFrame and 511 EntitySetAsDrawable (rtti-vtables.tsv).
// Layout: Ego_r.pdb CStopDrawing (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
// PMasterData +0x18. Filter model as the landed CTH_Trigger_Oakvale::Main 0x00EE0A20.
// Shape: the four hero-name strings are implicit-conversion temporaries of one `||`/`&&` chain (VC's construction
// flags in ebx, `or ebx,N` per temporary, destroyed in reverse after the chain lands in a frame bool); a plain `while`
// after the first IsDone return gives retail's rotated loop (test duplicated at entry and bottom).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
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
extern const char g_scriptNameHero[];              // 0x0125D1C8 "SCRIPT_NAME_HERO"
namespace NScript {
class CPersonalScript_alextestScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CStopDrawing;                        // the entity class below (naming wave 11)
};
}
using NScript::CPersonalScript_alextestScript;
class CalextestData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CPersonalScript_alextestScript::CStopDrawing : public CEntityScriptBase {
public:
        CPersonalScript_alextestScript* ParentClass;   // +0x14
    CalextestData*         PMasterData;            // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CPersonalScript_alextestScript::CStopDrawing::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        bool touched = Me.MsgIsTalkedToBy(g_scriptNameHero) || Me.MsgIsHitBy(g_scriptNameHero)
                    || (Me.MsgIsHitByAnySpecialAbilityFrom(g_scriptNameHero)
                        && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)0xe, g_scriptNameHero));
        if (touched) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetAsDrawable(Me, false);
        }
        Game->NewScriptFrame();
    }
}