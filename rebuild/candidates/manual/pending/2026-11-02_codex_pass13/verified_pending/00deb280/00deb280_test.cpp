#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <limits>
struct State {
 int frame,limit,checks,stop,things,strings,resources,movies,depth,paused,unpaused,timers,registered,deregistered,sets,reads,faces,starts,retries,predicates,releases,localSay,brotherSay,delay,waits,outros,categories,targets,conversations,people,lines,heroFaction,banditFaction,talkReads,hitReads,specialReads,healReads,victimReads,healthMe,healthLocal,healthBrother,initialHit,finalHit;
 bool talk,rescued,alive,busy,physical,ability,heal,initialValid,finalBusy,succeeded,thanks,killed;long timer;float health,localHealth,brotherHealth;std::vector<std::string>events;
 State():frame(0),limit(2),checks(0),stop(0),things(0),strings(0),resources(0),movies(0),depth(0),paused(0),unpaused(0),timers(0),registered(0),deregistered(0),sets(0),reads(0),faces(0),starts(0),retries(0),predicates(0),releases(0),localSay(0),brotherSay(0),delay(0),waits(0),outros(0),categories(0),targets(0),conversations(0),people(0),lines(0),heroFaction(0),banditFaction(0),talkReads(0),hitReads(0),specialReads(0),healReads(0),victimReads(0),healthMe(0),healthLocal(0),healthBrother(0),initialHit(0),finalHit(0),talk(false),rescued(false),alive(false),busy(false),physical(false),ability(false),heal(false),initialValid(false),finalBusy(false),succeeded(false),thanks(false),killed(false),timer(0),health(100),localHealth(100),brotherHealth(100){}
 bool term(){assert(checks<1000);++checks;return frame>=limit||(stop&&checks>=stop);}void event(const char*s){events.push_back(s);}
};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&s):text(s.text){++state->strings;}~CCharString(){--state->strings;}operator const char*()const{return text.c_str();}};
CCharString operator+(const char*a,const CCharString&b){return CCharString((a+b.text).c_str());}CCharString operator+(const CCharString&a,const char*b){return CCharString((a.text+b).c_str());}
class CScriptThing{public:int id;CScriptThing(int x=-1):id(x){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}CCharString GetDataString()const{assert(id==-1);return "HELPER_A";}bool IsAlive()const{assert(id==11);return state->alive;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->talkReads;return state->talk;}bool MsgIsHitBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->hitReads;return state->physical;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->specialReads;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(int a,const CCharString&s)const{assert(id==-1&&a==14&&s.text=="SCRIPT_NAME_HERO");++state->healReads;return state->heal;}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};enum ETextGroupSelectionMethod{GROUP_SELECT_RANDOM_NO_REPEAT=2};class TRetVoid{public:void Return(){}};
class CScriptGameResourceObjectScriptedThingBase{public:int id,left;bool valid;CScriptGameResourceObjectScriptedThingBase(int n=-1):id(n),left(0),valid(n==-1?state->initialValid:true){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->releases;state->event("release");}CScriptThing GetScriptThing()const{return CScriptThing(id==-1?12:10);}void Speak(const CScriptThing&h,const char*t,int a,bool b,bool c,bool d){assert(h.id==-2&&a==2&&!b&&c&&!d&&state->depth==1&&valid);if(id==-1){assert(std::string(t)=="TEXT_QST_B06_HELPER_A_CHAT");++state->localSay;}else{assert(id==10&&std::string(t)=="TEXT_QST_B06_BROTHER_ON_HIT_HELPERS");++state->brotherSay;}left=state->delay;state->event("say");}bool IsPerformingScriptTask(){++state->waits;return left-->0;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(!state->movies);++state->movies;state->event("movie+");}~CScriptGameResourceObjectMovieBase(){assert(!state->depth);--state->movies;state->event("movie-");}};
class CTimer{long TimerIndex;public:CTimer();~CTimer();bool operator==(long)const;long GetTime()const;CTimer&operator=(long);};
class CGameScriptInterfaceBase{public:long RegisterTimer(){assert(!state->timers);++state->timers;++state->registered;return 73+state->registered;}void DeregisterTimer(long index){assert(index==73+state->registered&&state->timers==1);--state->timers;++state->deregistered;}long GetTimer(long index)const{assert(index==73+state->registered);++state->reads;return state->timer;}void SetTimer(long index,long n){assert(index==73+state->registered&&n==1);state->timer=n;++state->sets;}private:CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void NewScriptFrame(){assert(state->frame<100);++state->frame;state->event("frame");}CScriptThing&GetHero(){return hero;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,int p){assert(m.id==-1&&p==4);++state->starts;r.valid=state->starts>state->retries;return r.valid;}void EntitySetInFaction(const CScriptThing&m,const CCharString&s){assert(m.id==-1);if(s.text=="FACTION_HERO")++state->heroFaction;else{assert(s.text=="FACTION_BANDITS");++state->banditFaction;state->event("bandit");}}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&h,bool b){assert(m.id==-1&&h.id==-2&&!b);++state->faces;}CScriptThing GetThingWithScriptName(const CCharString&s){if(s.text=="Victim"){++state->victimReads;return CScriptThing(11);}assert(s.text=="VictimsBrother");return CScriptThing(10);}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1&&!state->depth);}void PauseAllNonScriptedEntities(bool b){if(b){assert(!state->depth);++state->depth;++state->paused;}else{assert(state->depth==1);--state->depth;++state->unpaused;}}void SetCategoryActivity(const CCharString&s,bool b){assert(s.text=="ChiefsSonDied"&&b&&state->depth==1);++state->categories;}float GetHealth(const CScriptThing&m){if(m.id==-1){++state->healthMe;return state->health;}if(m.id==12){++state->healthLocal;return state->localHealth;}assert(m.id==10);++state->healthBrother;return state->brotherHealth;}long AddNewConversation(const CScriptThing&b,bool c,bool d){assert(b.id==10&&!c&&!d&&!state->depth);++state->conversations;return 91;}void AddPersonToConversation(long n,const CScriptThing&h){assert(n==91&&h.id==-2);++state->people;}void AddLineToConversation(long n,const CCharString&t,bool b,const CScriptThing&a,const CScriptThing&h){assert(n==91&&t.text=="TEXT_QST_B06_BROTHER_ON_RUN_AWAY_10"&&!b&&a.id==10&&h.id==-2);++state->lines;}void GiveThingBestEnemyTarget(const CScriptThing&m,const CScriptThing&h){assert(m.id==-1&&h.id==-2&&!state->depth&&state->banditFaction);++state->targets;state->event("target");}};
CGameScriptInterfaceBase* pDSTGame;
CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
CTimer::~CTimer(){pDSTGame->DeregisterTimer(TimerIndex);}
inline CTimer& CTimer::operator=(long n){pDSTGame->SetTimer(TimerIndex,n);return *this;}
inline long CTimer::GetTime() const{return pDSTGame->GetTimer(TimerIndex);}
inline bool CTimer::operator==(long n) const{return GetTime()==n;}
class CWideScreenMagicPauseEntities{protected:CGameScriptInterfaceBase* Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game):Game(&game){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingAlive{CScriptThing Thing;public:CIsThingAlive(const CScriptThing&t):Thing(t){assert(t.id==-1);}};class CQ_SunnyvaleMasterData{public:bool RansomVictimHaveVictimKilled;CQ_SunnyvaleMasterData():RansomVictimHaveVictimKilled(false){}};class CQ_RansomVictimScript{public:bool RescuedVictim,ThanksCutsceneStarting,MissionSucceeded,SomeoneDealingWithBeingHit;long HitEvilBrotherAndProtection;CScriptGameResourceObjectScriptedThingBase seh_Brother;CQ_RansomVictimScript():RescuedVictim(state->rescued),ThanksCutsceneStarting(false),MissionSucceeded(false),SomeoneDealingWithBeingHit(state->busy),HitEvilBrotherAndProtection(state->initialHit),seh_Brother(10){}void PlayCutscene(CCharString a,CCharString b){assert(a.text=="CS_RANSOM_OUTRO_EVIL"&&b.text=="NULL"&&state->depth==1&&ThanksCutsceneStarting);++state->outros;}class CVictimsBrotherHelper{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_RansomVictimScript&ParentClass;CQ_SunnyvaleMasterData*PMasterData;CVictimsBrotherHelper(CGameScriptInterfaceBase&g,CQ_RansomVictimScript&p,CQ_SunnyvaleMasterData&d):Game(g),ParentClass(p),PMasterData(&d){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CQ_RansomVictimScript::CVictimsBrotherHelper::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer TurningTimer;
    Game.EntitySetInFaction(Me,"FACTION_HERO");
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (TurningTimer==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            TurningTimer=1;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.RescuedVictim && Game.GetThingWithScriptName("Victim").IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.ThanksCutsceneStarting=true;
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                ParentClass.PlayCutscene("CS_RANSOM_OUTRO_EVIL","NULL");
                PMasterData->RansomVictimHaveVictimKilled=true;
                ParentClass.MissionSucceeded=true;
                Game.SetCategoryActivity("ChiefsSonDied",true);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B06_"+Me.GetDataString()+"_CHAT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (!ParentClass.SomeoneDealingWithBeingHit && (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.SomeoneDealingWithBeingHit=true;
            ++ParentClass.HitEvilBrotherAndProtection;
            if (ParentClass.HitEvilBrotherAndProtection==1 && Game.GetHealth(Me)>0.0f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(ParentClass.seh_Brother.GetScriptThing())>0.0f) {
                        ParentClass.seh_Brother.Speak(Game.GetHero(),"TEXT_QST_B06_BROTHER_ON_HIT_HELPERS",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (ParentClass.seh_Brother.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                ParentClass.SomeoneDealingWithBeingHit=false;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing Brother=Game.GetThingWithScriptName("VictimsBrother");
                long HLStyleConversationIndex=Game.AddNewConversation(Brother,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B06_BROTHER_ON_RUN_AWAY_10",false,Brother,Game.GetHero());
                Game.EntitySetInFaction(Me,"FACTION_BANDITS");
                Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
                if (seh_me.IsValid()) seh_me.Release();
                for (;;) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (ParentClass.HitEvilBrotherAndProtection>1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetInFaction(Me,"FACTION_BANDITS");
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            if (seh_me.IsValid()) seh_me.Release();
            for (;;) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

static int cases=0;
void run(State&s,bool complete){state=&s;++cases;bool hit=!s.busy&&(s.physical||(s.ability&&!s.heal));bool outro=s.talk&&s.rescued&&s.alive;bool first=hit&&s.initialHit+1==1&&s.health>0;bool chat=s.talk&&!outro&&s.localHealth>0;bool brother=first&&s.brotherHealth>0;bool revenge=hit&&!first;bool hostile=revenge||s.initialHit+(hit?1:0)>1;
 if(complete)s.limit=2+s.retries+(outro?1:0)+s.delay*((chat?1:0)+(brother?1:0));
 {CGameScriptInterfaceBase game;pDSTGame=&game;NScript::CQ_RansomVictimScript parent;NScript::CQ_SunnyvaleMasterData data;NScript::CQ_RansomVictimScript::CVictimsBrotherHelper me(game,parent,data);me.Main();s.finalHit=(int)parent.HitEvilBrotherAndProtection;s.finalBusy=parent.SomeoneDealingWithBeingHit;s.succeeded=parent.MissionSucceeded;s.thanks=parent.ThanksCutsceneStarting;s.killed=data.RansomVictimHaveVictimKilled;
  if(complete){assert(s.predicates==1&&s.starts==s.retries+1);assert(s.registered==1&&s.deregistered==1);assert(s.heroFaction==1&&s.talkReads==1);assert(s.faces==(s.timer==1&&s.sets?1:0));assert(s.sets==s.faces);assert(s.reads==1);assert(s.outros==(outro?1:0)&&s.categories==s.outros);assert(s.succeeded==outro&&s.killed==outro&&s.thanks==outro);assert(s.victimReads==((s.talk&&s.rescued)?1:0));assert(s.localSay==(chat?1:0)&&s.brotherSay==(brother?1:0));assert(s.healthLocal==(s.talk&&!outro?1:0));assert(s.healthMe==(hit&&s.initialHit+1==1?1:0));assert(s.healthBrother==(first?1:0));assert(s.paused==(s.talk?1:0)+(first?1:0));assert(s.finalHit==s.initialHit+(hit?1:0));assert(s.finalBusy==(hit?!first:s.busy));assert(s.hitReads==(!s.busy?1:0));assert(s.specialReads==(!s.busy&&!s.physical?1:0));assert(s.healReads==(!s.busy&&!s.physical&&s.ability?1:0));assert(s.conversations==(revenge?1:0)&&s.people==s.conversations&&s.lines==s.conversations);assert(s.targets==(hostile?1:0)&&s.banditFaction==s.targets);assert(s.releases==(s.initialValid?1:0)+(hostile?1:0));}
 }
 assert(!s.things&&!s.strings&&!s.resources&&!s.movies&&!s.depth&&!s.timers);assert(s.paused==s.unpaused&&s.registered==s.deregistered);
}
int main(){for(int mask=0;mask<128;++mask){State s;s.talk=(mask&1)!=0;s.rescued=(mask&2)!=0;s.alive=(mask&4)!=0;s.busy=(mask&8)!=0;s.physical=(mask&16)!=0;s.ability=(mask&32)!=0;s.heal=(mask&64)!=0;run(s,true);}for(int hit=-2;hit<=3;++hit)for(int mask=0;mask<16;++mask){State s;s.initialHit=hit;s.physical=(mask&1)!=0;s.ability=(mask&2)!=0;s.heal=(mask&4)!=0;s.busy=(mask&8)!=0;run(s,true);}float health[4]={100,0,-10,std::numeric_limits<float>::quiet_NaN()};for(int a=0;a<4;++a)for(int b=0;b<4;++b)for(int c=0;c<4;++c){State s;s.physical=true;s.talk=true;s.health=health[a];s.localHealth=health[b];s.brotherHealth=health[c];run(s,true);}for(int delay=0;delay<4;++delay)for(int retries=0;retries<4;++retries)for(int mode=0;mode<4;++mode){State s;s.delay=delay;s.retries=retries;s.talk=true;s.physical=true;s.rescued=s.alive=(mode&1)!=0;s.initialValid=(mode&2)!=0;run(s,true);}for(int timer=-2;timer<=3;++timer){State s;s.timer=timer;run(s,true);assert(s.faces==(timer==0?1:0));}for(int stop=1;stop<=100;++stop)for(int mode=0;mode<12;++mode){State s;s.stop=stop;s.limit=12;s.talk=(mode&1)!=0;s.physical=(mode&2)!=0;s.rescued=s.alive=(mode&4)!=0;s.busy=(mode&8)!=0;s.initialHit=mode%3;s.retries=mode%4;s.delay=mode%4;s.initialValid=mode%2!=0;run(s,false);}assert(cases==1558);std::puts("00deb280_TEST PASS");return 0;}
