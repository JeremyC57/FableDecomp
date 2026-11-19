// NScript::CQ_HerosOldHouseScript::WatchForBooty() -- watch the hidden booty's digging spot: every script frame, while the hero is away from Oak Bay wait for the region to load again; once the spot is no longer enabled (dug up) point the active quest's card at the second objective (Oak Bay) and stop
// Leaf: ?WatchForBooty@CQ_HerosOldHouseScript@NScript@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D8A240, length 610 bytes, plain `ret`.
// Shape: FableWin.pdb ?WatchForBooty@CQ_HerosOldHouseScript@NScript@@QAEXXZ (FableWin 0x037BA020): the booty fetched by script name, then
//   one loop with the new frame at its step and the terminating test first in the body; inside it the Oak Bay wait (test, terminating
//   test, wait loop with the per-frame terminating test, terminating test after it), then the digging-spot test with the objective
//   update and the return. Every return goes through TRetVoid::Return() (FableWin calls it before each ~CScriptThing; it emits no
//   code in retail). The local name booty is descriptive (no Ego_d.pdb record).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940,
//   ~CScriptThing 0x004AA840 (out of line at the deeper returns; expanded at the others: CCountedPointer release, operator delete
//   0x00BFE9BC, ~CBaseClass 0x0099A2E0); through Game (+0x40): NewScriptFrame (7), IsRegionLoaded (12), GetThingWithScriptName (72),
//   SetQuestCardObjective (296), IsDiggingSpotEnabled (576), GetActiveQuestName (655).
// Strings: "HiddenBooty" 0x012D50B4, "OakBay" 0x012D31E4, "" 0x0122D70E, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_02" 0x012D508C.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_HEROSOLDHOUSESCRIPT_WATCHFORBOOTY
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_HerosOldHouseScript.h"
#include "engine/TRetVoid.h"
// Retail expands the destructor at the outer returns and calls 0x004AA840 at the inner ones: the inline body, VC7.1 decides per site.
inline CScriptThing::~CScriptThing() { }

void NScript::CQ_HerosOldHouseScript::WatchForBooty()
{
    CScriptThing booty = Game->GetThingWithScriptName("HiddenBooty");
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return TRetVoid().Return();
        if (!Game->IsRegionLoaded("OakBay")) {
            if (IsActiveThreadTerminating()) return TRetVoid().Return();
            while (!Game->IsRegionLoaded("OakBay")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return TRetVoid().Return();
            }
            if (IsActiveThreadTerminating()) return TRetVoid().Return();
        }
        if (!Game->IsDiggingSpotEnabled(booty)) {
            if (IsActiveThreadTerminating()) return TRetVoid().Return();
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_02", "", "OakBay");
            return;
        }
    }
}
