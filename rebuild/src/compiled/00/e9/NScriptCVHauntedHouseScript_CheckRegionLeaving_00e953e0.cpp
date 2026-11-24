// CV_HauntedHouseScript::CheckRegionLeaving() -- while the Haunted House is loaded keep its creature generators off and the cellar door unusable; when the hero leaves, drop the undead counter from the quest display
// Leaf: ?CheckRegionLeaving@CV_HauntedHouseScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E953E0, 457 bytes (464 with padding), plain `ret`.
// Label: the manifest's "CV_ExposeMayorScript::DoMission" is propagated. This body is a CSpawnedFunc target stored at
//   0x00E94BF7 (its only reference), works on "HauntedHouse"/"HH_CellarDoor" and passes the script's GUI counter
//   (+0x4c) to RemoveQuestInfoElement -- Ego_r.pdb's CV_HauntedHouseScript has GuiCounter at 0x60 (- 0x14 = +0x4c)
//   and one spawned watcher, CheckRegionLeaving.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (out of line),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7),
//   IsRegionLoaded (slot 12), GetThingWithScriptName (slot 72, hidden result), RemoveQuestInfoElement (slot 338),
//   SetThingAsUsable (slot 447), SetCreatureGeneratorsEnabled (slot 558).
// Strings: "HauntedHouse" 0x012E21F0, "HH_CellarDoor" 0x012E97A0.
// Shape: the outer `while (!IsActiveThreadTerminating())` watcher loop (as 0x00D8EE50) around two WAIT_UNTIL waits:
//   `while (!x)` gives the neg/sbb/inc bool, `while (!(!x))` the setne one; every string is an implicit conversion.
// Layout: Ego_r.pdb CV_HauntedHouseScript - 0x14: UndeadBar +0x48, GuiCounter +0x4c; interface pointer +0x40.
//   Local model: CGameScriptInterface.h carries no virtual slots.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// NF336 (2026-11-24): moved off the row-local class model onto the shared engine headers (acctB Gate 0 prerequisite). The family
//   preamble below is shared by the CV_HauntedHouseScript object-mate rows (a per-row departure is marked where it occurs).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_DECLS
#define FABLE_RETAIL_CM_UNDEADHERE_MAIN
#define FABLE_RETAIL_CM_QUESTCONTROL_MAIN_PASS26
#define FABLE_RETAIL_CHHUNDEAD_MEMBERS
#define FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CV_HauntedHouseScript.h"
#include "engine/CGameScriptInterfaceBase.h"
// Callee respelled by the move: IsActiveThreadTerminating is the base member ?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ 0x00CB7940
//   (Ego_r.pdb + FableWin.pdb; the row-local model declared it on CV_HauntedHouseScript). Game is CLevelScriptBase's reference +0x40.
void NScript::CV_HauntedHouseScript::CheckRegionLeaving()
{
    while (!IsActiveThreadTerminating()) {
        Game.SetCreatureGeneratorsEnabled("HauntedHouse", false);
        while (!Game.IsRegionLoaded("HauntedHouse")) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptThing door = Game.GetThingWithScriptName("HH_CellarDoor");
            Game.SetThingAsUsable(door, false);
            while (!(!Game.IsRegionLoaded("HauntedHouse"))) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game.RemoveQuestInfoElement(GuiCounter);
        }
        Game.NewScriptFrame();
    }
}
