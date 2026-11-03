// Selected source fd87e84d02d5786422298f60d5abc6314b1d482b664c4b7964c3a748407b9d67

#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <limits>
struct Conversation{long id;std::vector<int>line;bool person,removed;Conversation(long n):id(n),person(false),removed(false){}};
struct State{int frame,limit,checks,stopCheck,things,strings,resources,timers,timerCreates,timerDestroys,timer,timerSets,timerGets,predicates,starts,startFailures,releases,targetCalls,friendGets,homeReads,posReads,moves,clearCalls,facing,taskUntil,taskDelay,moveUntil,moveDelay,objectives,pauses,removeCalls,changeAt,changeTo,changeAt2,changeTo2,detectAt;long paranoid;bool hide,seen,heard,quest,detectOn,stopRemoved;float distance,startDistance;std::vector<Conversation>conv;State():frame(0),limit(30),checks(0),stopCheck(0),things(0),strings(0),resources(0),timers(0),timerCreates(0),timerDestroys(0),timer(0),timerSets(0),timerGets(0),predicates(0),starts(0),startFailures(0),releases(0),targetCalls(0),friendGets(0),homeReads(0),posReads(0),moves(0),clearCalls(0),facing(0),taskUntil(0),taskDelay(0),moveUntil(999),moveDelay(2),objectives(0),pauses(0),removeCalls(0),changeAt(999),changeTo(0),changeAt2(999),changeTo2(0),detectAt(999),paranoid(0),hide(true),seen(false),heard(false),quest(false),detectOn(false),stopRemoved(true),distance(6.0f),startDistance(0.0f){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck)||(stopRemoved&&removeCalls>0);}};State*state;
class CCharString{public:std::string s;CCharString(const char*p):s(p){++state->strings;}CCharString(const CCharString&o):s(o.s){++state->strings;}~CCharString(){--state->strings;}};class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum EScriptEntityMoveType{ENTITY_MOVE_WALK=0};class TRetVoid{public:void Return(){}};
class CScriptThing{C3DVector pos;public:int id;CScriptThing(int n=-1):pos(4,5,6),id(n){++state->things;}CScriptThing(const CScriptThing&o):pos(o.pos),id(o.id){++state->things;}~CScriptThing(){--state->things;}C3DVector GetHomePos()const{assert(id==-1);++state->homeReads;return C3DVector(7,8,9);}const C3DVector&GetPos()const{assert(id==-1);++state->posReads;return pos;}};
class CTimer{public:CTimer(){assert(state->timers==0);++state->timers;++state->timerCreates;}~CTimer(){assert(state->timers==1);--state->timers;++state->timerDestroys;}CTimer&operator=(long t){assert(t==5||t==9);state->timer=static_cast<int>(t);++state->timerSets;return*this;}bool operator==(long t)const{assert(t==0);++state->timerGets;return state->timer==t;}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->timers==0);--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->releases;}bool IsPerformingScriptTask()const{assert(valid);return state->frame<state->taskUntil;}void MoveToPosition(const C3DVector&p,float n,EScriptEntityMoveType m,bool a,bool b){assert(valid&&p.X==1&&p.Y==2&&p.Z==3&&n==0&&m==ENTITY_MOVE_WALK&&!a&&!b);++state->moves;state->taskUntil=state->frame+state->taskDelay;state->moveUntil=state->frame+state->moveDelay;}void ClearCommands(){assert(valid&&state->paranoid!=5);++state->clearCalls;state->taskUntil=0;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void self(const CScriptThing&m)const{assert(m.id==-1);}void NewScriptFrame(){assert(state->frame<255);++state->frame;if(state->timer>0)--state->timer;if(state->frame>=state->moveUntil)state->startDistance=0;if(state->frame==state->changeAt)state->paranoid=state->changeTo;if(state->frame==state->changeAt2)state->paranoid=state->changeTo2;if(state->frame>=state->detectAt&&state->detectOn)state->seen=true;}CScriptThing&GetHero(){return hero;}CScriptThing GetThingWithScriptName(const CCharString&s){if(s.s=="SecretWhisperingFriend"){assert(state->predicates==0);++state->friendGets;return CScriptThing(1);}assert(s.s=="M_SecretWhisperLookAtPos");return CScriptThing(2);}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){self(m);assert(p==SCRIPT_AI_PRIORITY_HIGHEST&&!r.valid);return r.valid=++state->starts>state->startFailures;}bool IsQuestActive(const CCharString&q){assert(q.s=="Q_HeroSoulsMother");return state->quest;}void RemoveThing(const CScriptThing&m,bool a,bool b){self(m);assert(!a&&!b);++state->removeCalls;}void GiveThingBestEnemyTarget(const CScriptThing&m,const CScriptThing&h){self(m);assert(h.id==-2&&state->paranoid==5);++state->targetCalls;}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&mark,bool b){self(m);assert(mark.id==2&&!b&&state->paranoid!=5);++state->facing;}bool CanThingBe_Seen_ByOtherThing(const CScriptThing&h,const CScriptThing&m){assert(h.id==-2);self(m);return state->seen;}bool CanThingBe_Heard_ByOtherThing(const CScriptThing&h,const CScriptThing&m){assert(h.id==-2);self(m);return state->heard;}long AddNewConversation(const CScriptThing&m,bool a,bool b){self(m);assert(!a&&!b);long id=100+static_cast<long>(state->conv.size());state->conv.push_back(Conversation(id));return id;}Conversation&conv(long id){assert(id>=100&&id<100+static_cast<long>(state->conv.size()));return state->conv[static_cast<unsigned>(id-100)];}void AddPersonToConversation(long id,const CScriptThing&f){assert(f.id==1&&!conv(id).person);conv(id).person=true;}void AddLineToConversation(long id,const CCharString&s,bool b,const CScriptThing&speaker,const CScriptThing&listener){assert(!b&&conv(id).person&&!conv(id).removed);const char*texts[]={"TEXT_QST_054_WHISPERING","TEXT_QST_054_HERO_SPOTTED","TEXT_QST_054_HERO_HEARD","TEXT_QST_054_PRETENDING","TEXT_QST_054_REVERT_TO_WHISPERING","TEXT_QST_054_GIVING_CLUES_01","TEXT_QST_054_GIVING_CLUES_02","TEXT_QST_054_GIVING_CLUES_03","TEXT_QST_054_GIVING_CLUES_04","TEXT_QST_054_GIVING_CLUES_05","TEXT_QST_054_GIVING_CLUES_06","TEXT_QST_054_GIVING_CLUES_07"};int code=-1;for(int i=0;i<12;++i)if(s.s==texts[i])code=i;assert(code>=0);bool friendSpeaking=code==6||code==8||code==10;assert(speaker.id==(friendSpeaking?1:-1)&&listener.id==(friendSpeaking?-1:1));if(code<5)assert(conv(id).line.empty());else assert(code==5+static_cast<int>(conv(id).line.size()));conv(id).line.push_back(code);}void RemoveConversation(long id,bool b){assert(!b&&!conv(id).removed&&conv(id).line.size()==7);conv(id).removed=true;}void SetQuestCardObjective(const CCharString&a,const CCharString&b,const CCharString&c,const CCharString&d){assert(a.s=="V_MayorsInvitation"&&b.s=="TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_05"&&c.s=="OakBay"&&d.s.empty()&&state->paranoid==4&&!state->hide);++state->objectives;}void Pause(float t){assert(t==2);++state->pauses;}};
bool IsDistanceBetweenThingsUnder(const CScriptThing&m,const CScriptThing&h,float n){assert(m.id==-1&&h.id==-2&&(n==5||n==10||n==12));return state->distance<n;}bool IsDistanceBetweenThingsOver(const CScriptThing&m,const CScriptThing&h,float n){assert(m.id==-1&&h.id==-2&&n==10);return state->distance>n;}bool IsDistanceFromThingToPositionOver(const CScriptThing&m,const C3DVector&p,float n){assert(m.id==-1&&p.X==1&&p.Y==2&&p.Z==3&&n==0.5f);return state->startDistance>n;}
namespace NScript{class CIsThingAlive{CScriptThing Thing;public:CIsThingAlive(const CScriptThing&m):Thing(m){assert(m.id==-1);}};class CV_ParanoidWhispersScript{public:enum PARANOID_STATES{PS_NONE=0,PS_REPOSITION=1,PS_WHISPERING=2,PS_WHISTLING=3,PS_EXPLAINING_ALL=4,PS_ANGRY=5,PS_MAX=6};long&ParanoidState;bool&HideDiggingSpot;CV_ParanoidWhispersScript():ParanoidState(state->paranoid),HideDiggingSpot(state->hide){}class CSecretWhisperingMan{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CV_ParanoidWhispersScript&ParentClass;C3DVector StartPos;CSecretWhisperingMan(CGameScriptInterfaceBase&g,CV_ParanoidWhispersScript&p):Game(g),Me(-1),ParentClass(p),StartPos(1,2,3){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}
void NScript::CV_ParanoidWhispersScript::CSecretWhisperingMan::Main()
{
    TRetVoid _Fail_Val;
    CScriptThing Friend=Game.GetThingWithScriptName("SecretWhisperingFriend");
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
    CTimer TalkTimer;
    if (Game.IsQuestActive("Q_HeroSoulsMother")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Me,false,false);
    }
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.ParanoidState==PS_ANGRY) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            if (seh_me.IsValid()) seh_me.Release();
            while (ParentClass.ParanoidState==PS_ANGRY) {
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
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.ParanoidState==PS_REPOSITION) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            C3DVector HomePos=Me.GetHomePos();
            C3DVector MyPos=Me.GetPos();
            while (IsDistanceFromThingToPositionOver(Me,StartPos,0.5f) && ParentClass.ParanoidState==PS_REPOSITION) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!seh_me.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(StartPos,0,ENTITY_MOVE_WALK,false,false);
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.ParanoidState!=PS_ANGRY) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetThingWithScriptName("M_SecretWhisperLookAtPos"),false);
                while (ParentClass.ParanoidState==PS_REPOSITION) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.ParanoidState==PS_WHISPERING) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (TalkTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),12.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                TalkTimer=5;
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_WHISPERING",false,Me,Friend);
            }
            if (IsDistanceBetweenThingsUnder(Me,Game.GetHero(),5.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(),Me)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                    Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_HERO_SPOTTED",false,Me,Friend);
                    ParentClass.ParanoidState=PS_WHISTLING;
                } else if (Game.CanThingBe_Heard_ByOtherThing(Game.GetHero(),Me)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                    Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_HERO_HEARD",false,Me,Friend);
                    ParentClass.ParanoidState=PS_WHISTLING;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.ParanoidState=PS_EXPLAINING_ALL;
                    ParentClass.HideDiggingSpot=false;
                    Game.SetQuestCardObjective("V_MayorsInvitation","TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_05","OakBay","");
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        TalkTimer=9;
        while (ParentClass.ParanoidState==PS_WHISTLING) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (TalkTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),10.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                TalkTimer=9;
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_PRETENDING",false,Me,Friend);
            } else if (IsDistanceBetweenThingsOver(Me,Game.GetHero(),10.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_REVERT_TO_WHISPERING",false,Me,Friend);
                ParentClass.ParanoidState=PS_WHISPERING;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.ParanoidState==PS_EXPLAINING_ALL) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_01",false,Me,Friend);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_02",false,Friend,Me);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_03",false,Me,Friend);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_04",false,Friend,Me);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_05",false,Me,Friend);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_06",false,Friend,Me);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_07",false,Me,Friend);
            Game.Pause(2.0f);
            while (ParentClass.ParanoidState==PS_EXPLAINING_ALL && !IsDistanceBetweenThingsOver(Me,Game.GetHero(),10.0f) && !Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(),Me) && !Game.CanThingBe_Heard_ByOtherThing(Game.GetHero(),Me)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.ParanoidState!=PS_ANGRY) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.ParanoidState=PS_WHISPERING;
            }
            Game.RemoveConversation(HLStyleConversationIndex,false);
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}


int scenarios=0;int lines(const State&s,int code){int n=0;for(unsigned i=0;i<s.conv.size();++i)for(unsigned j=0;j<s.conv[i].line.size();++j)if(s.conv[i].line[j]==code)++n;return n;}void run(State&s){++scenarios;state=&s;{CGameScriptInterfaceBase game;NScript::CV_ParanoidWhispersScript parent;NScript::CV_ParanoidWhispersScript::CSecretWhisperingMan man(game,parent);man.Main();}assert(s.things==0&&s.strings==0&&s.resources==0&&s.timers==0&&s.predicates==1&&s.friendGets==1&&s.timerCreates==s.timerDestroys&&s.homeReads==s.posReads);for(unsigned i=0;i<s.conv.size();++i){assert(s.conv[i].person);assert(s.conv[i].line.size()==1||s.conv[i].line.size()==7);}}
int main(){
 {State s;run(s);assert(s.conv.empty()&&s.starts==1&&s.timerCreates==1);}
 {State s;s.paranoid=2;run(s);assert(lines(s,0)==6&&s.paranoid==2&&s.hide);}
 {State s;s.paranoid=2;s.distance=4;run(s);assert(s.paranoid==4&&!s.hide&&s.objectives==1&&s.pauses==1&&lines(s,0)==1&&lines(s,5)==1&&s.conv.back().line.size()==7&&!s.conv.back().removed);}
 {State s;s.paranoid=2;s.distance=4;s.seen=true;run(s);assert(s.paranoid==3&&lines(s,1)==1&&lines(s,2)==0&&lines(s,3)==3&&s.hide);}
 {State s;s.paranoid=2;s.distance=4;s.heard=true;run(s);assert(s.paranoid==3&&lines(s,2)==1&&lines(s,1)==0);}
 {State s;s.paranoid=3;s.distance=10.5f;run(s);assert(lines(s,4)==1&&s.paranoid==2);}
 {State s;s.paranoid=5;run(s);assert(s.targetCalls==1&&s.releases==1&&s.starts==1&&s.conv.empty());}
 {State s;s.paranoid=5;s.changeAt=4;s.changeTo=2;run(s);assert(s.targetCalls==1&&s.releases==1&&s.starts==2&&lines(s,0)>0);}
 float distances[10]={-1.0f,4.5f,5.0f,5.5f,9.5f,10.0f,10.5f,11.5f,12.0f,std::numeric_limits<float>::quiet_NaN()};
 for(int p=0;p<7;++p)for(int d=0;d<10;++d)for(int senses=0;senses<4;++senses){State s;s.paranoid=p;s.distance=distances[d];s.seen=(senses&1)!=0;s.heard=(senses&2)!=0;run(s);if(p==0||p==6)assert(s.conv.empty()&&s.moves==0);if(p==1)assert(s.homeReads==1&&s.clearCalls==1&&s.facing==1&&s.conv.empty());if(p==5)assert(s.targetCalls==1&&s.releases==1&&s.conv.empty());if(p==2&&d>=2){assert(s.paranoid==2&&s.hide);assert(lines(s,0)==(d<8?6:0));}if(p==2&&d<2){assert(lines(s,0)==1);if(senses&1)assert(lines(s,1)==1&&lines(s,2)==0);else if(senses&2)assert(lines(s,2)==1);else assert(s.objectives==1&&lines(s,5)==1);}if(p==3&&d==5)assert(s.paranoid==3&&s.conv.empty());}
 float offsets[5]={-1.0f,0.0f,0.5f,1.0f,std::numeric_limits<float>::quiet_NaN()};
 for(int o=0;o<5;++o)for(int delay=0;delay<5;++delay){State s;s.paranoid=1;s.startDistance=offsets[o];s.taskDelay=delay;s.moveDelay=1;s.changeAt=8;s.changeTo=2;run(s);assert(s.homeReads==1&&s.clearCalls==1&&s.facing==1&&s.moves==(o==3?(delay<2?2:1):0)&&lines(s,0)>0);}
 {State s;s.paranoid=1;s.startDistance=1;s.changeAt=3;s.changeTo=5;run(s);assert(s.clearCalls==0&&s.facing==0&&s.targetCalls==1);}
 {State s;s.paranoid=2;s.distance=4;s.detectOn=true;s.detectAt=6;run(s);assert(lines(s,5)==1&&s.conv[1].removed&&lines(s,1)==1);}
 {State s;s.paranoid=4;s.changeAt=6;s.changeTo=5;run(s);assert(lines(s,5)==1&&s.conv[0].removed&&s.targetCalls==1&&s.paranoid==5);}
 {State s;s.quest=true;run(s);assert(s.removeCalls==1&&s.conv.empty()&&s.timerCreates==1);}
 {State s;s.quest=true;s.stopRemoved=false;s.paranoid=2;run(s);assert(s.removeCalls==1&&lines(s,0)>0);}
 for(int fail=0;fail<8;++fail){State s;s.paranoid=2;s.startFailures=fail;run(s);assert(s.starts==fail+1&&lines(s,0)>0);}
 for(int stop=1;stop<=200;++stop)for(int mode=0;mode<20;++mode){State s;s.stopCheck=stop;s.limit=20;s.paranoid=2;switch(mode){case 0:break;case 1:s.distance=4;break;case 2:s.distance=4;s.seen=true;break;case 3:s.distance=4;s.heard=true;break;case 4:s.paranoid=1;break;case 5:s.paranoid=1;s.startDistance=1;s.taskDelay=3;s.moveDelay=1;break;case 6:s.paranoid=5;break;case 7:s.paranoid=5;s.changeAt=5;s.changeTo=2;break;case 8:s.paranoid=3;s.distance=6;break;case 9:s.paranoid=3;s.distance=11;break;case 10:s.paranoid=4;break;case 11:s.paranoid=4;s.seen=true;break;case 12:s.paranoid=4;s.heard=true;break;case 13:s.paranoid=4;s.changeAt=7;s.changeTo=5;break;case 14:s.quest=true;break;case 15:s.startFailures=30;break;case 16:s.distance=12;break;case 17:s.distance=std::numeric_limits<float>::quiet_NaN();break;case 18:s.paranoid=1;s.changeAt=4;s.changeTo=2;break;case 19:s.paranoid=0;break;}run(s);}
 assert(scenarios==4326);std::puts("00ebfd10_TEST PASS");return 0;
}