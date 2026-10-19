#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#define FABLE_ENGINE_CAISTATEGROUP_CREATUREUSECOVERBASE_H
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
struct CThingAICreature;
struct CAIBrain {void* __vftable;CThingAICreature* Creature;};
struct CAIStateGroupBase {void* __vftable;CAIBrain* PBrain;};
struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase {void* _pad_0x14[2];void* PCoverThing[2];void* CoverList[4];};
#endif
// CAIStateGroup_CreatureUseCoverBase::GetHiddenCoverPosAndSetUpCoverThing(C3DVector&) -- find a live available cover, release old cover, retain and occupy the new one; yield after a failed position test.
// Retail0x00970CD0,277 bytes. Uses shared UseCoverBase, Base and Brain storage. Inherited enemy handle+0x14 is opaque in the generated group header.
// CThing retail component map+0x44 differs from donor+0x48; narrow retail shell preserves audited fields. Brain dispatch slot2 is retained through a methods-only view of the shared header's vtable prefix.
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
struct C3DVector {float X,Y,Z;};
struct CThingRetail;
struct CIntelligentPointerRetail { virtual void v00(); void* PData; CThingRetail* GetPItem() const; void SetPItem(CThingRetail*); }; //00A01B50/00A01B90
struct CIntelligentVector { CIntelligentPointerRetail* first; CIntelligentPointerRetail* last; CIntelligentPointerRetail* capacity; unsigned char allocators[4];
 struct iterator { CIntelligentPointerRetail* p; explicit iterator(CIntelligentPointerRetail* q):p(q){} CIntelligentPointerRetail& operator*()const{return *p;} iterator operator++(int){iterator old(p);++p;return old;} bool operator!=(const iterator& other)const{return p!=other.p;} };
 iterator begin(){return iterator(first);} iterator end(){return iterator(last);} bool empty()const{return first==last;}
};
struct CBrainDispatch { virtual void v00();virtual void v01();virtual bool SuspendSuspendableProcess(); }; //slot2, 0x00A44840 verified in CAIBrain vtable0x012780C4
struct CTCOccupiableMethods { bool IsOccupied(); void SetOccupier(CThingRetail*); }; //0087C3F0,0087C390
struct CInterfaceMap {struct Entry{long type;void* component;};Entry* first;Entry* last;Entry* LowerBound(const long*);}; //0040F020
struct CThingRetail {
 unsigned char inherited[0x20];unsigned long UsingComponents[9];CInterfaceMap Components;
 unsigned char beforeDeath[0x91-0x4c];unsigned char deathFlags;
 __forceinline bool GetOccupiable(CTCOccupiableMethods** out) {
  if(UsingComponents[5]&0x1000) {
   long key=172;CInterfaceMap* map=&Components;CInterfaceMap::Entry* it=map->LowerBound(&key);
   if(it==map->last || 172<it->type)it=map->last;
   *out=(CTCOccupiableMethods*)it->component;return true;
  }return false;
 }
};
struct UseCoverMethods : CAIStateGroup_CreatureUseCoverBase {
 __forceinline CIntelligentPointerRetail& Enemy(){return *(CIntelligentPointerRetail*)_pad_0x14;}
 __forceinline CIntelligentPointerRetail& Cover(){return *(CIntelligentPointerRetail*)PCoverThing;}
 __forceinline CIntelligentVector& Candidates(){return *(CIntelligentVector*)CoverList;}
 bool GetHiddenCoverPos(C3DVector&,CThingRetail*); //00970930
 void ClearCover(); //00970B60
 bool GetHiddenCoverPosAndSetUpCoverThing(C3DVector&);
};
bool UseCoverMethods::GetHiddenCoverPosAndSetUpCoverThing(C3DVector& result)
{
 if(Candidates().empty())return false;
 CIntelligentVector::iterator it=Candidates().begin();
 do {
  if(!Enemy().GetPItem() || (Enemy().GetPItem()->deathFlags&1))return false;
  CIntelligentPointerRetail& candidate=*it++;
  if(candidate.GetPItem() && !(candidate.GetPItem()->deathFlags&1)) {
   if(GetHiddenCoverPos(result,candidate.GetPItem())) {
    CTCOccupiableMethods* occupiable;
    if(candidate.GetPItem()->GetOccupiable(&occupiable) && !occupiable->IsOccupied()) {
     ClearCover();
     Cover().SetPItem(candidate.GetPItem());
     occupiable->SetOccupier((CThingRetail*)((CAIStateGroupBase*)this)->PBrain->Creature);
     return true;
    }
   }else ((CBrainDispatch*)((CAIStateGroupBase*)this)->PBrain)->SuspendSuspendableProcess();
  }
 }while(it!=Candidates().end());
 return false;
}

static int failures,enemyGets,checks,suspends,clears,sets,occupies,lookups,eventCount;
static int events[64],scenarioNow,checkedIndices[8];
static CThingRetail things[7];static CTCOccupiableMethods occupancy[4];
static CInterfaceMap::Entry entries[4][2];
static CIntelligentPointerRetail* expectedEnemy;
static CIntelligentPointerRetail* expectedCover;
static CThingRetail* expectedActor;
static CAIBrain* expectedBrain;
static UseCoverMethods* expectedGroup;
static CThingRetail* currentCandidate;
static void Check(bool v){if(!v)++failures;}
static void Event(int n){events[eventCount++]=n;}
void CIntelligentPointerRetail::v00(){}
CThingRetail* CIntelligentPointerRetail::GetPItem() const {if(this==expectedEnemy)++enemyGets;return (CThingRetail*)PData;}
void CIntelligentPointerRetail::SetPItem(CThingRetail* p){Check(this==expectedCover);PData=p;++sets;Event(3);}
void CBrainDispatch::v00(){}
void CBrainDispatch::v01(){}
bool CBrainDispatch::SuspendSuspendableProcess(){Check((void*)this==(void*)expectedBrain);++suspends;Event(1);if(scenarioNow==6)things[0].deathFlags=1;if(scenarioNow==7)expectedGroup->Candidates().last=expectedGroup->Candidates().first+1;return false;}
CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* k){Check(*k==172);++lookups;for(int i=0;i<4;++i)if(this==&things[i+2].Components)return entries[i];++failures;return entries[0];}
bool CTCOccupiableMethods::IsOccupied(){for(int i=0;i<4;++i)if(this==occupancy+i)return i==2;++failures;return true;}
void CTCOccupiableMethods::SetOccupier(CThingRetail* p){Check(p==expectedActor && expectedCover->GetPItem()==currentCandidate);++occupies;Event(4);}
void UseCoverMethods::ClearCover(){Check(this==expectedGroup);++clears;Event(2);}
bool UseCoverMethods::GetHiddenCoverPos(C3DVector& p,CThingRetail* t){Check(this==expectedGroup);currentCandidate=t;int index=(int)(t-things);checkedIndices[checks++]=index;p.X=(float)index;p.Y=42;p.Z=7;return index!=2;}
static void Run(int scenario)
{
 scenarioNow=scenario;enemyGets=checks=suspends=clears=sets=occupies=lookups=eventCount=0;
 for(int i=0;i<7;++i){for(int j=0;j<9;++j)things[i].UsingComponents[j]=0;things[i].deathFlags=0;}
 for(int i=0;i<4;++i){entries[i][0].type=172;entries[i][0].component=occupancy+i;entries[i][1].type=999;entries[i][1].component=occupancy+i;things[i+2].Components.first=entries[i];things[i+2].Components.last=entries[i]+1;things[i+2].UsingComponents[5]=0x1000;}
 UseCoverMethods group;CAIBrain brain;CBrainDispatch dispatch;
 brain.__vftable=*(void**)&dispatch;brain.Creature=(CThingAICreature*)(things+1);
 ((CAIStateGroupBase*)&group)->PBrain=&brain;
 expectedEnemy=&group.Enemy();expectedCover=&group.Cover();expectedActor=things+1;expectedBrain=&brain;expectedGroup=&group;
 group.Enemy().PData=things;group.Cover().PData=things+6;
 CIntelligentPointerRetail candidates[4];for(int i=0;i<4;++i)candidates[i].PData=things+i+2;
 group.Candidates().first=candidates;group.Candidates().last=candidates+4;
 if(scenario==0)group.Candidates().last=candidates;
 if(scenario==1)group.Enemy().PData=0;
 if(scenario==2)things[0].deathFlags=1;
 if(scenario==3){candidates[0].PData=0;things[3].deathFlags=1;}
 if(scenario==4)things[3].UsingComponents[5]=0; // skip missing occupiable, then occupied, then free
 if(scenario==5){group.Candidates().last=candidates+3;things[3].UsingComponents[5]=0;}
 if(scenario==8){group.Candidates().first=candidates+3;entries[3][0].type=173;} // end-sentinel fallback
 C3DVector pos;pos.X=-1;pos.Y=-2;pos.Z=-3;
 bool result=group.GetHiddenCoverPosAndSetUpCoverThing(pos);
 bool success=scenario==3||scenario==4||scenario==8||scenario==9;
 if(scenario==9)success=true;
 // Default scenario9: first position fails/yields, then free candidate succeeds.
 Check(result==success);
 Check(clears==(success?1:0)&&sets==(success?1:0)&&occupies==(success?1:0));
 if(success){Check(group.Cover().GetPItem()==currentCandidate);Check(eventCount>=3);Check(events[eventCount-3]==2&&events[eventCount-2]==3&&events[eventCount-1]==4);Check(pos.Y==42&&pos.Z==7);}
 else Check(group.Cover().GetPItem()==things+6);
 if(scenario==0)Check(enemyGets==0&&checks==0&&suspends==0);
 if(scenario==1||scenario==2)Check(checks==0&&suspends==0);
 if(scenario==3)Check(checks==2&&suspends==0&&group.Cover().GetPItem()==things+5);
 if(scenario==4)Check(checks==4&&suspends==1&&group.Cover().GetPItem()==things+5);
 if(scenario==5)Check(checks==3&&suspends==1);
 if(scenario==6||scenario==7)Check(checks==1&&suspends==1);
 if(scenario==8)Check(checks==1&&suspends==0&&group.Cover().GetPItem()==things+5);
 if(scenario==9)Check(checks==2&&suspends==1&&group.Cover().GetPItem()==things+3);
}
int main(){for(int s=0;s<10;++s)Run(s);if(failures){std::printf("00970cd0_TEST FAIL %d\n",failures);return 1;}std::printf("00970cd0_TEST PASS\n");return 0;}