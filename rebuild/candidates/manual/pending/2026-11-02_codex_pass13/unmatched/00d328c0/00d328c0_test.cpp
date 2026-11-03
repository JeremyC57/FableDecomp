// Exact selected00d328c0 body, C++03 ownership and script-event fixture.
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
struct Config {int intro,decision,choice,needed,failMe,failHero,mode,stop,limit;Config():intro(0),decision(0),choice(0),needed(0),failMe(0),failHero(0),mode(0),stop(0),limit(60){}};
static Config cfg;static int frame,checks,phase,ticks,choices,neededFrames,startsMe,startsHero,failedMe,failedHero,strings,things,handles,movies,pauses,predicates,owners[3];static bool stopped,doneNeeded;static std::vector<std::string> events;
static void action(const std::string&s){assert(!stopped);events.push_back(s);}
class CCharString {public:std::string text;CCharString(const char*s=""):text(s){++strings;}CCharString(const CCharString&s):text(s.text){++strings;}~CCharString(){--strings;}bool operator<(const CCharString&s)const{return text<s.text;}};
class CScriptThing {public:int id;CScriptThing(int i=0):id(i){++things;}CScriptThing(const CScriptThing&t):id(t.id){++things;}~CScriptThing(){--things;}bool MsgIsHitByWithWeapon(const CCharString&a,const CCharString&b)const{assert(id==1&&a.text=="SCRIPT_NAME_HERO"&&b.text=="OBJECT_SWORD_OF_AEONS");action("hit-query");return cfg.mode==0&&choices>cfg.choice;}};
class CScriptGameResourceObjectScriptedThingBase {public:int id;CScriptGameResourceObjectScriptedThingBase():id(0){++handles;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&r):id(r.id){++handles;if(id)++owners[id];}~CScriptGameResourceObjectScriptedThingBase(){if(id)--owners[id];--handles;}CScriptGameResourceObjectScriptedThingBase&operator=(const CScriptGameResourceObjectScriptedThingBase&r){if(id)--owners[id];id=r.id;if(id)++owners[id];return *this;}bool IsValid()const{return id!=0;}void Release(){action("release");assert(id);--owners[id];id=0;}void ClearAllActionsIncludingLoopingAnimations(){assert(id==1);action("clear-loop");}void ClearAllActions(){assert(id==1);action("clear-actions");}void ClearCommands(){assert(id==1);action("clear-commands");}};
class CScriptGameResourceObjectMovieBase {public:CScriptGameResourceObjectMovieBase(){++movies;}~CScriptGameResourceObjectMovieBase(){assert(pauses==0);--movies;events.push_back("movie-destroy");}};
class CRGBColour {public:unsigned char R,G,B,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a):R(r),G(g),B(b),A(a){}};
enum EScriptAIPriority {SCRIPT_AI_PRIORITY_HIGHEST=4};
namespace NScript {class CQ_EndGameBossBattleScript;class CIsThingAlive {public:int id;CIsThingAlive(const CScriptThing&t):id(t.id){++predicates;}~CIsThingAlive(){--predicates;}};class CQ_SunnyvaleMasterData{public:long JackBossBattleResult;};}
static NScript::CQ_EndGameBossBattleScript*parent;
class GameMock {public:CScriptThing hero;GameMock():hero(2){}void NewScriptFrame();CScriptThing&GetHero(){return hero;}bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert((t.id==1||t.id==2)&&p==SCRIPT_AI_PRIORITY_HIGHEST&&!r.id);action(t.id==1?"start-me":"start-hero");int&start=t.id==1?startsMe:startsHero;int&failed=t.id==1?failedMe:failedHero;++start;if(failed<(t.id==1?cfg.failMe:cfg.failHero)){++failed;return false;}r.id=t.id;++owners[t.id];if(t.id==1&&phase==4){phase=5;ticks=0;}return true;}
void EntitySetTargetable(const CScriptThing&t,bool b){assert(t.id==1);action(b?"target-on":"target-off");}void EntitySetAsDamageable(const CScriptThing&t,bool b){assert(t.id==1);action(b?"damage-on":"damage-off");}void SetIsPushableByHero(CScriptThing t,bool b){assert(t.id==1&&!b);action("push-off");}void EntitySetAsKillable(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&b);action("kill-off");}void SetIsThingEpicSpellable(const CScriptThing&t,bool b){assert(t.id==1);action(b?"epic-on":"epic-off");}void SetThingAsConscious(const CScriptThing&t,bool b,const CCharString&s){assert(t.id==1&&s.text==(b?"":"FALL_UNCONSCIOUS"));action(b?"conscious-on":"conscious-off");}void SetIsThingForcePushable(const CScriptThing&t,bool b){assert(t.id==1&&!b);action("force-off");}void EntitySetEvadeAllHits(const CScriptThing&t,bool b){assert(t.id==1);action(b?"evade-on":"evade-off");if(b){phase=1;ticks=0;}}void ModifyThingHealth(const CScriptThing&t,float f,bool b){assert(t.id==1&&f==10000.0f&&!b);action("health");}void FadeScreenOut(float a,float b,CRGBColour c){assert(a==1.3f&&b==0.5f&&c.R==0&&c.G==0&&c.B==0&&c.A==255);action("fade");}void Pause(float f){assert(f==0.5f);action("pause");}
CScriptThing GetThingWithScriptName(const CCharString&s){int id=s.text=="EndGameSister"?1:s.text=="MK_JACK1_HOLDJACK"?3:s.text=="MK_END1_SISTER_STOP"?4:s.text=="CS_AEONS"?5:0;assert(id);action("get:"+s.text);return CScriptThing(id);}void EntityTeleportToThing(const CScriptThing&a,const CScriptThing&b,bool c){assert(a.id==1&&(b.id==3||b.id==4)&&!c);action(b.id==3?"teleport-hold":"teleport-stop");}void EntitySetAsDrawable(const CScriptThing&t,bool b){assert(t.id==1);action(b?"draw-on":"draw-off");}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&movies==1);action("movie");}void PauseAllNonScriptedEntities(bool b){if(b){action("pause-entities");++pauses;}else{assert(pauses==1);--pauses;events.push_back("unpause-entities");}}void RemoveThing(const CScriptThing&t,bool a,bool b){assert(t.id==5&&!a&&b);action("remove-aeons");}void GiveHeroWeapon(const CCharString&s,bool b){assert(s.text=="OBJECT_SWORD_OF_AEONS"&&!b);action("give-sword");}void SetWeaponAsHerosActiveWeapon(const CCharString&s){assert(s.text=="OBJECT_SWORD_OF_AEONS");action("equip-sword");phase=3;ticks=0;}};
class CWideScreenMagicPauseEntities {GameMock&game;public:CWideScreenMagicPauseEntities(GameMock&g):game(g){g.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){game.PauseAllNonScriptedEntities(false);}};
namespace NScript {class CQ_EndGameBossBattleScript {public:bool IntroFinished,FinalDecision,FinalDecisionMade,SisterNeededForCutscene,GuildmasterChat,FinalCutscenePlayed;CScriptGameResourceObjectScriptedThingBase seh_Jack;CScriptThing Jack;
class CEndGameSister{GameMock&Game;CScriptThing Me;CQ_EndGameBossBattleScript&ParentClass;CQ_SunnyvaleMasterData*PMasterData;void Main();public:CEndGameSister(GameMock&g,CQ_EndGameBossBattleScript&p,CQ_SunnyvaleMasterData&m):Game(g),Me(1),ParentClass(p),PMasterData(&m){}void Run(){Main();}void SetPredicate(const CIsThingAlive&p){assert(p.id==1);action("predicate");}bool IsActiveThreadTerminating()const{++checks;if((cfg.stop&&checks>=cfg.stop)||frame>=cfg.limit){stopped=true;return true;}return false;}};};}
void GameMock::NewScriptFrame(){action("frame");++frame;++ticks;if(phase==0&&ticks>cfg.intro)parent->IntroFinished=true;if(phase==1&&ticks>cfg.decision)parent->FinalDecision=true;if(phase==3){++choices;if(cfg.mode==1&&!doneNeeded&&choices>cfg.choice){parent->SisterNeededForCutscene=true;phase=4;ticks=0;}if(cfg.mode==2&&choices>cfg.choice)parent->FinalDecisionMade=true;}else if(phase==4){++neededFrames;if(ticks>cfg.needed){parent->SisterNeededForCutscene=false;doneNeeded=true;}}else if(phase==5){parent->FinalDecisionMade=true;}}
static void RunCutsceneMacro(const CCharString&s,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,void*a,void*b,bool c,bool d){assert(!a&&!b&&!c&&d&&movies==1&&pauses==1&&m.find("Hero")!=m.end()&&m.find("Hero")->second.id==2);if(s.text=="CS_JACK_DEATH"){assert(m.size()==1);action("death");}else{assert(s.text=="CS_JACK_BOSS_ENDING_EVIL"&&m.size()==2&&m.find("Sister")->second.id==1&&parent->FinalDecisionMade);action("evil");}}
void NScript::CQ_EndGameBossBattleScript::CEndGameSister::Main()

{
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    while(!ParentClass.IntroFinished) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    Game.EntitySetTargetable(Me,false);
    Game.EntitySetAsDamageable(Me,false);
    Game.SetIsPushableByHero(Me,false);
    Game.EntitySetAsKillable(Me,false,true);
    Game.SetIsThingEpicSpellable(Me,false);
    Game.SetThingAsConscious(Me,false,"FALL_UNCONSCIOUS");
    Game.SetIsThingForcePushable(Me,false);
    Game.EntitySetEvadeAllHits(Me,true);
    while(!ParentClass.FinalDecision) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    Game.EntitySetTargetable(Me,true);
    Game.EntitySetAsDamageable(Me,true);
    Game.EntitySetEvadeAllHits(Me,false);
    Game.ModifyThingHealth(Me,10000.0f,false);
    Game.SetIsThingEpicSpellable(Me,true);
    {
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if(seh_Hero.IsValid()) seh_Hero.Release();
        while(!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating()) return;
        }
        if(IsActiveThreadTerminating()) return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["Hero"]=seh_Hero;
        Game.FadeScreenOut(1.3f,0.5f,CRGBColour(0,0,0,255));
        Game.Pause(0.5f);
        Game.SetThingAsConscious(Me,true,"");
        seh_me.ClearAllActionsIncludingLoopingAnimations();
        seh_me.ClearAllActions();
        seh_me.ClearCommands();
        Game.EntityTeleportToThing(Game.GetThingWithScriptName("EndGameSister"),Game.GetThingWithScriptName("MK_JACK1_HOLDJACK"),false);
        Game.EntitySetAsDrawable(Game.GetThingWithScriptName("EndGameSister"),false);
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        RunCutsceneMacro("CS_JACK_DEATH",mymap,0,0,false,true);
        Game.EntityTeleportToThing(Game.GetThingWithScriptName("EndGameSister"),Game.GetThingWithScriptName("MK_END1_SISTER_STOP"),false);
        Game.EntitySetAsDrawable(Game.GetThingWithScriptName("EndGameSister"),true);
        Game.RemoveThing(Game.GetThingWithScriptName("CS_AEONS"),false,true);
        Game.GiveHeroWeapon("OBJECT_SWORD_OF_AEONS",false);
        Game.SetWeaponAsHerosActiveWeapon("OBJECT_SWORD_OF_AEONS");
    }
    while(!ParentClass.FinalDecisionMade) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
        if(Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO","OBJECT_SWORD_OF_AEONS")) {
            if(IsActiveThreadTerminating()) return;
            ParentClass.FinalDecisionMade=true;
            PMasterData->JackBossBattleResult=1;
            CScriptGameResourceObjectScriptedThingBase seh_Hero;
            if(seh_Hero.IsValid()) seh_Hero.Release();
            while(!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["Hero"]=seh_Hero;
            mymap["Sister"]=seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            RunCutsceneMacro("CS_JACK_BOSS_ENDING_EVIL",mymap,0,0,false,true);
            ParentClass.FinalCutscenePlayed=true;
        } else if(ParentClass.SisterNeededForCutscene) {
            if(IsActiveThreadTerminating()) return;
            if(seh_me.IsValid()) seh_me.Release();
            while(ParentClass.SisterNeededForCutscene) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            if(seh_me.IsValid()) seh_me.Release();
            while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
        }
    }
    while(!IsActiveThreadTerminating()) Game.NewScriptFrame();
}

struct Result{std::vector<std::string> events;int checks,me,hero,needed;bool decision,played,guild;long result;};
static Result run(const Config&c){cfg=c;frame=checks=phase=ticks=choices=neededFrames=startsMe=startsHero=failedMe=failedHero=0;stopped=doneNeeded=false;events.clear();Result r;{GameMock g;NScript::CQ_EndGameBossBattleScript p;parent=&p;p.IntroFinished=p.FinalDecision=p.FinalDecisionMade=p.SisterNeededForCutscene=p.FinalCutscenePlayed=false;p.GuildmasterChat=true;NScript::CQ_SunnyvaleMasterData m;m.JackBossBattleResult=-77;NScript::CQ_EndGameBossBattleScript::CEndGameSister t(g,p,m);t.Run();r.decision=p.FinalDecisionMade;r.played=p.FinalCutscenePlayed;r.guild=p.GuildmasterChat;r.result=m.JackBossBattleResult;}assert(strings==0&&things==0&&handles==0&&movies==0&&pauses==0&&predicates==0&&owners[1]==0&&owners[2]==0);r.events=events;r.checks=checks;r.me=startsMe;r.hero=startsHero;r.needed=neededFrames;return r;}
static int count(const Result&r,const char*s){int n=0;for(unsigned i=0;i<r.events.size();++i)if(r.events[i]==s)++n;return n;}
static void before(const Result&r,const char*a,const char*b){unsigned x=0,y=0;while(x<r.events.size()&&r.events[x]!=a)++x;while(y<r.events.size()&&r.events[y]!=b)++y;assert(x<y&&y<r.events.size());}
int main(){int scenarios=0;for(int mode=0;mode<3;++mode)for(int delay=0;delay<3;++delay)for(int failure=0;failure<3;++failure){Config c;c.mode=mode;c.intro=delay;c.decision=2-delay;c.choice=delay;c.needed=delay;c.failMe=failure;c.failHero=2-failure;Result r=run(c);++scenarios;assert(count(r,"death")==1&&r.guild&&r.decision);assert(count(r,"evil")==int(mode==0)&&r.played==(mode==0)&&r.result==(mode==0?1:-77));assert(r.me==1+failure+int(mode==1));assert(r.hero==1+(2-failure)+int(mode==0));assert(count(r,"release")==int(mode==1));assert(count(r,"movie")==1+int(mode==0));assert(count(r,"pause-entities")==count(r,"unpause-entities"));const char*order[]={"predicate","target-off","damage-off","push-off","kill-off","epic-off","conscious-off","force-off","evade-on","target-on","damage-on","evade-off","health","epic-on","fade","pause","conscious-on","clear-loop","clear-actions","clear-commands","teleport-hold","draw-off","movie","death","teleport-stop","draw-on","remove-aeons","give-sword","equip-sword","unpause-entities","movie-destroy"};for(unsigned k=1;k<sizeof(order)/sizeof(order[0]);++k)before(r,order[k-1],order[k]);if(mode==0)before(r,"death","evil");for(int stop=1;stop<=r.checks;++stop){c.stop=stop;run(c);++scenarios;}}
Config c;for(int limit=0;limit<30;++limit){c.limit=limit;run(c);++scenarios;}assert(scenarios>500);std::puts("00d328c0_TEST PASS");}
