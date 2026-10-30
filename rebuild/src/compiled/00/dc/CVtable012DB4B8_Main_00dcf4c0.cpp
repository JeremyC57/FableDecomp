// CVtable_012DB4B8::Main() -- Opening Graveyard entity script: register an alive-filter over Me, let the interface update once, and if the script is not done and the parent quest has started the undead cut-scene, remove Me (fading) through the interface
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCF4C0, length 119 bytes (padded to 128).
// Label correction: the catalog's "CQ_OpeningGraveyardSecretPassageScript::FadeObjectUp" is not a quest-script member:
// this body is slot 1 (Main, entity-script contract [dtor, Main, Init, ...]) of vtable 0x012DB4B8, stored by the factory
// 0x00DCF3D0 (`new` of 0x1c bytes: Game +4, Me +8 copied from the argument, ParentClass +0x14, PMasterData +0x18).
// No RTTI locator and no registered name in the factory, so the class is named by its vtable (the landed convention).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member at +4; filter vtable
// 0x012C32F0), the filter registration @ 0x00F35B10 (on `this`), CScriptThing::~CScriptThing @ 0x004AA840, IsDone
// @ 0x00F35B30 (x2), the game interface's virtual slots 7 (+0x1c) and 108 (+0x1b0, RemoveThing(thing, true, true)).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14 (the parent's byte at +0x50 is
// CQ_OpeningGraveyardSecretPassageScript::UndeadCutsceneStarted in the retail layout), PMasterData +0x18.
// Shape (landed CHTE_Prisoner::Main 0x00D69F50): the filter is a scoped 0x10-byte frame object, the address of Me stays
// in edi; the three-term condition is one `&&` chain re-testing IsDone after the parent flag.
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
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
struct CQ_OpeningGraveyardSecretPassageScript {
    unsigned char _pad[0x50];
    bool          UndeadCutsceneStarted;           // +0x50
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CVtable_012DB4B8 : public CEntityScriptBase {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    CQ_OpeningGraveyardSecretPassageScript* ParentClass;   // +0x14
    void*                 PMasterData;             // +0x18
    void Main();
};
void CVtable_012DB4B8::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (!IsActiveThreadTerminating() && ParentClass->UndeadCutsceneStarted && !IsActiveThreadTerminating())
        Game->RemoveThing(Me, true, true);
}