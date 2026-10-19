// CAIStateGroup_IceTrollIcicleAssault::Update() -- mark the target in danger, wait for current actions and perform the icicle assault before terminating.
// Retail009627c0 length260. Shared group/Base/Brain layout. CThing component map44 differs donor48.
// Action retail114 uses real combat-base virtual destructor; generated Into donor size12c differs native114.
#ifdef _MSC_VER
#include "engine/CAIStateGroup_IceTrollIcicleAssault.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureAction_IceTrollIcicleAssaultInto.h"
#else
struct CAIBrain{char prefix[24];void* Creature;};
struct CAIStateGroupBase{void* vtable;CAIBrain* PBrain;char state[12];};
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