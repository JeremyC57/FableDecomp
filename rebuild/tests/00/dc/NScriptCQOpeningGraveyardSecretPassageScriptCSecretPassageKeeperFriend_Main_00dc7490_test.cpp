// Selected source SHA256 21c5530dd70785ff3b3f78652070c4cd8a2b85f1a6e874a9ee8f6ebb7709e68b
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <limits>
#define CHECK(x) do { if(!(x)){ std::fprintf(stderr,"check %d: %s case=%d\n",__LINE__,#x,caseNumber); std::abort(); } } while(0)
static int caseNumber=0;
struct Config { int loops,physical,ability,heal,talk,claimDelay,taskDelay,startDelay,introDelay,stopAt; bool valid,initialValid; float distance,health; Config():loops(5),physical(-1),ability(-1),heal(-1),talk(1),claimDelay(0),taskDelay(0),startDelay(0),introDelay(0),stopAt(0),valid(true),initialValid(false),distance(3.5f),health(1.0f){} } cfg;
struct State { int guards,frames,loop,claims,releases,tries,busy,movies,pauses,things,strings,resources,conversations,killed,bodyEffects,terminationEffects; bool terminated; std::vector<std::string> events; std::vector<int> speech; std::vector<float> radii; std::vector<int> moves; State():guards(0),frames(0),loop(0),claims(0),releases(0),tries(0),busy(0),movies(0),pauses(0),things(0),strings(0),resources(0),conversations(0),killed(0),bodyEffects(0),terminationEffects(0),terminated(false){} } st;
static void effect(const char* x){CHECK(!st.terminated);++st.bodyEffects;st.events.push_back(x);}
class CCharString { public: std::string value; CCharString(const char* s):value(s){++st.strings;} CCharString(const CCharString& x):value(x.value){++st.strings;} ~CCharString(){--st.strings;} };
class C3DVector { public: float X,Y,Z; C3DVector():X(1.5f),Y(-2.0f),Z(0.5f){} };
class CScriptThing { public: int id; CScriptThing(int i=0):id(i){++st.things;} CScriptThing(const CScriptThing& x):id(x.id){++st.things;} ~CScriptThing(){--st.things;} const C3DVector& GetPos()const {CHECK(id==3);static C3DVector p;return p;} bool MsgIsHitBy(const CCharString& x)const {CHECK(x.value=="SCRIPT_NAME_HERO");return st.loop==cfg.physical;} bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& x)const {CHECK(x.value=="SCRIPT_NAME_HERO");return st.loop==cfg.ability;} bool MsgIsHitBySpecialAbilityFrom(int a,const CCharString& x)const {CHECK(a==14&&x.value=="SCRIPT_NAME_HERO");return st.loop==cfg.heal;} bool MsgIsTalkedToBy(const CCharString& x)const {CHECK(x.value=="SCRIPT_NAME_HERO");return cfg.talk==1||(cfg.talk==2&&st.loop%2==0);} };
enum { SCRIPT_AI_PRIORITY_HIGHEST=4,ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1,GROUP_SELECT_NONE=0,HERO_ABILITY_HEAL_LIFE_SPELL=14 };
class CScriptGameResourceObjectScriptedThingBase { public: bool valid; CScriptGameResourceObjectScriptedThingBase():valid(cfg.initialValid){++st.resources;} ~CScriptGameResourceObjectScriptedThingBase(){CHECK(st.movies==0&&st.pauses==0);--st.resources;} bool IsValid()const{return valid;} void Release(){effect("release");CHECK(valid);valid=false;++st.releases;st.tries=0;} void MoveToPosition(const C3DVector& p,float r,int m,bool a,bool b){effect("move");CHECK(valid&&!a&&b);CHECK(p.X==1.5f&&p.Y==-2.0f&&p.Z==0.5f);CHECK((r==3.0f&&m==0)||(r==2.0f&&m==1));st.radii.push_back(r);st.moves.push_back(m);st.busy=cfg.taskDelay;} void ClearCommands(){effect("clear");CHECK(valid);st.busy=0;} CScriptThing GetScriptThing()const{CHECK(valid&&st.movies==1&&st.pauses==1);return CScriptThing(1);} bool IsPerformingScriptTask()const {CHECK(valid);if(st.busy){--st.busy;return true;}return false;} void Speak(const CScriptThing& hero,const char* line,int method,bool a,bool b,bool c){effect("speak");CHECK(valid&&hero.id==2&&method==0&&!a&&b&&!c&&st.movies==1&&st.pauses==1);const char* names[]={"TEXT_QST_049_GRAVEFRIEND_GET_OFF","TEXT_QST_049_GRAVEFRIEND_GET_OFF_AGAIN","TEXT_QST_049_GRAVEFRIEND_GET_OFF_SHOUT","TEXT_QST_049_GRAVEFRIEND_GET_OFF_SHOUT_AGAIN"};int n=0;while(n<4&&std::strcmp(names[n],line))++n;CHECK(n<4);st.speech.push_back(n);st.busy=cfg.taskDelay;} };
class CScriptGameResourceObjectMovieBase {public: CScriptGameResourceObjectMovieBase(){CHECK(st.movies==0);++st.movies;} ~CScriptGameResourceObjectMovieBase(){CHECK(st.pauses==0&&st.movies==1);--st.movies;} };
class CGameScriptInterfaceBase {public: CScriptThing hero; CGameScriptInterfaceBase():hero(2){} void NewScriptFrame(){effect("frame");++st.frames;CHECK(st.frames<500);} bool StartScriptingEntity(const CScriptThing& me,CScriptGameResourceObjectScriptedThingBase& h,int p){effect("claim");CHECK(me.id==1&&p==4&&!h.valid);if(st.tries++<cfg.claimDelay)return false;h.valid=true;++st.claims;st.tries=0;return true;} void EntitySetAsKillable(const CScriptThing& x,bool a,bool b){CHECK(x.id==1&&b);effect(a?"killable1":"killable0");} void EntitySetTargetable(const CScriptThing& x,bool a){CHECK(x.id==1);effect(a?"target1":"target0");} void EntitySetAsDamageable(const CScriptThing& x,bool a){CHECK(x.id==1);effect(a?"damage1":"damage0");} CScriptThing GetThingWithScriptName(const CCharString& x){CHECK(x.value=="GraveFriendLeavingMarker");effect("find");return CScriptThing(3);} long AddNewConversation(const CScriptThing& x,bool a,bool b){CHECK(x.id==1&&!a&&!b);effect("conversation");++st.conversations;return 17;} CScriptThing& GetHero(){return hero;} void AddPersonToConversation(long n,const CScriptThing& x){CHECK(n==17&&x.id==2);effect("person");} void AddLineToConversation(long n,const CCharString& s,bool a,const CScriptThing& x,const CScriptThing& y){CHECK(n==17&&s.value=="TEXT_QST_049_GRAVEFRIEND_ATTACKED"&&!a&&x.id==1&&y.id==2);effect("line");} void StartMovieSequence(const CCharString& s,CScriptGameResourceObjectMovieBase&){CHECK(s.value.empty()&&st.movies==1);effect("movie");} void PauseAllNonScriptedEntities(bool b){if(b){effect("pause");CHECK(st.movies==1&&st.pauses==0);++st.pauses;}else{CHECK(st.pauses==1);--st.pauses;}} float GetHealth(const CScriptThing& x){CHECK(x.id==1);return cfg.health;} };
class CWideScreenMagicPauseEntities { CGameScriptInterfaceBase& game; public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& g):game(g){game.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){game.PauseAllNonScriptedEntities(false);} };
class TRetVoid {public:void Return(){CHECK(st.terminated);CHECK(st.bodyEffects==st.terminationEffects);} };
namespace NScript { class CIsThingConscious { public:CScriptThing Thing;CIsThingConscious(const CScriptThing& x):Thing(x){CHECK(x.id==1);} };class CQ_OpeningGraveyardSecretPassageScript {public: bool GravekeeperCutsceneStart,GravekeeperIntroduced;CQ_OpeningGraveyardSecretPassageScript():GravekeeperCutsceneStart(false),GravekeeperIntroduced(false){} class CSecretPassageKeeperFriend {public: CGameScriptInterfaceBase& Game;CScriptThing Me;CQ_OpeningGraveyardSecretPassageScript& ParentClass;CSecretPassageKeeperFriend(CGameScriptInterfaceBase& g,CQ_OpeningGraveyardSecretPassageScript& p):Game(g),Me(1),ParentClass(p){}void SetPredicate(const CIsThingConscious&){effect("predicate");}bool IsActiveThreadTerminating(){ParentClass.GravekeeperCutsceneStart=st.frames>cfg.startDelay;ParentClass.GravekeeperIntroduced=st.frames>cfg.introDelay;++st.guards;if(cfg.stopAt==st.guards){st.terminated=true;st.terminationEffects=st.bodyEffects;return true;}return false;}void Main();};}; }
static bool IsDistanceBetweenThingsOver(const CScriptThing& a,const CScriptThing& b,float r){CHECK(a.id==1&&b.id==3&&r==3.0f);++st.loop;return cfg.valid&&st.loop<=cfg.loops&&cfg.distance>r;}
static void FadeOutAndKillEntity(const CScriptThing& x,float a,bool b,bool c){CHECK(x.id==1&&a==1.0f&&b&&c&&st.busy==0&&st.movies==0&&st.pauses==0);effect("fade");++st.killed;}

void NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageKeeperFriend::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.EntitySetAsKillable(Me,false,true);
    Game.EntitySetTargetable(Me,false);
    Game.EntitySetAsDamageable(Me,false);
    while (!ParentClass.GravekeeperCutsceneStart) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.EntitySetAsKillable(Me,true,true);
    Game.EntitySetTargetable(Me,true);
    Game.EntitySetAsDamageable(Me,true);
    if (seh_me.IsValid()) seh_me.Release();
    while (!ParentClass.GravekeeperIntroduced) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing FriendEscape=Game.GetThingWithScriptName("GraveFriendLeavingMarker");
    seh_me.MoveToPosition(FriendEscape.GetPos(),3.0f,ENTITY_MOVE_WALK,false,true);
    long NumChatted=0;
    bool BeenHit=false;
    while (IsDistanceBetweenThingsOver(Me,FriendEscape,3.0f) && !BeenHit) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            BeenHit=true;
            long Conversation=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(Conversation,Game.GetHero());
            Game.AddLineToConversation(Conversation,"TEXT_QST_049_GRAVEFRIEND_ATTACKED",false,Me,Game.GetHero());
            seh_me.ClearCommands();
            seh_me.MoveToPosition(FriendEscape.GetPos(),2.0f,ENTITY_MOVE_RUN,false,true);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            if (NumChatted==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_049_GRAVEFRIEND_GET_OFF",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                NumChatted=1;
            }
            else if (NumChatted==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                NumChatted=2;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_049_GRAVEFRIEND_GET_OFF_AGAIN",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (NumChatted==2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                NumChatted=3;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_049_GRAVEFRIEND_GET_OFF_SHOUT",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (NumChatted==3) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                NumChatted=2;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_049_GRAVEFRIEND_GET_OFF_SHOUT_AGAIN",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            seh_me.MoveToPosition(FriendEscape.GetPos(),3.0f,ENTITY_MOVE_WALK,false,true);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (seh_me.IsPerformingScriptTask()) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    FadeOutAndKillEntity(Me,1.0f,true,true);
}


static void run(){++caseNumber;st=State();{CGameScriptInterfaceBase game;NScript::CQ_OpeningGraveyardSecretPassageScript owner;NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageKeeperFriend row(game,owner);row.Main();CHECK(st.resources==0&&st.movies==0&&st.pauses==0&&st.strings==0&&st.things==2);if(st.terminated){CHECK(st.bodyEffects==st.terminationEffects&&st.killed==0);}else{CHECK(st.killed==1&&st.claims==2);CHECK(st.releases==1+(cfg.initialValid?1:0));std::vector<std::string> toggles;for(unsigned i=0;i<st.events.size();++i)if(st.events[i].find("killable")==0||st.events[i].find("target")==0||st.events[i].find("damage")==0)toggles.push_back(st.events[i]);const char* want[]={"killable0","target0","damage0","killable1","target1","damage1"};CHECK(toggles.size()==6);for(int j=0;j<6;++j)CHECK(toggles[j]==want[j]);int rounds=(cfg.valid&&cfg.distance>3.0f)?cfg.loops:0;int hits=0,chats=0;std::vector<int> sequence,move;move.push_back(0);int speechState=0;for(int k=1;k<=rounds;++k){bool hit=k==cfg.physical||(k==cfg.ability&&k!=cfg.heal);if(hit){++hits;move.push_back(1);}bool talk=cfg.talk==1||(cfg.talk==2&&k%2==0);if(talk){++chats;if(cfg.health>0.0f)sequence.push_back(speechState);if(speechState==0)speechState=1;else if(speechState==1)speechState=2;else if(speechState==2)speechState=3;else speechState=2;move.push_back(0);}if(hit)break;}CHECK(st.conversations==hits&&st.speech==sequence&&st.moves==move);int movies=0;for(unsigned j=0;j<st.events.size();++j)if(st.events[j]=="movie")++movies;CHECK(movies==chats);CHECK(st.events.front()=="predicate"&&st.events.back()=="fade");}}CHECK(st.things==0);}
int main(){
 for(int loops=0;loops<10;++loops)for(int talk=0;talk<3;++talk)for(int hit=0;hit<8;++hit){cfg=Config();cfg.loops=loops;cfg.talk=talk;cfg.physical=(hit&1)?2:-1;cfg.ability=(hit&2)?2:-1;cfg.heal=(hit&4)?2:-1;run();}
 const float values[]={-1.0f,0.0f,1.5f,std::numeric_limits<float>::quiet_NaN()};for(int h=0;h<4;++h)for(int d=0;d<4;++d)for(int valid=0;valid<2;++valid){cfg=Config();cfg.health=values[h];const float dist[]={2.5f,3.0f,3.5f,std::numeric_limits<float>::quiet_NaN()};cfg.distance=dist[d];cfg.valid=valid!=0;cfg.loops=8;run();}
 for(int claim=0;claim<3;++claim)for(int task=0;task<3;++task)for(int start=0;start<3;++start)for(int intro=0;intro<3;++intro)for(int initial=0;initial<2;++initial){cfg=Config();cfg.claimDelay=claim;cfg.taskDelay=task;cfg.startDelay=start+1;cfg.introDelay=intro+start+2;cfg.initialValid=initial!=0;run();}
 for(int mode=0;mode<12;++mode)for(int stop=1;stop<=120;++stop){cfg=Config();cfg.stopAt=stop;cfg.loops=8;cfg.claimDelay=mode%3;cfg.taskDelay=mode%4;cfg.startDelay=mode%3+1;cfg.introDelay=mode%4+4;cfg.initialValid=mode%2!=0;cfg.talk=mode%3;cfg.physical=mode%4==0?2:-1;cfg.ability=mode%4==1?3:-1;cfg.heal=mode%4==2?3:-1;cfg.health=values[mode%4];run();}
 CHECK(caseNumber==1874);std::puts("00dc7490_TEST PASS");return 0;
}