// NScript::CV_SickChildScript::CMansLover::Main() -- the man's lover: talked to (or shown the man's love letter) while the hero carries the letter she asks for it, takes it and asks whether it is from the hero, and gives the hero her reply letter or a letter for the man (morality, lover state); otherwise she answers by the lover state; once leaving she walks to MansLoverLeavesHere and fades out there.
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Match: 4708 bytes outside audited relocation operands; original donor acceptance/rejection branch ownership restored.
// Shape: AnsweredYes or a presented letter selects acceptance; rejection speech belongs to that final decision, not the earlier question test. See PROBES.md and NAMING_EVIDENCE.md.
// Leaf: ?Main@CMansLover@CV_SickChildScript@NScript@@EAEXXZ
// Retail: 0x00EC9D30, 4708 bytes, thiscall.
//   0x00EC9C00 stores (0 0x00EC9D10, 2 Init 0x00EC9CB0, 3 GetParentScript 0x00EC9CA0, 4 OnPersist 0x00ECD930, 5 OnPredicateFail
//   0x00EC9CD0); Ego_r.pdb and FableWin.pdb ?Main@CMansLover@CV_SickChildScript@NScript@@EAEXXZ. FadeOutAndKillEntity 0x00CBE716 is
//   the PDB (const CScriptThing&, float, bool, bool) of engine/fable_script_fade.h (both bools true here).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/C3DVector.h"
#include "engine/CV_SickChildScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EScriptEntityMoveType.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
extern "C" int FABLE_CDECL rand();
#include "engine/fable_script_distance.h"
void NScript::CV_SickChildScript::CMansLover::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (ParentClass.MansLoverState==ML_NOT_RECEIVED_LETTER) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.SetThingHasInformation(Me,true,true,false);
    }
    CCharString PresentedItem;
    CCharString ItemName;
    bool TriggerSpeechFromPresentingItem=false;
    ItemName=pGlobalScriptDef->ObjectToGiveToMansLover;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsPresentedWithItem(PresentedItem) && ItemName==PresentedItem) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            TriggerSpeechFromPresentingItem=true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || TriggerSpeechFromPresentingItem) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (TriggerSpeechFromPresentingItem) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.FixMovieSequenceCamera(true);
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                Game.Pause(1.0f);
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),true);
                Game.NewScriptFrame();
                Game.CameraUseCameraPoint("CAM_REL_WOMANHERO",Me,-1.0f,0,-1);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.FixMovieSequenceCamera(true);
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                Game.Pause(1.0f);
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),true);
                Game.NewScriptFrame();
                Game.CameraUseCameraPoint("CAM_REL_WOMAN",Me,-1.0f,0,-1);
            }
            if (Game.IsObjectInThingsPossession(pGlobalScriptDef->ObjectToGiveToMansLover,Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                bool AnsweredYes=false;
                if (!TriggerSpeechFromPresentingItem) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_INTRO",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_GIVE_LETTER_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                    while (YNQAnswer<0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (YNQAnswer==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        AnsweredYes=true;
                    }

                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    AnsweredYes=true;
                }
                if (AnsweredYes || TriggerSpeechFromPresentingItem) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (TriggerSpeechFromPresentingItem) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        TriggerSpeechFromPresentingItem=false;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.CameraUseCameraPoint("CAM_REL_WOMANHERO",Me,-1.0f,0,-1);
                    }
                    Game.TakeObjectFromHero(pGlobalScriptDef->ObjectToGiveToMansLover);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_FROM_YOU_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_LETTER_FROM_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                    while (YNQAnswer<0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.CameraUseCameraPoint("CAM_REL_WOMAN",Me,-1.0f,0,-1);
                    if (YNQAnswer==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_20",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroObject(pGlobalScriptDef->ObjectFromMansLover,-1,false);
                        Game.GiveHeroMorality(pGlobalScriptDef->TellingTheTruth);
                        ParentClass.MansLoverState=ML_NOT_IN_LOVE_WITH_YOU;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_FROM_YOU_20",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_FROM_YOU_30",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroObject("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER",-1,false);
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_LIKES_YOU_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroMorality(pGlobalScriptDef->LyingToWomanMorality);
                        ParentClass.MansLoverState=ML_IN_LOVE_WITH_YOU;
                    }
                    Game.ClearThingHasInformation(Me);
                    LeavingRegion=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.CameraUseCameraPoint("CAM_REL_WOMANHERO",Me,-1.0f,0,-1);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_MAN_NOT_SEEN_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                if (LeavingRegion && rand()%2==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_LEAVING_REGION_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    switch (ParentClass.MansLoverState) {
                    case ML_NOT_RECEIVED_LETTER:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_INTRO_PRELETTER",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case ML_IN_LOVE_WITH_YOU:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case ML_NOT_IN_LOVE_WITH_YOU:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_MAN",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    default:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    }
                }
            }
            Game.FixMovieSequenceCamera(false);
        }
        if (LeavingRegion && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing ExitStageLeft=Game.GetThingWithScriptName("MansLoverLeavesHere");
            if (IsDistanceBetweenThingsOver(Me,ExitStageLeft,2.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToThing(ExitStageLeft,1.0f,ENTITY_MOVE_WALK,false,true,false,true);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FadeOutAndKillEntity(Me,1.5f,true,true);
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}