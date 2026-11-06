// NScript::CV_BordelloScript::CBordelloClient::Main() -- a bordello client: a female villager walks to the client stand and chats or complains when hit; a male client walks in, browses, is offered to a disguised hero (yes: pay 200, the quickie cutscene; no / disgusted / browse lines), then walks back to the spawn point and fades out
// Leaf: ?Main@CBordelloClient@CV_BordelloScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E44EA0, 7002 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 1 (Main) of the entity vtable 0x012E3B48 that the landed CBordelloClient::Alloc 0x00E3B0F0 stores (its Init
//   0x00E3B1A0 is slot 2); Ego_r.pdb / FableWin.pdb name ?Main@CBordelloClient@CV_BordelloScript@NScript@@EAEXXZ. Members BrainState /
//   ClientID / SexChance / NextLine (+0x1c..+0x28) are the ones the landed Init writes.
// Shape: the landed CBordelloGuard::Main 0x00E40420 opening; the parent's IsHeroDressedAsLady 0x00E3E320 / PlayCutscene 0x00E3E720 /
//   seh_Whore (+0xa4) / csargs (+0xb4); CTimer ctor/dtor/GetTime expand where VC7.1's inline budget allows, operator= is forced (one site).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CV_BordelloScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/fable_script_text.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { MOVE_TYPE_WALKING=0, MOVE_TYPE_RUNNING=1 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define BC_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define BC_SAY(TEXT) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return; \
        } \
        if (T) return; \
    }
#define BC_MOVIE \
    CScriptGameResourceObjectMovieBase movie; \
    Game->StartMovieSequence("",movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
#define BC_LINE(WHO,TAG,MAX) \
    if (NextLine>MAX) NextLine=1; \
    CCharString text=WHO+GFIntToCharString(ClientID+1)+TAG+GFIntToCharString(NextLine);
void NScript::CV_BordelloScript::CBordelloClient::Main()
{
    SetPredicate(CIsThingAlive(Me));
    NSF;
    if (T) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    BC_START
    CScriptThing stand=Game->GetThingWithScriptName("M_ClientStand");
    CScriptThing spawn=Game->GetThingWithScriptName("M_ClientSpawn");
    CTimer timer;
    if (Me.GetDefName()=="CREATURE_BS_VILLAGER_FEMALE") {
        if (T) return;
        bool arrived=false;
        for (;!T;NSF) {
            if (!arrived) {
                if (!seh_me.IsPerformingScriptTask()) {
                    if (T) return;
                    seh_me.MoveToPosition(stand.GetPos(),1.0f,MOVE_TYPE_WALKING,false,true);
                }
                if (IsDistanceBetweenThingsUnder(Me,stand,5.0f)) {
                    if (T) return;
                    arrived=true;
                    seh_me.Release();
                }
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                seh_me.ClearCommands();
                BC_START
                BC_MOVIE
                BC_LINE("TEXT_QST_B13_WOMAN","_CHAT_0",2)
                BC_SAY(text)
                NextLine++;
                if (arrived) {
                    if (T) return;
                    if (seh_me.IsValid()) seh_me.Release();
                }
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (T) return;
                seh_me.ClearCommands();
                BC_START
                BC_MOVIE
                BC_SAY("TEXT_QST_B13_WOMAN"+GFIntToCharString(ClientID+1)+"_ON_HIT")
                if (arrived) {
                    if (T) return;
                    if (seh_me.IsValid()) seh_me.Release();
                }
            }
        }
    }
    else {
        while (BrainState==1) {
            NSF;
            if (T) return;
            if (!seh_me.IsPerformingScriptTask()) {
                if (T) return;
                seh_me.MoveToPosition(stand.GetPos(),1.0f,MOVE_TYPE_WALKING,false,true);
            }
            if (IsDistanceBetweenThingsUnder(stand,Me,5.0f)) {
                if (T) return;
                BrainState=2;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                seh_me.ClearCommands();
                BC_MOVIE
                BC_LINE("TEXT_QST_B13_CLIENT","_ENTER_0",3)
                BC_SAY(text)
                NextLine++;
            }
        }
        if (T) return;
        if (seh_me.IsValid()) seh_me.Release();
        timer=120;
        while (BrainState==2) {
            NSF;
            if (T) return;
            if (timer.GetTime()==0 || ParentClass.BecomeNunnery) {
                if (T) return;
                BrainState=3;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                BC_START
                seh_me.ClearCommands();
                BC_MOVIE
                if (ParentClass.HeroTricking && ParentClass.IsHeroDressedAsLady()) {
                    if (T) return;
                    if (SexChance==0) {
                        if (T) return;
                        BC_LINE("TEXT_QST_B13_CLIENT","_INTERESTED_0",3)
                        BC_SAY(text)
                        NextLine++;
                        Game->GiveHeroYesNoQuestion("TEXT_QST_B13_CLIENT"+GFIntToCharString(ClientID+1)+"_SEX_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                        long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                        while (answer<0) {
                            NSF;
                            if (T) return;
                            answer=Game->MsgIsQuestionAnsweredYesOrNo();
                        }
                        if (T) return;
                        if (answer==1) {
                            if (T) return;
                            BC_LINE("TEXT_QST_B13_CLIENT","_ACCEPTED_0",3)
                            BC_SAY(text)
                            NextLine++;
                            Game->GiveHeroGold(200);
                            Game->SetNumberOfTimesHeroHasHadSex(Game->GetNumberOfTimesHeroHasHadSex()+1);
                            Game->SetCutsceneSkippable(false);
                            ParentClass.seh_Whore=seh_me;
                            ParentClass.HeroPartying=true;
                            ParentClass.csargs["$ENDLINE"]="TEXT_QST_B13_CLIENT"+GFIntToCharString(ClientID+1)+"_LEAVE_01";
                            ParentClass.PlayCutscene("CS_BORDELLO_PAIDFORSEX_QUICKIE",true);
                            Game->SetCutsceneSkippable(true);
                            BrainState=3;
                            Game->SetHeroAsHavingHadGaySex(true);
                        }
                        else {
                            if (T) return;
                            BC_LINE("TEXT_QST_B13_CLIENT","_DECLINED_0",3)
                            BC_SAY(text)
                            NextLine++;
                        }
                    }
                    else {
                        if (T) return;
                        BC_LINE("TEXT_QST_B13_CLIENT","_DISGUSTED_0",3)
                        BC_SAY(text)
                        NextLine++;
                    }
                }
                else {
                    if (T) return;
                    BC_LINE("TEXT_QST_B13_CLIENT","_BROWSE_0",3)
                    BC_SAY(text)
                    NextLine++;
                }
                ParentClass.seh_Whore.Release();
                if (seh_me.IsValid()) seh_me.Release();
            }
        }
        if (T) return;
        BC_START
        while (BrainState==3) {
            NSF;
            if (T) return;
            if (!seh_me.IsPerformingScriptTask()) {
                if (T) return;
                seh_me.MoveToPosition(spawn.GetPos(),1.0f,MOVE_TYPE_WALKING,false,true);
            }
            if (IsDistanceBetweenThingsUnder(spawn,Me,5.0f)) {
                if (T) return;
                BrainState=4;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return;
                seh_me.ClearCommands();
                BC_MOVIE
                BC_LINE("TEXT_QST_B13_CLIENT","_LEAVE_0",3)
                BC_SAY(text)
                NextLine++;
            }
        }
        if (T) return;
        if (BrainState==4) {
            if (T) return;
            FadeOutAndKillEntity(Me,2.0f,true,true);
            for (;;) {
                NSF;
                if (T) return;
            }
        }
        for (;;) {
            NSF;
            if (T) return;
        }
    }
}