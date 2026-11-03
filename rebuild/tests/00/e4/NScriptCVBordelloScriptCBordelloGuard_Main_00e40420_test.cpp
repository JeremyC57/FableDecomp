#include <cassert>
#include <cstdio>
#include <string>
#include <map>
#include <limits>
struct State { int frame,limit,checks,stop,things,strings,resources,movies,depth,paused,unpaused,starts,retries,releases,predicates,delay,waits,talkReads,hitReads,specialReads,healReads,healthReads,dressReads,nearReads,statusReads,cutscenes,assigned,speech[7];bool talk,tricking,party,dress,owned,nunnery,cutscene,sleeping,physical,special,heal,nearby,valid;float health;State():frame(0),limit(2),checks(0),stop(0),things(0),strings(0),resources(0),movies(0),depth(0),paused(0),unpaused(0),starts(0),retries(0),releases(0),predicates(0),delay(0),waits(0),talkReads(0),hitReads(0),specialReads(0),healReads(0),healthReads(0),dressReads(0),nearReads(0),statusReads(0),cutscenes(0),assigned(0),talk(false),tricking(false),party(false),dress(false),owned(false),nunnery(false),cutscene(false),sleeping(false),physical(false),special(false),heal(false),nearby(false),valid(false),health(100){for(int i=0;i<7;++i)speech[i]=0;}bool term(){assert(checks<2000);++checks;return frame>=limit||(stop&&checks>=stop);}};State*state;
class CCharString {public:std::string text;CCharString(const char*s=""):text(s){++state->strings;}CCharString(const CCharString&s):text(s.text){++state->strings;}~CCharString(){--state->strings;}CCharString&operator=(const CCharString&s){text=s.text;return *this;}operator const char*()const{return text.c_str();}};
CCharString operator+(const CCharString&a,const CCharString&b){return CCharString((a.text+b.text).c_str());}CCharString operator+(const char*a,const CCharString&b){return CCharString((std::string(a)+b.text).c_str());}
class CScriptThing{public:int id;CScriptThing(int x=-1):id(x){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->talkReads;return state->talk;}bool MsgIsHitBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->hitReads;return state->physical;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");++state->specialReads;return state->special;}bool MsgIsHitBySpecialAbilityFrom(int a,const CCharString&s)const{assert(id==-1&&a==14&&s.text=="SCRIPT_NAME_HERO");++state->healReads;return state->heal;}};
bool IsDistanceBetweenThingsUnder(const CScriptThing&m,const CScriptThing&h,float n){assert(m.id==-1&&h.id==-2&&n==3);++state->nearReads;return state->nearby;}
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0,GROUP_SELECT_RANDOM_NO_REPEAT=2};class TRetVoid{public:void Return(){}};
const char* tags[6]={"TEXT_QST_B13_GUARD_EMPLOYED_NUNNERY","TEXT_QST_B13_GUARD_STRANGE_LADY","TEXT_QST_B13_GUARD_HERO_EXHAUSTED","TEXT_QST_B13_GUARD_FEMALE_NO_ENTRY","TEXT_QST_B13_GUARD_GREETING","TEXT_QST_B13_GUARD_MALE_NO_ENTRY"};
std::string suffix(){return state->dress?(state->tricking?"_HEROWHORE":"_HEROLADY"):"_HEROMAN";}
class CScriptGameResourceObjectScriptedThingBase{public:int left;bool valid;CScriptGameResourceObjectScriptedThingBase():left(0),valid(state->valid){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}CScriptGameResourceObjectScriptedThingBase&operator=(const CScriptGameResourceObjectScriptedThingBase&o){valid=o.valid;left=o.left;++state->assigned;return *this;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->releases;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(10);}void Speak(const CScriptThing&h,const char*t,int a,bool b,bool c,bool d){assert(h.id==-2&&!b&&c&&!d&&state->depth==1&&valid);int k=-1;for(int i=0;i<6;++i)if(std::string(t)==tags[i])k=i;if(k>=0)assert(a==0);else{assert(std::string(t)=="TEXT_QST_B13_GUARD_ATTACKED"+suffix()&&a==2);k=6;}++state->speech[k];left=state->delay;}bool IsPerformingScriptTask(){++state->waits;return left-->0;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(!state->movies);++state->movies;}~CScriptGameResourceObjectMovieBase(){assert(!state->depth);--state->movies;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void NewScriptFrame(){assert(state->frame<256);++state->frame;}CScriptThing&GetHero(){return hero;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,int p){assert(m.id==-1&&p==4);++state->starts;r.valid=state->starts>state->retries;return r.valid;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1&&!state->depth);}void PauseAllNonScriptedEntities(bool b){if(b){assert(!state->depth);++state->depth;++state->paused;}else{assert(state->depth==1);--state->depth;++state->unpaused;}}float GetHealth(const CScriptThing&m){assert(m.id==10);++state->healthReads;return state->health;}};
class CWideScreenMagicPauseEntities{protected:CGameScriptInterfaceBase* Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game):Game(&game){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingAlive{CScriptThing Thing;public:CIsThingAlive(const CScriptThing&t):Thing(t){assert(t.id==-1);}};class CV_BordelloScript{public:bool HeroPartying,HeroTricking,MagicianSleeping,CutscenePlaying,PlayerOwned,BecomeNunnery;CScriptGameResourceObjectScriptedThingBase seh_Guard;std::map<std::string,CCharString>csargs;CV_BordelloScript():HeroPartying(state->party),HeroTricking(state->tricking),MagicianSleeping(state->sleeping),CutscenePlaying(state->cutscene),PlayerOwned(state->owned),BecomeNunnery(state->nunnery){}bool IsHeroDressedAsLady(){++state->dressReads;return state->dress;}CCharString GetHeroStatusTextTag(){++state->statusReads;if(HeroTricking&&IsHeroDressedAsLady())return "_HEROWHORE";if(IsHeroDressedAsLady())return "_HEROLADY";return "_HEROMAN";}void PlayCutscene(CCharString n,bool b){assert(n.text=="CS_BORDELLO_KICKEDOUT"&&!b&&state->depth==1&&csargs.size()==1);assert(csargs["$DIALOGUE"].text=="TEXT_QST_B13_GUARD_ENTRY_REFUSED"+suffix());++state->cutscenes;csargs.clear();}class CBordelloGuard{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CV_BordelloScript&ParentClass;CBordelloGuard(CGameScriptInterfaceBase&g,CV_BordelloScript&p):Game(g),ParentClass(p){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CV_BordelloScript::CBordelloGuard::Main()
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
    ParentClass.seh_Guard=seh_me;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (ParentClass.BecomeNunnery) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B13_GUARD_EMPLOYED_NUNNERY",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (ParentClass.HeroTricking && ParentClass.IsHeroDressedAsLady()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B13_GUARD_STRANGE_LADY",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (ParentClass.HeroPartying) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B13_GUARD_HERO_EXHAUSTED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (ParentClass.IsHeroDressedAsLady()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B13_GUARD_FEMALE_NO_ENTRY",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (ParentClass.PlayerOwned) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B13_GUARD_GREETING",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B13_GUARD_MALE_NO_ENTRY",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),CCharString("TEXT_QST_B13_GUARD_ATTACKED")+ParentClass.GetHeroStatusTextTag(),GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (!ParentClass.HeroPartying && !(ParentClass.HeroTricking && ParentClass.IsHeroDressedAsLady()) && !ParentClass.PlayerOwned && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),3.0f) && !ParentClass.CutscenePlaying && !ParentClass.MagicianSleeping) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.csargs["$DIALOGUE"]="TEXT_QST_B13_GUARD_ENTRY_REFUSED"+ParentClass.GetHeroStatusTextTag();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            ParentClass.PlayCutscene("CS_BORDELLO_KICKEDOUT",false);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

static int cases=0;void run(State&s,bool complete){state=&s;++cases;bool hit=s.physical||(s.special&&!s.heal),good=s.health>0;int kind=s.nunnery?0:(s.tricking&&s.dress)?1:s.party?2:s.dress?3:s.owned?4:5;bool distanceRead=!s.party&&!(s.tricking&&s.dress)&&!s.owned;bool kick=distanceRead&&s.nearby&&!s.cutscene&&!s.sleeping;if(complete)s.limit=2+s.retries+s.delay*((s.talk&&good?1:0)+(hit&&good?1:0));{CGameScriptInterfaceBase game;NScript::CV_BordelloScript parent;NScript::CV_BordelloScript::CBordelloGuard me(game,parent);me.Main();if(complete){assert(parent.seh_Guard.valid&&s.assigned==1);assert(parent.csargs.empty());}}assert(!s.things&&!s.strings&&!s.resources&&!s.movies&&!s.depth);assert(s.paused==s.unpaused);if(complete){assert(s.predicates==1&&s.starts==s.retries+1&&s.releases==(s.valid?1:0));assert(s.talkReads==1&&s.hitReads==1&&s.specialReads==(!s.physical?1:0)&&s.healReads==(!s.physical&&s.special?1:0));assert(s.healthReads==(s.talk?1:0)+(hit?1:0));for(int i=0;i<6;++i)assert(s.speech[i]==(s.talk&&good&&kind==i?1:0));assert(s.speech[6]==(hit&&good?1:0));assert(s.nearReads==(distanceRead?1:0)&&s.cutscenes==(kick?1:0));assert(s.statusReads==(hit&&good?1:0)+(kick?1:0));assert(s.paused==(s.talk?1:0)+(hit?1:0)+(kick?1:0));assert(s.waits==(s.delay+1)*((s.talk&&good?1:0)+(hit&&good?1:0)));int reads=0;if(s.talk&&!s.nunnery){if(s.tricking)++reads;if(!(s.tricking&&s.dress)&&!s.party)++reads;}if(!s.party&&s.tricking)++reads;int statusCost=s.tricking?(s.dress?1:2):1;reads+=statusCost*s.statusReads;assert(s.dressReads==reads);}}
void flags(State&s,int m){s.talk=(m&1)!=0;s.tricking=(m&2)!=0;s.party=(m&4)!=0;s.dress=(m&8)!=0;s.owned=(m&16)!=0;s.nunnery=(m&32)!=0;s.cutscene=(m&64)!=0;s.sleeping=(m&128)!=0;s.physical=(m&256)!=0;s.special=(m&512)!=0;s.heal=(m&1024)!=0;s.nearby=(m&2048)!=0;}
int main(){float health[4]={100,0,-10,std::numeric_limits<float>::quiet_NaN()};for(int m=0;m<4096;++m)for(int h=0;h<4;++h){State s;flags(s,m);s.health=health[h];run(s,true);}for(int r=0;r<4;++r)for(int d=0;d<4;++d)for(int m=0;m<32;++m){State s;flags(s,(m<<1)|1|256|2048);s.retries=r;s.delay=d;s.valid=(m&1)!=0;run(s,true);}for(int stop=1;stop<=100;++stop)for(int m=0;m<24;++m){State s;flags(s,(m<<1)|1|256|2048);s.limit=30;s.stop=stop;s.retries=m%4;s.delay=m%4;run(s,false);}assert(cases==19296);std::puts("00e40420_TEST PASS");return 0;}