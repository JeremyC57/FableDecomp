// NScript::CV_ChickenKickingScript::CChickenMaster::Main() -- the chicken-kicking organiser: the intro and quest card, the kick-off explanation and the rules question, the timed chicken-kicking rounds (fade out / in, spectators once the ghost is gone, chicken counts and the record), the results and rewards, and the repeat offer
// Grade: DIFFER(15210v15210) -- 16 lines, register colouring only: at the third FadeScreenOut 0x00E6716A retail loads the Game vptr into edx (eax still holds the in-place CRGBColour pointer), ours into eax; at the spectator-creation call 0x00E671E0 (Game vfunc +0x120 with two lea pushes) retail uses eax for the vptr and edx for the addresses, ours the reverse; sites 1 and 3 have identical surrounding code but colour differently, so the choice is global; every other byte matches up to relocations
// Next idea: none local; chunk 818 c4-c10 and chunk 820 a1-a10 (do-while(0) Ts and whole-branch wraps, named bool, branch scope, hoisted CRGBColour, swapped branches) all leave the 16 lines; a5 (no inline ~CScriptThing) and a8 (plain inline GFIntToWideString) are worse -- needs a different spelling elsewhere in the 15 KB body that shifts global colouring
// Leaf: ?Main@CChickenMaster@CV_ChickenKickingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E64FB0, 15210 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): slot 1 of the CChickenMaster entity vtable 0x012E747C that Alloc 0x00E634B0 stores (slot 0 scalar
//   deleting dtor 0x00E63570, 2 Init 0x00E63560, 3 GetParentScript 0x00E63550, 4 OnPersist 0x00E641B0); Ego_r.pdb and FableWin.pdb
//   both hold ?Main@CChickenMaster@CV_ChickenKickingScript@NScript@@EAEXXZ.
// Layout: engine/NScript_CV_ChickenKickingScript.h with the chunk 818 CHICKENMASTER opt-in and the
//   CScriptGameResourceObjectScriptedThingBase FORCEINLINE_ASSIGN opt-in (hdr/README.md).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CV_CHICKENKICKINGSCRIPT_CHICKENMASTER
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN
#include "engine/EHeroAbility.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_map.h"
#include "engine/NScript_CV_ChickenKickingScript.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
bool FABLE_FASTCALL IsDistanceFromThingToPositionUnder(const CScriptThing&, const C3DVector&, float);   // 0x00CBE4B7
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
extern CScriptDef* pGlobalScriptDef;         // 0x0143E90C
extern CWideString g_UnprovenWideString_0143E908;   // 0x0143E908 (landed descriptive spelling)
template<> std::vector<CWideString>::~vector();     // 0x00412130
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::CScriptThing(const CScriptThing& other):CBaseClass(other),PImp(other.PImp){}
inline CScriptThing::~CScriptThing() { }
__forceinline CWideString __fastcall GFIntToWideString(long value)   // 0x00405F00 (expanded here)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define CM_SAY(TEXT) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return; \
        } \
        if (T) return; \
    }
#define CM_HIT(EXTRA) \
    (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || hit EXTRA)
#define CM_CHAT(LINE_TEXT) \
    if (T) return; \
    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false); \
    long conversation=Game->AddNewConversation(Me,false,false); \
    Game->AddPersonToConversation(conversation,Game->GetHero()); \
    LINE_TEXT \
    chat_timer=8; \
    while (chat_timer.GetTime()>0 && !hit && !ParentClass.TalkedTo) { \
        NSF; \
        if (T) return; \
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) { \
            if (T) return; \
            ParentClass.TalkedTo=true; \
        } \
        if (CM_HIT()) { \
            if (T) return; \
            hit=true; \
        } \
    } \
    if (T) return; \
    chat_timer=15;
#define CM_SPECTATORS \
    Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED",Game->GetThingWithScriptName("Spectator1").GetPos(),"Spectator",false); \
    Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED",Game->GetThingWithScriptName("Spectator2").GetPos(),"Spectator",false); \
    Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED",Game->GetThingWithScriptName("Spectator3").GetPos(),"Spectator",false);
#define CM_LINE(TEXT) ParentClass.csargs["$LINE"]=TEXT;

void NScript::CV_ChickenKickingScript::CChickenMaster::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        NSF;
        if (T) return;
    }
    if (T) return;
    ParentClass.seh_ChickenMaster=seh_me;
    Game->EntitySetAppearanceMorphSeed(Me,4);
    Game->SetThingHasInformation(Me,false,false,false);
    Game->SetIsPushableByHero(Me,false);
    Game->EntitySetAsKillable(Me,false,true);
    C3DVector home=Me.GetPos();
    CTimer chat_timer;
    chat_timer=0;
    CTimer spare_timer;
    spare_timer=0;
    bool hit=false;
    if (ParentClass.KnowGhostHasGone) {
        if (T) return;
        Game->EntityTeleportToThing(Me,Game->GetThingWithScriptName("MK_CK_ORG"),false);
    }
    while (!ParentClass.KnowGhostHasGone) {
        NSF;
        if (T) return;
        if (IsDistanceBetweenThingsUnder(Game->GetHero(),Me,5.5f) && IsDistanceFromThingToPositionUnder(Me,home,6.0f) && chat_timer.GetTime()<=0) {
            CM_CHAT(
                if (GhostChat) {
                    if (T) return;
                    Game->AddLineToConversation(conversation,"TEXT_QST_B17_MASTER_EARLY_ASIDE_FIRST_POSTCHAT",false,Me,Game->GetHero());
                }
                else {
                    if (T) return;
                    Game->AddLineToConversation(conversation,"TEXT_QST_B17_MASTER_EARLY_ASIDE_FIRST_PRECHAT_10",false,Me,Game->GetHero());
                }
            )
        }
        if (CM_HIT()) {
            if (T) return;
            hit=false;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("",movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.PlayCutscene("CS_CHICKING_HITGUYTOP");
            ParentClass.RanOff=true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || ParentClass.TalkedTo) {
            if (T) return;
            ParentClass.TalkedTo=false;
            Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_CHICKEN_KICKING",Game->GetActiveQuestName(),false);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_01","","");
            if (!GhostChat) {
                if (T) return;
                GhostChat=true;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (!Game->GetThingWithScriptName("GhostFisherman").IsAlive()) {
                    if (T) return;
                    ParentClass.PlayCutscene("CS_CHICKING_INITIALWALK1");
                    ParentClass.KnowGhostHasGone=true;
                    Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_02","","");
                }
                else {
                    if (T) return;
                    CM_SAY("TEXT_QST_B17_MASTER_INITIAL_MEETING")
                }
            }
            else {
                if (T) return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (!Game->GetThingWithScriptName("GhostFisherman").IsAlive()) {
                    if (T) return;
                    ParentClass.PlayCutscene("CS_CHICKING_INITIALWALK2");
                    ParentClass.KnowGhostHasGone=true;
                    Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_02","","");
                }
                else {
                    if (T) return;
                    CM_SAY("TEXT_QST_B17_MASTER_RETURN_MEETING")
                }
            }
        }
    }
    if (T) return;
    NSF;
    if (T) return;
    CScriptThing organiser_marker=Game->GetThingWithScriptName("MK_CK_ORG");
    for (;!T;NSF) {
        if (IsDistanceBetweenThingsUnder(Game->GetHero(),Me,5.5f) && IsDistanceBetweenThingsUnder(organiser_marker,Me,6.0f) && chat_timer.GetTime()<=0) {
            CM_CHAT(
                Game->AddLineToConversation(conversation,"TEXT_QST_B17_MASTER_GAME_ASIDE",false,Me,Game->GetHero());
            )
        }
        if (CM_HIT(|| ParentClass.SpectatorsUnderAttack)) {
            if (T) return;
            ParentClass.SpectatorsUnderAttack=false;
            hit=false;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("",movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.PlayCutscene("CS_CHICKING_HITGUYBOTTOM");
            ParentClass.RanOff=true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || ParentClass.TalkedTo) {
            if (T) return;
            ParentClass.TalkedTo=false;
            if (HaveTalked) {
                if (T) return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                CM_SAY("TEXT_QST_B17_MASTER_GREETING_RETURN")
                Game->GiveHeroYesNoQuestion("TEXT_QST_B17_GREETING_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    NSF;
                    if (T) return;
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (T) return;
                if (answer==1) {
                    if (T) return;
                    if (Game->GetHeroGold()>=50) {
                        if (T) return;
                        if (HeroHasPlayed) {
                            if (T) return;
                            CM_SAY("TEXT_QST_B17_MASTER_START_KICKING_RETURN")
                        }
                        else {
                            if (T) return;
                            CM_SAY("TEXT_QST_B17_MASTER_START_KICKING")
                            HeroHasPlayed=true;
                        }
                        Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                        if (ParentClass.KnowGhostHasGone && !ParentClass.SpectatorsCreated) {
                            if (T) return;
                            CM_SPECTATORS
                            ParentClass.SpectatorsCreated=true;
                        }
                        Game->ConfiscateAllHeroWeapons();
                        Game->SetHeroWillAsUsable(false);
                        Game->SetTeleportingAsActive(false);
                        Game->GiveHeroGold(-50);
                        KickingChickens=true;
                    }
                    else {
                        if (T) return;
                        CM_SAY("TEXT_QST_B17_MASTER_NO_MONEY")
                        KickingChickens=false;
                    }
                }
                else {
                    if (T) return;
                    CM_SAY("TEXT_QST_B17_MASTER_DONT_KICK")
                    KickingChickens=false;
                }
            }
            else {
                if (T) return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                HaveTalked=true;
                if (seh_me.IsPerformingScriptTask() && IsDistanceBetweenThingsOver(organiser_marker,Me,6.0f)) {
                    if (T) return;
                    seh_me.ClearCommands();
                    CM_SAY("TEXT_QST_B17_MASTER_QUICK_WALK")
                    Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                    Game->EntityTeleportToThing(Me,Game->GetThingWithScriptName("MK_CK_ORG"),false);
                    if (ParentClass.KnowGhostHasGone && !ParentClass.SpectatorsCreated) {
                        if (T) return;
                        CM_SPECTATORS
                        ParentClass.SpectatorsCreated=true;
                    }
                }
                else {
                    if (T) return;
                    Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                    if (ParentClass.KnowGhostHasGone && !ParentClass.SpectatorsCreated) {
                        if (T) return;
                        CM_SPECTATORS
                        ParentClass.SpectatorsCreated=true;
                    }
                    Game->EntityTeleportToThing(Me,Game->GetThingWithScriptName("MK_CK_ORG"),false);
                }
                ParentClass.PlayCutscene("CS_CHICKING_INTRO");
                Game->FadeScreenIn();
                Game->GiveHeroYesNoQuestion("TEXT_QST_B17_GREETING_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    NSF;
                    if (T) return;
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (T) return;
                if (answer==1) {
                    if (T) return;
                    if (Game->GetHeroGold()>=50) {
                        if (T) return;
                        ParentClass.PlayCutscene("CS_CHICKING_START");
                        HeroHasPlayed=true;
                        Game->ConfiscateAllHeroWeapons();
                        Game->SetHeroWillAsUsable(false);
                        Game->SetTeleportingAsActive(false);
                        Game->GiveHeroGold(-50);
                        KickingChickens=true;
                    }
                    else {
                        if (T) return;
                        ParentClass.PlayCutscene("CS_CHICKING_NOCASH");
                    }
                }
                else {
                    if (T) return;
                    ParentClass.PlayCutscene("CS_CHICKING_NOKICK");
                }
            }
            if (KickingChickens) {
                if (T) return;
                long chicken_counter=Game->AddQuestInfoCounter("HUD_ICON_CHICKEN_BROWN",5,1.0f);
                long best_counter=Game->AddQuestInfoCounter("HUD_ICON_MULTI_ARROW",0,1.0f);
                long score_counter=Game->AddQuestInfoCounter("HUD_ICON_ARROW",0,1.0f);
                Game->UpdateQuestInfoCounter(best_counter,PMasterData->MaxChickenKickingScore,-1);
                CCharString cutscene;
                Game->DisplayQuestInfo(true);
                Game->Pause(1.0f);
                Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("CK_HeroStart"),false);
                Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                long chickens_kicked=0;
                long score=0;
                while (chickens_kicked<5) {
                    NSF;
                    if (T) return;
                    CScriptThing chicken=Game->CreateCreature("CREATURE_KICKING_CHICKEN_01",Game->GetThingWithScriptName("CK_ChickenStart").GetPos(),"KickedChicken",false);
                    Game->SetPlayerCreatureOnlyTarget(chicken);
                    Game->SetIsPushableByHero(chicken,false);
                    Game->UpdateQuestInfoCounter(score_counter,score,-1);
                    Game->UpdateQuestInfoCounter(chicken_counter,chickens_kicked,-1);
                    Game->FadeScreenIn();
                    if (!ParentClass.InfoDisplayed) {
                        if (T) return;
                        Game->DisplayGameInfo("TEXT_QST_B17_TUTORIAL_10");
                        while (!Game->MsgIsGameInfoClickedPast()) {
                            NSF;
                            if (T) return;
                        }
                        if (T) return;
                        Game->DisplayGameInfo("TEXT_QST_B17_TUTORIAL_20");
                        while (!Game->MsgIsGameInfoClickedPast()) {
                            NSF;
                            if (T) return;
                        }
                        if (T) return;
                        Game->DisplayGameInfo("TEXT_QST_B17_TUTORIAL_30");
                        while (!Game->MsgIsGameInfoClickedPast()) {
                            NSF;
                            if (T) return;
                        }
                        if (T) return;
                        ParentClass.InfoDisplayed=true;
                    }
                    ParentClass.ChickenLanded=false;
                    while (!ParentClass.ChickenLanded) {
                        NSF;
                        if (T) return;
                    }
                    if (T) return;
                    {
                        CScriptGameResourceObjectMovieBase movie;
                        Game->StartMovieSequence("",movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        cutscene="CS_CHICKING_LANDED";
                        if (ParentClass.DistanceBand==4) {
                            if (T) return;
                            CM_LINE("TEXT_QST_B17_MASTER_TOO_FAR_NEW")
                            cutscene="CS_CHICKING_MISSED";
                        }
                        else if (ParentClass.DistanceBand==0) {
                            if (T) return;
                            CM_LINE("TEXT_QST_B17_MASTER_TOO_WEAK_NEW")
                        }
                        else if (ParentClass.DistanceBand==-1) {
                            if (T) return;
                            cutscene="CS_CHICKING_FOWL";
                        }
                        else {
                            if (T) return;
                            if (ParentClass.FinalSector==1) {
                                if (T) return;
                                if (ParentClass.DistanceBand==3) {
                                    if (T) return;
                                    CM_LINE("TEXT_QST_B17_MASTER_FAR_LEFT_NEW")
                                    score+=100;
                                }
                                else if (ParentClass.DistanceBand==2) {
                                    if (T) return;
                                    CM_LINE("TEXT_QST_B17_MASTER_CENTRE_LEFT_NEW")
                                    score+=50;
                                }
                                else if (ParentClass.DistanceBand==1) {
                                    if (T) return;
                                    CM_LINE("TEXT_QST_B17_MASTER_FRONT_LEFT_NEW")
                                    score+=25;
                                }
                            }
                            else if (ParentClass.FinalSector==2) {
                                if (T) return;
                                if (ParentClass.DistanceBand==3) {
                                    if (T) return;
                                    CM_LINE("TEXT_QST_B17_MASTER_FAR_CENTRE_NEW")
                                    score+=10;
                                }
                                else if (ParentClass.DistanceBand==2) {
                                    if (T) return;
                                    CM_LINE("TEXT_QST_B17_MASTER_CENTRE_CENTRE_NEW")
                                    score+=25;
                                }
                                else if (ParentClass.DistanceBand==1) {
                                    if (T) return;
                                    CM_LINE("TEXT_QST_B17_MASTER_NEAR_CENTRE_NEW")
                                    score+=10;
                                }
                            }
                            else if (ParentClass.FinalSector==3) {
                                if (T) return;
                                if (ParentClass.DistanceBand==3) {
                                    if (T) return;
                                    CM_LINE("TEXT_QST_B17_MASTER_FAR_RIGHT_NEW")
                                    score+=100;
                                }
                                else if (ParentClass.DistanceBand==2) {
                                    if (T) return;
                                    CM_LINE("TEXT_QST_B17_MASTER_CENTRE_RIGHT_NEW")
                                    score+=50;
                                }
                                else if (ParentClass.DistanceBand==1) {
                                    if (T) return;
                                    CM_LINE("TEXT_QST_B17_MASTER_NEAR_RIGHT_NEW")
                                    score+=25;
                                }
                            }
                            else if (ParentClass.FinalSector==0) {
                                if (T) return;
                                CM_LINE("TEXT_QST_B17_MASTER_OFF_COURSE")
                                cutscene="CS_CHICKING_MISSED";
                            }
                            else {
                                if (T) return;
                                CM_LINE("TEXT_QST_B17_MASTER_NOWHERE")
                                cutscene="CS_CHICKING_MISSED";
                            }
                        }
                        ParentClass.PlayCutscene(cutscene);
                    }
                    Game->RemoveThing(Game->GetThingWithScriptName("KickedChicken"),false,true);
                    ++chickens_kicked;
                    Game->ResetPlayerCreatureOnlyTarget();
                }
                if (T) return;
                Game->RemoveQuestInfoElement(score_counter);
                Game->RemoveQuestInfoElement(best_counter);
                Game->RemoveQuestInfoElement(chicken_counter);
                Game->ReturnAllConfiscatedItemsToHero();
                Game->SetHeroWillAsUsable(true);
                Game->SetTeleportingAsActive(true);
                std::vector<CWideString> params;
                params.push_back(GFIntToWideString(score));
                g_UnprovenWideString_0143E908=Game->GetFormattedString("TEXT_QST_B17_SCORE",params);
                cutscene="CS_CHICKING_END";
                float fscore=(float)score;
                if (fscore>=pGlobalScriptDef->CHK_HighPrize) {
                    if (T) return;
                    ParentClass.csargs["$ANIM"]="CHICK_KICK_HIGH";
                    ParentClass.csargs["$LOOP"]="CHICK_KICK_HIGH_LOOP";
                    if (ParentClass.PrizesWon & CV_ChickenKickingScript::PRZ_TOP_PRIZE) {
                        if (T) return;
                        if (score<=PMasterData->MaxChickenKickingScore) {
                            if (T) return;
                            CM_LINE("TEXT_QST_B17_MASTER_TOP_PRIZE_AGAIN")
                        }
                        else {
                            if (T) return;
                            PMasterData->MaxChickenKickingScore=score;
                            CM_LINE("TEXT_QST_B17_MASTER_TOP_PRIZE_AGAIN_HIGH")
                        }
                        ParentClass.csargs["$PRIZE"]="200";
                    }
                    else {
                        if (T) return;
                        PMasterData->MaxChickenKickingScore=score;
                        ParentClass.PrizesWon|=CV_ChickenKickingScript::PRZ_TOP_PRIZE;
                        cutscene="CS_CHICKING_TOPPRIZE";
                    }
                }
                else if (fscore>=pGlobalScriptDef->CHK_MidPrize) {
                    if (T) return;
                    ParentClass.csargs["$ANIM"]="CHICK_KICK_HIGH";
                    ParentClass.csargs["$LOOP"]="CHICK_KICK_HIGH_LOOP";
                    if (ParentClass.PrizesWon & CV_ChickenKickingScript::PRZ_MEDIUM_PRIZE) {
                        if (T) return;
                        if (score<=PMasterData->MaxChickenKickingScore) {
                            if (T) return;
                            CM_LINE("TEXT_QST_B17_MASTER_MIDDLE_PRIZE_AGAIN")
                        }
                        else {
                            if (T) return;
                            PMasterData->MaxChickenKickingScore=score;
                            CM_LINE("TEXT_QST_B17_MASTER_MIDDLE_PRIZE_AGAIN_HIGH")
                        }
                    }
                    else {
                        if (T) return;
                        CM_LINE("TEXT_QST_B17_MASTER_MIDDLE_PRIZE_FIRST")
                        ParentClass.PrizesWon|=CV_ChickenKickingScript::PRZ_MEDIUM_PRIZE;
                        cutscene="CS_CHICKING_MIDPRIZE";
                    }
                    ParentClass.csargs["$PRIZE"]="100";
                }
                else if (fscore>=pGlobalScriptDef->CHK_LowPrize) {
                    if (T) return;
                    ParentClass.csargs["$ANIM"]="CHICK_KICK_MID";
                    ParentClass.csargs["$LOOP"]="CHICK_KICK_MID_LOOP";
                    if (ParentClass.PrizesWon & CV_ChickenKickingScript::PRZ_LOW_PRIZE) {
                        if (T) return;
                        if (score<=PMasterData->MaxChickenKickingScore) {
                            if (T) return;
                            CM_LINE("TEXT_QST_B17_MASTER_LOW_PRIZE_AGAIN")
                        }
                        else {
                            if (T) return;
                            PMasterData->MaxChickenKickingScore=score;
                            CM_LINE("TEXT_QST_B17_MASTER_LOW_PRIZE_AGAIN_HIGH")
                        }
                    }
                    else {
                        if (T) return;
                        CM_LINE("TEXT_QST_B17_MASTER_LOW_PRIZE_FIRST")
                        ParentClass.PrizesWon|=CV_ChickenKickingScript::PRZ_LOW_PRIZE;
                        cutscene="CS_CHICKING_LOWPRIZE";
                    }
                    ParentClass.csargs["$PRIZE"]="50";
                }
                else {
                    if (T) return;
                    ParentClass.csargs["$ANIM"]="CHICK_KICK_LOW";
                    ParentClass.csargs["$LOOP"]="CHICK_KICK_LOW_LOOP";
                    if (score<=PMasterData->MaxChickenKickingScore) {
                        if (T) return;
                        CM_LINE("TEXT_QST_B17_MASTER_NO_PRIZE_AGAIN")
                    }
                    else {
                        if (T) return;
                        PMasterData->MaxChickenKickingScore=score;
                        CM_LINE("TEXT_QST_B17_MASTER_NO_PRIZE_AGAIN_HIGH")
                    }
                }
                if (score==PMasterData->MaxChickenKickingScore) {
                    if (T) return;
                    Game->UpdateOnlineScore_ChickenKick(fscore);
                }
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                ParentClass.PlayCutscene(cutscene);
                g_UnprovenWideString_0143E908=L"";
            }
        }
    }
}
