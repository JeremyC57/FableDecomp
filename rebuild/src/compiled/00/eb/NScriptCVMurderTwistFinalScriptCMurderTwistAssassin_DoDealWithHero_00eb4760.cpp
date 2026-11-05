// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::DoDealWithHero() -- the assassin attacked by the hero: in a paused movie sequence he complains, offers the hero a bribe (yes/no); taking it pays 2000 gold, then the morality hit and the kill-the-sheriff objective (AssassinState 3); refusing brings a second offer of 2500 gold, whose refusal turns him hostile (AssassinState 2)
// Leaf: ?DoDealWithHero@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB4760, 2075 bytes, thiscall, plain `ret` (private, non-virtual: AAE).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?DoDealWithHero@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@AAEXXZ;
//   its only caller is CMurderTwistAssassin::DoWatchingForTrouble 0x00EB40C0 (call at 0x00EB46F4, after
//   WaitForEnflameReactionIfNecessary 0x00EB0700), donor member order DoWatchingForTrouble / IsFeelingThreatened /
//   DoDealWithHero / DoRemindingHeroToKillSheriff = retail 0x00EB40C0 / 0x00EB4710 / 0x00EB4760 / 0x00EB4F80, and every text
//   literal is a TEXT_QST_B07_ASSASSIN_* line. The ledger's `NScript::CMurderTwistAssassin` lacked the enclosing script.
// Callees: CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430 (the expanded MovieBase ctor/dtor), CCountedPointer<MovieBase>
//   dtor 0x006E7AB0 (out of line), MovieBase dtor COMDAT 0x006E7B80, ~CScriptThing 0x004AA840, CCharString 0x0099EBF0 / ~
//   0x0099EAE0, CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; handle slots 12 GetScriptThing (+0x30), 13 Speak
//   (+0x34), 26 IsPerformingScriptTask (+0x68); CGameScriptInterfaceBase slots 7 NewScriptFrame, 39 MsgIsQuestionAnsweredYesOrNo,
//   70 GetHero, 114 GiveHeroYesNoQuestion, 126 GiveHeroGold, 156 GiveHeroMorality, 264 GetHealth, 296 SetQuestCardObjective,
//   370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 655 GetActiveQuestName; pGlobalScriptDef 0x0143E90C
//   (HeroTakesAssassinGoldMorality +0x70C), 0.0f 0x0122DEDC.
// Layout: hdr/engine/CV_MurderTwistFinalScript.h opt-in FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN (ParentClass +0x14,
//   MyEntityHandle +0x1C); parent AssassinState +0x4C.
// Shape: the chunk 746 sheriff dialogue model: shared MovieBase with the out-of-line CCountedPointer<MovieBase> dtor
//   declaration, the RAII pause guard, `return _Fail_Val.Return();` at every terminate exit; both accepted arms end in the same
//   AssassinState 3 tail (VC7.1 tail-merges them).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN
#include "engine/CV_MurderTwistFinalScript.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRetVoid.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline CScriptThing::~CScriptThing() { }
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };

void NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::DoDealWithHero()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectMovieBase speech_movie;
    Game->StartMovieSequence("", speech_movie);
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
    if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
        MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_ASSASSIN_ATTACKED", GROUP_SELECT_NONE, false, true, false);
        while (MyEntityHandle.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    Game->GiveHeroYesNoQuestion("TEXT_QST_B07_ASSASSIN_BRIBE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
    long answer = Game->MsgIsQuestionAnsweredYesOrNo();
    while (answer < 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        answer = Game->MsgIsQuestionAnsweredYesOrNo();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (answer == 1) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_ASSASSIN_BRIBE_ACCEPTED", GROUP_SELECT_NONE, false, true, false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        Game->GiveHeroGold(2000);
        if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_ASSASSIN_BRIBE_ACCEPTED_THANKS", GROUP_SELECT_NONE, false, true, false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        ParentClass->AssassinState = ASSASSIN_STATE_3;
        Game->GiveHeroMorality(pGlobalScriptDef->HeroTakesAssassinGoldMorality);
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MURDER_TWIST_OBJECTIVE_KILL_SHERIFF", "OakBay", "");
    } else {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_ASSASSIN_BRIBE2", GROUP_SELECT_NONE, false, true, false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        Game->GiveHeroYesNoQuestion("TEXT_QST_B07_ASSASSIN_BRIBE2_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
        long answer2 = Game->MsgIsQuestionAnsweredYesOrNo();
        while (answer2 < 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            answer2 = Game->MsgIsQuestionAnsweredYesOrNo();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (answer2 == 1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_ASSASSIN_BRIBE2_ACCEPTED", GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            Game->GiveHeroGold(2500);
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_ASSASSIN_BRIBE2_ACCEPTED_THANKS", GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass->AssassinState = ASSASSIN_STATE_3;
            Game->GiveHeroMorality(pGlobalScriptDef->HeroTakesAssassinGoldMorality);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MURDER_TWIST_OBJECTIVE_KILL_SHERIFF", "OakBay", "");
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_ASSASSIN_BRIBE2_DECLINED", GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass->AssassinState = ASSASSIN_STATE_2;
        }
    }
}