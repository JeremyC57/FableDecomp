// NUISystem::CDynamicList::RemoveChild(unsigned long) -- remove child from scrolling list and adjust selection before base deletion
// Retail00541120,268bytes; native component ChildrenB0 and derived BInitialised15C,TopIndex168,SelectionIndex16C,ScrollingChildren174.
// No CDynamicList header available. Actual CComponent header retained under donor alias: ChildrenB8 versus nativeB0, DefIndex158 versus native130.
// PDB CDynamicList layout436 has derived fields+30; native CChangingStateComponent base ends15C.
// NativeVT1246E44 slot142 ResetPositions=5416B0; base RemoveChild533BC0; pointer std::find535FE4 fastcall ret8; memmoveBFEAE6.
// PDB names list CDynamicList::RemoveChild,DeleteChild,GetDataFromDef consecutively; native slots64/148 distinguish the two index domains.
// Leaf: RemoveChild
#include <stddef.h>
#include <string.h>
#include "rebuild_abi.h"
// NF75 (round 7, land1117fu q10919): the child search is STLport 4.6 find(first, last, val) (inline forwarder) over the out-of-line
//   std::__find<NUISystem::CComponent**, NUISystem::CComponent*> 0x00535FE4, whose body the TU sees (stlport/fable_algorithm.h
//   FIND_RANDOM_DISPATCH + FIND_NOINLINE_BODY, the landed 0x007BF510 form); was the same-TU noinline stand-in
//   NUISystem::FindChild(.., const RandomAccessTag&) behind a FindRange forwarder, with local iterator-tag structs.
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#define CComponent CComponentDonor
#include "engine/_quarantine/CComponent.h"
#undef CComponent
// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00533BC0 ?RemoveChild@CComponent@NUISystem@@UAEXK@Z (was ?RemoveBaseChild@CDynamicList@NUISystem@@QAEXK@Z)
namespace NUISystem { class CComponent { public: virtual void RemoveChild(unsigned long); }; }
namespace NUISystem {
class CComponent;
struct ChildPointer {CComponent* Data;void* CountInfo;CComponent* GetPItem()const{return Data;}};
struct ChildArray {ChildPointer* Start;ChildPointer* Finish;ChildPointer* Limit;ChildPointer* begin()const{return Start;}unsigned long size()const{return Finish-begin();}ChildPointer& operator[](unsigned long n){return *(begin()+n);}};
struct ScrollingArray {CComponent** Start;CComponent** Finish;CComponent** Limit;unsigned long size()const{return Finish-Start;}CComponent*& operator[](unsigned long n){return Start[n];}
 void erase(CComponent** pos){CComponent** next=pos+1;if(Finish!=next)memmove(pos,next,reinterpret_cast<char*>(Finish)-reinterpret_cast<char*>(next));--Finish;}
};
class CDynamicList { public:
 void* VTable;unsigned char ComponentPrefix[0xb0-4];ChildArray Children;unsigned char ComponentSuffix[0x15c-0xbc];
 long BInitialised;float PositionOffsetX,PositionOffsetY;long TopIndex,SelectionIndex,NumVisible;ScrollingArray ScrollingChildren;
 typedef void(CDynamicList::*ResetMethod)();
 void ResetPositions(){(this->*static_cast<ResetMethod*>(VTable)[142])();}
 void RemoveChild(unsigned long);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDynamicList,Children)==0xb0);
FABLE_STATIC_ASSERT(offsetof(CDynamicList,ScrollingChildren)==0x174);
FABLE_STATIC_ASSERT(sizeof(CDynamicList::ResetMethod)==4);
#endif
void CDynamicList::RemoveChild(unsigned long index){
 if(index<Children.size()){
  CComponent* child=Children[index].GetPItem();
  CComponent** first=ScrollingChildren.Start;CComponent** last=ScrollingChildren.Finish;
  CComponent** found=std::find(first,last,child);
  if(found!=last){
   long scrollingIndex=found-first;
   ScrollingChildren.erase(found);
   if(SelectionIndex>scrollingIndex)--SelectionIndex;
   else if((unsigned long)SelectionIndex>=ScrollingChildren.size()){
    if(ScrollingChildren.size())SelectionIndex=ScrollingChildren.size()-1;else SelectionIndex=0;
   }
   if(TopIndex>SelectionIndex)--TopIndex;
   if(BInitialised)ResetPositions();
  }
  reinterpret_cast<NUISystem::CComponent*>(this)->NUISystem::CComponent::RemoveChild(index);
 }
}
}