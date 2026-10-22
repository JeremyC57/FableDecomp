#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#define FABLE_ENGINE_CAISTATEGROUP_CREATUREADVANCEINCOVER_H
#define FABLE_ENGINE_CAISTATEGROUP_CREATUREUSECOVERBASE_H
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
struct CThingAICreature;
struct CAIBrain { CThingAICreature* Creature; };
struct CAIStateGroupBase { void* __vftable; CAIBrain* PBrain; };
struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase {
 void* _pad_0x14[2]; void* PCoverThing[2];
};
struct CAIStateGroup_CreatureAdvanceInCover : CAIStateGroup_CreatureUseCoverBase {};
#endif
// CAIStateGroup_CreatureAdvanceInCover::GetCoverThingList(CThingVector&) -- collect unoccupied cover closer to the enemy than the present cover (or creature).
// Retail 0x00978FD0, 287 bytes excluding padding. Shared AdvanceInCover/UseCoverBase/Base/Brain headers audited.
// UseCoverBase exposes PCoverThing at the retail unaligned +0x1f; inherited intelligent enemy handle at +0x14 is opaque in its generated header.
// Search 0x009793D0 audited: calls the four bound predicates, then appends matching things to the supplied vector. No predicate or callback is omitted.
#include <math.h>
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
struct C3DVector { float X,Y,Z; };
struct C2DVector { float X,Y; C2DVector(const C3DVector& p):X(p.X),Y(p.Y){} };
struct CThingRetail { const C3DVector& GetPosition() const; }; // 0x004C73D0
struct CIntelligentPointerRetail { CThingRetail* GetPItem() const; }; // 0x00A01B50 (opaque handle methods)
struct CThingVector { CThingRetail** first; CThingRetail** last; CThingRetail** limit; unsigned char allocators[4]; };
struct CThingFilter_CanBeUsedAsCover { CThingFilter_CanBeUsedAsCover() {} bool operator()(CThingRetail*) const; }; // 0x006A7AB0
struct CThingFilter_IsOccupiableOccupied { CThingFilter_IsOccupiableOccupied() {} bool operator()(CThingRetail*) const; }; // 0x006A8A30
struct CNotOccupied { CThingFilter_IsOccupiableOccupied predicate; CNotOccupied() {} };
struct CThingFilter_IsNot { CThingRetail* Thing; CThingFilter_IsNot(CThingRetail* t):Thing(t){} bool operator()(CThingRetail*) const; }; // 0x006A76A0
struct CThingFilter_In2DRange { C3DVector Pos; float Range; CThingFilter_In2DRange(const C3DVector& p,float r):Pos(p),Range(r){} bool operator()(CThingRetail*) const; }; // 0x006A76B0
struct CFilterBind4 {
 const CThingFilter_CanBeUsedAsCover* F1; const CNotOccupied* F2; const CThingFilter_IsNot* F3; const CThingFilter_In2DRange* F4;
 CFilterBind4(const CThingFilter_CanBeUsedAsCover& a,const CNotOccupied& b,const CThingFilter_IsNot& c,const CThingFilter_In2DRange& d):F1(&a),F2(&b),F3(&c),F4(&d){}
};
struct C2DBoxI { long minX,minY,maxX,maxY; };
struct C2DBox { float minX,minY,maxX,maxY;
 C2DBox(const C2DVector& p,float radius) { minX=p.X-radius; minY=p.Y-radius; maxX=p.X+radius; maxY=p.Y+radius; }
 C2DBoxI ToInteger() const; // 0x00A55A30, audited four float-to-long conversions
};
struct CThingSearchToolsRetail { void FindInRoughArea(const C2DBoxI&,const CFilterBind4&,CThingVector&); }; // 0x009793D0
struct AdvanceInCoverMethods : CAIStateGroup_CreatureAdvanceInCover {
 __forceinline CIntelligentPointerRetail& Cover() { return *(CIntelligentPointerRetail*)((CAIStateGroup_CreatureUseCoverBase*)this)->PCoverThing; }
 __forceinline CIntelligentPointerRetail& Enemy() { return *(CIntelligentPointerRetail*)((CAIStateGroup_CreatureUseCoverBase*)this)->_pad_0x14; }
 CThingSearchToolsRetail* PeekThingSearchTools(); // 0x008FD000
 void GetCoverThingList(CThingVector&);
};
void AdvanceInCoverMethods::GetCoverThingList(CThingVector& output)
{
 CThingRetail* current=Cover().GetPItem();
 if(!current) current=(CThingRetail*)((CAIStateGroupBase*)this)->PBrain->Creature;
 const C3DVector& enemy=Enemy().GetPItem()->GetPosition();
 const C3DVector& start=current->GetPosition();
 float dx=start.X-enemy.X,dy=start.Y-enemy.Y;
 float radius=(float)sqrt(dx*dx+dy*dy);
 C2DVector center(Enemy().GetPItem()->GetPosition());
 PeekThingSearchTools()->FindInRoughArea(C2DBox(center,radius).ToInteger(),CFilterBind4(CThingFilter_CanBeUsedAsCover(),CNotOccupied(),CThingFilter_IsNot(current),CThingFilter_In2DRange(Enemy().GetPItem()->GetPosition(),radius)),output);
}

static int failures,coverGets,enemyGets,positionGets,boxConversions,searchPeeks,searchCalls;
static int coverTests,occupiedTests,notTests,rangeTests;
static CThingRetail things[8];
static C3DVector positions[8];
static bool coverFlags[8],occupiedFlags[8];
static CThingRetail* currentCover;
static CThingRetail* expectedCurrent;
static CIntelligentPointerRetail *coverHandle,*enemyHandle;
static CThingSearchToolsRetail searchTools;
static float expectedRadius;
static CThingVector* expectedOutput;
static void Check(bool b) { if(!b) ++failures; }
static int Index(const CThingRetail* t) {
 for(int i=0;i<8;++i) if(t==things+i) return i;
 ++failures; return 0;
}
CThingRetail* CIntelligentPointerRetail::GetPItem() const {
 if(this==coverHandle) { ++coverGets; return currentCover; }
 Check(this==enemyHandle); ++enemyGets; return things;
}
const C3DVector& CThingRetail::GetPosition() const { ++positionGets; return positions[Index(this)]; }
bool CThingFilter_CanBeUsedAsCover::operator()(CThingRetail* t) const { ++coverTests; return coverFlags[Index(t)]; }
bool CThingFilter_IsOccupiableOccupied::operator()(CThingRetail* t) const { ++occupiedTests; return occupiedFlags[Index(t)]; }
bool CThingFilter_IsNot::operator()(CThingRetail* t) const { ++notTests; return Thing!=t; }
bool CThingFilter_In2DRange::operator()(CThingRetail* t) const {
 ++rangeTests;
 const C3DVector& p=positions[Index(t)]; float dx=Pos.X-p.X,dy=Pos.Y-p.Y;
 return dx*dx+dy*dy < Range*Range;
}
C2DBoxI C2DBox::ToInteger() const {
 ++boxConversions;
 Check(fabs(minX-(positions[0].X-expectedRadius))<0.0001);
 Check(fabs(minY-(positions[0].Y-expectedRadius))<0.0001);
 Check(fabs(maxX-(positions[0].X+expectedRadius))<0.0001);
 Check(fabs(maxY-(positions[0].Y+expectedRadius))<0.0001);
 C2DBoxI result; result.minX=(long)minX; result.minY=(long)minY;
 result.maxX=(long)maxX; result.maxY=(long)maxY; return result;
}
CThingSearchToolsRetail* AdvanceInCoverMethods::PeekThingSearchTools() { ++searchPeeks; return &searchTools; }
void CThingSearchToolsRetail::FindInRoughArea(const C2DBoxI& box,const CFilterBind4& f,CThingVector& out) {
 ++searchCalls; Check(this==&searchTools && &out==expectedOutput);
 Check(box.minX==(long)(positions[0].X-expectedRadius));
 Check(box.minY==(long)(positions[0].Y-expectedRadius));
 Check(box.maxX==(long)(positions[0].X+expectedRadius));
 Check(box.maxY==(long)(positions[0].Y+expectedRadius));
 Check(f.F1 && f.F2 && f.F3 && f.F4 && f.F3->Thing==expectedCurrent);
 Check(f.F4->Pos.X==positions[0].X && f.F4->Pos.Y==positions[0].Y && f.F4->Pos.Z==positions[0].Z);
 Check(fabs(f.F4->Range-expectedRadius)<0.0001);
 // Exercise every bound predicate exactly as audited in the retail search callee.
 for(int i=1;i<8;++i) {
  CThingRetail* t=things+i;
  if((*f.F1)(t) && !f.F2->predicate(t) && (*f.F3)(t) && (*f.F4)(t)) {
   Check(out.last<out.limit); *out.last++=t;
  }
 }
}
static void Run(int scenario) {
 coverGets=enemyGets=positionGets=boxConversions=searchPeeks=searchCalls=0;
 coverTests=occupiedTests=notTests=rangeTests=0;
 for(int i=0;i<8;++i) { positions[i].X=1;positions[i].Y=2;positions[i].Z=(float)(100*i);coverFlags[i]=true;occupiedFlags[i]=false; }
 positions[0].Z=123;
 positions[1].X=4;positions[1].Y=6;
 positions[2].X=7;positions[2].Y=10;
 positions[3].X=3;positions[3].Y=2; // usable nearby cover
 occupiedFlags[4]=true;
 coverFlags[5]=false;
 positions[6].X=21;
 positions[7].X=6; // exactly radius5 in default scenario
 currentCover=things+1; expectedCurrent=currentCover;expectedRadius=5;
 if(scenario==1) { currentCover=0; expectedCurrent=things+2;expectedRadius=10; }
 if(scenario==2) { positions[1].X=1;positions[1].Y=2;expectedRadius=0; }
 if(scenario==3) { positions[0].X=0.5f;positions[0].Y=-0.5f;positions[1].X=2;positions[1].Y=1.5f;expectedRadius=2.5f; }
 AdvanceInCoverMethods group;CAIBrain brain;
 ((CAIStateGroupBase*)&group)->PBrain=&brain; brain.Creature=(CThingAICreature*)(things+2);
 coverHandle=&group.Cover();enemyHandle=&group.Enemy();
 CThingRetail* storage[10];storage[0]=things;CThingVector output;
 output.first=storage;output.last=storage+1;output.limit=storage+10;expectedOutput=&output;
 group.GetCoverThingList(output);
 Check(coverGets==1 && enemyGets==3 && positionGets==4 && boxConversions==1 && searchPeeks==1 && searchCalls==1);
 Check(coverTests==7 && occupiedTests==6 && notTests==5);
 Check(output.first==storage && storage[0]==things);
 if(scenario==0) Check(output.last==storage+2 && storage[1]==things+3);
 if(scenario==1) Check(output.last==storage+4 && storage[1]==things+1 && storage[2]==things+3 && storage[3]==things+7);
 if(scenario==2) Check(output.last==storage+1);
 if(scenario==3) Check(output.last==storage+1);
}
int main(){ for(int i=0;i<4;++i) Run(i); if(failures){std::printf("00978fd0_TEST FAIL %d\n",failures);return 1;} std::printf("00978fd0_TEST PASS\n");return 0; }