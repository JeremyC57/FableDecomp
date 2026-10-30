// NScript::CQ_TraderEscortScript::CTE_BanditLeader::Main() -- bandit leader entity script: register an alive filter on Me, wait until the hero is within 15 of Me, play the TEXT_QST_067_BANDIT_ATTACK line (Me to the hero), make the hero Me's best enemy target, then idle until done
// Identity (naming wave 11): ?Main@CTE_BanditLeader@CQ_TraderEscortScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012DF5E0 slot 1 is stored by the factory 0x00E03500 registered as "TE_BanditLeader"
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
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
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

// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_TraderEscortScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CTE_BanditLeader;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_TraderEscortScript;
namespace NScript {
class CQ_TraderEscortScript::CTE_BanditLeader : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_TraderEscortScript::CTE_BanditLeader::Main()
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