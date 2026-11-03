#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
static int strings=0,things=0,handles=0,movies=0,pauses=0,predicates=0,owners=0,timers=0,timerReads=0,timerSets=0,nearHeroReads=0,nearOtherReads=0;
static long timerValue;
static std::vector<std::string> ambient;
static int conversations,people;
static int frame,checks,failures,tasks,starts,talkCalls,hitCalls,anyCalls,healCalls,healthCalls;
static bool stopped=false;
static std::vector<std::string> events,speeches;
static void action(const char*s){assert(!stopped);events.push_back(s);}
class CCharString{public:std::string text;CCharString():text(){++strings;}CCharString(const char*s):text(s){++strings;}CCharString(const CCharString&s):text(s.text){++strings;}CCharString&operator=(const CCharString&s){text=s.text;return *this;}~CCharString(){--strings;}operator const char*()const{return text.c_str();}};
static CCharString GFIntToCharString(long n){char b[50];std::sprintf(b,"%ld",n);return CCharString(b);}
static CCharString operator+(const char*s,const CCharString&t){return CCharString((std::string(s)+t.text).c_str());}
struct Config{bool talk,hit,special,heal,otherAlive,mushroom;int fail,wait,stop,limit,talked,hits,maxLine;float health;bool nearHero,nearOther;int timer,tick;Config():talk(false),hit(false),special(false),heal(false),otherAlive(true),mushroom(false),fail(0),wait(0),stop(0),limit(6),talked(0),hits(0),maxLine(20),health(100),nearHero(true),nearOther(true),timer(0),tick(1){}};
static Config config;
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};
class CScriptThing{public:int id;CScriptThing(int i=0):id(i){++things;}CScriptThing(const CScriptThing&t):id(t.id){++things;}~CScriptThing(){--things;}bool IsAlive()const{assert(id==3);return config.otherAlive;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");++talkCalls;return config.talk;}bool MsgIsHitBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");++hitCalls;return config.hit;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");++anyCalls;return config.special;}bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{assert(id==1&&a==14&&s.text=="SCRIPT_NAME_HERO");++healCalls;return config.heal;}};
class CScriptGameResourceObjectScriptedThingBase{bool owned;public:CScriptGameResourceObjectScriptedThingBase():owned(false){++handles;}~CScriptGameResourceObjectScriptedThingBase(){if(owned)--owners;--handles;}void acquire(){assert(!owned);owned=true;++owners;}bool IsValid()const{return owned;}void Release(){action("release");if(owned)--owners;owned=false;}CScriptThing GetScriptThing()const{assert(owned);return CScriptThing(1);}void Speak(const CScriptThing&t,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(owned&&t.id==2&&m==0&&!a&&b&&!c);action("speak");speeches.push_back(s);tasks=config.wait;}bool IsPerformingScriptTask()const{return tasks>0;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++movies;}~CScriptGameResourceObjectMovieBase(){assert(pauses==0);--movies;}};
class TRetVoid{public:void Return(){}};
class CTimer{public:CTimer(){assert(!timers);++timers;timerValue=config.timer;}~CTimer(){assert(!movies&&!pauses);--timers;}long GetTime()const{++timerReads;return timerValue;}bool operator==(long n)const{return GetTime()==n;}CTimer&operator=(long n){timerValue=n;++timerSets;return *this;}};
bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float d){assert(a.id==1);if(b.id==2){assert(d==20);++nearHeroReads;return config.nearHero;}assert(b.id==3&&d==10);++nearOtherReads;return config.nearOther;}
class GameMock{CScriptThing hero;public:long AddNewConversation(const CScriptThing&m,bool a,bool b){assert(m.id==1&&!a&&!b);++conversations;return conversations;}void AddPersonToConversation(long n,const CScriptThing&t){assert(n==conversations&&t.id==3);++people;}void AddLineToConversation(long n,const CCharString&t,bool b,const CScriptThing&a,const CScriptThing&z){assert(n==conversations&&!b&&((a.id==1&&z.id==3)||(a.id==3&&z.id==1)));ambient.push_back(t.text+(a.id==1?":A>B":":B>A"));}GameMock():hero(2){}void NewScriptFrame(){action("frame");++frame;if(tasks)--tasks;if(timers&&timerValue>0){timerValue-=config.tick;if(timerValue<0)timerValue=0;}}bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&s,EScriptAIPriority p){assert(p==4&&t.id==1);action("start");++starts;if(failures){--failures;return false;}s.acquire();return true;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&movies==1);action("movie");}void PauseAllNonScriptedEntities(bool b){if(b){action("pause");++pauses;}else{assert(pauses==1&&movies==1);--pauses;events.push_back("unpause");}}float GetHealth(const CScriptThing&t){assert(t.id==1);++healthCalls;return config.health;}CScriptThing&GetHero(){return hero;}void RemoveThing(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&b);action("remove");}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="TalkingTrader2");action("lookup");return CScriptThing(3);}void EntitySetAsKillable(const CScriptThing&t,bool a,bool b){assert(t.id==1&&a&&b);action("killable");}void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&t,bool a){assert(t.id==1&&a);action("combo");}bool TextEntryExists(const CCharString&s){assert(s.text.find("TEXT_QST_B10_TRADERA_ONHIT_")==0);action("text");long n=0;std::sscanf(s.text.c_str()+std::string("TEXT_QST_B10_TRADERA_ONHIT_").size(),"%ld",&n);return n<=config.maxLine;}};
class CWideScreenMagicPauseEntities{GameMock&g;public:CWideScreenMagicPauseEntities(GameMock&x):g(x){g.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g.PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&t){assert(t.id==1);++predicates;}~CIsThingAlive(){--predicates;}};class CV_SickChildScript{public:bool GotFishingSpotMushroom;class CTalkingTrader1{GameMock&Game;CScriptThing Me;CV_SickChildScript&ParentClass;void Main();public:long TimesTalkedTo,TimesHit;CTalkingTrader1(GameMock&g,CV_SickChildScript&p):Game(g),Me(1),ParentClass(p),TimesTalkedTo(config.talked),TimesHit(config.hits){}void Run(){Main();}void SetPredicate(const CIsThingAlive&){action("predicate");}bool IsActiveThreadTerminating()const{++checks;if((config.stop&&checks>=config.stop)||frame>=config.limit){stopped=true;return true;}return false;}};};}

void NScript::CV_SickChildScript::CTalkingTrader1::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing TalkToThisGuy=Game.GetThingWithScriptName("TalkingTrader2");
    CTimer SaySomethingTimer;
    long ThingsSaid=0;
    long LineNumber=10;
    if(ParentClass.GotFishingSpotMushroom) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Me,false,true);
    }
    while(!IsActiveThreadTerminating()) {
        if(SaySomethingTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),20.0f) && IsDistanceBetweenThingsUnder(Me,TalkToThisGuy,10.0f)) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,TalkToThisGuy);
            switch(ThingsSaid) {
                case 0:
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERA_CHAT1_10",false,Me,TalkToThisGuy);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERB_CHAT1_20",false,TalkToThisGuy,Me);
                    break;
                case 1:
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERA_CHAT2_10",false,Me,TalkToThisGuy);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERB_CHAT2_20",false,TalkToThisGuy,Me);
                    break;
                case 2:
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERA_CHAT3_10",false,Me,TalkToThisGuy);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERB_CHAT3_20",false,TalkToThisGuy,Me);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERA_CHAT3_30",false,TalkToThisGuy,Me);
                    break;
                case 3:
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERA_CHAT4_10",false,Me,TalkToThisGuy);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_TRADERB_CHAT4_20",false,TalkToThisGuy,Me);
                    break;
            }
            if(ThingsSaid<3) {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                ++ThingsSaid;
            } else {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                ThingsSaid=0;
            }
            SaySomethingTimer=16;
        }
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString TalkString;
            switch(TimesTalkedTo) {
                case 0: TalkString="TEXT_QST_B10_TRADERA_INTRO"; break;
                case 1: TalkString="TEXT_QST_B10_TRADERA_REPEATA"; break;
                case 2: TalkString="TEXT_QST_B10_TRADERA_REPEATB_10"; TimesTalkedTo=-1; break;
            }
            ++TimesTalkedTo;
            SaySomethingTimer=SaySomethingTimer.GetTime()+5;
            if(TalkToThisGuy.IsAlive()) {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),TalkString,GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_TRADERA_OTHERTRADERDEAD_10",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) && Game.GetHealth(Me)>0.0f) {
            if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            if(TimesHit>2) {
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAsKillable(Me,true,true);
                Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,true);
            }
            CCharString FullText;
            FullText="TEXT_QST_B10_TRADERA_ONHIT_"+GFIntToCharString(LineNumber);
            if(!Game.TextEntryExists(FullText)) {
                LineNumber=10;
                FullText="TEXT_QST_B10_TRADERA_ONHIT_"+GFIntToCharString(LineNumber);
            }
            LineNumber+=10;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),FullText,GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if(IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ++TimesHit;
        }
        Game.NewScriptFrame();
    }
}

struct Result{std::vector<std::string> events,speech,ambient;int talked,hits,checks,starts,talk,hit,any,heal,health,conversations,people,timerReads,timerSets,nearHero,nearOther;long timer;};
static Result run(const Config&c){config=c;frame=-1;checks=tasks=starts=talkCalls=hitCalls=anyCalls=healCalls=healthCalls=0;failures=c.fail;events.clear();speeches.clear();ambient.clear();conversations=people=timerReads=timerSets=nearHeroReads=nearOtherReads=0;stopped=false;Result r;{GameMock g;NScript::CV_SickChildScript p;p.GotFishingSpotMushroom=c.mushroom;NScript::CV_SickChildScript::CTalkingTrader1 t(g,p);t.Run();r.talked=t.TimesTalkedTo;r.hits=t.TimesHit;}assert(strings==0&&things==0&&handles==0&&movies==0&&pauses==0&&predicates==0&&owners==0&&timers==0);r.ambient=ambient;r.conversations=conversations;r.people=people;r.timerReads=timerReads;r.timerSets=timerSets;r.nearHero=nearHeroReads;r.nearOther=nearOtherReads;r.timer=timerValue;r.events=events;r.speech=speeches;r.checks=checks;r.starts=starts;r.talk=talkCalls;r.hit=hitCalls;r.any=anyCalls;r.heal=healCalls;r.health=healthCalls;return r;}
static int count(const Result&r,const char*s){int n=0;for(unsigned i=0;i<r.events.size();++i)if(r.events[i]==s)++n;return n;}
static int scenarios=0;
static void sweep(Config c){Result r=run(c);++scenarios;for(int i=1;i<=r.checks;++i){c.stop=i;Result q=run(c);++scenarios;assert(q.checks>=i||frame>=c.limit);}}
int main(){Config c;Result r;
 c=Config();r=run(c);assert(r.talked==0&&r.hits==0&&r.starts==1&&r.health==0&&r.speech.empty());sweep(c);
 c.talk=true;r=run(c);assert(r.speech.size()==6&&r.talked==0);for(int i=0;i<6;++i)assert(r.speech[i]==(i%3==0?"TEXT_QST_B10_TRADERA_INTRO":i%3==1?"TEXT_QST_B10_TRADERA_REPEATA":"TEXT_QST_B10_TRADERA_REPEATB_10"));sweep(c);
 c.otherAlive=false;r=run(c);assert(r.speech.size()==6);for(int i=0;i<6;++i)assert(r.speech[i]=="TEXT_QST_B10_TRADERA_OTHERTRADERDEAD_10");sweep(c);
 c=Config();c.hit=true;r=run(c);assert(r.hits==6&&count(r,"killable")==3&&count(r,"combo")==3&&r.any==0&&r.heal==0);for(int i=0;i<6;++i)assert(r.speech[i]==(i%2==0?"TEXT_QST_B10_TRADERA_ONHIT_10":"TEXT_QST_B10_TRADERA_ONHIT_20"));sweep(c);
 for(int flags=0;flags<8;++flags)for(int h=0;h<5;++h){c=Config();c.hit=(flags&1)!=0;c.special=(flags&2)!=0;c.heal=(flags&4)!=0;union{unsigned int bits;float value;}n;n.bits=0x7fc00000;c.health=h==0?-1.0f:h==1?0.0f:h==2?1.0f:h==3?n.value:100.0f;r=run(c);bool eligible=(c.hit||(c.special&&!c.heal))&&c.health>0;assert(r.hits==int(eligible)*6);assert(r.any==(c.hit?0:6));assert(r.heal==(!c.hit&&c.special?6:0));sweep(c);}
 for(int talked=-1;talked<=3;++talked)for(int alive=0;alive<2;++alive){c=Config();c.talk=true;c.otherAlive=alive!=0;c.talked=talked;sweep(c);}
 for(int hits=2;hits<=4;++hits){c=Config();c.hit=true;c.hits=hits;c.limit=1;r=run(c);assert(count(r,"killable")==int(hits>2));sweep(c);}
 for(int fail=0;fail<3;++fail)for(int wait=0;wait<3;++wait)for(int alive=0;alive<2;++alive){c=Config();c.talk=c.hit=c.mushroom=true;c.otherAlive=alive!=0;c.fail=fail;c.wait=wait;r=run(c);assert(r.starts==fail+1&&count(r,"remove")==1);sweep(c);}
 c=Config();c.hit=true;c.maxLine=0;r=run(c);for(unsigned i=0;i<r.speech.size();++i)assert(r.speech[i]=="TEXT_QST_B10_TRADERA_ONHIT_10");sweep(c);

 c=Config();c.limit=8;c.tick=16;r=run(c);assert(r.conversations==8&&r.people==8&&r.timerSets==8&&r.ambient.size()==18);unsigned at=0;for(int cycle=0;cycle<2;++cycle)for(int stage=1;stage<=4;++stage){char a[96],b[96];std::sprintf(a,"TEXT_QST_B10_TRADERA_CHAT%d_10:A>B",stage);std::sprintf(b,"TEXT_QST_B10_TRADERB_CHAT%d_20:B>A",stage);assert(r.ambient[at++]==a);assert(r.ambient[at++]==b);if(stage==3)assert(r.ambient[at++]=="TEXT_QST_B10_TRADERA_CHAT3_30:B>A");}sweep(c);
 for(int timer=-1;timer<=2;++timer)for(int near=0;near<4;++near)for(int flags=0;flags<16;++flags){c=Config();c.timer=timer;c.nearHero=(near&1)!=0;c.nearOther=(near&2)!=0;c.talk=(flags&1)!=0;c.hit=(flags&2)!=0;c.otherAlive=(flags&4)!=0;c.mushroom=(flags&8)!=0;c.limit=2;r=run(c);assert(r.people==r.conversations);if(timer==0){assert(r.nearHero>=1);if(!c.nearHero)assert(r.nearOther==0);if(!c.nearHero||!c.nearOther)assert(!r.conversations);}if(timer<0&&!c.talk)assert(!r.nearHero&&!r.nearOther&&!r.conversations);sweep(c);}
 assert(scenarios==4585);std::puts("00ecbdf0_TEST PASS");}
