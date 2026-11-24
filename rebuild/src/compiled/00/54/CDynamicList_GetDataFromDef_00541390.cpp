// NUISystem::CDynamicList::GetDataFromDef() -- copy list parameters and append children excluded from the non-scrolling index list
// Retail00541390,271bytes; native component ChildrenB0 and derived BInitialised15C,TopIndex168,SelectionIndex16C,ScrollingChildren174.
// No CDynamicList header available. Actual CComponent header retained under donor alias: ChildrenB8 versus nativeB0, DefIndex158 versus native130.
// PDB CDynamicList layout436 has derived fields+30; native CChangingStateComponent base ends15C.
// NativeVT1246E44 slot108 const LoadAndReturnDef=530EC0; find index535F5A fastcall ret8; vector insert485DBC ret14.
// Actual PDB LoadAndReturnDef returns CDefPointer<const CUIDef>; release uses CIVCountedPointeeBase::Delete()const slot1=42D930.
// Leaf: ?GetDataFromDef@CDynamicList@NUISystem@@IAEXXZ
#include "fable_compiler.h"
#include <stddef.h>
#include <string.h>
#include <new>
#include "engine/CIVCountedPointeeBase.h"
#include "rebuild_abi.h"
// NF75 (round 7, land1117fu q10918): the index search is STLport 4.6 find(first, last, val) (inline forwarder) over the out-of-line
//   std::__find<const unsigned long*, unsigned long> 0x00535F5A, whose body the TU sees (stlport/fable_algorithm.h
//   FIND_RANDOM_DISPATCH + FIND_NOINLINE_BODY, the landed 0x007BF510 form); was the same-TU noinline stand-in
//   NUISystem::FindIndex(.., const RandomAccessTag&).
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
// NF213 (2026-11-21, q6333 / q6342): the header-less ScrollingArray stand-in (ScrollingArray::InsertOverflow with a FalseType tag,
//   bound to 0x00485DBC) is the STLport vector<NUISystem::CComponent*> it stood for (Ego_r.pdb CDynamicList::ScrollingChildren).
//   push_back is the header body; its growth call now binds the protected IAE
//   ?_M_insert_overflow@?$vector@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@IAEXPAPAVCComponent@NUISystem@@ABQAV34@ABU__true_type@2@I_N@Z
//   (the 0x00485DBC self leaf), declaration-only as at HEAD (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY: no pick-any copy).
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
//   push_back's pick-any COMDAT takes the owner 0x00486024's size-optimised frame-pointer body through the scaffolding opt-in
//   FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY (Fidelity: scaffolding, not retail); the row still expands it.
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#define CComponent CComponentDonor
#include "engine/_quarantine/CComponent.h"
#undef CComponent
namespace NUISystem {
class CComponent;
struct ChildPointer {CComponent* Data;void* CountInfo;CComponent* GetPItem()const{return Data;}operator CComponent*()const{return GetPItem();}};
struct ChildArray {ChildPointer* Start;ChildPointer* Finish;ChildPointer* Limit;unsigned long size()const{return Finish-Start;}ChildPointer& operator[](unsigned long n){return Start[n];}};
struct IndexArray {unsigned long* Start;unsigned long* Finish;unsigned long* Limit;};
struct Vector2 {float X,Y;};
// CUIDef has packed definition data: native PositionOffset142 follows Wrapping140/Inverted141.
// PDB donor PositionOffset15E is likewise unaligned, following bool15C/15D. No CUIDef header exists.
#pragma pack(push,1)
class CUIDef:public CIVCountedPointeeBase { public:
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
class CDynamicList { public:
 void* VTable;unsigned char ComponentPrefix[0xb0-4];ChildArray Children;unsigned char ComponentSuffix[0x15c-0xbc];
 long BInitialised;Vector2 PositionOffset;long TopIndex,SelectionIndex,NumVisible;std::vector<CComponent*> ScrollingChildren;
 // Lowering of actual LoadAndReturnDef() const: thiscall, hidden stack result address, same address in EAX.
 typedef DefinitionResultStorage*(CDynamicList::*LoadDefinitionMethod)(DefinitionResultStorage*)const;

 FABLE_FORCEINLINE void AppendScrollingChild(CComponent* child){ScrollingChildren.push_back(child);}
 protected: void GetDataFromDef();
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
  if(std::find<const unsigned long*>(definition->NonScrollingChildren.Start,last,i)==last){AppendScrollingChild(Children[i]);}
 }
}
}