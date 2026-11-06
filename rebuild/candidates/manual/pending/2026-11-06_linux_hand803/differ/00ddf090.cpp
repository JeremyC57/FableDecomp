// NScript::CQ_PrisonWardenGameScript::CPrisonWarden::Main() -- the warden poetry minigame: intro cutscene, a noise bar and a poem bar, the warden reads poem lines while the hero sneaks; noise or being seen plays the caught cutscene, a full poem bar ends the game, an opened diary plays the book cutscene (and hands over the cell key on later visits).
// Grade: DIFFER(4455v4599) -- 669 lines: retail expands the first two cast-map stores (CBaseObject::operator= 0x0099A3B0 + the inline CCountedPointer share) and the seh_me dtor at the first start-scripting exit; this build calls ??4 0x008ABD10 at all eight stores (a __forceinline operator= expands all eight, 4989) and frames the locals differently (seh_me +0x38 vs +0x28, PoemBar at +0xB0 in retail).
// Next idea: find what spends VC7.1's inline budget before the intro block in this build (retail inlines the intro block in full: map ctor, seh_hero ctor, movie ctor, both operator= stores) -- compare the TU prelude with a landed cast-map row that expands its first store inline.
// Leaf: ?Main@CPrisonWarden@CQ_PrisonWardenGameScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SPRINTF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CQ_PRISONWARDENGAMESCRIPT_ENTITIES
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_PrisonWardenGameScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
void NScript::CQ_PrisonWardenGameScript::CPrisonWarden::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->EntitySheatheWeapons(Me,false);
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"]=seh_hero;
        mymap["GUARD"]=seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        if (PMasterData->PrisonGameDoneBefore) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            RunCutsceneMacro("CS_PRISON_GAME_INTRO_SHORT",mymap,0,0,false,true);
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            PMasterData->PrisonGameDoneBefore=true;
            RunCutsceneMacro("CS_PRISON_GAME_INTRO",mymap,0,0,false,true);
        }
        Game->FixMovieSequenceCamera(false);
    }
    Game->Pause(0.5f);
    Game->DisplayQuestInfo(true);
    long PoemBar=Game->AddQuestInfoBar((float)pGlobalScriptDef->PrisonPoetryMaxLines,0.0f,CRGBColour(255,0,0,255),CRGBColour(0,255,0,255),"HUD_CLOCK_ICON","",1.0f);
    long NoiseBar=Game->AddQuestInfoBar(1.0f,0.0f,CRGBColour(255,0,0,255),CRGBColour(0,255,0,255),"HUD_QUEST_ICON_GUARD","",1.0f);
    Game->FadeScreenIn();
    float Noise=0.0f;
    long PoemLines=0;
    long Conversation=-1;
    C3DVector LastHeroPos=Game->GetHero().GetPos();
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        float Moved=(Game->GetHero().GetPos()-LastHeroPos).GetMagnitude();
        if (Game->CanThingBe_Heard_ByOtherThing(Game->GetHero(),Me)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Noise+=Moved*pGlobalScriptDef->PrisonWardenGameSneakModifier;
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Moved>0.0f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Noise+=pGlobalScriptDef->PrisonWardenGameSneakModifier*0.4f*Moved;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Noise-=0.01f;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(),Me) || Noise>=1.0f) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->UpdateQuestInfoBar(NoiseBar,1.0f,-1.0f,-1.0f);
            Game->RemoveConversation(Conversation,false);
            PoemLines=PoemLines-1;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            mymap["GUARD"]=seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_PRISON_GAME_NOISY",mymap,0,0,false,true);
            Game->FixMovieSequenceCamera(false);
            Noise=0.0f;
        }
        else if (Noise<0.0f) Noise=0.0f;
        else if (Noise>1.0f) Noise=1.0f;
        Game->UpdateQuestInfoBar(NoiseBar,Noise,-1.0f,-1.0f);
        if (!ParentClass.SecretDiaryOpened) {
            if (Conversation==-1 || !Game->IsConversationActive(Conversation)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                PoemLines=PoemLines+1;
                if (PoemLines>=pGlobalScriptDef->PrisonPoetryMaxLines) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    {
                        CScriptGameResourceObjectScriptedThingBase seh_hero;
                        Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                        mymap["HERO"]=seh_hero;
                        mymap["GUARD"]=seh_me;
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        Game->FixMovieSequenceCamera(true);
                        RunCutsceneMacro("CS_PRISON_GAME_FINISHED",mymap,0,0,false,true);
                        Game->FixMovieSequenceCamera(false);
                    }
                    Game->DeactivateQuestLater("Q_PrisonWardenGame",1);
                    for (;;) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Conversation=Game->AddNewConversation(Me,false,false);
                CCharString PoemLine;
                PoemLine.Sprintf("TEXT_QST_055_WARDEN_POEM_%d_%.2d",PMasterData->PrisonRaceNumber%3+1,PoemLines);
                Game->AddLineToConversation(Conversation,PoemLine,false,Me,CScriptThing());
                Game->UpdateQuestInfoBar(PoemBar,(float)PoemLines,-1.0f,-1.0f);
            }
            LastHeroPos=Game->GetHero().GetPos();
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->RemoveConversation(Conversation,false);
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["GUARD"]=seh_me;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_PRISON_GAME_BOOK_START",mymap,0,0,false,false);
                CCharString BookName("OBJECT_BOOK_OPEN_01");
                CScriptThing HeroReadPoint;
                if (ParentClass.BookOpened==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    HeroReadPoint=Game->GetThingWithScriptName("HeroReadBookRed");
                    BookName="OBJECT_BOOK_OPEN_01";
                }
                else if (ParentClass.BookOpened==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    HeroReadPoint=Game->GetThingWithScriptName("HeroReadBookGreen");
                    BookName="OBJECT_BOOK_OPEN_04";
                }
                else if (ParentClass.BookOpened==2) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    HeroReadPoint=Game->GetThingWithScriptName("HeroReadBookBlue");
                    BookName="OBJECT_BOOK_OPEN_02";
                }
                Game->EntityTeleportToThing(Game->GetHero(),HeroReadPoint,false);
                Game->EntitySetFacingAngle(Game->GetHero(),HeroReadPoint.GetAngleXY(),true);
                CScriptThing OpenBook=Game->CreateObject(BookName,ParentClass.SThingBookOpened.GetPos(),"");
                Game->EntitySetFacingAngle(OpenBook,ParentClass.SThingBookOpened.GetAngleXY()+0.25f,true);
                Game->RemoveThing(ParentClass.SThingBookOpened,false,true);
                if (PMasterData->PrisonRaceNumber>=1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    RunCutsceneMacro("CS_PRISON_GAME_BOOK_KEY",mymap,0,0,false,true);
                    Game->MakeHeroCarryItemInHand("");
                    Game->TakeObjectFromHero("OBJECT_PRISON_CELL_KEY");
                    Game->GiveHeroObject("OBJECT_PRISON_CELL_KEY",-1,false);
                    PMasterData->PrisonKeyStolenByHero=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    RunCutsceneMacro("CS_PRISON_GAME_BOOK_EMPTY",mymap,0,0,false,true);
                }
                Game->FixMovieSequenceCamera(false);
            }
            Game->DeactivateQuestLater("Q_PrisonWardenGame",1);
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
