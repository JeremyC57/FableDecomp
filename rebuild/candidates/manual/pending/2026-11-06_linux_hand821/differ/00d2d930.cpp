// NScript::CQ_EndGameFocalSitesScript::CEndGameJackOfBlades::Main() -- Jack of Blades at the end-game focal site: marks himself on the mini-map, waits for the hero (near Jack, near the site's hero marker, or a hit), builds the focal site base and active objects, plays the site's CS_FOCALSITE_<n> cutscene with the hero and that site's companion (Scythe, -, Thunder after the hero reaches the Hobbe-cave radius check, Scarlet Robe), fades out and teleports the hero on to the next site's HSP, then leaves a teleporter effect marked on the mini-map and records his position.
// Grade: DIFFER(5005v5094) -- the speech MovieBase dtor at the four cutscene scope exits only (0x00D2E028 / 0x00D2E2DC / 0x00D2E768 / 0x00D2EB30): retail expands the compiler-generated dtor (`call 0x006E7AB0` PImp ; `mov [esp+0x80],0x0126008C` ; `call 0x0099A430`), ours calls the COMDAT 0x006E7B80; the rest (the four MarkerHero assignments, the tail merges) matches
// Next idea: (chunk 821) the c2 diagnostic knob inlS=15 (never a landing flag) gives RELOCATION_MATCH (S14 = default, S16 flips other inlines): the four scope-end sites lose to c2's caller-size inline penalty (IL + growth past the inlS cap) -- do-while(0) movie scopes, implicit movie ctor, implicit STB op= and a trimmed tail do not lift it; lane gap (inliner budget), park
// Leaf: ?Main@CEndGameJackOfBlades@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2D930, 5094 bytes. Name: slot 1 of the entity vtable 0x012CC2E8 that the landed Alloc
//   0x00D2C260 stores (0 0x00D2C320, 2 Init 0x00D2C310, 3 GetParentScript 0x00D2C300); Ego_r.pdb and FableWin.pdb
//   ?Main@CEndGameJackOfBlades@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ. Not CPermanentBarrier::Main (that is the landed
//   0x00D2D830). CScriptThing::operator= forced inline over the out-of-line CCountedPointer<CScriptThing>::operator= 0x00704580
//   (all four MarkerHero sites expand and tail-merge as retail; plain inline expands only the first). Layout:
//   engine/CQ_EndGameFocalSitesScript.h opt-in FABLE_RETAIL_CQ_ENDGAMEFOCALSITESSCRIPT_JACKOFBLADES (hdr overlay).
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
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
template<> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& other);
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }
template<> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>& other);
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp=other.PImp; return *this; }
inline CScriptThing::CScriptThing() { }
__forceinline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
void NScript::CQ_EndGameFocalSitesScript::CEndGameJackOfBlades::Main()
{
    Game->MiniMapAddMarker(Me,"HUD_ORB_GREEN_SMALL");
    Game->EntitySetAsDamageable(Me,false);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing MarkerHero;
    if (ParentClass.CurrentFocalSiteRegion==1) {
        if (IsActiveThreadTerminating()) return;
        MarkerHero=Game->GetThingWithScriptName("MK_FS1_HERO");
    }
    else if (ParentClass.CurrentFocalSiteRegion==2) {
        if (IsActiveThreadTerminating()) return;
        MarkerHero=Game->GetThingWithScriptName("MK_FS2_HERO");
    }
    else if (ParentClass.CurrentFocalSiteRegion==3) {
        if (IsActiveThreadTerminating()) return;
        MarkerHero=Game->GetThingWithScriptName("MK_FS3_HERO");
    }
    else {
        if (IsActiveThreadTerminating()) return;
        MarkerHero=Game->GetThingWithScriptName("MK_FS4_HERO");
    }
    while (!IsDistanceBetweenThingsUnder(Me,Game->GetHero(),15.0f) && !IsDistanceBetweenThingsUnder(MarkerHero,Game->GetHero(),5.0f) && !(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    ParentClass.JackReached=true;
    CScriptThing FocalSite=Game->GetThingWithScriptName("EndGameFocalSite");
    Game->CreateObject("OBJECT_FOCAL_SITE_BASE_01",FocalSite.GetPos(),"");
    Game->CreateObject("OBJECT_FOCAL_SITE_ACTIVE_01",FocalSite.GetPos(),"EndGameActiveFocalSite");
    ParentClass.JackLeaves=true;
    if (ParentClass.CurrentFocalSiteRegion==1) {
        if (IsActiveThreadTerminating()) return;
        CScriptThing Scythe=Game->GetThingWithScriptName("EndGameScythe");
        CScriptGameResourceObjectScriptedThingBase seh_scythe;
        if (seh_scythe.IsValid()) seh_scythe.Release();
        while (!Game->StartScriptingEntity(Scythe,seh_scythe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid()) seh_hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            mymap["JACK"]=seh_me;
            mymap["SCYTHE"]=seh_scythe;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FOCALSITE_1",mymap,0,0,false,true);
            Game->FixMovieSequenceCamera(false);
            Game->FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0,255));
        }
        Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("EndGameSecondHSP"),false);
    }
    else if (ParentClass.CurrentFocalSiteRegion==2) {
        if (IsActiveThreadTerminating()) return;
        Game->TransitionToTheme("ENVIRONMENT_EOW_04",5.0f);
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid()) seh_hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            mymap["JACK"]=seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FOCALSITE_2",mymap,0,0,false,true);
            Game->FixMovieSequenceCamera(false);
            Game->FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0,255));
        }
        Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("HobbeCave2HSP"),false);
    }
    else if (ParentClass.CurrentFocalSiteRegion==3) {
        if (IsActiveThreadTerminating()) return;
        while (!IsDistanceBetweenThingsUnder(Game->GetHero(),Game->GetThingWithScriptName("M_HC_HeroRadiusCheck"),4.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->TransitionToTheme("ENVIRONMENT_EOW_04",5.0f);
        CScriptThing Thunder=Game->GetThingWithScriptName("EndGameThunder");
        {
            CScriptGameResourceObjectScriptedThingBase seh_thunder;
            if (seh_thunder.IsValid()) seh_thunder.Release();
            while (!Game->StartScriptingEntity(Thunder,seh_thunder,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                if (seh_hero.IsValid()) seh_hero.Release();
                while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["JACK"]=seh_me;
                mymap["THUNDER"]=seh_thunder;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_FOCALSITE_3",mymap,0,0,false,true);
                Game->FixMovieSequenceCamera(false);
                Game->FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0,255));
            }
        }
        Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("BowerstoneJailHSP"),false);
    }
    else {
        if (IsActiveThreadTerminating()) return;
        Game->TransitionToTheme("ENVIRONMENT_EOW_04",5.0f);
        CScriptThing Robe=Game->GetThingWithScriptName("EndGameScarletRobe");
        CScriptGameResourceObjectScriptedThingBase seh_robe;
        if (seh_robe.IsValid()) seh_robe.Release();
        while (!Game->StartScriptingEntity(Robe,seh_robe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid()) seh_hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"]=seh_hero;
            mymap["JACK"]=seh_me;
            mymap["MUM"]=seh_robe;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FOCALSITE_4",mymap,0,0,false,true);
            Game->FixMovieSequenceCamera(false);
            Game->FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0,255));
            Game->RemoveThing(Robe,false,true);
        }
        Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("GuildTrainingHSP"),false);
    }
    ParentClass.EndCutScenePlayed=true;
    CScriptThing Teleporter=Game->CreateEffect("NEWTELEPORTER2",Me.GetPos(),"TeleporterResidue",0.0f,false,false);
    Game->MiniMapAddMarker(Teleporter,"HUD_ORB_QUEST_CORE");
    Game->MiniMapRemoveMarker(Game->GetThingWithScriptName("EndGameFocalSite"));
    ParentClass.TeleportPosition=Me.GetPos();
    while (!false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
}
