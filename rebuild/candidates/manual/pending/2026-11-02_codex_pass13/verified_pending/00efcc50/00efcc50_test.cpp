#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
static int strings=0,things=0,handles=0,owned=0,movies=0,pauses=0,predicates=0;
static int frame,checks,starts,tasks,failures,displays,healthCalls,anyCalls,healCalls,resets,world,mark;
static float age;
static bool stopped;
static std::vector<std::string> events,lines,times;
struct Config{bool talk,hit,special,heal;float initialAge,health;unsigned healthMask;int stop,limit,wait,initialFail,step,rate;Config():talk(false),hit(false),special(false),heal(false),initialAge(30),health(100),healthMask(127),stop(0),limit(1),wait(0),initialFail(0),step(3),rate(2){}};static Config config;
static void action(const char*s){assert(!stopped);events.push_back(s);}
class CCharString{public:std::string text;CCharString(const char*s=""):text(s){++strings;}CCharString(const CCharString&s):text(s.text){++strings;}~CCharString(){--strings;}};
class C2DVector{public:float X,Y;C2DVector(float x,float y):X(x),Y(y){}};
class CRGBColour{public:unsigned char R,G,B,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a):R(r),G(g),B(b),A(a){}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};
class CScriptThing{public:int id;CScriptThing(int x=0):id(x){++things;}CScriptThing(const CScriptThing&t):id(t.id){++things;}~CScriptThing(){--things;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");return config.talk&&displays==1;}bool MsgIsHitBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");return config.hit&&displays==1;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");++anyCalls;return config.special&&displays==1;}bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{assert(id==1&&a==14&&s.text=="SCRIPT_NAME_HERO");++healCalls;return config.heal;}};
class CScriptGameResourceObjectScriptedThingBase{bool valid;public:CScriptGameResourceObjectScriptedThingBase():valid(false){++handles;}~CScriptGameResourceObjectScriptedThingBase(){if(valid)--owned;--handles;}void acquire(){assert(!valid);valid=true;++owned;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;--owned;action("release");}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(1);}void Speak(const CScriptThing&t,const char*s,ETextGroupSelectionMethod n,bool a,bool b,bool c){assert(valid&&t.id==2&&n==0&&!a&&b&&!c&&movies==1&&pauses==1);lines.push_back(s);action("speak");tasks=config.wait;}bool IsPerformingScriptTask()const{return tasks>0;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++movies;}~CScriptGameResourceObjectMovieBase(){assert(pauses==0);--movies;events.push_back("endMovie");}};
class GameMock{CScriptThing hero;public:GameMock():hero(2){}void NewScriptFrame(){assert(strings==(movies?1:0));action("frame");++frame;world+=config.step;if(tasks)--tasks;}void SetThingHasInformation(const CScriptThing&t,bool a,bool b,bool c){assert(t.id==1&&!a&&b&&!c);action("info");}bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&s,EScriptAIPriority p){assert(t.id==1&&p==4);action("start");++starts;if(failures){--failures;return false;}s.acquire();return true;}void ShowOnScreenMessage(const C2DVector&p,const CCharString&t,const CRGBColour&c,const CCharString&font){assert(p.X==240&&p.Y==80&&c.R==255&&c.G==255&&c.B==255&&c.A==255&&font.text.empty());char q[64];long expected=(long)((float)(world-mark)/(float)config.rate);if(expected<0)expected=0;std::sprintf(q,"%ld",expected);assert(t.text==q);times.push_back(t.text);++displays;action("display");}unsigned long PlaySoundOnThing(const CScriptThing&t,const CCharString&s){assert(t.id==1);assert(s.text=="SND_TROLLRISING_01"||s.text=="SND_WOMANSCREAM_01");action(s.text=="SND_TROLLRISING_01"?"talkSound":"hitSound");return 123;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&movies==1);action("movie");}void PauseAllNonScriptedEntities(bool b){if(b){assert(movies==1&&pauses==0);++pauses;action("pause");}else{assert(movies==1&&pauses==1);--pauses;events.push_back("unpause");}}CScriptThing&GetHero(){return hero;}float GetHeroAge(){return age;}void SetHeroAge(float a){assert(a==20||a==55);action("age");age=a;}void EntitySetAppearanceSeed(const CScriptThing&t,unsigned long n){assert(t.id==1&&n==12345);action("seed");}float GetHealth(const CScriptThing&t){assert(t.id==1&&movies==1&&pauses==1);unsigned bit=1u<<(healthCalls%7);++healthCalls;return(config.healthMask&bit)?config.health:0.0f;}};
class CWideScreenMagicPauseEntities{GameMock&g;public:CWideScreenMagicPauseEntities(GameMock&x):g(x){g.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g.PauseAllNonScriptedEntities(false);}};
class CCounter{public:CCounter(){mark=world;action("counter");}CCounter&operator=(float n){assert(n==0);mark=world;++resets;action("reset");return *this;}float GetCounter()const{long elapsed=world-mark;if(elapsed<0)elapsed=0;return(float)elapsed/(float)config.rate;}};
static CCharString GFIntToCharString(long n){char q[64];std::sprintf(q,"%ld",n);return CCharString(q);}
namespace NScript{class CIsThingConscious{public:CIsThingConscious(const CScriptThing&t){assert(t.id==1);++predicates;}~CIsThingConscious(){--predicates;}};class CMarkTestScriptsScript{public:class CTestMan{GameMock&Game;CScriptThing Me;void Main();public:CTestMan(GameMock&g):Game(g),Me(1){}void Run(){Main();}void SetPredicate(const CIsThingConscious&){action("predicate");}bool IsActiveThreadTerminating()const{++checks;if((config.stop&&checks>=config.stop)||frame>=config.limit){stopped=true;return true;}return false;}};};}
void NScript::CMarkTestScriptsScript::CTestMan::Main()

{
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    Game.SetThingHasInformation(Me,false,true,false);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    CCounter Test;
    Test=0.0f;
    while(!IsActiveThreadTerminating()) {
      {
        CCharString Time=GFIntToCharString((long)Test.GetCounter());
        Game.ShowOnScreenMessage(C2DVector(240.0f,80.0f),Time,CRGBColour(255,255,255,255),"");
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return;
            Test=0.0f;
            Game.PlaySoundOnThing(Me,"SND_TROLLRISING_01");
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHeroAge()>40.0f) {
                if(IsActiveThreadTerminating()) return;
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"My, you're an old fart!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
            } else {
                if(IsActiveThreadTerminating()) return;
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 1!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 2!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 3!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 4!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 5!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 6!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"Line 7!",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
            }
        }
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if(IsActiveThreadTerminating()) return;
            Game.PlaySoundOnThing(Me,"SND_WOMANSCREAM_01");
            Game.EntitySetAppearanceSeed(Me,12345);
            if(Game.GetHeroAge()>40.0f) {
                if(IsActiveThreadTerminating()) return;
                Game.SetHeroAge(20.0f);
            } else {
                if(IsActiveThreadTerminating()) return;
                Game.SetHeroAge(55.0f);
            }
        }
      }
        Game.NewScriptFrame();
    }
}

struct Result{std::vector<std::string> events,lines,times;int checks,health,any,heal,resets;float age;};
static Result run(const Config&c){config=c;frame=-1;checks=starts=tasks=displays=healthCalls=anyCalls=healCalls=resets=world=mark=0;age=c.initialAge;failures=c.initialFail;stopped=false;events.clear();lines.clear();times.clear();{GameMock game;NScript::CMarkTestScriptsScript::CTestMan script(game);script.Run();}assert(strings==0&&things==0&&handles==0&&owned==0&&movies==0&&pauses==0&&predicates==0);Result r;r.events=events;r.lines=lines;r.times=times;r.checks=checks;r.health=healthCalls;r.any=anyCalls;r.heal=healCalls;r.resets=resets;r.age=age;return r;}
static int count(const Result&r,const char*s){int n=0;for(unsigned i=0;i<r.events.size();++i)if(r.events[i]==s)++n;return n;}
static void before(const Result&r,const char*a,const char*b){unsigned x=0,y=0;while(x<r.events.size()&&r.events[x]!=a)++x;while(y<r.events.size()&&r.events[y]!=b)++y;assert(x<y&&y<r.events.size());}
static int scenarios=0;
static void sweep(Config c){Result r=run(c);++scenarios;for(int i=1;i<=r.checks;++i){c.stop=i;run(c);++scenarios;}}
int main(){Config c;Result r;union{unsigned int bits;float value;}nan;nan.bits=0x7fc00000;
 for(int ag=0;ag<4;++ag)for(int he=0;he<4;++he){c=Config();c.talk=true;c.initialAge=ag==0?39:ag==1?40:ag==2?41:nan.value;c.health=he==0?-1:he==1?0:he==2?100:nan.value;r=run(c);int n=ag==2?1:7;assert(r.health==n&&r.lines.size()==(he==2?(unsigned)n:0)&&r.resets==2);if(he==2){if(ag==2)assert(r.lines[0]=="My, you're an old fart!");else for(int j=1;j<=7;++j){char q[32];std::sprintf(q,"Line %d!",j);assert(r.lines[j-1]==q);}}before(r,"display","talkSound");before(r,"talkSound","movie");before(r,"movie","unpause");before(r,"unpause","endMovie");sweep(c);}
 for(int mask=0;mask<128;++mask){c=Config();c.talk=true;c.healthMask=mask;r=run(c);unsigned n=0;for(int i=0;i<7;++i)if(mask&(1<<i)){char q[32];std::sprintf(q,"Line %d!",i+1);assert(r.lines[n++]==q);}assert(r.health==7&&r.lines.size()==n);sweep(c);}
 for(int flags=0;flags<8;++flags)for(int ag=0;ag<4;++ag){c=Config();c.hit=(flags&1)!=0;c.special=(flags&2)!=0;c.heal=(flags&4)!=0;c.initialAge=ag==0?39:ag==1?40:ag==2?41:nan.value;r=run(c);bool hit=c.hit||(c.special&&!c.heal);assert(count(r,"hitSound")==int(hit)&&count(r,"seed")==int(hit)&&count(r,"age")==int(hit));if(hit){assert(r.age==(ag==2?20:55));before(r,"hitSound","seed");before(r,"seed","age");}assert(r.any==(c.hit?0:1)&&r.heal==(!c.hit&&c.special?1:0));sweep(c);}
 for(int fail=0;fail<3;++fail)for(int wait=0;wait<3;++wait){c=Config();c.talk=c.hit=true;c.initialFail=fail;c.wait=wait;c.limit=25;r=run(c);assert(r.health==7&&r.lines.size()==7&&r.age==55);before(r,"endMovie","hitSound");sweep(c);}
 c=Config();c.limit=5;r=run(c);assert(r.times.size()==5&&r.times[0]=="0"&&r.times[1]=="1"&&r.times[2]=="3"&&r.times[3]=="4"&&r.times[4]=="6");sweep(c);c.step=-3;r=run(c);for(unsigned i=0;i<r.times.size();++i)assert(r.times[i]=="0");sweep(c);
 assert(scenarios>2000);std::puts("00efcc50_TEST PASS");}
