// Complete reconstructed Main body with deterministic callee stubs.
#include <cstdio>
#include <cstring>
#include <map>
class C3DVector { public: float X,Y,Z;C3DVector():X(0),Y(0),Z(0){} };
class CRGBColour { public: CRGBColour(int,int,int,int){} };
class CCharString { public: const char* Text;CCharString(const char* t=""):Text(t){}bool operator<(const CCharString& x)const{return std::strcmp(Text,x.Text)<0;} };
struct GameStub;static GameStub* Current;static int Frames,Resources;
class CScriptThing { public:
 const char* Name;CScriptThing(const char* n=""):Name(n){}
 const C3DVector& GetPos()const{static C3DVector p;return p;}
 bool MsgIsHitBy(const char*)const;
 bool MsgIsHitByAnySpecialAbilityFrom(const char*)const;
 bool MsgIsHitBySpecialAbilityFrom(int,const char*)const;
};
class CScriptGameResourceObjectScriptedThingBase { public:
 const char* Name;bool Valid;
 CScriptGameResourceObjectScriptedThingBase():Name(""),Valid(false){++Resources;}
 CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& x):Name(x.Name),Valid(x.Valid){++Resources;}
 ~CScriptGameResourceObjectScriptedThingBase(){--Resources;}
 bool IsValid()const{return Valid;}void Release(){Valid=false;}
};
class CScriptGameResourceObjectMovieBase { public:CScriptGameResourceObjectMovieBase(){++Resources;}~CScriptGameResourceObjectMovieBase(){--Resources;} };
struct GameStub {
 int TerminateAt,ScriptDelays,Objects,Cutscenes,Transitions,Removed,Markers,Movies;
 bool Near,Radius,NormalHit,AbilityHit,Heal;const char* Scene;const char* Destination;const char* Companion;CScriptThing Hero;
 void MiniMapAddMarker(const CScriptThing&,const char*){++Markers;}
 void EntitySetAsDamageable(const CScriptThing&,bool){}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int){if(ScriptDelays>0){--ScriptDelays;return false;}h.Name=t.Name;h.Valid=true;return true;}
 void NewScriptFrame(){++Frames;}const CScriptThing& GetHero(){return Hero;}
 CScriptThing GetThingWithScriptName(const char* n){return CScriptThing(n);}
 void CreateObject(const char*,const C3DVector&,const char*){++Objects;}
 void StartMovieSequence(const char*,CScriptGameResourceObjectMovieBase&){++Movies;}
 void FixMovieSequenceCamera(bool){}void FadeScreenOut(float,float,const CRGBColour&){}
 void EntityTeleportToThing(const CScriptThing&,const CScriptThing& marker,bool){Destination=marker.Name;}
 void TransitionToTheme(const char*,float){++Transitions;}void RemoveThing(const CScriptThing&,bool,bool){++Removed;}
 CScriptThing CreateEffect(const char*,const C3DVector&,const char* n,float,bool,bool){return CScriptThing(n);}
 void MiniMapRemoveMarker(const CScriptThing&){}
};
bool CScriptThing::MsgIsHitBy(const char*)const{return Current->NormalHit;}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const char*)const{return Current->AbilityHit;}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int,const char*)const{return Current->Heal;}
struct CWideScreenMagicPauseEntities {CWideScreenMagicPauseEntities(GameStub&){++Resources;}~CWideScreenMagicPauseEntities(){--Resources;} };
struct ParentStub {int CurrentFocalSiteRegion;bool JackReached,JackLeaves,EndCutScenePlayed;C3DVector TeleportPosition;};
enum {SCRIPT_AI_PRIORITY_HIGHEST=4,HERO_ABILITY_HEAL_LIFE_SPELL=14};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float r){return r==4.0f?Current->Radius:Current->Near;}
static void RunCutsceneMacro(const char* scene,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& cast,int,int,bool,bool){
 ++Current->Cutscenes;Current->Scene=scene;Current->Companion="";
 if(cast.find(CCharString("SCYTHE"))!=cast.end())Current->Companion=cast[CCharString("SCYTHE")].Name;
 if(cast.find(CCharString("THUNDER"))!=cast.end())Current->Companion=cast[CCharString("THUNDER")].Name;
 if(cast.find(CCharString("MUM"))!=cast.end())Current->Companion=cast[CCharString("MUM")].Name;
}
struct TRetVoid {void Return()const{} };
struct Jack {GameStub* Game;CScriptThing Me;ParentStub ParentClass;bool IsActiveThreadTerminating()const{return Frames>=Game->TerminateAt;}void Main();};
void Jack::Main()

{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
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
}
static void Reset(GameStub& g,Jack& t,int region){
 Frames=0;Resources=0;Current=&g;g.TerminateAt=16;g.ScriptDelays=0;g.Objects=g.Cutscenes=g.Transitions=g.Removed=g.Markers=g.Movies=0;
 g.Near=g.Radius=true;g.NormalHit=g.AbilityHit=g.Heal=false;g.Scene=g.Destination=g.Companion="";g.Hero.Name="Hero";t.Game=&g;t.Me.Name="Jack";
 t.ParentClass.CurrentFocalSiteRegion=region;t.ParentClass.JackReached=t.ParentClass.JackLeaves=t.ParentClass.EndCutScenePlayed=false;
}
static bool Eq(const char*a,const char*b){return std::strcmp(a,b)==0;}
int main(){bool ok=true;GameStub g;Jack t;const char* scenes[]={"CS_FOCALSITE_1","CS_FOCALSITE_2","CS_FOCALSITE_3","CS_FOCALSITE_4"};const char* dest[]={"EndGameSecondHSP","HobbeCave2HSP","BowerstoneJailHSP","GuildTrainingHSP"};const char* companions[]={"EndGameScythe","","EndGameThunder","EndGameScarletRobe"};
 for(int region=1;region<=4;++region){Reset(g,t,region);g.ScriptDelays=2;t.Main();ok=ok&&g.Objects==2&&g.Cutscenes==1&&g.Movies==1&&g.Markers==2&&Eq(g.Scene,scenes[region-1])&&Eq(g.Destination,dest[region-1])&&Eq(g.Companion,companions[region-1])&&g.Transitions==(region==1?0:1)&&g.Removed==(region==4?1:0)&&t.ParentClass.JackReached&&t.ParentClass.JackLeaves&&t.ParentClass.EndCutScenePlayed&&Resources==0;}
 Reset(g,t,1);g.Near=false;g.TerminateAt=2;t.Main();ok=ok&&g.Objects==0&&g.Cutscenes==0&&!t.ParentClass.JackReached&&Resources==0;
 Reset(g,t,1);g.Near=false;g.AbilityHit=g.Heal=true;g.TerminateAt=2;t.Main();ok=ok&&g.Objects==0&&g.Cutscenes==0&&Resources==0;
 Reset(g,t,1);g.Near=false;g.NormalHit=g.Heal=true;t.Main();ok=ok&&g.Cutscenes==1&&g.Objects==2&&Resources==0;
 Reset(g,t,3);g.Radius=false;g.TerminateAt=2;t.Main();ok=ok&&g.Objects==2&&g.Cutscenes==0&&t.ParentClass.JackReached&&!t.ParentClass.EndCutScenePlayed&&Resources==0;
 Reset(g,t,4);g.TerminateAt=0;t.Main();ok=ok&&g.Objects==0&&g.Cutscenes==0&&Resources==0;
 std::printf(ok?"00d2d930_TEST PASS\n":"00d2d930_TEST FAIL\n");return ok?0:1;
}