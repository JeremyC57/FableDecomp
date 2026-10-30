// NScript::CQ_EndGameBossBattleScript::CEndGameJackOfBladesBoss::Main() -- the final boss entity script: watch that Jack stays alive, mark him on the mini-map with the quest-core orb, wait for the boss battle script to flag the fight ready, then point Jack at the hero and idle until the script is done
// Leaf: Main
// Identity (naming wave 11): ?Main@CEndGameJackOfBladesBoss@CQ_EndGameBossBattleScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012CC6B4 slot 1 is stored by the factory 0x00D2FA40 registered as "EndGameJackOfBladesBoss"
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
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
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
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;
namespace NScript {
class CQ_EndGameBossBattleScript {
public:
    unsigned char _pad[0x48];
    bool JackReadyToFight;                         // +0x48
public:
    class CEndGameJackOfBladesBoss;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_EndGameBossBattleScript;
struct CQ_SunnyvaleMasterData;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_EndGameBossBattleScript::CEndGameJackOfBladesBoss : public CEntityScriptBase {
public:
        CQ_EndGameBossBattleScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;       // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_EndGameBossBattleScript::CEndGameJackOfBladesBoss::Main()
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