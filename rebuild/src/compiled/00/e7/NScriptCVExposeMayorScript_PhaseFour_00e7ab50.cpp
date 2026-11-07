// NScript::CV_ExposeMayorScript::PhaseFour() -- stage 3 of the Expose-the-Mayor quest: wait for the HauntedHouseCellar level, play the ghost cut-scene once, set objective 4 and wait until the hero holds the evidence; then either (hero married Lady Grey) play the married cut-scene, take the evidence and drop the quest card (bribe path), or play the choice cut-scene and ask the yes / no question: yes takes the evidence (thunder-dead or plain agree cut-scene) on the bribe path, no plays the refuse cut-scene on the expose path; finally stage 4
// Leaf: ?PhaseFour@CV_ExposeMayorScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7AB50, 1275 bytes, thiscall, plain `ret`, no EH frame.
// Identity (two sources): Ego_r.pdb name set ?PhaseFour@CV_ExposeMayorScript@NScript@@QAEXXZ; the landed DoMission 0x00E7A410
//   calls it (`call 0x00E7AB50`) between PhaseThree 0x00E7A9F0 and PhaseFive 0x00E7A740, and the body tests StageNumber == 3
//   and ends with StageNumber = 4 (the quest's STAGE_IN_PATH sequence).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; CV_ExposeMayorScript::PlayCutscene(CCharString, bool, CCharString) 0x00E78100; the implicit
//   CScriptGameResourceObjectMovieBase ctor 0x006E7B60 / dtor 0x006E7B80 (COMDAT calls); Game (+0x40) slots 7 NewScriptFrame,
//   11 IsLevelLoaded, 39 MsgIsQuestionAnsweredYesOrNo, 70 GetHero, 114 GiveHeroYesNoQuestion, 125 TakeObjectFromHero,
//   184 IsObjectInThingsPossession, 294 RemoveQuestCardFromHero, 296 SetQuestCardObjective, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities (the inline CWideScreenMagicPauseEntities, its Game pointer kept in edi), 655 GetActiveQuestName.
// Layout (engine/CV_ExposeMayorScript.h, Ego_r.pdb - 0x14): Game +0x40, MasterScriptData +0x44, Path +0x48, StageNumber +0x4c,
//   MayorExposeGhostCutscenePlayed +0x55, Evidence +0x98; CQ_SunnyvaleMasterData DefeatedThunder +0x61, KilledThunder +0x63,
//   HeroMarriedLadyGrey +0xd7.
// Shape: the translator's terminate check opens every block; the two cut-scene arms each own a movie + pause pair whose
//   tails VC7.1 cross-jumps; both answer arms start with a terminate check, which VC7.1 hoists above the `answer == 1` test.
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_ExposeMayorScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"

void NScript::CV_ExposeMayorScript::PhaseFour()
{
    if (StageNumber == 3) {
        if (IsActiveThreadTerminating()) return;
        while (!Game->IsLevelLoaded("HauntedHouseCellar")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (!MayorExposeGhostCutscenePlayed) {
            if (IsActiveThreadTerminating()) return;
            PlayCutscene("CS_MAYOREXPOSE_GHOST", true, "NULL");
            MayorExposeGhostCutscenePlayed = true;
        }
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_04", "HauntedHouseCellar", "");
        while (!Game->IsObjectInThingsPossession(Evidence, Game->GetHero())) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (MasterScriptData->HeroMarriedLadyGrey) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            PlayCutscene("CS_MAYOREXPOSE_GREYMARRIED", false, "NULL");
            Game->TakeObjectFromHero(Evidence);
            Game->RemoveQuestCardFromHero(Game->GetActiveQuestName());
            Path = (PATH_ID)2;
        } else {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            PlayCutscene("CS_MAYOREXPOSE_GREYCHOICE", false, "NULL");
            Game->GiveHeroYesNoQuestion("TEXT_QST_B18_LADYGREY_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long answer = Game->MsgIsQuestionAnsweredYesOrNo();
            while (answer < 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                answer = Game->MsgIsQuestionAnsweredYesOrNo();
            }
            if (IsActiveThreadTerminating()) return;
            if (answer == 1) {
                if (IsActiveThreadTerminating()) return;
                if (MasterScriptData->KilledThunder || MasterScriptData->DefeatedThunder) {
                    if (IsActiveThreadTerminating()) return;
                    PlayCutscene("CS_MAYOREXPOSE_GREYAGREE_THUNDERDEAD", false, "NULL");
                } else {
                    if (IsActiveThreadTerminating()) return;
                    PlayCutscene("CS_MAYOREXPOSE_GREYAGREE", false, "NULL");
                }
                Game->TakeObjectFromHero(Evidence);
                Path = (PATH_ID)2;
            } else {
                if (IsActiveThreadTerminating()) return;
                PlayCutscene("CS_MAYOREXPOSE_GREYREFUSE", false, "NULL");
                Path = (PATH_ID)1;
            }
        }
    }
    StageNumber = (STAGE_IN_PATH)4;
}