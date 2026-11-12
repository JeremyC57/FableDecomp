// NScript::CQ_PrisonWardenGameScript::CPrisonWarden::Main() -- the warden poetry minigame: intro cutscene, a noise bar and a poem bar, the warden reads poem lines while the hero sneaks; noise or being seen plays the caught cutscene, a full poem bar ends the game, an opened diary plays the book cutscene (and hands over the cell key on later visits).
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: entry cast assignments expand; scoped depth0 keeps the six cold assignments called at retail008ABD10. Plain source and row budgets failed before the existing force-inline assignment option was selected.
// Match: 4599 bytes outside audited relocations. Original donor branch ownership and grouped native float product recover the retail body.
// Leaf: ?Main@CPrisonWarden@CQ_PrisonWardenGameScript@NScript@@EAEXXZ
// Retail: 0x00DDF090, 4599 bytes, thiscall.
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN
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
#define FABLE_RETAIL_CQ_PRISONWARDENGAMESCRIPT_MASTER_ACCESSOR
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
#include "engine/GFLimit.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/EHeroAbility.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
void NScript::CQ_PrisonWardenGameScript::CPrisonWarden::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.EntitySheatheWeapons(Me,false);
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"]=seh_hero;
        mymap["GUARD"]=seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        Game.FixMovieSequenceCamera(true);
        if (GetMasterData()->PrisonGameDoneBefore) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            RunCutsceneMacro("CS_PRISON_GAME_INTRO_SHORT",mymap,0,0,false,true);
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            GetMasterData()->PrisonGameDoneBefore=true;
            RunCutsceneMacro("CS_PRISON_GAME_INTRO",mymap,0,0,false,true);
        }
        Game.FixMovieSequenceCamera(false);
    }
    Game.Pause(0.5f);
    Game.DisplayQuestInfo(true);
    long PoemTimeBarIndex=Game.AddQuestInfoBar((float)pGlobalScriptDef->PrisonPoetryMaxLines,0.0f,CRGBColour(255,0,0,255),CRGBColour(0,255,0,255),"HUD_CLOCK_ICON","",1.0f);
    long SneakNoiseIndex=Game.AddQuestInfoBar(1.0f,0.0f,CRGBColour(255,0,0,255),CRGBColour(0,255,0,255),"HUD_QUEST_ICON_GUARD","",1.0f);
    Game.FadeScreenIn();
    float HeroNoiseValue=0.0f;
    long CurrentLineOfPoetry=0;
    long ConversationPoetryIndex=-1;
    C3DVector OldHeroPos=Game.GetHero().GetPos();
    float DistanceTravelled=0.0f;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        DistanceTravelled=(Game.GetHero().GetPos()-OldHeroPos).GetMagnitude();
        if (Game.CanThingBe_Heard_ByOtherThing(Game.GetHero(),Me)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            HeroNoiseValue+=DistanceTravelled*pGlobalScriptDef->PrisonWardenGameSneakModifier;
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (DistanceTravelled>0.0f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                HeroNoiseValue+=(pGlobalScriptDef->PrisonWardenGameSneakModifier*0.4f)*DistanceTravelled;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                HeroNoiseValue-=0.01f;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(),Me) || HeroNoiseValue>=1.0f) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.UpdateQuestInfoBar(SneakNoiseIndex,1.0f,-1.0f,-1.0f);
            Game.RemoveConversation(ConversationPoetryIndex,false);
            CurrentLineOfPoetry=CurrentLineOfPoetry-1;
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
// Inline-budget: retail calls both cast assignments at this cold site; entry assignments expand.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            mymap["HERO"]=seh_hero;
            mymap["GUARD"]=seh_me;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_PRISON_GAME_NOISY",mymap,0,0,false,true);
            Game.FixMovieSequenceCamera(false);
            HeroNoiseValue=0.0f;
        }
        GFLimit(HeroNoiseValue,0,1);
        Game.UpdateQuestInfoBar(SneakNoiseIndex,HeroNoiseValue,-1.0f,-1.0f);
        if (ParentClass.SecretDiaryOpened) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.RemoveConversation(ConversationPoetryIndex,false);
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
// Inline-budget: retail calls both cast assignments at this cold site; entry assignments expand.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                mymap["HERO"]=seh_hero;
                mymap["GUARD"]=seh_me;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_PRISON_GAME_BOOK_START",mymap,0,0,false,false);
                CCharString ObjectName("OBJECT_BOOK_OPEN_01");
                CScriptThing BookMarker;
                if (ParentClass.BookOpened==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    BookMarker=Game.GetThingWithScriptName("HeroReadBookRed");
                    ObjectName="OBJECT_BOOK_OPEN_01";
                }
                else if (ParentClass.BookOpened==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    BookMarker=Game.GetThingWithScriptName("HeroReadBookGreen");
                    ObjectName="OBJECT_BOOK_OPEN_04";
                }
                else if (ParentClass.BookOpened==2) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    BookMarker=Game.GetThingWithScriptName("HeroReadBookBlue");
                    ObjectName="OBJECT_BOOK_OPEN_02";
                }
                Game.EntityTeleportToThing(Game.GetHero(),BookMarker,false);
                Game.EntitySetFacingAngle(Game.GetHero(),BookMarker.GetAngleXY(),true);
                CScriptThing NewBook=Game.CreateObject(ObjectName,ParentClass.SThingBookOpened.GetPos(),"");
                Game.EntitySetFacingAngle(NewBook,ParentClass.SThingBookOpened.GetAngleXY()+0.25f,true);
                Game.RemoveThing(ParentClass.SThingBookOpened,false,true);
                if (GetMasterData()->PrisonRaceNumber>=1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    RunCutsceneMacro("CS_PRISON_GAME_BOOK_KEY",mymap,0,0,false,true);
                    Game.MakeHeroCarryItemInHand("");
                    Game.TakeObjectFromHero("OBJECT_PRISON_CELL_KEY");
                    Game.GiveHeroObject("OBJECT_PRISON_CELL_KEY",-1,false);
                    GetMasterData()->PrisonKeyStolenByHero=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    RunCutsceneMacro("CS_PRISON_GAME_BOOK_EMPTY",mymap,0,0,false,true);
                }
                Game.FixMovieSequenceCamera(false);
            }
            Game.DeactivateQuestLater("Q_PrisonWardenGame",1);
            while (true) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }

        if (ConversationPoetryIndex==-1 || !Game.IsConversationActive(ConversationPoetryIndex)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CurrentLineOfPoetry=CurrentLineOfPoetry+1;
            if (CurrentLineOfPoetry>=pGlobalScriptDef->PrisonPoetryMaxLines) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
// Inline-budget: retail calls both cast assignments at this cold site; entry assignments expand.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    mymap["HERO"]=seh_hero;
                    mymap["GUARD"]=seh_me;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    Game.FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_PRISON_GAME_FINISHED",mymap,0,0,false,true);
                    Game.FixMovieSequenceCamera(false);
                }
                Game.DeactivateQuestLater("Q_PrisonWardenGame",1);
                while (true) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                CCharString poem_line;
                poem_line.Sprintf("TEXT_QST_055_WARDEN_POEM_%d_%.2d",GetMasterData()->PrisonRaceNumber%3+1,CurrentLineOfPoetry);
                Game.AddLineToConversation(HLStyleConversationIndex,poem_line,false,Me,CScriptThing());
                ConversationPoetryIndex=HLStyleConversationIndex;
                Game.UpdateQuestInfoBar(PoemTimeBarIndex,(float)CurrentLineOfPoetry,-1.0f,-1.0f);
            }
        }
        OldHeroPos=Game.GetHero().GetPos();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}