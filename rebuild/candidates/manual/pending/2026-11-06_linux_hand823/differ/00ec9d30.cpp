// NScript::CV_SickChildScript::CMansLover::Main() -- the man's lover: talked to (or shown the man's love letter) while the hero carries the letter she asks for it, takes it and asks whether it is from the hero, and gives the hero her reply letter or a letter for the man (morality, lover state); otherwise she answers by the lover state; once leaving she walks to MansLoverLeavesHere and fades out there.
// Grade: DIFFER(4708v4708) -- 58 difflines, block layout only around the give-letter question (0x00ECA2E0..0x00ECA465): retail falls through from `cmp edi,1; jne` into the threaded answer==1 -> give-letter block (the ShownLetter arm jumps back to it) and routes those terminate checks to the other of the two identical exit copies (0x00ECA325 / 0x00ECAF49); ours cross-jumps the answer==1 terminate test into the ShownLetter-else one and lays the NOT_SEEN else arm first
// Next idea: a source shape that keeps the answer==1 and ShownLetter-else terminate tests distinct at cross-jump time without duplicating the give-letter code; chunk 823 a1-a11 (plain return, function-scope flag, goto join, do-while, store-before-T, inverted T, answer!=1 first) gave 58 or worse
// Leaf: ?Main@CMansLover@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC9D30, 4708 bytes. Name: slot 1 of the entity vtable 0x012F1D74 that the landed Alloc
//   0x00EC9C00 stores (0 0x00EC9D10, 2 Init 0x00EC9CB0, 3 GetParentScript 0x00EC9CA0, 4 OnPersist 0x00ECD930, 5 OnPredicateFail
//   0x00EC9CD0); Ego_r.pdb and FableWin.pdb ?Main@CMansLover@CV_SickChildScript@NScript@@EAEXXZ. FadeOutAndKillEntity 0x00CBE716 is
//   the PDB (const CScriptThing&, float, bool, bool) of engine/fable_script_fade.h (both bools true here).
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
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
extern "C" int __cdecl rand();
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE3EA
void NScript::CV_SickChildScript::CMansLover::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (ParentClass.MansLoverState==ML_NOT_RECEIVED_LETTER) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->SetThingHasInformation(Me,true,true,false);
    }
    CCharString PresentedItem;
    CCharString LoveLetter;
    bool ShownLetter=false;
    LoveLetter=pGlobalScriptDef->ObjectToGiveToMansLover;
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsPresentedWithItem(PresentedItem) && LoveLetter==PresentedItem) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ShownLetter=true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || ShownLetter) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (ShownLetter) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->FixMovieSequenceCamera(true);
                Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                Game->Pause(1.0f);
                Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),true);
                Game->NewScriptFrame();
                Game->CameraUseCameraPoint("CAM_REL_WOMANHERO",Me,-1.0f,0,-1);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->FixMovieSequenceCamera(true);
                Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                Game->Pause(1.0f);
                Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),true);
                Game->NewScriptFrame();
                Game->CameraUseCameraPoint("CAM_REL_WOMAN",Me,-1.0f,0,-1);
            }
            if (Game->IsObjectInThingsPossession(pGlobalScriptDef->ObjectToGiveToMansLover,Game->GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                bool GiveLetter=false;
                if (!ShownLetter) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_INTRO",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_GIVE_LETTER_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    while (answer<0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        GiveLetter=true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->CameraUseCameraPoint("CAM_REL_WOMANHERO",Me,-1.0f,0,-1);
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_MAN_NOT_SEEN_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    GiveLetter=true;
                }
                if (GiveLetter) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (ShownLetter) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        ShownLetter=false;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->CameraUseCameraPoint("CAM_REL_WOMANHERO",Me,-1.0f,0,-1);
                    }
                    Game->TakeObjectFromHero(pGlobalScriptDef->ObjectToGiveToMansLover);
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_FROM_YOU_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_LETTER_FROM_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long answer2=Game->MsgIsQuestionAnsweredYesOrNo();
                    while (answer2<0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        answer2=Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->CameraUseCameraPoint("CAM_REL_WOMAN",Me,-1.0f,0,-1);
                    if (answer2==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_20",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game->GiveHeroObject(pGlobalScriptDef->ObjectFromMansLover,-1,false);
                        Game->GiveHeroMorality(pGlobalScriptDef->TellingTheTruth);
                        ParentClass.MansLoverState=ML_NOT_IN_LOVE_WITH_YOU;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_FROM_YOU_20",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_FROM_YOU_30",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game->GiveHeroObject("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER",-1,false);
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_LIKES_YOU_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game->GiveHeroMorality(pGlobalScriptDef->LyingToWomanMorality);
                        ParentClass.MansLoverState=ML_IN_LOVE_WITH_YOU;
                    }
                    Game->ClearThingHasInformation(Me);
                    LeavingRegion=true;
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                if (LeavingRegion && rand()%2==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_LEAVING_REGION_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    switch (ParentClass.MansLoverState) {
                    case ML_NOT_RECEIVED_LETTER:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_INTRO_PRELETTER",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case ML_IN_LOVE_WITH_YOU:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case ML_NOT_IN_LOVE_WITH_YOU:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_MAN",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    default:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    }
                }
            }
            Game->FixMovieSequenceCamera(false);
        }
        if (LeavingRegion && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing LeavePoint=Game->GetThingWithScriptName("MansLoverLeavesHere");
            if (IsDistanceBetweenThingsOver(Me,LeavePoint,2.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToThing(LeavePoint,1.0f,ENTITY_MOVE_WALK,false,true,false,true);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FadeOutAndKillEntity(Me,1.5f,true,true);
            }
        }
    }
}
