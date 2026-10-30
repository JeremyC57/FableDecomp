// CProximityPodium::Main() -- entity script: register an alive predicate over the podium, then each time the hero comes within 2 units switch the podium's activation trigger on for ten seconds and off again, until the script is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFB360, length 256 bytes, plain `ret` (entity-script vtable slot at
// 0x012F6B24).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (the predicate's member), CEntityScriptBase::SetPredicate
// 0x00F35B10 and IsActiveThreadTerminating 0x00F35B30 (both on `this`; manifest labels propagated), ~CIsThingAlive
// 0x00EF7AB0 (out of line), IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float)
// (`__fastcall`, 0x00CBE2FF), CScriptThing::SetActivationTriggerStatus (virtual slot 69, +0x114); through the entity's
// game script interface (+4): NewScriptFrame (slot 7, +0x1c), GetHero (slot 70, +0x118), Pause(float) (slot 376, +0x5e0).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}.
// Shape: the landed CHTE_Prisoner::Main model (0x00D69F50): the predicate is a scoped frame object; `if (done) return;`
// then `while (!done) { while (!near) { yield; if (done) return; } if (done) return; ...; yield; }` -- both loops
// rotated, the inner test duplicated.
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace N_00ef7ab0 { struct CIsThingAlive {                                 // vtable 0x012C2FE8: a predicate holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                  // 0x00EF7AB0
}; } using N_00ef7ab0::CIsThingAlive;
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CProximityPodium {
public:
    void*                 __vftable;                 // +0x00
    CGameScriptInterface* Game;                      // +0x04
    CScriptThing          Me;                        // +0x08
    void SetPredicate(CIsThingAlive& predicate);     // 0x00F35B10
    bool IsActiveThreadTerminating();                // 0x00F35B30
    void Main();
};

void CProximityPodium::Main()
{
    {
        CIsThingAlive predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Me.SetActivationTriggerStatus(true);
        Game->Pause(10.0f);
        Me.SetActivationTriggerStatus(false);
        Game->NewScriptFrame();
    }
}