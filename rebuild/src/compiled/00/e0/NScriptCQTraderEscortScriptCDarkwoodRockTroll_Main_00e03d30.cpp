// NScript::CQ_TraderEscortScript::CDarkwoodRockTroll::Main() -- the Darkwood rock troll entity script: alive filter on Me, sic the troll on the hero, wait (a script frame at a time) until its health falls to zero, then set the quest card objective to TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03 at BarrowFields and clear the quest's ShownEarthTroll flag
// Leaf: ?Main@CDarkwoodRockTroll@CQ_TraderEscortScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E03D30, 335 bytes, plain `ret` (entity-script vtable slot 1, Main).
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CDarkwoodRockTroll@CQ_TraderEscortScript@NScript@@EAEXXZ (private virtual; the PDB
//   also lists the ctor, Init, OnPredicateFail, GetParentScript and Alloc of the nested entity class).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840 (the NScript::CIsThingAlive predicate,
//   vtable 0x012C32F0, engine/CIsThingAlive.h), CEntityScriptBase::SetPredicate 0x00F35B10 and IsActiveThreadTerminating
//   0x00F35B30 (engine/CEntityScriptBase.h), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; through Game
//   (engine/CGameScriptInterface.h): slots 7 NewScriptFrame, 70 GetHero, 264 GetHealth, 296 SetQuestCardObjective, 451
//   GiveThingBestEnemyTarget, 655 GetActiveQuestName.
// Strings: "BarrowFields" 0x012C5D94 (twice), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03" 0x012DF66C. Float pool 0.0f 0x0122DEDC.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}; Ego_r.pdb CDarkwoodRockTroll ParentClass +0x14, PMasterData +0x18,
//   HealthBarID +0x1C. CQ_TraderEscortScript has no shared header; its ShownEarthTroll (Ego_r.pdb +0x85) is retail +0x55, as the
//   landed CDarkwoodRockTroll::Init 0x00E03BF0 (which sets it) and OnPredicateFail 0x00E03B00 (which tests it) use it.
// Shape: the scoped predicate object of the landed entity-script Mains (CUndeadSoldier::Main 0x00D88F90); the health wait is
//   `while (!(GetHealth(Me) <= 0.0f))` (`test ah,41h ; jnp` out / `jp` back); the objective line is the landed OnPredicateFail
//   statement: four implicit CCharString temporaries built last-argument first, GetActiveQuestName's result in its own slot.
//   What fixed the old residue (the outer vtable fetched into edi before the pushes): the real slot signatures from the shared
//   header -- slot 70 is GetHero (its CScriptThing& feeds slot 451 GiveThingBestEnemyTarget(Me, hero)) and slot 296 takes
//   four strings, the first being GetActiveQuestName(); the old local interface modelled both calls with the wrong arity.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CIsThingAlive.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
using NScript::CIsThingAlive;
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderEscortScript {                          // retail offsets; only the flag this row writes
public:
    unsigned char _pad0[0x55];
    bool          ShownEarthTroll;                      // +0x55 (Ego_r.pdb +0x85)
    class CDarkwoodRockTroll;
};
class CQ_TraderEscortScript::CDarkwoodRockTroll : public CEntityScriptBase {
private:
    virtual void Main();
public:
    CQ_TraderEscortScript*  ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    long                    HealthBarID;                // +0x1c
};

void CQ_TraderEscortScript::CDarkwoodRockTroll::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (!(Game->GetHealth(Me) <= 0.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields");
    ParentClass->ShownEarthTroll = false;
}
}