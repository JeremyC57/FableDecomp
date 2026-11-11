#include <stdio.h>
#include <string>
static bool failed,dependency_ok;static int line_failed;
#define CHECK(x) do{if(!(x)){failed=true;line_failed=__LINE__;return;}}while(0)
static int string_live,string_created,string_destroyed,action_live,actions_created,actions_destroyed,enemy_temps;static std::string trace;
class CCharString{ public:std::string text;bool alive;CCharString(const char*s):text(s),alive(true){++string_live;++string_created;}CCharString(const CCharString&o):text(o.text),alive(true){dependency_ok=dependency_ok&&o.alive;++string_live;++string_created;}~CCharString(){dependency_ok=dependency_ok&&alive;alive=false;--string_live;++string_destroyed;}};
class CDefPointeeBase{ public:mutable int refs;};
template<class T>class CDefPointer{ public:T*Object;CDefPointer(T*p=0):Object(p){if(Object)++Object->refs;}CDefPointer(const CDefPointer&o):Object(o.Object){if(Object)++Object->refs;}~CDefPointer(){if(Object)--Object->refs;}T*operator->()const{return Object;}};
class CBriarRoseDef:public CDefPointeeBase{ public:float SecondsBetweenTeleportAway;};
class CThingBaseDef:public CDefPointeeBase{ public:CBriarRoseDef*child;mutable int calls;template<class T>bool GetSubDef(CDefPointer<const T>*out)const{dependency_ok=dependency_ok&&refs==2&&!out->Object;++calls;out->Object=child;++child->refs;trace+='D';return true;}};
class CMainGameComponent{ public:static long fps;static long GetConstantFPS(){return fps;}};long CMainGameComponent::fps;
class CWorld{ public:static long frame;static long GetFrame(){return frame;}};long CWorld::frame;
class CTCParamsBase{ public:};class CTCBase{ public:};
class CTCPhysicalShieldSpell:public CTCBase{ public:long SpellLevel,DurationTimer;void SetAICasting(long v){SpellLevel=v;trace+='L';}void SetTimeLimit(long v){DurationTimer=v;trace+='T';}};
class CThingCreatureBase{ public:int refs,id;CThingBaseDef*base;CTCPhysicalShieldSpell*shield;long fps_on_add;CDefPointer<const CThingBaseDef>GetPBaseDef()const{return CDefPointer<const CThingBaseDef>(base);}CTCBase*AddTC(const CCharString&name,bool on,const CTCParamsBase*params){dependency_ok=dependency_ok&&name.alive&&name.text=="CTCPhysicalShieldSpell"&&on&&!params&&string_live>0&&base->refs==1&&base->child->refs==2;trace+='S';CMainGameComponent::fps=fps_on_add;return shield;}};
class CThingAICreature:public CThingCreatureBase{ public:};
template<class T>class CIntelligentPointer{ public:T*p;bool temporary;CIntelligentPointer(T*q=0,bool t=false):p(q),temporary(t){if(p)++p->refs;if(temporary)++enemy_temps;}CIntelligentPointer(const CIntelligentPointer&o):p(o.p),temporary(o.temporary){if(p)++p->refs;if(temporary)++enemy_temps;}~CIntelligentPointer(){if(p)--p->refs;if(temporary){--enemy_temps;trace+='E';}}T&operator*()const{return *p;}};
class CCreatureActionBase{ public:virtual ~CCreatureActionBase(){}};
class CCombatActionBase:public CCreatureActionBase{ public:CThingCreatureBase*owner;CCombatActionBase(CThingCreatureBase&o):owner(&o){++action_live;++actions_created;}CCombatActionBase(const CCombatActionBase&o):owner(o.owner){++action_live;++actions_created;}virtual~CCombatActionBase(){--action_live;++actions_destroyed;trace+='B';}};
class CCreatureAction_BriarRoseIntoInvulnerable:public CCombatActionBase{ public:CIntelligentPointer<CThingCreatureBase>PEnemy;CCreatureAction_BriarRoseIntoInvulnerable(CThingCreatureBase&a,CThingCreatureBase&b):CCombatActionBase(a),PEnemy(&b,true){trace+='A';}};
enum EAIActionReturn{ACTION0,ACTION1,ACTION2};
template<class A,class B>B*GFDebugPDynamicCast(A*a,B*){return static_cast<B*>(a);}
struct CAIStateGroup_BriarRose{CThingAICreature*creature;CIntelligentPointer<CThingCreatureBase>PEnemy;CWorld*world;CThingBaseDef*base;CBriarRoseDef*def;bool first,second,mutate_wait;int runnable_calls,terminated,actions,world_calls;long NextRunFrame,action_frame,action_fps;EAIActionReturn returned;CThingAICreature&GetCreature(){return *creature;}bool IsStillRunnable(){++runnable_calls;trace+='R';return runnable_calls==1?first:second;}CWorld&GetWorld(){++world_calls;trace+='W';return *world;}void Terminate(){++terminated;trace+='X';dependency_ok=dependency_ok&&action_live==0&&enemy_temps==0&&base->refs==1&&def->refs==(first?2:1);}EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase&action,bool wait){const CCreatureAction_BriarRoseIntoInvulnerable&a=static_cast<const CCreatureAction_BriarRoseIntoInvulnerable&>(action);dependency_ok=dependency_ok&&!wait&&a.owner==creature&&a.PEnemy.p==PEnemy.p&&action_live==1&&enemy_temps==1&&PEnemy.p->refs==2&&string_live==0&&def->refs==2;trace+='P';++actions;CWorld::frame=action_frame;CMainGameComponent::fps=action_fps;if(mutate_wait)def->SecondsBetweenTeleportAway=64.0f;return returned;}void Update();};
void CAIStateGroup_BriarRose::Update()
{
 if(!IsStillRunnable()){Terminate();return;}
 CDefPointer<const CBriarRoseDef>pdef;
 GetCreature().GetPBaseDef()->GetSubDef(&pdef);
 float seconds_between_teleport=pdef->SecondsBetweenTeleportAway;
 NextRunFrame=(long)(GetWorld().GetFrame()+CMainGameComponent::GetConstantFPS()*seconds_between_teleport);
 CTCPhysicalShieldSpell*ptc_shield=GFDebugPDynamicCast(GetCreature().AddTC("CTCPhysicalShieldSpell",true,0),(CTCPhysicalShieldSpell*)0);
 const long SHIELD_LEVEL=2;
 const float SECONDS_FOR_SHIELD_TO_BE_ACTIVE=30.0f;
 ptc_shield->SetAICasting(SHIELD_LEVEL);
 ptc_shield->SetTimeLimit((long)(CMainGameComponent::GetConstantFPS()*SECONDS_FOR_SHIELD_TO_BE_ACTIVE));
 EAIActionReturn ret=AIAction_PerformActionAndWait(CCreatureAction_BriarRoseIntoInvulnerable(GetCreature(),*PEnemy),false);
 if(!IsStillRunnable()){Terminate();return;}
 NextRunFrame=(long)(GetWorld().GetFrame()+CMainGameComponent::GetConstantFPS()*seconds_between_teleport);
 Terminate();
}

static void one(int first,int second,int wait_index,int fps_index,int frame_index,int changed,int ret,int mutate)
{
 failed=false;dependency_ok=true;string_live=string_created=string_destroyed=action_live=actions_created=actions_destroyed=enemy_temps=0;trace="";
 const float waits[]={-2.0f,-0.5f,0.0f,2.5f};const long frames[]={-128,-1,0,1024};const long fpss[]={1,15,32};float initial_wait=waits[wait_index];long initial_frame=frames[frame_index],initial_fps=fpss[fps_index];
 CBriarRoseDef def;def.refs=1;def.SecondsBetweenTeleportAway=initial_wait;CThingBaseDef base;base.refs=1;base.child=&def;base.calls=0;CTCPhysicalShieldSpell shield;shield.SpellLevel=-1;shield.DurationTimer=-1;CThingAICreature creature;creature.id=7;creature.refs=0;creature.base=&base;creature.shield=&shield;creature.fps_on_add=fpss[changed];CThingCreatureBase enemy;enemy.id=11;enemy.refs=0;CWorld world;CWorld::frame=initial_frame;CMainGameComponent::fps=initial_fps;
 {
 CAIStateGroup_BriarRose g;g.creature=&creature;g.PEnemy.p=&enemy;g.PEnemy.temporary=false;++enemy.refs;g.world=&world;g.base=&base;g.def=&def;g.first=first!=0;g.second=second!=0;g.mutate_wait=mutate!=0;g.runnable_calls=g.terminated=g.actions=g.world_calls=0;g.NextRunFrame=-777;g.action_frame=initial_frame+16;g.action_fps=fpss[(changed+1)%3];g.returned=(EAIActionReturn)ret;
 g.Update();CHECK(dependency_ok&&g.terminated==1&&g.runnable_calls==(first?2:1));CHECK(g.actions==(first?1:0)&&base.calls==(first?1:0));CHECK(g.world_calls==(first?(second?2:1):0));CHECK(base.refs==1&&def.refs==1&&enemy.refs==1&&creature.refs==0);CHECK(action_live==0&&enemy_temps==0&&actions_created==actions_destroyed);CHECK(string_live==0&&string_created==string_destroyed);CHECK(string_created==(first?1:0));
 long expected=-777;if(first){double schedule=double(initial_frame)+double(initial_fps)*initial_wait;expected=(long)schedule;if(second)expected=(long)(double(g.action_frame)+double(g.action_fps)*initial_wait);CHECK(shield.SpellLevel==2&&shield.DurationTimer==creature.fps_on_add*30);CHECK(def.SecondsBetweenTeleportAway==(mutate?64.0f:initial_wait));CHECK(CWorld::frame==g.action_frame&&CMainGameComponent::fps==g.action_fps);CHECK(trace==(second?"RDWSLTAPEBRWX":"RDWSLTAPEBRX"));}else{CHECK(shield.SpellLevel==-1&&shield.DurationTimer==-1&&def.SecondsBetweenTeleportAway==initial_wait);CHECK(CWorld::frame==initial_frame&&CMainGameComponent::fps==initial_fps);CHECK(trace=="RX");}CHECK(g.NextRunFrame==expected);
 }
 CHECK(enemy.refs==0&&base.refs==1&&def.refs==1);
}
int main(){unsigned long cases=0;for(int f=0;f<2;++f)for(int s=0;s<2;++s)for(int w=0;w<4;++w)for(int p=0;p<3;++p)for(int n=0;n<4;++n)for(int c=0;c<3;++c)for(int r=0;r<3;++r)for(int m=0;m<2;++m){one(f,s,w,p,n,c,r,m);if(failed){printf("00956e70_TEST FAIL line%d case%lu\n",line_failed,cases);return 1;}++cases;}if(cases!=3456)return 2;puts("00956e70_TEST PASS");return 0;}