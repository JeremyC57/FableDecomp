#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <limits>
struct State {
 int frame,limit,checks,stop,things,strings,resources,movies,pause,paused,unpaused,timers,registered,deregistered,predicates,starts,releases,retries,delay,leverDelay,leverReads,healthReads,talkReads,hitReads,specialReads,healReads,enemyReads,comboReads,flourishReads,randomReads,timerReads,timerSets,objectives,opened,markers,followed,region,music,setup,conversations,lines,speech[2],randoms[3];
 bool found,talk,physical,ability,heal,enemy,flourish,valid; int combo; long timer; float health; std::string line;
 State():frame(0),limit(2),checks(0),stop(0),things(0),strings(0),resources(0),movies(0),pause(0),paused(0),unpaused(0),timers(0),registered(0),deregistered(0),predicates(0),starts(0),releases(0),retries(0),delay(0),leverDelay(0),leverReads(0),healthReads(0),talkReads(0),hitReads(0),specialReads(0),healReads(0),enemyReads(0),comboReads(0),flourishReads(0),randomReads(0),timerReads(0),timerSets(0),objectives(0),opened(0),markers(0),followed(0),region(0),music(0),setup(0),conversations(0),lines(0),found(false),talk(false),physical(false),ability(false),heal(false),enemy(false),flourish(false),valid(false),combo(0),timer(0),health(100) {speech[0]=speech[1]=0;randoms[0]=randoms[1]=randoms[2]=1;}
 bool term(){assert(checks<3000);++checks;return frame>=limit||(stop&&checks>=stop);}
};State* state;
class CCharString {public:std::string text;CCharString(const char*p):text(p){++state->strings;}CCharString(const CCharString&o):text(o.text){++state->strings;}~CCharString(){--state->strings;}};
class CScriptThing {public:int id;CScriptThing(int n=-1):id(n){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}bool MsgIsUsedBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");return state->leverReads++>=state->leverDelay;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->talkReads;return state->talk;}bool MsgIsHitBy(const CCharString&s)const{assert(id==-1);if(s.text.empty()){++state->enemyReads;return state->enemy;}assert(s.text=="SCRIPT_NAME_HERO");++state->hitReads;return state->physical;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->specialReads;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(int n,const CCharString&s)const{assert(id==-1&&n==14&&s.text=="SCRIPT_NAME_HERO");++state->healReads;return state->heal;}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};enum ETextGroupSelectionMethod{GROUP_SELECT_RANDOM_NO_REPEAT=2};enum EMusicSetType{MUSIC_SET_DANGER=24};class TRetVoid{public:void Return(){}};
int fixture_rand(){assert(state->randomReads<300);return state->randoms[(state->randomReads++)%3];}
#define rand fixture_rand
class CScriptGameResourceObjectScriptedThingBase {public:bool valid;int left;CScriptGameResourceObjectScriptedThingBase():valid(state->valid),left(0){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(!state->movies&&!state->timers);--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->releases;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(10);}void Speak(const CScriptThing&h,const char*t,int n,bool a,bool b,bool c){assert(valid&&h.id==-2&&n==2&&!a&&b&&!c&&state->pause==1);std::string s(t);if(s=="TEXT_QST_055_MOTHER_LETS_GO")++state->speech[0];else{assert(s=="TEXT_QST_055_MOTHER_ON_HIT");++state->speech[1];}left=state->delay;}bool IsPerformingScriptTask(){return left-->0;}};
class CScriptGameResourceObjectMovieBase {public:CScriptGameResourceObjectMovieBase(){assert(!state->movies);++state->movies;}~CScriptGameResourceObjectMovieBase(){assert(!state->pause);--state->movies;}};
class CTimer {public:CTimer(){assert(!state->timers);++state->timers;++state->registered;}~CTimer(){assert(!state->movies);--state->timers;++state->deregistered;}bool operator==(long n)const{assert(n==0);++state->timerReads;return state->timer==n;}CTimer&operator=(long n){assert(n==15);state->timer=n;++state->timerSets;return *this;}};
class CGameScriptInterfaceBase {CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void NewScriptFrame(){assert(state->frame<100);++state->frame;}CScriptThing&GetHero(){return hero;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,int p){assert(m.id==-1&&p==4);++state->starts;r.valid=(state->starts%(state->retries+1))==0;return r.valid;}void EntitySetAsKillable(const CScriptThing&m,bool a,bool b){assert(m.id==-1&&!a&&!b&&state->setup==0);++state->setup;}void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&m,bool b){assert(m.id==-1&&!b&&state->setup==1);++state->setup;}void MiniMapAddMarker(const CScriptThing&m,const CCharString&s){assert(m.id==-1&&s.text=="HUD_ORB_QUEST_CORE"&&state->setup==2);++state->setup;}CScriptThing GetThingWithScriptName(const CCharString&s){if(s.text=="MotherCellLever")return CScriptThing(1);assert(s.text=="MotherCellDoor");return CScriptThing(2);}void EntitySetTargetable(const CScriptThing&m,bool b){assert(m.id==2&&!b);}void OpenDoor(const CScriptThing&m){assert(m.id==2);++state->opened;}CCharString GetActiveQuestName(){return "Q_PrisonEscape";}void SetQuestCardObjective(const CCharString&q,const CCharString&a,const CCharString&b,const CCharString&c){assert(q.text=="Q_PrisonEscape"&&a.text=="TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06"&&b.text=="GraveyardCircle"&&c.text.empty());++state->objectives;}void MiniMapSetMarkerGraphic(const CScriptThing&m,const CCharString&s){assert(m.id==-1&&s.text=="HUD_ORB_GREEN_SMALL");++state->markers;}void EntitySetDeedReactionsEnabled(const CScriptThing&m,bool b){assert(m.id==-1&&!b&&state->setup==3);++state->setup;}void EntitySetOpinionReactionsEnabled(const CScriptThing&m,bool b){assert(m.id==-1&&!b&&state->setup==4);++state->setup;}void EntitySetWillBeUsingNarrator(const CScriptThing&m,const CCharString&s){assert(m.id==-1&&s.text=="SCARLET_ROBE"&&state->setup==5);++state->setup;}void EntityFollowThing(const CScriptThing&m,const CScriptThing&h,float d,bool b){assert(m.id==-1&&h.id==-2&&d==3&&b);++state->followed;}void SetEntityAsRegionFollowing(const CScriptThing&h,const CScriptThing&m,bool b){assert(h.id==-2&&m.id==-1&&b);++state->region;}void OverrideMusic(EMusicSetType t,bool a,bool b){assert(t==MUSIC_SET_DANGER&&!a&&!b);++state->music;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1&&!state->pause);}void PauseAllNonScriptedEntities(bool b){if(b){assert(!state->pause);++state->pause;++state->paused;}else{assert(state->pause==1);--state->pause;++state->unpaused;}}float GetHealth(const CScriptThing&m){assert(m.id==10&&state->pause==1);++state->healthReads;return state->health;}long GetPlayerCreatureCombatMultiplierRunningNumHits(){++state->comboReads;return state->combo;}bool IsPlayerCreatureFlourishEnabled(){++state->flourishReads;return state->flourish;}long AddNewConversation(const CScriptThing&m,bool a,bool b){assert(m.id==-1&&!a&&!b&&!state->pause);++state->conversations;return 123;}void AddLineToConversation(long n,const CCharString&t,bool b,const CScriptThing&m,const CScriptThing&h){assert(n==123&&!b&&m.id==-1&&h.id==-2);state->line=t.text;++state->lines;}};
class CWideScreenMagicPauseEntities {CGameScriptInterfaceBase&Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(g){Game.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game.PauseAllNonScriptedEntities(false);}};
namespace NScript {class CIsThingAlive {CScriptThing Thing;public:CIsThingAlive(const CScriptThing&t):Thing(t){assert(t.id==-1);}};class CQ_SecretPassageScript {public:bool MotherFound;CQ_SecretPassageScript():MotherFound(state->found){}class CMotherTorture {public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_SecretPassageScript&ParentClass;CMotherTorture(CGameScriptInterfaceBase&g,CQ_SecretPassageScript&p):Game(g),ParentClass(p){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CQ_SecretPassageScript::CMotherTorture::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game.EntitySetAsKillable(Me,false,false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game.MiniMapAddMarker(Me,"HUD_ORB_QUEST_CORE");
    if (!ParentClass.MotherFound) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing MotherLever=Game.GetThingWithScriptName("MotherCellLever");
        CScriptThing MotherCellDoor=Game.GetThingWithScriptName("MotherCellDoor");
        Game.EntitySetTargetable(MotherCellDoor,false);
        while (!MotherLever.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.OpenDoor(MotherCellDoor);
        Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06","GraveyardCircle","");
        Game.MiniMapSetMarkerGraphic(Me,"HUD_ORB_GREEN_SMALL");
        ParentClass.MotherFound=true;
        if (seh_me.IsValid()) seh_me.Release();
    }
    Game.EntitySetDeedReactionsEnabled(Me,false);
    Game.EntitySetOpinionReactionsEnabled(Me,false);
    Game.EntitySetWillBeUsingNarrator(Me,"SCARLET_ROBE");
    Game.EntityFollowThing(Me,Game.GetHero(),3.0f,true);
    Game.SetEntityAsRegionFollowing(Game.GetHero(),Me,true);
    Game.OverrideMusic(MUSIC_SET_DANGER,false,false);
    CTimer MotherSpeechTimer;
    for (;!IsActiveThreadTerminating();Game.NewScriptFrame()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_055_MOTHER_LETS_GO",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_055_MOTHER_ON_HIT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        } else if (MotherSpeechTimer==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_HIT_BY_ENEMY",false,Me,Game.GetHero());
                MotherSpeechTimer=15;
            }
            else if (Game.GetPlayerCreatureCombatMultiplierRunningNumHits()>3 && rand()%100==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_FIGHTING_COMPLIMENT",false,Me,Game.GetHero());
                MotherSpeechTimer=15;
            }
            else if (Game.IsPlayerCreatureFlourishEnabled() && rand()%200==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_USE_FLOURISH",false,Me,Game.GetHero());
                MotherSpeechTimer=15;
            }
            else if (rand()%400==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_NOT_FAR_NOW",false,Me,Game.GetHero());
                MotherSpeechTimer=15;
            }
        }
    }
}

static int cases=0;
void run(State&s,bool complete){state=&s;++cases;bool hit=s.physical||(s.ability&&!s.heal);int speeches=(s.talk?1:0)+(hit?1:0);int acquires=(!s.found?1:0)+speeches;long oldTimer=s.timer;int expectedRandom=0;std::string line;
 if(!hit&&oldTimer==0){if(s.enemy)line="TEXT_QST_055_MOTHER_HIT_BY_ENEMY";else if(s.combo>3&&s.randoms[expectedRandom++]%100==0)line="TEXT_QST_055_MOTHER_FIGHTING_COMPLIMENT";else if(s.flourish&&s.randoms[expectedRandom++]%200==0)line="TEXT_QST_055_MOTHER_USE_FLOURISH";else if(s.randoms[expectedRandom++]%400==0)line="TEXT_QST_055_MOTHER_NOT_FAR_NOW";}
 if(complete)s.limit=2+acquires*s.retries+(!s.found?s.leverDelay:0)+(s.health>0?speeches*s.delay:0);
 {CGameScriptInterfaceBase game;NScript::CQ_SecretPassageScript parent;NScript::CQ_SecretPassageScript::CMotherTorture mother(game,parent);mother.Main();if(complete){assert(parent.MotherFound);assert(s.predicates==1&&s.setup==6&&s.followed==1&&s.region==1&&s.music==1);assert(s.registered==1&&s.deregistered==1);assert(s.starts==acquires*(s.retries+1));assert(s.releases==acquires+(s.valid&&acquires?1:0));assert(s.leverReads==(!s.found?s.leverDelay+1:0));assert(s.opened==(!s.found?1:0)&&s.objectives==s.opened&&s.markers==s.opened);}}
 assert(!s.things&&!s.strings&&!s.resources&&!s.movies&&!s.pause&&!s.timers);assert(s.paused==s.unpaused&&s.registered==s.deregistered);
 if(complete){assert(s.talkReads==1&&s.hitReads==1);assert(s.specialReads==(!s.physical?1:0)&&s.healReads==(!s.physical&&s.ability?1:0));assert(s.paused==speeches&&s.healthReads==speeches);assert(s.speech[0]==(s.talk&&s.health>0?1:0)&&s.speech[1]==(hit&&s.health>0?1:0));assert(s.timerReads==(!hit?1:0));assert(s.enemyReads==(!hit&&oldTimer==0?1:0));assert(s.comboReads==(!hit&&oldTimer==0&&!s.enemy?1:0));assert(s.flourishReads==(!hit&&oldTimer==0&&!s.enemy&&line!="TEXT_QST_055_MOTHER_FIGHTING_COMPLIMENT"?1:0));assert(s.randomReads==expectedRandom&&s.line==line);assert(s.conversations==(line.empty()?0:1)&&s.lines==s.conversations&&s.timerSets==s.conversations);assert(s.timer==(line.empty()?oldTimer:15));}}
int main(){float health[4]={100,0,-5,std::numeric_limits<float>::quiet_NaN()};for(int flags=0;flags<512;++flags)for(int h=0;h<4;++h)for(int random=0;random<6;++random){State s;s.found=(flags&1)!=0;s.talk=(flags&2)!=0;s.physical=(flags&4)!=0;s.ability=(flags&8)!=0;s.heal=(flags&16)!=0;s.enemy=(flags&32)!=0;s.combo=(flags&64)?4:3;s.flourish=(flags&128)!=0;s.valid=(flags&256)!=0;s.health=health[h];int values[6]={0,1,100,200,400,999};s.randoms[0]=values[random];s.randoms[1]=values[(random+2)%6];s.randoms[2]=values[(random+4)%6];run(s,true);}for(int t=-2;t<4;++t)for(int mode=0;mode<8;++mode){State s;s.timer=t;s.talk=(mode&1)!=0;s.physical=(mode&2)!=0;s.enemy=(mode&4)!=0;run(s,true);}for(int r=0;r<4;++r)for(int d=0;d<4;++d)for(int l=0;l<4;++l)for(int f=0;f<2;++f){State s;s.retries=r;s.delay=d;s.leverDelay=l;s.found=f!=0;s.talk=s.physical=true;s.valid=true;run(s,true);}for(int stop=1;stop<=100;++stop)for(int mode=0;mode<32;++mode){State s;s.stop=stop;s.limit=8;s.found=(mode&1)!=0;s.talk=(mode&2)!=0;s.physical=(mode&4)!=0;s.enemy=(mode&8)!=0;s.valid=(mode&16)!=0;s.retries=mode%3;s.delay=mode%3;s.leverDelay=mode%3;run(s,false);}assert(cases==15664);std::puts("00dee2e0_TEST PASS");return 0;}
