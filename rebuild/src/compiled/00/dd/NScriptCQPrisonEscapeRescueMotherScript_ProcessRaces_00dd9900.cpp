// NScript::CQ_PrisonEscapeRescueMotherScript::ProcessRaces() -- unless the mother-rescued checkpoint is set, loop until the thread ends:
// wait for RaceStarting, clear it, activate Q_PrisonRace, wait until it is no longer active, then ResetCells
// Leaf: ?ProcessRaces@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD9900.
// Identity: CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0 spawns it as thread "ProcessRaces" (member-pointer relocation, landed
//   RELOCATION_MATCH, binds this spelling); Ego_r.pdb + FableWin.pdb ?ProcessRaces@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ;
//   reads RaceStarting +0x6b / MotherRescuedFromPrisonAutoSaveCheckPoint +0x7a and calls ResetCells 0x00DD9A50 (members of
//   engine/CQ_PrisonEscapeRescueMotherScript.h). Was: ?EndMission@CV_ChapelOfEvilScript@@QAEXXZ (a label propagated across bodies).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots NewScriptFrame / ActivateQuest / IsQuestActive;
//   ResetCells 0x00DD9A50; CCharString 0x0099EBF0 / ~ 0x0099EAE0.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
#include "engine/CGameScriptInterface.h"

void NScript::CQ_PrisonEscapeRescueMotherScript::ProcessRaces()
{
    if (MotherRescuedFromPrisonAutoSaveCheckPoint) {
        if (IsActiveThreadTerminating())
            return;
        return;
    }
    while (!IsActiveThreadTerminating()) {
        while (!RaceStarting) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        RaceStarting = false;
        Game->ActivateQuest("Q_PrisonRace");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!(!Game->IsQuestActive("Q_PrisonRace"))) {   // WAIT_UNTIL(!active)
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ResetCells();
        Game->NewScriptFrame();
    }
}