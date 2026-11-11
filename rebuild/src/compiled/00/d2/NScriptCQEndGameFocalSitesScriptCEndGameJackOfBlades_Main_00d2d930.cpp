// NScript::CQ_EndGameFocalSitesScript::CEndGameJackOfBlades::Main() -- Jack of Blades at the end-game focal site: marks himself on the mini-map, waits for the hero (near Jack, near the site's hero marker, or a hit), builds the focal site base and active objects, plays the site's CS_FOCALSITE_<n> cutscene with the hero and that site's companion (Scythe, -, Thunder after the hero reaches the Hobbe-cave radius check, Scarlet Scarlet), fades out and teleports the hero on to the next site's HSP, then leaves a teleporter effect marked on the mini-map and records his position.
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: /d2inlS15 expands the four implicit movie scope-end destructors; plain C++ and scoped loop shapes failed in hand821, while S14 stays called and S16 changes other sites.
// Flags: /d2inlS15
// Leaf: ?Main@CEndGameJackOfBlades@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ
// Retail: 0x00D2D930, 5094 bytes, thiscall.
//   0x00D2C260 stores (0 0x00D2C320, 2 Init 0x00D2C310, 3 GetParentScript 0x00D2C300); Ego_r.pdb and FableWin.pdb
//   ?Main@CEndGameJackOfBlades@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ. Not CPermanentBarrier::Main (that is the landed
//   0x00D2D830). CScriptThing::operator= forced inline over the out-of-line CCountedPointer<CScriptThing>::operator= 0x00704580
//   (all four HeroMarker sites expand and tail-merge as retail; plain inline expands only the first). Layout:
//   engine/CQ_EndGameFocalSitesScript.h opt-in FABLE_RETAIL_CQ_ENDGAMEFOCALSITESSCRIPT_JACKOFBLADES (hdr overlay).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTTHING_NULLCOORD
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CQ_ENDGAMEFOCALSITESSCRIPT_JACKOFBLADES
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_EndGameFocalSitesScript.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/EHeroAbility.h"
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
template<> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& other);
FABLE_FORCEINLINE CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }
template<> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>& other);
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp=other.PImp; return *this; }
inline CScriptThing::CScriptThing() { }
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
void NScript::CQ_EndGameFocalSitesScript::CEndGameJackOfBlades::Main()
{
    TRetVoid _Fail_Val;
    Game.MiniMapAddMarker(Me,"HUD_ORB_GREEN_SMALL");
    Game.EntitySetAsDamageable(Me,false);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing HeroMarker;
    if (ParentClass.CurrentFocalSiteRegion==1) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        HeroMarker=Game.GetThingWithScriptName("MK_FS1_HERO");
    }
    else if (ParentClass.CurrentFocalSiteRegion==2) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        HeroMarker=Game.GetThingWithScriptName("MK_FS2_HERO");
    }
    else if (ParentClass.CurrentFocalSiteRegion==3) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        HeroMarker=Game.GetThingWithScriptName("MK_FS3_HERO");
    }
    else {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        HeroMarker=Game.GetThingWithScriptName("MK_FS4_HERO");
    }
    while (!IsDistanceBetweenThingsUnder(Me,Game.GetHero(),15.0f) && !IsDistanceBetweenThingsUnder(HeroMarker,Game.GetHero(),5.0f) && !(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")))) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    ParentClass.JackReached=true;
    CScriptThing FocalSite=Game.GetThingWithScriptName("EndGameFocalSite");
    Game.CreateObject("OBJECT_FOCAL_SITE_BASE_01",FocalSite.GetPos(),"");
    Game.CreateObject("OBJECT_FOCAL_SITE_ACTIVE_01",FocalSite.GetPos(),"EndGameActiveFocalSite");
    ParentClass.JackLeaves=true;
    if (ParentClass.CurrentFocalSiteRegion==1) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing Scythe=Game.GetThingWithScriptName("EndGameScythe");
        CScriptGameResourceObjectScriptedThingBase seh_Scythe;
        if (seh_Scythe.IsValid()) seh_Scythe.Release();
        while (!Game.StartScriptingEntity(Scythe,seh_Scythe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid()) seh_hero.Release();
            while (!Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            mymap["JACK"]=seh_me;
            mymap["SCYTHE"]=seh_Scythe;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FOCALSITE_1",mymap,0,0,false,true);
            Game.FixMovieSequenceCamera(false);
            Game.FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0,255));
        }
        Game.EntityTeleportToThing(Game.GetHero(),Game.GetThingWithScriptName("EndGameSecondHSP"),false);
    }
    else if (ParentClass.CurrentFocalSiteRegion==2) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.TransitionToTheme("ENVIRONMENT_EOW_04",5.0f);
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid()) seh_hero.Release();
            while (!Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            mymap["JACK"]=seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FOCALSITE_2",mymap,0,0,false,true);
            Game.FixMovieSequenceCamera(false);
            Game.FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0,255));
        }
        Game.EntityTeleportToThing(Game.GetHero(),Game.GetThingWithScriptName("HobbeCave2HSP"),false);
    }
    else if (ParentClass.CurrentFocalSiteRegion==3) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!IsDistanceBetweenThingsUnder(Game.GetHero(),Game.GetThingWithScriptName("M_HC_HeroRadiusCheck"),4.0f)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.TransitionToTheme("ENVIRONMENT_EOW_04",5.0f);
        CScriptThing Thunder=Game.GetThingWithScriptName("EndGameThunder");
        {
            CScriptGameResourceObjectScriptedThingBase seh_Thunder;
            if (seh_Thunder.IsValid()) seh_Thunder.Release();
            while (!Game.StartScriptingEntity(Thunder,seh_Thunder,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                if (seh_hero.IsValid()) seh_hero.Release();
                while (!Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["JACK"]=seh_me;
                mymap["THUNDER"]=seh_Thunder;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_FOCALSITE_3",mymap,0,0,false,true);
                Game.FixMovieSequenceCamera(false);
                Game.FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0,255));
            }
        }
        Game.EntityTeleportToThing(Game.GetHero(),Game.GetThingWithScriptName("BowerstoneJailHSP"),false);
    }
    else {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.TransitionToTheme("ENVIRONMENT_EOW_04",5.0f);
        CScriptThing Scarlet=Game.GetThingWithScriptName("EndGameScarletRobe");
        CScriptGameResourceObjectScriptedThingBase seh_Scarlet;
        if (seh_Scarlet.IsValid()) seh_Scarlet.Release();
        while (!Game.StartScriptingEntity(Scarlet,seh_Scarlet,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid()) seh_hero.Release();
            while (!Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            mymap["JACK"]=seh_me;
            mymap["MUM"]=seh_Scarlet;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FOCALSITE_4",mymap,0,0,false,true);
            Game.FixMovieSequenceCamera(false);
            Game.FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0,255));
            Game.RemoveThing(Scarlet,false,true);
        }
        Game.EntityTeleportToThing(Game.GetHero(),Game.GetThingWithScriptName("GuildTrainingHSP"),false);
    }
    ParentClass.EndCutScenePlayed=true;
    CScriptThing Effect=Game.CreateEffect("NEWTELEPORTER2",Me.GetPos(),"TeleporterResidue",0.0f,false,false);
    Game.MiniMapAddMarker(Effect,"HUD_ORB_QUEST_CORE");
    Game.MiniMapRemoveMarker(Game.GetThingWithScriptName("EndGameFocalSite"));
    ParentClass.TeleportPosition=Me.GetPos();
    while (!false) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}