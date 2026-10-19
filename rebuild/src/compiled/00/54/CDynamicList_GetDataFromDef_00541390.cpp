// NUISystem::CDynamicList::GetDataFromDef() -- copy list parameters and append children excluded from the non-scrolling index list
// Retail00541390,271bytes; native component ChildrenB0 and derived BInitialised15C,TopIndex168,SelectionIndex16C,ScrollingChildren174.
// No CDynamicList header available. Actual CComponent header retained under donor alias: ChildrenB8 versus nativeB0, DefIndex158 versus native130.
// PDB CDynamicList layout436 has derived fields+30; native CChangingStateComponent base ends15C.
// NativeVT1246E44 slot108 const LoadAndReturnDef=530EC0; find index535F5A fastcall ret8; vector insert485DBC ret14.
// Actual PDB LoadAndReturnDef returns CDefPointer<const CUIDef>; release uses CIVCountedPointeeBase::Delete()const slot1=42D930.
// Leaf: GetDataFromDef
#include <stddef.h>
#include <string.h>
#include <new>
#include "engine/CIVCountedPointeeBase.h"
#include "rebuild_abi.h"
#define CComponent CComponentDonor
#include "engine/_quarantine/CComponent.h"
#undef CComponent
namespace NUISystem {
struct CComponent;
struct ChildPointer {CComponent* Data;void* CountInfo;CComponent* GetPItem()const{return Data;}operator CComponent*()const{return GetPItem();}};
struct RandomAccessTag {RandomAccessTag(){}};struct FalseType {FalseType(){}};
const unsigned long* __fastcall FindIndex(const unsigned long*,const unsigned long*,const unsigned long&,const RandomAccessTag&); //00535F5A
#pragma auto_inline(off)
const unsigned long* __fastcall FindIndex(const unsigned long* first,const unsigned long* last,const unsigned long& value,const RandomAccessTag&){
 long groups=(last-first)/4;
 for(;groups>0;--groups){if(*first==value)return first;++first;if(*first==value)return first;++first;if(*first==value)return first;++first;if(*first==value)return first;++first;}
 switch(last-first){case 3:if(*first==value)return first;++first;case 2:if(*first==value)return first;++first;case 1:if(*first==value)return first;}return last;
}
#pragma auto_inline(on)
struct ChildArray {ChildPointer* Start;ChildPointer* Finish;ChildPointer* Limit;unsigned long size()const{return Finish-Start;}ChildPointer& operator[](unsigned long n){return Start[n];}};
struct IndexArray {unsigned long* Start;unsigned long* Finish;unsigned long* Limit;};
struct Vector2 {float X,Y;};
// CUIDef has packed definition data: native PositionOffset142 follows Wrapping140/Inverted141.
// PDB donor PositionOffset15E is likewise unaligned, following bool15C/15D. No CUIDef header exists.
#pragma pack(push,1)
struct CUIDef:CIVCountedPointeeBase {
 unsigned char DefinitionPrefix[0x94-8];IndexArray NonScrollingChildren;unsigned char DefinitionMiddle[0x138-0xa0];long UpLimit,DownLimit;bool Wrapping,Inverted;Vector2 PositionOffset;
 typedef void(CUIDef::*DeleteMethod)()const;
 void DecRef()const{--const_cast<CUIDef*>(this)->IVRefCount;}
 long GetRefCount()const{return IVRefCount;}
 void ReleaseReference()const{DecRef();if(GetRefCount()==0)(this->*static_cast<DeleteMethod*>(__vftable)[1])();}
};
#pragma pack(pop)
// Native return-buffer projection of CDefPointer<const CUIDef>: the called method constructs
// its sole counted pointer in this storage. Default initialization performs no stores; no copy occurs.
struct DefinitionResultStorage {
 const CUIDef* Data;
 ~DefinitionResultStorage(){if(Data)Data->ReleaseReference();}
 const CUIDef* operator->()const{return Data;}
};
struct ScrollingArray {CComponent** Start;CComponent** Finish;CComponent** Limit;
 void InsertOverflow(CComponent**,CComponent* const&,const FalseType&,unsigned long,bool); //00485DBC
 void push_back(CComponent* const& value){if(Finish!=Limit){if(Finish)new(Finish) CComponent*(value);++Finish;}else InsertOverflow(Finish,value,FalseType(),1,true);}
};
struct CDynamicList {
 void* VTable;unsigned char ComponentPrefix[0xb0-4];ChildArray Children;unsigned char ComponentSuffix[0x15c-0xbc];
 long BInitialised;Vector2 PositionOffset;long TopIndex,SelectionIndex,NumVisible;ScrollingArray ScrollingChildren;
 // Lowering of actual LoadAndReturnDef() const: thiscall, hidden stack result address, same address in EAX.
 typedef DefinitionResultStorage*(CDynamicList::*LoadDefinitionMethod)(DefinitionResultStorage*)const;

 __forceinline void AppendScrollingChild(CComponent* child){ScrollingChildren.push_back(child);}
 void GetDataFromDef();
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDynamicList,Children)==0xb0);
FABLE_STATIC_ASSERT(offsetof(CDynamicList,ScrollingChildren)==0x174);
FABLE_STATIC_ASSERT(offsetof(CUIDef,NonScrollingChildren)==0x94);
FABLE_STATIC_ASSERT(offsetof(CUIDef,PositionOffset)==0x142);
FABLE_STATIC_ASSERT(sizeof(CDynamicList::LoadDefinitionMethod)==4);
FABLE_STATIC_ASSERT(sizeof(CUIDef::DeleteMethod)==4);
#endif
void CDynamicList::GetDataFromDef(){
 DefinitionResultStorage definition;
 (this->*static_cast<LoadDefinitionMethod*>(VTable)[108])(&definition);
 PositionOffset=definition->PositionOffset;
 NumVisible=definition->DownLimit-definition->UpLimit;
 for(unsigned long i=0;i<Children.size();++i){
  const unsigned long* last=definition->NonScrollingChildren.Finish;
  if(FindIndex(definition->NonScrollingChildren.Start,last,i,RandomAccessTag())==last){AppendScrollingChild(Children[i]);}
 }
}
}