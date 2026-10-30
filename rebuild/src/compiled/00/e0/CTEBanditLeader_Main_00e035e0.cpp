// CTE_BanditLeader::Main() -- bandit leader entity script: register an alive filter on Me, wait until the hero is within 15 of Me, play the TEXT_QST_067_BANDIT_ATTACK line (Me to the hero), make the hero Me's best enemy target, then idle until done
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E035E0, length 336 bytes (plain `ret`; entity-script slot 1 = Main).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840 (the alive filter,
//   vtable 0x012C32F0, inline ctor and dtor), the filter registration @ 0x00F35B10 (manifest "CWorldMap::
//   AllocateMapThingUID" is propagated), IsDone @ 0x00F35B30 (manifest "CCreatureAction_TrollWhackGroundBase::Initialise"
//   is propagated), IsDistanceBetweenThingsUnder @ 0x00CBE2FF (__fastcall, distance pushed before the GetHero call),
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, and the game interface slots
//   (ghidra_out/cgsi_slots_aligned.tsv) 7 NewScriptFrame, 70 GetHero, 364 AddNewConversation, 365
//   AddPersonToConversation, 366 AddLineToConversation, 451 GiveThingBestEnemyTarget.
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8} (landed CHTE_Prisoner::Main 0x00D69F50).
// Shape: the filter (vtable kept in ebx for its inline dtor) and the line's string are scoped frame objects sharing one
//   slot; the conversation id stays in ebx; the wait is `while (!test) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }`.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                          // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
    virtual ~CScriptThing();                          // 0x004AA840
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                           // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                         // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                     // +0x04
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

class CTE_BanditLeader : public CEntityScriptBase {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    void Main();
};

void CTE_BanditLeader::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    {
        CCharString text("TEXT_QST_067_BANDIT_ATTACK");
        Game->AddLineToConversation(conversation, text, false, Me, Game->GetHero());
    }
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}