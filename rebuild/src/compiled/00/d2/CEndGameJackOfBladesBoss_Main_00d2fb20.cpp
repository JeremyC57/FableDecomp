// CEndGameJackOfBladesBoss::Main() -- the final boss entity script: watch that Jack stays alive, mark him on the mini-map with the quest-core orb, wait for the boss battle script to flag the fight ready, then point Jack at the hero and idle until the script is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D2FB20, length 222 bytes (+ int3 padding), `ret`.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 and ~CScriptThing @ 0x004AA840 (the CIsThingAlive
// filter, vtable 0x012C32F0), the filter registration @ 0x00F35B10 and IsDone @ 0x00F35B30 on `this` (labels propagated;
// the landed CHTE_Prisoner::Main 0x00D69F50 names them), CCharString::CCharString(const char*, long) @ 0x0099EBF0
// ("HUD_ORB_QUEST_CORE" @ 0x01258AB0) / ~CCharString @ 0x0099EAE0, CGameScriptInterface slots 7 NewScriptFrame (+0x1c),
// 350 MiniMapAddMarker (+0x578), 70 GetHero (+0x118) and 451 GiveThingBestEnemyTarget (+0x70c).
// Layout = Ego_r.pdb CEndGameJackOfBladesBoss (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
// ParentClass (CQ_EndGameBossBattleScript*) +0x14, PMasterData +0x18; the parent's "fight ready" byte at +0x48 (unnamed
// in the PDB layout, named descriptively).
// Shape: the landed Prisoner Main prologue; the marker name is an implicit-conversion temporary; the readiness wait is
// `while (flag != true) { yield; if (IsActiveThreadTerminating()) return; }` (the constant 1 hoisted into bl); the target call nests the
// GetHero call (both vtable loads up front); the idle wait is a bottom-tested do/while.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
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
class CIsThingAlive : public NScript::CPredicateBase { public:                             // vtable 0x012C32F0
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
struct CQ_EndGameBossBattleScript {
    unsigned char _pad[0x48];
    bool JackReadyToFight;                         // +0x48
};
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CEndGameJackOfBladesBoss : public CEntityScriptBase {
public:
    void*                       __vftable;         // +0x00
    CGameScriptInterface*       Game;              // +0x04
    CScriptThing                Me;                // +0x08
    CQ_EndGameBossBattleScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;       // +0x18
    void Main();
};

void CEndGameJackOfBladesBoss::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    while (ParentClass->JackReadyToFight != true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}