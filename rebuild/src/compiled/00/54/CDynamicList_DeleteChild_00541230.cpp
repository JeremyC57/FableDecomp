// NUISystem::CDynamicList::DeleteChild(unsigned long) -- remove scrolling child and locate its index for base deletion
// Retail00541230,280bytes; native component ChildrenB0 and derived BInitialised15C,TopIndex168,SelectionIndex16C,ScrollingChildren174.
// No CDynamicList header available. Actual CComponent header retained under donor alias: ChildrenB8 versus nativeB0, DefIndex158 versus native130.
// PDB CDynamicList layout436 has derived fields+30; native CChangingStateComponent base ends15C.
// NativeVT1246E44 slot142 ResetPositions=5416B0; base RemoveChild533BC0; pointer std::find535FE4 fastcall ret8; memmoveBFEAE6.
// PDB names list CDynamicList::RemoveChild,DeleteChild,GetDataFromDef consecutively; native slots64/148 distinguish the two index domains.
// Leaf: DeleteChild
#include <stddef.h>
#include <string.h>
#include "rebuild_abi.h"
#define CComponent CComponentDonor
#include "engine/_quarantine/CComponent.h"
#undef CComponent
namespace NUISystem {
struct CComponent;
struct ChildPointer {CComponent* Data;void* CountInfo;CComponent* GetPItem()const{return Data;}};
struct InputTag {InputTag(){}};struct ForwardTag:InputTag {};struct BidirectionalTag:ForwardTag {};struct RandomAccessTag:BidirectionalTag {};
CComponent** __fastcall FindChild(CComponent**,CComponent**,CComponent* const&,const RandomAccessTag&); //00535FE4
#pragma auto_inline(off)
CComponent** __fastcall FindChild(CComponent** first,CComponent** last,CComponent* const& value,const RandomAccessTag&){
 long groups=(last-first)/4;
 for(;groups>0;--groups){if(*first==value)return first;++first;if(*first==value)return first;++first;if(*first==value)return first;++first;if(*first==value)return first;++first;}
 switch(last-first){case 3:if(*first==value)return first;++first;case 2:if(*first==value)return first;++first;case 1:if(*first==value)return first;}
 return last;
}
#pragma auto_inline(on)
inline CComponent** FindRange(CComponent** first,CComponent** last,CComponent* const& value){return FindChild(first,last,value,RandomAccessTag());}
struct ChildArray {ChildPointer* Start;ChildPointer* Finish;ChildPointer* Limit;ChildPointer* begin()const{return Start;}unsigned long size()const{return Finish-begin();}ChildPointer& operator[](unsigned long n){return *(begin()+n);}};
struct ScrollingArray {CComponent** Start;CComponent** Finish;CComponent** Limit;unsigned long size()const{return Finish-Start;}CComponent*& operator[](unsigned long n){return Start[n];}
 void erase(CComponent** pos){CComponent** next=pos+1;if(Finish!=next)memmove(pos,next,reinterpret_cast<char*>(Finish)-reinterpret_cast<char*>(next));--Finish;}
};
struct CDynamicList {
 void* VTable;unsigned char ComponentPrefix[0xb0-4];ChildArray Children;unsigned char ComponentSuffix[0x15c-0xbc];
 long BInitialised;float PositionOffsetX,PositionOffsetY;long TopIndex,SelectionIndex,NumVisible;ScrollingArray ScrollingChildren;
 typedef void(CDynamicList::*ResetMethod)();
 void ResetPositions(){(this->*static_cast<ResetMethod*>(VTable)[142])();}
 void RemoveBaseChild(unsigned long); //00533BC0 CComponent::RemoveChild
 void DeleteChild(unsigned long);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDynamicList,Children)==0xb0);
FABLE_STATIC_ASSERT(offsetof(CDynamicList,ScrollingChildren)==0x174);
FABLE_STATIC_ASSERT(sizeof(CDynamicList::ResetMethod)==4);
#endif
void CDynamicList::DeleteChild(unsigned long index){
 if(index<ScrollingChildren.size()){
  unsigned long childIndex=0;
  for(;childIndex<Children.size();++childIndex)if(Children[childIndex].GetPItem()==ScrollingChildren[index])break;
  ScrollingChildren.erase(ScrollingChildren.Start+index);
  if(SelectionIndex>index)--SelectionIndex;
  else if((unsigned long)SelectionIndex>=ScrollingChildren.size()){
   if(ScrollingChildren.size())SelectionIndex=ScrollingChildren.size()-1;else SelectionIndex=0;
  }
  if(TopIndex>SelectionIndex)--TopIndex;
  if(BInitialised)ResetPositions();
  RemoveBaseChild(childIndex);
 }
}
}