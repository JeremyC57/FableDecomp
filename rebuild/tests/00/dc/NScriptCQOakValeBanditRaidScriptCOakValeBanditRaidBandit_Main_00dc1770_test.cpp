// Exact selected body, with independent branch, argument, map ownership and cleanup assertions.
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <limits>
struct State {
 int frame,limit,checks,stop,things,strings,resources,movies,depth,paused,unpaused,predicates,attached,openUntil,recallAt,initialCounter,counter,spotted,findCalls,healthCalls,heroHits,anyHits,specialHits,healHits,seenCalls,equalCalls,soundStops,targets,clears,cutscenes,which,killFalse,killTrue,behaviourPaused,behaviourFree,started[3],failures[3];
 bool aware,awareAgain,gate,whistle,inrange,same,physical,ability,heal,any,seen,sound;float health;std::vector<std::string> events;
 State():frame(0),limit(1),checks(0),stop(0),things(0),strings(0),resources(0),movies(0),depth(0),paused(0),unpaused(0),predicates(0),attached(0),openUntil(0),recallAt(100),initialCounter(0),counter(0),spotted(-1),findCalls(0),healthCalls(0),heroHits(0),anyHits(0),specialHits(0),healHits(0),seenCalls(0),equalCalls(0),soundStops(0),targets(0),clears(0),cutscenes(0),which(0),killFalse(0),killTrue(0),behaviourPaused(0),behaviourFree(0),aware(false),awareAgain(false),gate(false),whistle(false),inrange(false),same(false),physical(false),ability(false),heal(false),any(false),seen(false),sound(false),health(100){for(int i=0;i<3;++i)started[i]=failures[i]=0;}
 bool term(){++checks;return frame>=limit||(stop&&checks>=stop);}
 void event(const char*s){events.push_back(s);}
};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&s):text(s.text){++state->strings;}~CCharString(){--state->strings;}bool operator<(const CCharString&o)const{return text<o.text;}};
class CScriptThing{public:int id;CScriptThing(int i=-1):id(i){++state->things;}CScriptThing(const CScriptThing&s):id(s.id){++state->things;}~CScriptThing(){--state->things;}bool operator==(const CScriptThing&o)const{assert(o.id==-1&&id==99);++state->equalCalls;return state->same;}bool MsgIsHitBy(const CCharString&s)const{assert(id==-1);if(s.text.empty()){++state->anyHits;return state->any;}assert(s.text=="SCRIPT_NAME_HERO");++state->heroHits;return state->physical;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->specialHits;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(int a,const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO"&&a==14);++state->healHits;return state->heal;}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};enum ECutsceneBehaviour{CUTSCENE_BEHAVIOUR_DEFAULT=0,CUTSCENE_BEHAVIOUR_PAUSED=1,CUTSCENE_BEHAVIOUR_NOT_PAUSED=2};class TRetVoid{public:void Return(){}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;int id;CScriptGameResourceObjectScriptedThingBase():valid(false),id(0){++state->resources;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&o):valid(o.valid),id(o.id){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}CScriptGameResourceObjectScriptedThingBase&operator=(const CScriptGameResourceObjectScriptedThingBase&o){valid=o.valid;id=o.id;return *this;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(!state->movies);++state->movies;state->event("movie+");}~CScriptGameResourceObjectMovieBase(){assert(!state->depth);--state->movies;state->event("movie-");}};
void advanceParent();
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void NewScriptFrame(){assert(state->frame<255);++state->frame;advanceParent();state->event("frame");}CScriptThing&GetHero(){return hero;}void EntityAttachToScript(const CScriptThing&m,const CCharString&s){assert(m.id==-1&&s.text=="Q_OakValeBanditRaid");++state->attached;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,int p){assert(p==4);int i=m.id==-2?0:m.id==-1?1:2;assert(i!=2||(m.id==10||m.id==11));++state->started[i];r.valid=state->started[i]>state->failures[i];r.id=m.id;return r.valid;}void EntitySetCutsceneBehaviour(const CScriptThing&m,ECutsceneBehaviour b){assert(m.id==-1);if(b==CUTSCENE_BEHAVIOUR_PAUSED)++state->behaviourPaused;else{assert(b==CUTSCENE_BEHAVIOUR_NOT_PAUSED);++state->behaviourFree;}}float GetHealth(const CScriptThing&m){assert(m.id==-1);++state->healthCalls;return state->health;}bool CanThingBe_Seen_ByOtherThing(const CScriptThing&a,const CScriptThing&b){assert(a.id==-2&&b.id==-1);++state->seenCalls;return state->seen;}bool IsSoundPlaying(unsigned long s){assert(s==51);return state->sound;}void StopSound(unsigned long s){assert(s==51&&state->sound);++state->soundStops;state->event("stopSound");}void EntitySetAsKillable(const CScriptThing&m,bool a,bool b){assert(m.id==-1&&b);if(a){assert(state->cutscenes==1);++state->killTrue;}else ++state->killFalse;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1&&!state->depth);}void PauseAllNonScriptedEntities(bool b){if(b){assert(!state->depth);++state->depth;++state->paused;}else{assert(state->depth==1);--state->depth;++state->unpaused;}}void GiveThingBestEnemyTarget(const CScriptThing&a,const CScriptThing&b){assert(a.id==-1&&b.id==-2&&!state->depth);++state->targets;}void ClearThingBestEnemyTarget(const CScriptThing&m){assert(m.id==-1&&!state->depth);++state->clears;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase&Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(g){Game.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game.PauseAllNonScriptedEntities(false);}};
void RunCutsceneMacro(const CCharString&n,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,void*a,void*b,bool c,bool d){assert(!a&&!b&&!c&&d&&state->depth==1);assert(m["HERO"].id==-2&&m["HERO"].valid&&m["ME"].id==-1&&m["ME"].valid);if(n.text=="CS_BANDITRAID_GATECLOSE_MERC"){assert(m.size()==3&&m["MERC"].id==10+state->spotted&&m["MERC"].valid);state->which=2;}else{assert(n.text=="CS_BANDITRAID_GATECLOSE"&&m.size()==2);state->which=1;}++state->cutscenes;state->event("cutscene");}
namespace NScript{class CIsThingConscious{CScriptThing Thing;public:CIsThingConscious(const CScriptThing&t):Thing(t){assert(t.id==-1);}};class CQ_OakValeBanditRaidScript{public:bool BanditsAwareOfPlayer,BanditsAwareOfPlayerAgain,GateClosed;std::vector<CScriptThing>HerosFollowers;bool OpenGates;long SoundPlayed;bool StopWhistling,IsBanditInRange;CScriptThing BanditInRange;long BanditsStopChasing;CQ_OakValeBanditRaidScript():BanditsAwareOfPlayer(state->aware),BanditsAwareOfPlayerAgain(false),GateClosed(false),OpenGates(state->openUntil>0),SoundPlayed(51),StopWhistling(false),IsBanditInRange(state->inrange),BanditInRange(99),BanditsStopChasing(state->initialCounter){HerosFollowers.push_back(CScriptThing(10));HerosFollowers.push_back(CScriptThing(11));}long CanThingSeeAnythingInVector(CScriptThing&m,std::vector<CScriptThing>&v){assert(m.id==-1&&&v==&HerosFollowers);++state->findCalls;return state->spotted;}class COakValeBanditRaidBandit{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_OakValeBanditRaidScript&ParentClass;COakValeBanditRaidBandit(CGameScriptInterfaceBase&g,CQ_OakValeBanditRaidScript&p):Game(g),ParentClass(p){}void SetPredicate(const CIsThingConscious&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}NScript::CQ_OakValeBanditRaidScript*parent;
void advanceParent(){if(state->frame>=state->openUntil)parent->OpenGates=false;if(state->frame==state->recallAt)parent->BanditsStopChasing=1;}

void NScript::CQ_OakValeBanditRaidScript::COakValeBanditRaidBandit::Main()
{
    TRetVoid _Fail_Val;
    long spotted;
    SetPredicate(CIsThingConscious(Me));
    Game.EntityAttachToScript(Me,"Q_OakValeBanditRaid");
    while (ParentClass.OpenGates) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.EntitySetCutsceneBehaviour(Me,CUTSCENE_BEHAVIOUR_PAUSED);
    for (;;) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.BanditsAwareOfPlayer) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            spotted=ParentClass.CanThingSeeAnythingInVector(Me,ParentClass.HerosFollowers);
            if (ParentClass.BanditsStopChasing==0 && Game.GetHealth(Me)>0.0f &&
                (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Me.MsgIsHitBy("") || Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(),Me) || spotted>=0 ||
                 (ParentClass.IsBanditInRange && ParentClass.BanditInRange==Me))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.IsBanditInRange=false;
                ParentClass.StopWhistling=true;
                ParentClass.BanditsAwareOfPlayer=true;
                if (Game.IsSoundPlaying(ParentClass.SoundPlayed)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.StopSound(ParentClass.SoundPlayed);
                }
                Game.EntitySetAsKillable(Me,false,true);
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                if (seh_hero.IsValid()) seh_hero.Release();
                while (!Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_me;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetCutsceneBehaviour(Me,CUTSCENE_BEHAVIOUR_NOT_PAUSED);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["ME"]=seh_me;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (spotted>=0 && !(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) && !Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(),Me)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptThing ICU=ParentClass.HerosFollowers[spotted];
                    CScriptGameResourceObjectScriptedThingBase seh_merc;
                    if (seh_merc.IsValid()) seh_merc.Release();
                while (!Game.StartScriptingEntity(ICU,seh_merc,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    mymap["MERC"]=seh_merc;
                    RunCutsceneMacro("CS_BANDITRAID_GATECLOSE_MERC",mymap,0,0,false,true);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    RunCutsceneMacro("CS_BANDITRAID_GATECLOSE",mymap,0,0,false,true);
                }
                ParentClass.GateClosed=true;
                Game.EntitySetCutsceneBehaviour(Me,CUTSCENE_BEHAVIOUR_PAUSED);
                Game.EntitySetAsKillable(Me,true,true);
            }
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            bool GoBackToWandering;
            do {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                spotted=ParentClass.CanThingSeeAnythingInVector(Me,ParentClass.HerosFollowers);
                if (Game.GetHealth(Me)>0.0f && (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Me.MsgIsHitBy("") || Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(),Me) || spotted>=0)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.BanditsAwareOfPlayerAgain=true;
                }
                GoBackToWandering=ParentClass.BanditsStopChasing!=0;
            } while (!GoBackToWandering);
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            --ParentClass.BanditsStopChasing;
            Game.ClearThingBestEnemyTarget(Me);
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        Game.NewScriptFrame();
    }
}


void run(State&s){state=&s;{CGameScriptInterfaceBase g;NScript::CQ_OakValeBanditRaidScript p;parent=&p;NScript::CQ_OakValeBanditRaidScript::COakValeBanditRaidBandit t(g,p);t.Main();s.aware=p.BanditsAwareOfPlayer;s.awareAgain=p.BanditsAwareOfPlayerAgain;s.gate=p.GateClosed;s.whistle=p.StopWhistling;s.inrange=p.IsBanditInRange;s.counter=p.BanditsStopChasing;assert(s.predicates==1&&s.attached==1&&s.things==5&&s.strings==0&&s.resources==0&&s.movies==0&&!s.depth&&s.paused==s.unpaused);}assert(!s.things&&!s.strings&&!s.resources);}
int main(){int cases=0;
 for(int flags=0;flags<128;++flags)for(int spot=-1;spot<=1;++spot){State s;s.physical=(flags&1)!=0;s.ability=(flags&2)!=0;s.heal=(flags&4)!=0;s.any=(flags&8)!=0;s.seen=(flags&16)!=0;s.inrange=(flags&32)!=0;s.same=(flags&64)!=0;s.spotted=spot;s.sound=true;bool heroHit=s.physical||(s.ability&&!s.heal);bool detect=heroHit||s.any||s.seen||spot>=0||(s.inrange&&s.same);bool merc=spot>=0&&!heroHit&&!s.seen;run(s);assert(s.aware==detect&&s.whistle==detect&&s.gate==detect&&s.cutscenes==int(detect));assert(s.which==(detect?(merc?2:1):0)&&s.soundStops==int(detect)&&s.killFalse==int(detect)&&s.killTrue==int(detect));assert(s.started[0]==int(detect)&&s.started[1]==int(detect)&&s.started[2]==int(detect&&merc));assert(s.behaviourPaused==1+int(detect)&&s.behaviourFree==int(detect)&&s.targets==0);++cases;}
 for(int h=0;h<4;++h)for(int counter=-1;counter<=1;++counter){State s;s.physical=true;s.health=h==0?100.0f:h==1?0.0f:h==2?-1.0f:std::numeric_limits<float>::quiet_NaN();s.initialCounter=counter;run(s);bool detect=h==0&&counter==0;assert(s.cutscenes==int(detect)&&s.aware==detect&&s.healthCalls==int(counter==0));assert(s.heroHits==int(detect));++cases;}
 for(int mask=0;mask<32;++mask){State s;s.aware=true;s.limit=2;s.recallAt=1;s.physical=(mask&1)!=0;s.ability=(mask&2)!=0;s.heal=(mask&4)!=0;s.any=(mask&8)!=0;s.seen=(mask&16)!=0;bool detect=s.physical||(s.ability&&!s.heal)||s.any||s.seen;run(s);assert(s.targets==1&&s.clears==1&&s.counter==0&&s.cutscenes==0&&s.awareAgain==detect&&s.findCalls==1);++cases;}
 for(int spot=-1;spot<=1;++spot){State s;s.aware=true;s.spotted=spot;s.limit=2;s.recallAt=1;run(s);assert(s.awareAgain==(spot>=0)&&s.clears==1);++cases;}
 for(int h=0;h<4;++h){State s;s.aware=true;s.physical=true;s.limit=2;s.recallAt=1;s.health=h==0?100.0f:h==1?0.0f:h==2?-1.0f:std::numeric_limits<float>::quiet_NaN();run(s);assert(s.awareAgain==(h==0)&&s.clears==1&&s.counter==0&&s.heroHits==int(h==0));++cases;}
 for(int open=0;open<4;++open)for(int actor=0;actor<3;++actor)for(int fail=0;fail<4;++fail){State s;s.limit=20;s.openUntil=open;s.spotted=1;s.failures[actor]=fail;s.recallAt=15;run(s);assert(s.cutscenes==1&&s.which==2&&s.started[actor]==fail+1&&s.gate&&s.clears==1);++cases;}
 for(int actor=0;actor<3;++actor){State s;s.spotted=0;s.limit=5;s.failures[actor]=100;run(s);assert(s.cutscenes==0&&s.killFalse==1&&s.killTrue==0&&s.started[actor]>=4&&s.started[(actor+1)%3]<=1);++cases;}
 {State s;s.openUntil=100;s.limit=5;run(s);assert(!s.findCalls&&!s.behaviourPaused&&!s.cutscenes);++cases;}
 {State s;s.aware=true;s.limit=5;run(s);assert(s.targets==1&&!s.clears&&s.findCalls==4);++cases;}
 for(int mode=0;mode<12;++mode)for(int stop=1;stop<=100;++stop){State s;s.stop=stop;s.limit=20;s.physical=mode==1;s.any=mode==2;s.seen=mode==3;s.spotted=mode>=4&&mode<=8?1:-1;s.sound=mode==5;s.failures[0]=mode==6?3:0;s.failures[1]=mode==7?3:0;s.failures[2]=mode==8?3:0;s.aware=mode>=9;s.recallAt=mode>=9?5:100;s.openUntil=mode==11?3:0;run(s);assert(s.checks<=stop);++cases;}
 assert(cases==1688);std::puts("00dc1770_TEST PASS");}