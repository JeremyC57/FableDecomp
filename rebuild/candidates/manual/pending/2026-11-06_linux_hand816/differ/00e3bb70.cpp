// NScript::CV_BordelloScript::CMadame::Main() -- the bordello madame: the intro cut-scene once, then (until the hero owns the bordello) faces the hero when he comes near, takes a beer, offers the hero work when he is dressed as a lady, plays the magician-leaves cut-scene when the hero carries the deeds, chit-chat, attacked lines; once owned, the nunnery conversation (work offer, become-a-nunnery question and confirmation) and the attacked-pimp line.
// Grade: DIFFER(10069v10149) -- blocker 1 (predicate copy called, by-value SetIsPushableByHero Me copy expanded) + the per-site movie split: retail expands the movie ctor at the beer / work-offer / deeds / attacked scenes but calls 0x006E7B60 at the intro, EARN_SOME, chit-chat and both loop-2 scenes, and expands the movie dtor only at the beer / deeds / attacked scenes (the work-offer, EARN_SOME and chit-chat scenes share one called ~MovieBase tail); a2 (movie inline-visible, declared-only ~CCountedPointer<MovieBase>) expands every first-loop ctor; plus the mixed ~CTimer exit pads (called at the natural end, expanded DeregisterTimer at two early exits)
// Next idea: blocker 1, then the movie split (frame-slot sharing of the scene movies and return-pad cross-jumping per the chunk 814 lesson): try per-scene blocks and the FORCEINLINE_CTOR movie opt-in on the first-loop scenes only
// Leaf: ?Main@CMadame@CV_BordelloScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3BB70, 10149 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 1 (Main) of the CMadame entity vtable 0x012E3AAC that Alloc 0x00E3AAB0 stores (slot 2 Init 0x00E3AB60);
//   Ego_r.pdb / FableWin.pdb ?Main@CMadame@CV_BordelloScript@NScript@@EAEXXZ.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#include "fable_string.h"
CCharString __fastcall GFIntToCharString(long value);       // 0x0099F830
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/EHeroAbility.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_BordelloScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define MD_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define MD_SAY(TEXT,G) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,G,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return; \
        } \
        if (T) return; \
    }
#define MD_MOVIE \
    CScriptGameResourceObjectMovieBase movie; \
    Game->StartMovieSequence("",movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
#define MD_SCENE \
    MD_MOVIE \
    Game->FixMovieSequenceCamera(true); \
    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false); \
    Game->Pause(1.0f); \
    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),true); \
    NSF; \
    Game->CameraUseCameraPoint("CAM_B_MADAM",Me,-1.0f,0,-1);
#define MD_ASK(TEXT) \
    Game->GiveHeroYesNoQuestion(TEXT,"TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true); \
    long answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    while (answer<0) { \
        NSF; \
        if (T) return; \
        answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    } \
    if (T) return;
#define MD_FACE_WHEN_NEAR \
    if (timer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),8.0f)) { \
        if (T) return; \
        timer=4; \
        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false); \
    }
void NScript::CV_BordelloScript::CMadame::Main()
{
    SetPredicate(CIsThingAlive(Me));
    NSF;
    if (T) return;
    if (!ParentClass.BecomeNunnery) {
        if (T) return;
        Game->SetThingHasInformation(Me,true,false,false);
    }
    Game->SetIsPushableByHero(Me,false);
    Game->EntitySetAsKillable(Me,false,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    MD_START
    ParentClass.seh_Madam=seh_me;
    CTimer timer;
    CCharString item;
    timer=2;
    if (!DoneIntro) {
        if (T) return;
        MD_MOVIE
        if (ParentClass.IsHeroDressedAsLady()) {
            if (T) return;
            ParentClass.csargs["$LADYINTRO"]="TEXT_CS_B13_INTRO_HEROLADY_10";
        }
        else {
            if (T) return;
            ParentClass.csargs["$LADYINTRO"]="TEXT_CS_B13_INTRO_10";
        }
        ParentClass.PlayCutscene("CS_BORDELLO_INTRO",false);
        DoneIntro=true;
    }
    long chat=1;
    while (!ParentClass.PlayerOwned) {
        NSF;
        if (T) return;
        MD_FACE_WHEN_NEAR
        if (Me.MsgIsPresentedWithItem(item) && item=="OBJECT_BEER_TANKARD") {
            if (T) return;
            MD_MOVIE
            MD_SAY("TEXT_QST_B13_MADAME_GIVEN_BEER",GROUP_SELECT_NONE)
            Game->GiveHeroObject("OBJECT_BEER_TANKARD",-1,true);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            if (!ParentClass.HeroTricking && ParentClass.IsHeroDressedAsLady()) {
                if (T) return;
                MD_SCENE
                MD_SAY("TEXT_QST_B13_MADAME_WORK_QUESTION",GROUP_SELECT_NONE)
                MD_ASK("TEXT_QST_B13_MADAME_WORK_QUESTION_TEXT")
                if (answer==1) {
                    if (T) return;
                    MD_SAY("TEXT_QST_B13_MADAME_HERO_JOINED",GROUP_SELECT_NONE)
                    ParentClass.HeroTricking=true;
                }
                else {
                    if (T) return;
                    MD_SAY("TEXT_QST_B13_MADAME_HERO_DECLINED",GROUP_SELECT_NONE)
                }
                Game->FixMovieSequenceCamera(false);
            }
            else {
                if (T) return;
                if (Game->IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO",Game->GetHero())) {
                    if (T) return;
                    MD_MOVIE
                    ParentClass.PlayCutscene("CS_BORDELLO_MAGICIAN_LEAVES",false);
                    ParentClass.PlayerOwned=true;
                    Game->SetHouseOwnedByPlayer(Game->GetThingWithScriptName("BordelloHouse"),true,true);
                }
                else if (ParentClass.HeroTricking && ParentClass.IsHeroDressedAsLady()) {
                    if (T) return;
                    MD_SCENE
                    MD_SAY("TEXT_QST_B13_MADAME_HERO_EARN_SOME",GROUP_SELECT_NONE)
                    Game->FixMovieSequenceCamera(false);
                }
                else {
                    if (T) return;
                    MD_SCENE
                    if (chat>5) chat=1;
                    CCharString line="TEXT_QST_B13_MADAME_CHIT_CHAT_0"+GFIntToCharString(chat);
                    MD_SAY(line,GROUP_SELECT_NONE)
                    chat++;
                    Game->FixMovieSequenceCamera(false);
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (T) return;
            MD_SCENE
            MD_SAY(CCharString("TEXT_QST_B13_MADAME_ATTACKED")+ParentClass.GetHeroStatusTextTag(),GROUP_SELECT_RANDOM_NO_REPEAT)
            Game->FixMovieSequenceCamera(false);
        }
    }
    if (T) return;
    while (!T) {
        MD_FACE_WHEN_NEAR
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            MD_SCENE
            if (ParentClass.BecomeNunnery) {
                if (T) return;
                MD_SAY("TEXT_QST_B13_MADAME_NUNNERY_THANKYOU",GROUP_SELECT_NONE)
            }
            else {
                if (T) return;
                if (ParentClass.IsHeroDressedAsLady()) {
                    if (T) return;
                    if (!ParentClass.HeroTricking) {
                        if (T) return;
                        MD_SAY("TEXT_QST_B13_MADAME_WORK_QUESTION",GROUP_SELECT_NONE)
                        MD_ASK("TEXT_QST_B13_MADAME_WORK_QUESTION_TEXT")
                        if (answer==1) {
                            if (T) return;
                            if (ParentClass.PlayerOwned) {
                                if (T) return;
                                MD_SAY("TEXT_QST_B13_MADAME_HERO_JOINED_OWNED",GROUP_SELECT_NONE)
                            }
                            else {
                                if (T) return;
                                if (ParentClass.PlayerOwned) {
                                    if (T) return;
                                    MD_SAY("TEXT_QST_B13_MADAME_HERO_JOINED_OWNED",GROUP_SELECT_NONE)
                                }
                                else {
                                    if (T) return;
                                    MD_SAY("TEXT_QST_B13_MADAME_HERO_JOINED",GROUP_SELECT_NONE)
                                }
                            }
                            ParentClass.HeroTricking=true;
                        }
                        else {
                            if (T) return;
                            MD_SAY("TEXT_QST_B13_MADAME_HERO_DECLINED",GROUP_SELECT_NONE)
                        }
                    }
                    else {
                        if (T) return;
                        MD_SAY("TEXT_QST_B13_MADAME_HERO_EARN_SOME",GROUP_SELECT_NONE)
                    }
                }
                else {
                    if (T) return;
                    if (ParentClass.BecomeNunnery) {
                        if (T) return;
                        MD_SAY("TEXT_QST_B13_MADAME_NUNNERY_THANKYOU",GROUP_SELECT_NONE)
                    }
                    else {
                        if (T) return;
                        if (!MentionedNunnery) {
                            if (T) return;
                            MentionedNunnery=true;
                            MD_SAY("TEXT_QST_B13_MADAME_BECOME_NUNNERY",GROUP_SELECT_NONE)
                        }
                        else {
                            if (T) return;
                            MD_SAY("TEXT_QST_B13_MADAME_BECOME_NUNNERY_REPEAT",GROUP_SELECT_RANDOM_NO_REPEAT)
                        }
                        MD_ASK("TEXT_QST_B13_MADAME_BECOME_NUNNERY_QUESTION")
                        if (answer==1) {
                            if (T) return;
                            MD_SAY("TEXT_QST_B13_MADAME_BECOME_NUNNERY_SURE",GROUP_SELECT_NONE)
                            MD_ASK("TEXT_QST_B13_MADAME_BECOME_NUNNERY_SURE_QUESTION")
                            if (answer==1) {
                                if (T) return;
                                MD_SAY("TEXT_QST_B13_MADAME_BECOME_NUNNERY_THANKS",GROUP_SELECT_NONE)
                                Game->TakeObjectFromHero("OBJECT_DEEDS_BORDELLO");
                                Game->SetHouseOwnedByPlayer(Game->GetThingWithScriptName("BordelloHouse"),false,false);
                                ParentClass.BecomeNunnery=true;
                                Game->ClearThingHasInformation(Me);
                                Game->GiveHeroRenownPoints(100);
                                Game->GiveHeroMorality(0.2f);
                            }
                            else {
                                if (T) return;
                                MD_SAY("TEXT_QST_B13_MADAME_BECOME_NUNNERY_RECONSIDER",GROUP_SELECT_NONE)
                            }
                        }
                        else {
                            if (T) return;
                            MD_SAY("TEXT_QST_B13_MADAME_BECOME_NUNNERY_DECLINED",GROUP_SELECT_NONE)
                        }
                    }
                }
            }
            Game->FixMovieSequenceCamera(false);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (T) return;
            MD_SCENE
            MD_SAY(CCharString("TEXT_QST_B13_MADAME_ATTACKED_PIMP"),GROUP_SELECT_RANDOM_NO_REPEAT)
            Game->FixMovieSequenceCamera(false);
        }
        NSF;
    }
}
