// CDBSummoner::Main() -- the summoner entity script: register an alive filter over Me, update once, give Me the hero as best enemy target, wait frame by frame until the parent quest's state word (+0x48) reaches 4, then unless the script is done remove Me (RemoveThing(Me, true, true))
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D25D80, 176 bytes (entity-script vtable slot 1 = Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member; filter vtable 0x012C32F0), the filter registration
//          @ 0x00F35B10 (on `this`), ~CScriptThing @ 0x004AA840, IsDone @ 0x00F35B30, CGameScriptInterface slots 7 (+0x1c, frame
//          update), 70 (+0x118, GetHero 0x00891CA0), 451 (+0x70c, GiveThingBestEnemyTarget 0x008A0260) and 108 (+0x1b0, RemoveThing).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14; the parent's state word at +0x48 (descriptive).
// Shape: the landed CSummonerToKill::Main (0x00D83DC0) model: a scoped filter object (its vtable constant kept in ebx for both
//          stores), the nested `Game->GiveThingBestEnemyTarget(Me, Game->GetHero())` (the outer vtable fetched before the inner
//          call), then a frame loop on the parent's state that bails out when the script is done (the constant 4 in ebx).
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                              // 12 bytes
    unsigned char _body[12 - 4];
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
struct CDBSummonerParent {                         // the owning quest script (descriptive)
    unsigned char _body[0x48];
    long          m_state;                         // +0x48
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CDBSummoner : public CEntityScriptBase {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    CDBSummonerParent*    ParentClass;             // +0x14
    void*                 PMasterData;             // +0x18
    void Main();
};

void CDBSummoner::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (ParentClass->m_state != 4) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, true, true);
}