// CHHUndead::Main() -- haunted-house undead script: register an alive-filter over Me, start a script frame; unless done, when the parent script's flag (+0x50) is set remove Me (unless done by then), then run script frames until done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E95E80, length 137 bytes.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, the filter registration @ 0x00F35B10,
// the filter's out-of-line destructor @ 0x00E241B0, IsDone @ 0x00F35B30; CGameScriptInterface slots 7 NewScriptFrame and
// 108 RemoveThing(const CScriptThing&, bool, bool) (called with true, true).
// Layout = Ego_r.pdb CHHUndead (32 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14
// (CV_HauntedHouseScript*, the tested byte at +0x50 is unnamed in the PDB), PMasterData +0x18, RaiseUndeadBarAmount +0x1c.
// Shape: as the landed CHTE_Prisoner::Main 0x00D69F50; the early exits are plain returns (the second IsDone test
// jumps straight to the epilogue), the wait loop a `do/while`.
namespace NScript { class CPredicateBase; }
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
struct CV_HauntedHouseScript {
    unsigned char _pad[0x50];
    bool          m_raiseUndead;                   // +0x50
};
struct CQ_SunnyvaleMasterData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

class CHHUndead : public CEntityScriptBase {
public:
        CV_HauntedHouseScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    float                  RaiseUndeadBarAmount;   // +0x1c
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};

void CHHUndead::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (ParentClass->m_raiseUndead) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, true, true);
    }
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}