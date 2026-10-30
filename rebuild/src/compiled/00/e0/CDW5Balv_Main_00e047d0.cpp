// CDW5_Balv::Main() -- the balverine entity script: register an alive filter over Me, update once, wait frame by frame until Me is aware of the hero, give Me the hero as best enemy target, then idle frame by frame until the script is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E047D0, 178 bytes (+ int3 padding; entity-script vtable slot 1 = Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member; filter vtable 0x012C32F0), the filter registration
//          @ 0x00F35B10 (on `this`), ~CScriptThing @ 0x004AA840, IsDone @ 0x00F35B30, CScriptThing::IsAwareOfHero (virtual slot 16,
//          +0x40; 0x004AAAC0), CGameScriptInterface slots 7 (+0x1c, frame update), 70 (+0x118, GetHero 0x00891CA0) and 451 (+0x70c,
//          GiveThingBestEnemyTarget 0x008A0260).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8 (polymorphic, 12 bytes)}, ParentClass +0x14.
// Shape: the landed CSummonerToKill::Main (0x00D83DC0) model: a scoped filter object (its vtable constant kept in ebx for both
//          stores), a frame loop on Me's virtual awareness test that bails out when the script is done, the nested
//          `Game->GiveThingBestEnemyTarget(Me, Game->GetHero())` (the outer vtable fetched before the inner call) and a final
//          update-until-done loop.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                              // 12 bytes: vptr + the counted implementation
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual bool IsAwareOfHero() const;            // slot 16 (+0x40)
    unsigned char _body[8];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    virtual ~CScriptThing();                               // 0x004AA840
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
class CIsThingAlive : public NScript::CPredicateBase { public:                              // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CDW5_Balv : public CEntityScriptBase {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    void*                 ParentClass;             // +0x14
    void*                 PMasterData;             // +0x18
    void Main();
};

void CDW5_Balv::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!Me.IsAwareOfHero()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}