// NScript::CV_DemonDoorsScript::CDF_KeyHoarder::Main() -- ask for the silver keys and open the demon door.
// Leaf: ?Main@CDF_KeyHoarder@CV_DemonDoorsScript@NScript@@EAEXXZ
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CV_DemonDoorsScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();

void NScript::CV_DemonDoorsScript::CDF_KeyHoarder::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetThingPersistent(Me,true);
    Game->EntitySetAsDamageable(Me,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game->EntitySetAsKillable(Me,false,false);
    CScriptThing region_exit=Game->GetNearestWithDefName(Me,"REGION_EXIT_POINT");
    Game->SetRegionExitAsActive(region_exit,false);
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            Game->CameraUseCameraPoint(Game->GetThingWithScriptName("CAM_NDD_FACE"),0,0,-1.0f,0,-1);
            Game->EntitySetAsDrawable(Game->GetHero(),false);
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_B19_SILVER_DOOR_INTRO",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            long answer;
            Game->GiveHeroYesNoQuestion("TEXT_QST_B19_SILVER_DOOR_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
            while ((answer=Game->MsgIsQuestionAnsweredYesOrNo())<0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (answer==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B19_SILVER_DOOR_REFUSAL",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->EntitySetAsDrawable(Game->GetHero(),true);
                Game->FixMovieSequenceCamera(false);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!Game->IsObjectInThingsPossession("OBJECT_SILVER_KEY",Game->GetHero())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_B19_SILVER_DOOR_ACCEPT_NONE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_KeyHoarder");
                    Game->EntitySetAsDrawable(Game->GetHero(),true);
                    Game->SetRegionExitAsActive(region_exit,true);
                    Game->FixMovieSequenceCamera(false);
                    return;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_B19_SILVER_DOOR_FIRST_OK",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_B19_SILVER_DOOR_SECOND_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    while ((answer=Game->MsgIsQuestionAnsweredYesOrNo())<0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B19_SILVER_DOOR_REFUSAL",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game->EntitySetAsDrawable(Game->GetHero(),true);
                        Game->FixMovieSequenceCamera(false);
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B19_SILVER_DOOR_ACCEPT_SOME",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        while (Game->IsObjectInThingsPossession("OBJECT_SILVER_KEY",Game->GetHero())) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game->TakeObjectFromHero("OBJECT_SILVER_KEY");
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_KeyHoarder");
                        Game->EntitySetAsDrawable(Game->GetHero(),true);
                        Game->SetRegionExitAsActive(region_exit,true);
                        Game->FixMovieSequenceCamera(false);
                        return;
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}