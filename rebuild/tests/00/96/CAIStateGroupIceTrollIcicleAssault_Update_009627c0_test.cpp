// CAIStateGroup_IceTrollIcicleAssault::Update() -- mark the target in danger, wait for current actions and perform the icicle assault before terminating.
// Retail009627c0 length260. Shared group/Base/Brain layout. CThing component map44 differs donor48.
// Action retail114 uses real combat-base virtual destructor; generated Into donor size12c differs native114.
#ifdef _MSC_VER
#include "engine/CAIStateGroup_IceTrollIcicleAssault.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureAction_IceTrollIcicleAssaultInto.h"
#else
class CAIBrain{ public:char prefix[24];void* Creature;};
class CAIStateGroupBase{ public:void* vtable;CAIBrain* PBrain;char state[12];};
struct CAIStateGroup_IceTrollIcicleAssault:CAIStateGroupBase{void* PTarget[2];};
#define __forceinline inline
#endif
struct CIceThing;
struct CStealth {void SetAsInDanger(CIceThing*,long);}; //00747b80
struct CIceMap{struct Entry{long type;void* component;};Entry* first;Entry* last;Entry* LowerBound(const long*);}; //0040f020
struct CIceThing {char prefix[0x20];unsigned long components[9];CIceMap map;char beforeDead[0x45];unsigned char dead;
bool IsPerformingAnyAction(); //006622a0
__forceinline bool GetStealth(CStealth** out){if(components[1]&0x400000){long key=54;CIceMap* m=&map;CIceMap::Entry* it=m->LowerBound(&key);if(it==m->last||54<it->type)it=m->last;*out=(CStealth*)it->component;return true;}return false;}
};
struct CIcePointer {CIceThing* GetPItem()const;}; //00a01b50, methods view over opaque shared PTarget
struct CIceCombatAction {virtual ~CIceCombatAction();char state[0x110];}; //0062e900
struct CIceTrollIcicleAssaultIntoNative:CIceCombatAction{CIceTrollIcicleAssaultIntoNative(CIceThing*,CIceThing*);}; //00962990
struct IceAssaultMethods:CAIStateGroup_IceTrollIcicleAssault {
__forceinline CIcePointer& Target(){return *(CIcePointer*)PTarget;}
__forceinline CIceThing* Creature(){return (CIceThing*)((CAIStateGroupBase*)this)->PBrain->Creature;}
__forceinline bool TargetAlive(){if(Target().GetPItem())if(!(Target().GetPItem()->dead&1))return true;return false;}
void Terminate();long AIAction_DoNothing();long AIAction_CombatPerformActionAndWait(CIceCombatAction*,bool); //008fcf40,0097e200,0097da40
void Update();
};
void IceAssaultMethods::Update(){
if(!TargetAlive()){Terminate();return;}
CStealth* stealth;
if(Target().GetPItem()->GetStealth(&stealth))stealth->SetAsInDanger(Creature(),3);
while(Creature()->IsPerformingAnyAction())if(AIAction_DoNothing()==1)goto finish;
if(TargetAlive()){
CIceThing* actor=Creature();
long result=AIAction_CombatPerformActionAndWait(&CIceTrollIcicleAssaultIntoNative(actor,Target().GetPItem()),false);
if(result==2){Terminate();return;}
}
finish:Terminate();
}

#include <cstdio>
static int errors,scenario,queries,idles,dangers,actions,terminates,constructed,destroyed,alive;
static CIceThing owner,target;static CStealth stealth;static CIceThing* targetValue;static IceAssaultMethods* expected;
CIceThing* CIcePointer::GetPItem()const{if(this!=(CIcePointer*)expected->PTarget)++errors;return targetValue;}
CIceMap::Entry* CIceMap::LowerBound(const long* key){if(*key!=54)++errors;Entry* p=first;while(p!=last&&p->type<*key)++p;return p;}
void CStealth::SetAsInDanger(CIceThing* p,long status){++dangers;if(this!=&stealth||p!=&owner||status!=3)++errors;}
bool CIceThing::IsPerformingAnyAction(){++queries;if(this!=&owner)++errors;return (scenario==3||scenario==4||scenario==7)&&queries<3;}
long IceAssaultMethods::AIAction_DoNothing(){++idles;if(scenario==3)return 1;if(scenario==4)target.dead=1;return 4;}
void IceAssaultMethods::Terminate(){++terminates;if(alive)++errors;}
CIceTrollIcicleAssaultIntoNative::CIceTrollIcicleAssaultIntoNative(CIceThing* a,CIceThing* b){++constructed;++alive;if(a!=&owner||b!=&target)++errors;}
CIceCombatAction::~CIceCombatAction(){++destroyed;--alive;}
long IceAssaultMethods::AIAction_CombatPerformActionAndWait(CIceCombatAction* a,bool wait){++actions;if(!a||wait||alive!=1)++errors;return scenario==6?4:2;}
int main(){IceAssaultMethods group;CAIBrain brain;CIceMap::Entry entries[2];expected=&group;
#ifdef _MSC_VER
((CAIStateGroupBase*)&group)->PBrain=&brain;brain.Creature=(CThingAICreature*)&owner;
#else
group.PBrain=&brain;brain.Creature=&owner;
#endif
entries[0].type=54;entries[0].component=&stealth;entries[1].type=99;entries[1].component=&stealth;target.map.first=entries;target.map.last=entries+1;
for(scenario=0;scenario<8;++scenario){queries=idles=dangers=actions=terminates=constructed=destroyed=alive=0;targetValue=scenario==0?0:&target;target.dead=scenario==1?1:0;
for(int i=0;i<9;++i)target.components[i]=0;target.components[1]=scenario==2?0:0x400000;entries[0].type=scenario==5?55:54;
group.Update();
if(terminates!=1||alive)++errors;
if(scenario<=1){if(queries||idles||dangers||actions||constructed||destroyed)++errors;}
else {if(dangers!=(scenario!=2))++errors;int count=scenario==3||scenario==4?0:1;if(actions!=count||constructed!=count||destroyed!=count)++errors;int wantIdle=scenario==3?1:scenario==4||scenario==7?2:0;if(idles!=wantIdle||queries!=wantIdle+(scenario==3?0:1))++errors;}
}
if(errors){std::printf("009627c0_TEST FAIL %d\n",errors);return 1;}std::printf("009627c0_TEST PASS\n");return 0;}