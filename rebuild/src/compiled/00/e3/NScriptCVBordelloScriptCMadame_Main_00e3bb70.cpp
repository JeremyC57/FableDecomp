// NScript::CV_BordelloScript::CMadame::Main() -- the bordello madame: the intro cut-scene once, then (until the hero owns the bordello) faces the hero when he comes near, takes a beer, offers the hero work when he is dressed as a lady, plays the magician-leaves cut-scene when the hero carries the deeds, chit-chat, attacked lines; once owned, the nunnery conversation (work offer, become-a-nunnery question and confirmation) and the attacked-pimp line.
// Inline-budget: flag /d2inlT69 -- window T69..T70 (T71 = default, 10181): calls the chit-chat / work-scene ~movie 0x006E7B80 (tail-merged at 0x00E3CAAB) and keeps the deeds / beer / attacked scene ends expanded, as retail does. Also two scoped `#pragma inline_depth(0)` regions: (1) round the CIsThingAlive ctor definition -- the predicate ctor expands with the CScriptThing copy ctor called (0x004ABE90 at 0x00E3BB9F), as in same-TU landed CBordelloLady::Main 0x00E3EB10 (same region); without it DIFFER 10165v10149; (2) round the chit-chat movie declaration (retail calls the ctor 0x006E7B60 at 0x00E3C8BE). Also FABLE_FORCEINLINE on the row-local CTimer::operator=(long) and GetTime() (retail expands every timer use: pDSTGame SetTimer / GetTimer vtable calls, no CTimer call; plain inline: DIFFER 10149v10149 for either) and FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (forced pause-entities ctor; without it DIFFER 10179v10149).
// Grade: RELOCATION_MATCH (10149v10149) with /d2inlT69 -- wfE3 #3. Every call / expand decision is retail's (inline ~CTimer = landed text; scoped inline_depth(0) round the chit-chat movie declaration, retail calls the ctor 0x006E7B60 at 0x00E3C8BE). The last residue (retail's unread entry store `mov [esp+9Ch],edi` 0x00E3BB81 + four alignment fillers) is closed by the dead release asserts below (Inferred line 5).
// Flags: /d2inlT69
// Inferred: the debug build's set-up checks after `TurnAndFaceTimer=2;`, compiled out (FABLE_RETAIL_ASSERT, fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM): the speech handle scripts the live madame (not the hero), the bordello house exists, and every fixed line she says is in the text bank (TextEntryExists, slot 358 0x00892070; one assert per scene). Their wording is INFERRED (no FableWin text for them exists here: FableWin.exe is not on this machine and ghidra_out holds no decomp of this function); what the retail bytes EVIDENCE is the count and the region: they hold exactly 32 conditional temporaries (4 GetScriptThing results, the "BordelloHouse" string and lookup, 26 line keys). Retail's entry store 0x00E3BB81 writes a frame word nothing reads: VC7.1 numbers a conditional-temporary flag bit for every temporary right of `&&` / `||` even inside the dead `if (0)` arm of the release assert, opens a second flags word once 32 bits are numbered, and c2 deletes the dead arm leaving only that word's entry `$T = 0`. Retail's attacked-condition bits are 1,2,4 / 8,16,32 of the SECOND word ([esp+2Ch] 0x00E3CB41..0x00E3CB77, 0x00E3E06F..), so exactly 32 dead bits precede `if (Me.MsgIsHitBy(...` and none fall between the two attacked tests: measured 31 / 33 one-temporary asserts DIFFER (10155 / 10149v10149), 32 RELOCATION_MATCH; this set plus the parent's seh_Madam check (36) DIFFER 10153v10149, without the house check (30) DIFFER 10155v10149; a per-speech check inside MD_SAY (bits between the attacked tests) DIFFER 10156v10149. Asserts without a conditional temporary number no bits (40 x FABLE_RETAIL_ASSERT(seh_me.IsValid()): DIFFER 10133v10149). Under FABLE_PORTABLE / other compilers the asserts are ((void)0); they cannot change behaviour.
// Leaf: ?Main@CMadame@CV_BordelloScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3BB70, 10149 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 1 (Main) of the CMadame entity vtable 0x012E3AAC that Alloc 0x00E3AAB0 stores (slot 2 Init 0x00E3AB60);
//   Ego_r.pdb / FableWin.pdb ?Main@CMadame@CV_BordelloScript@NScript@@EAEXXZ.
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
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
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_ASSERT_DEAD_ARM
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
#include "fable_assert.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
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
    CScriptGameResourceObjectMovieBase speech_movie; \
    Game->StartMovieSequence("",speech_movie); \
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
    if (TurnAndFaceTimer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),8.0f)) { \
        if (T) return; \
        TurnAndFaceTimer=4; \
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
    CTimer TurnAndFaceTimer;
    CCharString PresentedItem;
    TurnAndFaceTimer=2;
    // Inferred (header line 5): the debug build's set-up checks, compiled out; 32 conditional temporaries in all.
    // The speech handle scripts the live madame, not the hero (the speech, facing and camera calls go through it).
    FABLE_RETAIL_ASSERT(!seh_me.IsValid() || (seh_me.GetScriptThing() == Me && !seh_me.GetScriptThing().IsNull() && seh_me.GetScriptThing().IsAlive() && seh_me.GetScriptThing() != Game->GetHero()));
    // The bordello house exists (the deeds scene and the nunnery answer pass it to SetHouseOwnedByPlayer).
    FABLE_RETAIL_ASSERT(Me.IsNull() || !Game->GetThingWithScriptName("BordelloHouse").IsNull());
    // Every fixed line she says is in the text bank, scene by scene (the attacked lines are built from the hero's status tag).
    // The intro cut-scene lines.
    FABLE_RETAIL_ASSERT(!seh_me.IsValid() || (Game->TextEntryExists("TEXT_CS_B13_INTRO_HEROLADY_10") && Game->TextEntryExists("TEXT_CS_B13_INTRO_10")));
    // The beer.
    FABLE_RETAIL_ASSERT(!seh_me.IsValid() || Game->TextEntryExists("TEXT_QST_B13_MADAME_GIVEN_BEER"));
    // The work offer and its answers.
    FABLE_RETAIL_ASSERT(!seh_me.IsValid() || (Game->TextEntryExists("TEXT_QST_B13_MADAME_WORK_QUESTION") && Game->TextEntryExists("TEXT_QST_B13_MADAME_WORK_QUESTION_TEXT") && Game->TextEntryExists("TEXT_OBJECT_HERO_ANSWER_YES") && Game->TextEntryExists("TEXT_OBJECT_HERO_ANSWER_NO") && Game->TextEntryExists("TEXT_QST_B13_MADAME_HERO_JOINED") && Game->TextEntryExists("TEXT_QST_B13_MADAME_HERO_DECLINED")));
    // The hero at work.
    FABLE_RETAIL_ASSERT(!seh_me.IsValid() || Game->TextEntryExists("TEXT_QST_B13_MADAME_HERO_EARN_SOME"));
    // The five chit-chat lines.
    FABLE_RETAIL_ASSERT(!seh_me.IsValid() || (Game->TextEntryExists("TEXT_QST_B13_MADAME_CHIT_CHAT_01") && Game->TextEntryExists("TEXT_QST_B13_MADAME_CHIT_CHAT_02") && Game->TextEntryExists("TEXT_QST_B13_MADAME_CHIT_CHAT_03") && Game->TextEntryExists("TEXT_QST_B13_MADAME_CHIT_CHAT_04") && Game->TextEntryExists("TEXT_QST_B13_MADAME_CHIT_CHAT_05")));
    // Attacked once the bordello is the hero's.
    FABLE_RETAIL_ASSERT(!seh_me.IsValid() || Game->TextEntryExists("TEXT_QST_B13_MADAME_ATTACKED_PIMP"));
    // The owned bordello and the nunnery offer.
    FABLE_RETAIL_ASSERT(!seh_me.IsValid() || (Game->TextEntryExists("TEXT_QST_B13_MADAME_NUNNERY_THANKYOU") && Game->TextEntryExists("TEXT_QST_B13_MADAME_HERO_JOINED_OWNED") && Game->TextEntryExists("TEXT_QST_B13_MADAME_BECOME_NUNNERY") && Game->TextEntryExists("TEXT_QST_B13_MADAME_BECOME_NUNNERY_REPEAT") && Game->TextEntryExists("TEXT_QST_B13_MADAME_BECOME_NUNNERY_QUESTION") && Game->TextEntryExists("TEXT_QST_B13_MADAME_BECOME_NUNNERY_SURE") && Game->TextEntryExists("TEXT_QST_B13_MADAME_BECOME_NUNNERY_SURE_QUESTION") && Game->TextEntryExists("TEXT_QST_B13_MADAME_BECOME_NUNNERY_THANKS") && Game->TextEntryExists("TEXT_QST_B13_MADAME_BECOME_NUNNERY_RECONSIDER") && Game->TextEntryExists("TEXT_QST_B13_MADAME_BECOME_NUNNERY_DECLINED")));
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
        if (Me.MsgIsPresentedWithItem(PresentedItem) && PresentedItem=="OBJECT_BEER_TANKARD") {
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
#pragma inline_depth(0)
                    CScriptGameResourceObjectMovieBase speech_movie;
#pragma inline_depth()
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->FixMovieSequenceCamera(true);
                    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                    Game->Pause(1.0f);
                    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),true);
                    NSF;
                    Game->CameraUseCameraPoint("CAM_B_MADAM",Me,-1.0f,0,-1);
                    if (chat>5) chat=1;
                    CCharString full_text_name="TEXT_QST_B13_MADAME_CHIT_CHAT_0"+GFIntToCharString(chat);
                    MD_SAY(full_text_name,GROUP_SELECT_NONE)
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