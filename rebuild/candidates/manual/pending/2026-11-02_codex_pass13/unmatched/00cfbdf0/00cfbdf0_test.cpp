#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct State{int frame,limit,checks,stopCheck,things,timers,predicates,sex,initialCount;long initialPosition,initialFlash,initialConv;bool removed;float distance;int n,disabledAt[17],effects,conversations,people,removes;std::string text[17];std::vector<int>teleports,facings,scans,destroys;std::vector<long>assignments;std::vector<std::string>lines;State():frame(0),limit(8),checks(0),stopCheck(0),things(0),timers(0),predicates(0),sex(1),initialCount(0),initialPosition(0),initialFlash(0),initialConv(0),removed(false),distance(5),n(1),effects(0),conversations(0),people(0),removes(0){for(int i=0;i<17;++i)disabledAt[i]=999;}bool term(){++checks;return removed||frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString():text(){}CCharString(const char*s):text(s){}CCharString(const std::string&s):text(s){}bool operator!=(const char*s)const{return text!=s;}};
CCharString GFIntToCharString(long n){char b[40];std::sprintf(b,"%ld",n);return CCharString(b);}CCharString operator+(const char*s,const CCharString&t){return CCharString(std::string(s)+t.text);}
struct C3DVector{int owner;C3DVector(int n):owner(n){}};
class CScriptThing{public:int id;CScriptThing(int n=-3):id(n){++state->things;}CScriptThing(const CScriptThing&t):id(t.id){++state->things;}~CScriptThing(){--state->things;}void SetDataString(const CCharString&s){assert(id>=100&&id<=116);state->text[id-100]=s.text;}CCharString GetDataString()const{assert(id>=100&&id<=116);return CCharString(state->text[id-100]);}C3DVector GetPos()const{assert(id==-1);return C3DVector(id);}};
class CTimer{int serial;long until;public:CTimer():serial(state->timers),until(serial==0?state->initialFlash:state->initialConv){assert(serial<2);++state->timers;if(until>=0)until+=state->frame;}~CTimer(){assert(state->timers==serial+1);--state->timers;state->destroys.push_back(serial);}CTimer&operator=(long n){assert((serial==0&&n==8)||(serial==1&&n==16));state->assignments.push_back(n);until=state->frame+n;return *this;}bool operator==(long n)const{long v=until<0?until:until<=state->frame?0:until-state->frame;return v==n;}};
class TRetVoid{public:void Return(){}};enum ESex{SEX_NULL=0,SEX_MALE=1,SEX_FEMALE=2,NO_OF_SEXES=3};
bool IsDistanceBetweenThingsUnder(const CScriptThing&m,const CScriptThing&h,float d){assert(m.id==-1&&h.id==-2&&d==15.0f);return state->distance<d;}
class CGameScriptInterfaceBase{public:void NewScriptFrame(){assert(state->frame<1000);++state->frame;}CScriptThing GetNearestWithScriptName(const CScriptThing&t,const CCharString&s){assert(s.text=="GhostPos");assert(t.id==-3||(t.id>=0&&t.id<state->n));int i=t.id==-3?16:t.id;state->scans.push_back(i);return CScriptThing(100+i);}void EntityTeleportToThing(const CScriptThing&m,const CScriptThing&t,bool b){assert(m.id==-1&&t.id>=100&&!b);state->teleports.push_back(t.id-100);}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&t,bool b){assert(m.id==-1&&t.id>=0&&!b);state->facings.push_back(t.id);}bool IsDiggingSpotEnabled(const CScriptThing&t){int i=t.id==-3?16:t.id;return state->frame<state->disabledAt[i];}CScriptThing GetHero(){return CScriptThing(-2);}CScriptThing CreateEffect(const CCharString&e,const C3DVector&p,const CCharString&s,float f,bool a,bool b){assert(e.text=="Ghost_Appear_01"&&p.owner==-1&&s.text.empty()&&f==0.0f&&!a&&!b);++state->effects;return CScriptThing(-4);}long AddNewConversation(const CScriptThing&m,bool a,bool b){assert(m.id==-1&&!a&&!b);return ++state->conversations;}void AddPersonToConversation(long i,const CScriptThing&h){assert(i==state->conversations&&h.id==-2);++state->people;}ESex EntityGetSex(const CScriptThing&m){assert(m.id==-1);return ESex(state->sex);}void AddLineToConversation(long i,const CCharString&s,bool b,const CScriptThing&m,const CScriptThing&h){assert(i==state->conversations&&!b&&m.id==-1&&h.id==-2);state->lines.push_back(s.text);}void RemoveThing(const CScriptThing&m,bool a,bool b){assert(m.id==-1&&!a&&b);++state->removes;state->removed=true;}};
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&m){assert(m.id==-1);}};class CQ_AwakeningTheOracleScript{public:long GraveNumbersGivenOut,PositionTaken;std::vector<CScriptThing>DiggingPoints;CQ_AwakeningTheOracleScript():GraveNumbersGivenOut(state->initialCount),PositionTaken(state->initialPosition){for(int i=0;i<state->n;++i)DiggingPoints.push_back(CScriptThing(i));}class CNecropolisGhost{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_AwakeningTheOracleScript&ParentClass;long MyGraveNumber;CNecropolisGhost(CGameScriptInterfaceBase&g,CQ_AwakeningTheOracleScript&p):Game(g),Me(-1),ParentClass(p),MyGraveNumber(0){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CQ_AwakeningTheOracleScript::CNecropolisGhost::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing DiggingPoint;
    ++ParentClass.GraveNumbersGivenOut;
    MyGraveNumber=ParentClass.GraveNumbersGivenOut;
    if (ParentClass.DiggingPoints.size()>ParentClass.PositionTaken) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        DiggingPoint=ParentClass.DiggingPoints[ParentClass.PositionTaken];
        ++ParentClass.PositionTaken;
    }
    {
        CScriptThing GhostStandsHere=Game.GetNearestWithScriptName(DiggingPoint,"GhostPos");
        GhostStandsHere.SetDataString("SpotTaken");
        Game.EntityTeleportToThing(Me,GhostStandsHere,false);
    }
    for (;!false;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CTimer DiggingFlash;
        CTimer ConvDelay;
        while (Game.IsDiggingSpotEnabled(DiggingPoint)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (DiggingFlash==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.CreateEffect("Ghost_Appear_01",Me.GetPos(),"",0.0f,false,false);
                DiggingFlash=8;
            }
            if (ConvDelay==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),15.0f) && MyGraveNumber<13) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.CreateEffect("Ghost_Appear_01",Me.GetPos(),"",0.0f,false,false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                if (Game.EntityGetSex(Me)==SEX_MALE) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B03_GHOST_MALE_NEAR_GRAVE"+GFIntToCharString(MyGraveNumber),false,Me,Game.GetHero());
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B03_GHOST_NEAR_GRAVE"+GFIntToCharString(MyGraveNumber),false,Me,Game.GetHero());
                }
                ConvDelay=16;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.CreateEffect("Ghost_Appear_01",Me.GetPos(),"",0.0f,false,false);
        if (ParentClass.DiggingPoints.size()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            for (long i=0;i<ParentClass.DiggingPoints.size();++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                DiggingPoint=ParentClass.DiggingPoints[i];
                if (Game.GetNearestWithScriptName(DiggingPoint,"GhostPos").GetDataString()!="SpotTaken") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    break;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing GhostStandsHere=Game.GetNearestWithScriptName(DiggingPoint,"GhostPos");
            if (GhostStandsHere.GetDataString()!="SpotTaken") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntityTeleportToThing(Me,GhostStandsHere,false);
                GhostStandsHere.SetDataString("SpotTaken");
                ++ParentClass.GraveNumbersGivenOut;
                MyGraveNumber=ParentClass.GraveNumbersGivenOut;
                Game.EntitySetFacingAngleTowardsThing(Me,DiggingPoint,false);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.RemoveThing(Me,false,true);
            }
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.RemoveThing(Me,false,true);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

struct Result{long count,position,grave;};
Result run(State&s){state=&s;Result r;{CGameScriptInterfaceBase g;NScript::CQ_AwakeningTheOracleScript p;NScript::CQ_AwakeningTheOracleScript::CNecropolisGhost t(g,p);t.Main();assert(s.things==s.n+1&&s.timers==0&&s.predicates==1&&s.conversations==s.people);r.count=p.GraveNumbersGivenOut;r.position=p.PositionTaken;r.grave=t.MyGraveNumber;}assert(s.things==0);for(unsigned i=0;i<s.destroys.size();i+=2){assert(i+1<s.destroys.size()&&s.destroys[i]==1&&s.destroys[i+1]==0);}return r;}
int main(){int cases=0;
 {State s;Result r=run(s);assert(r.count==1&&r.position==1&&r.grave==1&&s.effects==2&&s.conversations==1&&s.lines[0]=="TEXT_QST_B03_GHOST_MALE_NEAR_GRAVE1"&&s.text[0]=="SpotTaken");++cases;}
 for(int sex=0;sex<4;++sex)for(int num=0;num<14;++num){State s;s.sex=sex;s.initialCount=num-1;s.limit=3;Result r=run(s);assert(r.grave==num);assert(s.conversations==(num<13?1:0));if(num<13){char b[64];std::sprintf(b,"%s%ld",sex==1?"TEXT_QST_B03_GHOST_MALE_NEAR_GRAVE":"TEXT_QST_B03_GHOST_NEAR_GRAVE",r.grave);assert(s.lines[0]==b);}++cases;}
 for(int mode=0;mode<4;++mode){State s;s.limit=3;s.distance=mode==0?14.999f:mode==1?15.0f:mode==2?16.0f:std::numeric_limits<float>::quiet_NaN();run(s);assert(s.conversations==(mode==0?1:0));++cases;}
 {State s;s.limit=28;run(s);assert(s.effects==6&&s.conversations==2&&s.assignments.size()==6);++cases;}
 {State s;s.limit=3;s.initialFlash=-1;s.initialConv=-1;run(s);assert(s.effects==0&&s.lines.empty()&&s.assignments.empty());++cases;}
 {State s;s.limit=3;s.initialFlash=8;s.initialConv=16;run(s);assert(s.effects==0&&s.lines.empty());++cases;}
 {State s;s.n=3;s.disabledAt[0]=2;s.disabledAt[1]=3;s.disabledAt[2]=4;s.limit=20;Result r=run(s);assert(s.removes==1&&r.count==3&&r.position==1&&r.grave==3&&s.teleports.size()==3&&s.facings.size()==2&&s.teleports[1]==1&&s.teleports[2]==2);++cases;}
 {State s;s.n=3;s.disabledAt[0]=2;s.text[1]="SpotTaken";s.limit=4;Result r=run(s);assert(r.count==2&&s.teleports.size()==2&&s.teleports[1]==2);++cases;}
 {State s;s.n=2;s.disabledAt[0]=2;s.text[1]="spottaken";s.limit=4;run(s);assert(s.teleports.size()==2&&s.teleports[1]==1);++cases;}
 {State s;s.n=2;s.disabledAt[0]=2;s.text[1]="SpotTaken";s.limit=20;Result r=run(s);assert(r.count==1&&s.removes==1&&s.teleports.size()==1&&s.facings.empty());++cases;}
 for(int mode=0;mode<3;++mode){State s;s.n=2;s.initialPosition=mode==0?-1:mode==1?2:5;s.limit=3;Result r=run(s);assert(r.position==s.initialPosition&&s.teleports[0]==16&&s.text[16]=="SpotTaken");++cases;}
 {State s;s.n=0;s.disabledAt[16]=0;s.limit=20;Result r=run(s);assert(s.removes==1&&s.teleports.size()==1&&r.position==0&&r.count==1);++cases;}
 {State s;s.n=3;s.initialPosition=1;s.limit=3;Result r=run(s);assert(r.position==2&&s.teleports[0]==1&&s.text[0].empty());++cases;}
 for(int mode=0;mode<6;++mode)for(int stop=1;stop<=100;++stop){State s;s.stopCheck=stop;s.limit=30;s.n=mode==0?0:3;s.sex=mode%4;s.initialPosition=mode==1?-1:0;s.initialCount=mode==2?12:0;for(int i=0;i<17;++i)s.disabledAt[i]=mode==3?2+i:mode==4?0:999;if(mode==5){s.text[1]="SpotTaken";s.text[2]="SpotTaken";s.disabledAt[0]=2;}run(s);assert(s.checks<=stop);++cases;}
 assert(cases==673);std::puts("00cfbdf0_TEST PASS");}
