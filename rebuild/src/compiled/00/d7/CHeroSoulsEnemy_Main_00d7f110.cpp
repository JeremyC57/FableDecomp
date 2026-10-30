// CHeroSoulsEnemy::Main() -- Hero Souls arena enemy script: register an alive-filter over the script's thing (Me), start a script frame, and unless the script is already done pause a second, set the hero as Me's best enemy target and keep running script frames until done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D7F110, length 142 bytes.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, the filter registration @ 0x00F35B10,
// CScriptThing::~CScriptThing @ 0x004AA840, IsDone @ 0x00F35B30; CGameScriptInterface (vtable 0x01260F0C) slots
// 7 NewScriptFrame, 376 Pause(1.0f), 70 GetHero, 451 GiveThingBestEnemyTarget (names from rtti-vtables.tsv + manifest).
// Layout = Ego_r.pdb CHeroSoulsEnemy (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
// ParentClass +0x14, PMasterData +0x18.
// Shape: as the landed CHTE_Prisoner::Main 0x00D69F50 (scoped filter, `do/while` wait); the target call is the
// nested `Game->GiveThingBestEnemyTarget(Me, Game->GetHero())` (the outer vtable fetched before the inner call).
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
class CIsThingAlive : public NScript::CPredicateBase { public:                             // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
struct CQ_HeroSoulsArenaScript;
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CHeroSoulsEnemy : public CEntityScriptBase {
public:
    void*                    __vftable;            // +0x00
    CGameScriptInterface*    Game;                 // +0x04
    CScriptThing             Me;                   // +0x08
    CQ_HeroSoulsArenaScript* ParentClass;          // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;          // +0x18
    void Main();
};

void CHeroSoulsEnemy::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (!IsActiveThreadTerminating()) {
        Game->Pause(1.0f);
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        do {
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
}