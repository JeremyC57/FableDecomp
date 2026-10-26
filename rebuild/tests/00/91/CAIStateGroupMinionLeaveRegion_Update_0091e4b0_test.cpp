#include <cstdio>
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define FABLE_ENGINE_CAISTATEGROUP_MINIONLEAVEREGION_H
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CWORLD_H
#define FABLE_ENGINE_CTHINGSEARCHTOOLS_H
struct CAIStateGroup_MinionLeaveRegion { unsigned char _pad_0x04[sizeof(void*)]; float ExitPoint_X,ExitPoint_Y,ExitPoint_Z; };
class CThingAICreature;
class CAIBrain { public: void* __vftable; CThingAICreature* Creature; };
class CThingSearchTools { public:};
class CWorld { public: CThingSearchTools* PThingSearchTools_ptr; };
#endif
// BEGIN EXACT CANDIDATE
// CAIStateGroup_MinionLeaveRegion::Update() -- leave through exit, suspend in limbo, then relocate to a random entrance
// Retail0x0091E4B0,273 bytes. Callees MoveToPos97EC00,SetInLimbo4C8CF0,DoNothing97E200,
// GetPWorld8FCFC0,GetAllRegionNodes91E660,GetPWorldSeed8FCFD0,GFROR13497890,GetPosition4C73D0,Terminate8FCF40.
#include "engine/CAIStateGroup_MinionLeaveRegion.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include <stdlib.h>
class C3DVector { public: float X,Y,Z; C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){} };
struct CThingBody {
 virtual void v00();
 virtual void v01();
 virtual void v02();
 virtual void v03();
 virtual void v04();
 virtual void v05();
 virtual void v06();
 virtual void v07();
 virtual void v08();
 virtual void v09();
 virtual void v10();
 virtual void v11();
 virtual void v12();
 virtual void v13();
 virtual void v14();
 virtual void v15();
 virtual void v16();
 virtual void v17();
 virtual void v18();
 virtual void v19();
 virtual void v20();
 virtual void v21();
 virtual void v22();
 virtual void v23();
 virtual void v24();
 virtual void v25();
 virtual void v26();
 virtual void v27();
 virtual void v28();
 virtual void v29();
 virtual void v30();
 virtual void SetPosition(const C3DVector&); };
class CThing { public: unsigned char prefix[0x60]; CThingBody* Body; void SetInLimbo(bool); const C3DVector& GetPosition() const; };
struct CBrainVirtual { virtual void v0(); virtual void v1(); virtual bool SuspendSuspendableProcess(); };
struct ThingVector { CThing** first; CThing** last; CThing** capacity; ThingVector():first(0),last(0),capacity(0){}  };
struct CThingFilter_IsRegionNode { bool Exit; CThingFilter_IsRegionNode(bool exit):Exit(exit){} };
struct CThingSearchToolsMethods : CThingSearchTools { void GetAllRegionNodes(const CThingFilter_IsRegionNode&,ThingVector*); };
unsigned long __fastcall GFROR13(unsigned long);
struct CAIStateGroup_MinionLeaveRegion_Adapter : CAIStateGroup_MinionLeaveRegion {
 CAIBrain* Brain() { return *(CAIBrain**)&_pad_0x04[0]; }
 C3DVector& ExitPosition() { return *(C3DVector*)&ExitPoint_X; }
 long AIAction_MoveToPos(long,C3DVector,bool*,long,bool,bool,CThing*,bool,float);
 long AIAction_DoNothing(); CWorld* GetPWorld(); unsigned long* GetPWorldSeed(); void Terminate(); void Update();
 __forceinline void GetNodes(const CThingFilter_IsRegionNode& filter,ThingVector* nodes) { CWorld* world=GetPWorld(); ((CThingSearchToolsMethods*)world->PThingSearchTools_ptr)->GetAllRegionNodes(filter,nodes); }
};
void CAIStateGroup_MinionLeaveRegion_Adapter::Update() {
 bool reached;
 if(AIAction_MoveToPos(1,ExitPosition(),&reached,3,true,true,0,true,0.0f)!=2) { Terminate(); return; }
 ((CThing*)Brain()->Creature)->SetInLimbo(true);
 ((CBrainVirtual*)Brain())->SuspendSuspendableProcess();
 while(AIAction_DoNothing()==4) {}
 ((CThing*)Brain()->Creature)->SetInLimbo(false);
 ThingVector nodes;
 GetNodes(CThingFilter_IsRegionNode(false),&nodes);
 unsigned long count=nodes.last-nodes.first;
 CThing** first=nodes.first;
 unsigned long* seed=GetPWorldSeed();
 *seed=*seed*0x24a1+0x24df;
 *seed=GFROR13(*seed);
 unsigned long index=count?*seed%count:0;
 CThing* chosen=first[index];
 ((CThing*)Brain()->Creature)->Body->SetPosition(chosen->GetPosition());
 Terminate();
 if(first) free(first);
}
// END EXACT CANDIDATE
static int errors,moveResult,idles,waits,limbos,suspends,searches,terminations,positions,seedReads,scenario;
static unsigned long seed;
static CWorld world;
static CThing creature, entrances[3];
static CThingBody body;
static C3DVector* expectedPosition;
static CThingSearchToolsMethods search;
static void Check(bool b) { if(!b) ++errors; }
void CThingBody::v00() {}
void CThingBody::v01() {}
void CThingBody::v02() {}
void CThingBody::v03() {}
void CThingBody::v04() {}
void CThingBody::v05() {}
void CThingBody::v06() {}
void CThingBody::v07() {}
void CThingBody::v08() {}
void CThingBody::v09() {}
void CThingBody::v10() {}
void CThingBody::v11() {}
void CThingBody::v12() {}
void CThingBody::v13() {}
void CThingBody::v14() {}
void CThingBody::v15() {}
void CThingBody::v16() {}
void CThingBody::v17() {}
void CThingBody::v18() {}
void CThingBody::v19() {}
void CThingBody::v20() {}
void CThingBody::v21() {}
void CThingBody::v22() {}
void CThingBody::v23() {}
void CThingBody::v24() {}
void CThingBody::v25() {}
void CThingBody::v26() {}
void CThingBody::v27() {}
void CThingBody::v28() {}
void CThingBody::v29() {}
void CThingBody::v30() {}

void CThingBody::SetPosition(const C3DVector& p) { ++positions; Check(this==&body && &p==expectedPosition); }
void CBrainVirtual::v0() {} void CBrainVirtual::v1() {}
bool CBrainVirtual::SuspendSuspendableProcess() { ++suspends; return false; }
void CThing::SetInLimbo(bool b) { Check(this==&creature); Check(b==(limbos==0)); ++limbos; }
const C3DVector& CThing::GetPosition() const { Check(this==&entrances[scenario%3]); return *expectedPosition; }
unsigned long __fastcall GFROR13(unsigned long x) { Check(x==(7UL*0x24a1+0x24df)); return scenario; }
long CAIStateGroup_MinionLeaveRegion_Adapter::AIAction_MoveToPos(long repeat,C3DVector pos,bool* reached,long movement,bool a,bool b,CThing* target,bool c,float f) {
 Check(repeat==1 && pos.X==1 && pos.Y==2 && pos.Z==3 && reached && movement==3 && a && b && !target && c && f==0); *reached=true; return moveResult;
}
long CAIStateGroup_MinionLeaveRegion_Adapter::AIAction_DoNothing() { ++waits; return waits<=idles?4:2; }
void CAIStateGroup_MinionLeaveRegion_Adapter::Terminate() { ++terminations; }
CWorld* CAIStateGroup_MinionLeaveRegion_Adapter::GetPWorld() { return &world; }
unsigned long* CAIStateGroup_MinionLeaveRegion_Adapter::GetPWorldSeed() { ++seedReads; return &seed; }
void CThingSearchToolsMethods::GetAllRegionNodes(const CThingFilter_IsRegionNode& filter,ThingVector* nodes) {
 ++searches; Check(this==&search && !filter.Exit && !nodes->first && !nodes->last && !nodes->capacity);
 nodes->first=(CThing**)malloc(sizeof(CThing*)*3); nodes->last=nodes->capacity=nodes->first+3;
 for(int i=0;i<3;++i) nodes->first[i]=&entrances[i];
}
int main() {
 CAIStateGroup_MinionLeaveRegion_Adapter state; CAIBrain brain; CBrainVirtual virtualBrain;
 brain.__vftable=*(void**)&virtualBrain; brain.Creature=(CThingAICreature*)&creature; *(CAIBrain**)state._pad_0x04=&brain;
 state.ExitPoint_X=1; state.ExitPoint_Y=2; state.ExitPoint_Z=3;
 float position[3]={4,5,6}; expectedPosition=(C3DVector*)position; creature.Body=&body; world.PThingSearchTools_ptr=&search;
 for(scenario=0;scenario<8;++scenario) {
  seed=7; idles=scenario%4; moveResult=scenario<3?scenario+3:2;
  waits=limbos=suspends=searches=terminations=positions=seedReads=0;
  state.Update(); Check(terminations==1);
  if(moveResult!=2) Check(waits==0 && limbos==0 && suspends==0 && searches==0 && positions==0 && seed==7);
  else Check(waits==idles+1 && limbos==2 && suspends==1 && searches==1 && positions==1 && seedReads==1 && seed==(unsigned long)scenario);
 }
 if(errors) { std::printf("0091e4b0_TEST FAIL %d\n",errors); return 1; }
 std::printf("0091e4b0_TEST PASS\n"); return 0;
}