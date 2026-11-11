// NScript::CV_BordelloScript::CMagicman::Main() -- the bordello's magician owner: until the hero owns the bordello, walks home, offers the beer quick-access slot when targeted, faces and touts at a passing hero, talks (pimping / touting / pimp hat / chatter), complains when hit, drinks presented beers (six lines, then the drunk cut-scene, then too-much); once the hero works there, the dressed-as-lady lines and the party question (sex cut-scene); then removes himself.
// Grade: DIFFER(14772v15104) -- 220 masked lines, 51 hunks, 64 jump-target mismatches (chunk H40c, one grade): the predicate lever (inline_depth(0) round the row-local CIsThingAlive ctor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE) is byte-neutral here, the draft is unchanged. Residue as chunk 819 measured: retail expands the return epilogue at 0x00E445B8-0x00E4472E and 0x00E4474C-0x00E447F8 where ours shares one, the FEMALE hit thread-exit calls ~MovieBase where retail jumps to the shared pad 0x00E4489C; 332 bytes short
// Next idea: tools/jt.sh first (the thread-exit `jne` targets 0x00E448CC / 0x00E448E6 / 0x00E44903 map to different pads): the loop spelling of the three state loops (`for (;; NSF) { if (T) return; ...}` v `while (!T)`), then one in-body inline_depth(0) at the exits retail calls; the caller-size effect of statements written in both arms of an if / else (0x00E64FB0, chunk H40c) may be what makes retail expand the late epilogues H40f (3 grades): baseline 220 lines / 14772; `while (!ParentClass.PlayerOwned && true)` 223 / 14788; `|| false` 236 / 14716 v 15104: the outer loop factor is not the lever for the two expanded late epilogues.
// Leaf: ?Main@CMagicman@CV_BordelloScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E40E80, 15104 bytes, thiscall, plain `ret`, no EH frame, then the two 6-entry BeersDrunk jump
//   tables 0x00E44950 / 0x00E44968.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the CMagicman entity vtable (0x012E3B30);
//   Ego_r.pdb and FableWin.pdb both hold ?Main@CMagicman@CV_BordelloScript@NScript@@EAEXXZ.
// Data: engine/CV_BordelloScript.h (HeroPartying +0x48, HeroTricking +0x49, PlayerOwned +0x4c, HeroFoundDeedsLocation +0x50,
//   BeersDrunk +0x6c, BeerSetToDPad +0x70, seh_Boss +0x74; CMagicman DoneToutIntro +0x1c, MentionedPimpHat +0x1d).
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
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { MOVE_TYPE_WALKING=0, MOVE_TYPE_RUNNING=1 };
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
inline CScriptThing::~CScriptThing() { }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
__forceinline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define MM_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define MM_SAY(TEXT) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return; \
        } \
        if (T) return; \
    }
#define MM_MOVIE \
    CScriptGameResourceObjectMovieBase movie; \
    Game->StartMovieSequence("",movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
#define MM_CAMERA \
    Game->FixMovieSequenceCamera(true); \
    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false); \
    Game->Pause(1.0f); \
    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),true); \
    NSF; \
    Game->CameraUseCameraPoint("CAM_B_OWNER",Me,-1.0f,0,-1);
#define MM_SCENE \
    seh_me.ClearCommands(); \
    MM_MOVIE \
    MM_CAMERA
#define MM_ROUTINE \
    if (IsDistanceFromThingToPositionOver(Me,Me.GetHomePos(),2.0f) && !seh_me.IsPerformingScriptTask()) { \
        if (T) return; \
        seh_me.MoveToPosition(Me.GetHomePos(),1.0f,MOVE_TYPE_WALKING,false,true); \
    } \
    if (!ParentClass.BeerSetToDPad && Game->GetHeroTargetedThing()==Me) { \
        if (T) return; \
        ParentClass.BeerSetToDPad=true; \
        Game->SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD",2,1000000); \
        ParentClass.AddSpawnedFunction(new CSpawnedFunc<CV_BordelloScript>(CCharString("ParentClass.")+CCharString("WatchForHeroLeavingRegionWithBeer"),0,&ParentClass,&CV_BordelloScript::WatchForHeroLeavingRegionWithBeer),CCharString("")); \
    } \
    else if (ParentClass.BeerSetToDPad && !(Game->GetHeroTargetedThing()==Me)) { \
        if (T) return; \
        ParentClass.BeerSetToDPad=false; \
    } \
    if (faceTimer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),8.0f)) { \
        if (T) return; \
        faceTimer=4; \
        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false); \
    } \
    if (toutTimer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),15.0f)) { \
        if (T) return; \
        if (!Game->TextEntryExists("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0"+GFIntToCharString(tout))) { \
            if (T) return; \
            tout=1; \
        } \
        long conversation=Game->AddNewConversation(Me,false,false); \
        Game->AddPersonToConversation(conversation,Game->GetHero()); \
        Game->AddLineToConversation(conversation,"TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0"+GFIntToCharString(tout),false,Me,Game->GetHero()); \
        tout++; \
        toutTimer=30; \
    }
#define MM_HIT \
    if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) { \
        if (T) return; \
        MM_SCENE \
        if (ParentClass.IsHeroDressedAsLady()) { \
            if (T) return; \
            MM_SAY("TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE") \
        } \
        else { \
            if (T) return; \
            MM_SAY("TEXT_QST_B13_MAGICMAN_ATTACKED_MALE") \
        } \
        Game->FixMovieSequenceCamera(false); \
    }
#define MM_BEER(PARTY) \
    switch (ParentClass.BeersDrunk) { \
    case 0: \
        MM_SAY("TEXT_QST_B13_MAGICMAN_BEER1"+ParentClass.GetHeroStatusTextTag()) \
        break; \
    case 1: \
        MM_SAY("TEXT_QST_B13_MAGICMAN_BEER2"+ParentClass.GetHeroStatusTextTag()) \
        break; \
    case 2: \
        MM_SAY("TEXT_QST_B13_MAGICMAN_BEER3"+ParentClass.GetHeroStatusTextTag()) \
        break; \
    case 3: \
        MM_SAY("TEXT_QST_B13_MAGICMAN_BEER4"+ParentClass.GetHeroStatusTextTag()) \
        break; \
    case 4: \
        MM_SAY("TEXT_QST_B13_MAGICMAN_BEER5"+ParentClass.GetHeroStatusTextTag()) \
        break; \
    case 5: \
        MM_SAY("TEXT_QST_B13_MAGICMAN_BEER6"+ParentClass.GetHeroStatusTextTag()) \
        if (ParentClass.HeroFoundDeedsLocation) { \
            if (T) return; \
            ParentClass.PlayCutscene("CS_BORDELLO_MAGICIANDRUNK_QUICKIE",false); \
        } \
        else { \
            if (T) return; \
            ParentClass.PlayCutscene("CS_BORDELLO_MAGICIANDRUNK",false); \
            ParentClass.HeroFoundDeedsLocation=true; \
        } \
        PARTY \
        break; \
    default: \
        MM_SAY("TEXT_QST_B13_MAGICMAN_BEERTOOMUCH") \
        Game->GiveHeroObject("OBJECT_BEER_TANKARD",-1,true); \
        break; \
    } \
    ParentClass.BeersDrunk++;
void NScript::CV_BordelloScript::CMagicman::Main()
{
    SetPredicate(CIsThingAlive(Me));
    NSF;
    if (T) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    MM_START
    ParentClass.seh_Boss=seh_me;
    CCharString item;
    CTimer faceTimer;
    CTimer toutTimer;
    toutTimer=40;
    long chat=1;
    long tout=1;
    while (!ParentClass.PlayerOwned) {
        NSF;
        if (T) return;
        while (!ParentClass.HeroTricking && !ParentClass.PlayerOwned) {
            NSF;
            if (T) return;
            MM_ROUTINE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                MM_SCENE
                if (ParentClass.IsHeroDressedAsLady()) {
                    if (T) return;
                    MM_SAY("TEXT_QST_B13_MAGICMAN_PIMPING")
                }
                else if (!DoneToutIntro) {
                    if (T) return;
                    DoneToutIntro=true;
                    MM_SAY("TEXT_QST_B13_MAGICMAN_TOUTING")
                }
                else {
                    if (T) return;
                    if (!MentionedPimpHat && Game->IsWearingClothingItem(Game->GetHero(),"OBJECT_HERO_HAT_PIMP")) {
                        if (T) return;
                        MM_SAY("TEXT_QST_B13_MAGICMAN_PIMP_HAT")
                        MentionedPimpHat=true;
                    }
                    else {
                        if (T) return;
                        if (chat>4) chat=1;
                        CCharString line="TEXT_QST_B13_MAGICMAN_CHATTER_0"+GFIntToCharString(chat);
                        MM_SAY(line)
                        chat++;
                    }
                }
                Game->FixMovieSequenceCamera(false);
            }
            MM_HIT
            if (Me.MsgIsPresentedWithItem(item) && item=="OBJECT_BEER_TANKARD") {
                if (T) return;
                MM_SCENE
                MM_BEER(ParentClass.HeroPartying=true;)
                Game->FixMovieSequenceCamera(false);
            }
        }
        if (T) return;
        while (!ParentClass.PlayerOwned) {
            NSF;
            if (T) return;
            MM_ROUTINE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                MM_SCENE
                if (!ParentClass.IsHeroDressedAsLady()) {
                    if (T) return;
                    MM_SAY("TEXT_QST_B13_MAGICMAN_TOUTING")
                }
                else {
                    if (T) return;
                    if (ParentClass.IsHeroWearingBeard()) {
                        if (T) return;
                        MM_SAY("TEXT_QST_B13_MAGICMAN_BEARD_LADY")
                    }
                    else {
                        if (T) return;
                        if (ParentClass.IsHeroWearingTash()) {
                            if (T) return;
                            MM_SAY("TEXT_QST_B13_MAGICMAN_BEARD_LADY_MOUSTACHE")
                        }
                        else {
                            if (T) return;
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Me,"TEXT_QST_B13_MAGICMAN_LOVELY_SKIN",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    NSF;
                                    if (T) return;
                                }
                                if (T) return;
                            }
                            if (Game->IsWearingClothingItem(Game->GetHero(),"OBJECT_HERO_HAT_WHOREWIG")) {
                                if (T) return;
                                MM_SAY("TEXT_QST_B13_MAGICMAN_LOVELY_HAIR")
                                Game->GiveHeroYesNoQuestion("TEXT_QST_B13_MAGICMAN_PARTY","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                                while (answer<0) {
                                    NSF;
                                    if (T) return;
                                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                                }
                                if (T) return;
                                if (answer==1) {
                                    if (T) return;
                                    MM_SAY("TEXT_QST_B13_MAGICMAN_HORNY")
                                    Game->GiveHeroGold(1000);
                                    Game->SetNumberOfTimesHeroHasHadSex(Game->GetNumberOfTimesHeroHasHadSex()+1);
                                    Game->SetCutsceneSkippable(false);
                                    if (ParentClass.HeroFoundDeedsLocation) {
                                        if (T) return;
                                        ParentClass.PlayCutscene("CS_BORDELLO_MAGICIANSEX_QUICKIE",true);
                                    }
                                    else {
                                        if (T) return;
                                        ParentClass.PlayCutscene("CS_BORDELLO_MAGICIANSEX",true);
                                        ParentClass.HeroFoundDeedsLocation=true;
                                    }
                                    Game->SetCutsceneSkippable(true);
                                    ParentClass.HeroPartying=true;
                                    ParentClass.HeroFoundDeedsLocation=true;
                                    Game->SetHeroAsHavingHadGaySex(true);
                                }
                                else {
                                    if (T) return;
                                    MM_SAY("TEXT_QST_B13_MAGICMAN_REJECTED")
                                }
                            }
                            else {
                                if (T) return;
                                MM_SAY("TEXT_QST_B13_MAGICMAN_BAD_HAIR")
                            }
                        }
                    }
                }
                Game->FixMovieSequenceCamera(false);
            }
            MM_HIT
            if (Me.MsgIsPresentedWithItem(item) && item=="OBJECT_BEER_TANKARD") {
                if (T) return;
                seh_me.ClearCommands();
                MM_CAMERA
                MM_MOVIE
                MM_BEER(;)
                Game->FixMovieSequenceCamera(false);
            }
        }
        if (T) return;
    }
    if (T) return;
    Game->RemoveThing(Me,false,true);
}
