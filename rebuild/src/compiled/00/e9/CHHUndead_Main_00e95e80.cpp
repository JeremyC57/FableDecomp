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
class CScriptThing { public:                              // 12 bytes
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    ~CScriptThing();                               // 0x004AA840
};
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
class CIsThingAlive : public NScript::CPredicateBase { public:                             // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                              // 0x00E241B0 (out of line in this module)
};
}
using NScript::CIsThingAlive;
struct CV_HauntedHouseScript {
    unsigned char _pad[0x50];
    bool          m_raiseUndead;                   // +0x50
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CHHUndead : public CEntityScriptBase {
public:
    void*                  __vftable;              // +0x00
    CGameScriptInterface*  Game;                   // +0x04
    CScriptThing           Me;                     // +0x08
    CV_HauntedHouseScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    float                  RaiseUndeadBarAmount;   // +0x1c
    void Main();
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