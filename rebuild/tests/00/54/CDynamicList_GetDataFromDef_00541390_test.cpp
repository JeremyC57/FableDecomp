// Behaviour test for 0x00541390 NUISystem::CDynamicList::GetDataFromDef (NF213, 2026-11-21: rewritten on the shared headers; the HEAD
// test was an expanded copy that still called the FindIndex stand-in). The row source is copied verbatim (stlport/fable_containers.h
// with the overflow declaration-only, stlport/fable_algorithm.h); the harness defines vector::_M_insert_overflow (the 0x00485DBC member
// the row calls) out of class, counting the growth calls, and reaches the protected vector words through a derived accessor. The
// components are opaque NUISystem::CComponent pointers into a byte array (no class model).
#include <stdio.h>
#include <assert.h>
// ---- source (verbatim) ----
// NUISystem::CDynamicList::GetDataFromDef() -- copy list parameters and append children excluded from the non-scrolling index list
// Retail00541390,271bytes; native component ChildrenB0 and derived BInitialised15C,TopIndex168,SelectionIndex16C,ScrollingChildren174.
// No CDynamicList header available. Actual CComponent header retained under donor alias: ChildrenB8 versus nativeB0, DefIndex158 versus native130.
// PDB CDynamicList layout436 has derived fields+30; native CChangingStateComponent base ends15C.
// NativeVT1246E44 slot108 const LoadAndReturnDef=530EC0; find index535F5A fastcall ret8; vector insert485DBC ret14.
// Actual PDB LoadAndReturnDef returns CDefPointer<const CUIDef>; release uses CIVCountedPointeeBase::Delete()const slot1=42D930.
// Leaf: GetDataFromDef
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
  if(std::find<const unsigned long*>(definition->NonScrollingChildren.Start,last,i)==last){AppendScrollingChild(Children[i]);}
 }
}
}
// ---- harness ----
namespace NUISystem {
static CUIDef* activeDefinition;static CDynamicList* activeList;static int loads,releases,overflows;static bool shrink;static CComponent** heapBuffer;
struct DefinitionFixture:CUIDef {void Dispose()const{assert(this==activeDefinition&&IVRefCount==0);++releases;}};
struct ListFixture:CDynamicList {DefinitionResultStorage* Load(DefinitionResultStorage* out)const{assert(this==activeList);++loads;out->Data=activeDefinition;++activeDefinition->IVRefCount;return out;}};
typedef std::vector<CComponent*> ComponentVector;
struct VectorWords:ComponentVector {
 static CComponent** ComponentVector::* Start(){return &VectorWords::_M_start;}
 static CComponent** ComponentVector::* Finish(){return &VectorWords::_M_finish;}
 static CComponent**& Limit(ComponentVector& v){return (v.*&VectorWords::_M_end_of_storage)._M_data;}
};
static void SetWords(ComponentVector& v,CComponent** s,CComponent** f,CComponent** l){v.*VectorWords::Start()=s;v.*VectorWords::Finish()=f;VectorWords::Limit(v)=l;}
}
namespace std {
template<class _Tp,class _Alloc> void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& value,const __true_type&,size_type count,bool atEnd){
 using namespace NUISystem;
 assert(this==&activeList->ScrollingChildren&&position==this->_M_finish&&this->_M_finish==this->_M_end_of_storage._M_data&&count==1&&atEnd);++overflows;
 unsigned long n=this->_M_finish-this->_M_start;CComponent** replacement=new CComponent*[n+8];for(unsigned long i=0;i<n;++i)replacement[i]=this->_M_start[i];replacement[n]=value;delete[] heapBuffer;heapBuffer=replacement;
 this->_M_start=replacement;this->_M_finish=replacement+n+1;this->_M_end_of_storage._M_data=replacement+n+8;if(shrink)activeList->Children.Finish=activeList->Children.Start+2;}
}
using namespace NUISystem;
static char componentBytes[7];
static CComponent* Node(int i){return reinterpret_cast<CComponent*>(&componentBytes[i]);}
int main(){CDynamicList::LoadDefinitionMethod loadTable[109];for(int i=0;i<109;++i)loadTable[i]=0;loadTable[108]=static_cast<CDynamicList::LoadDefinitionMethod>(&ListFixture::Load);CUIDef::DeleteMethod deleteTable[2];deleteTable[0]=0;deleteTable[1]=static_cast<CUIDef::DeleteMethod>(&DefinitionFixture::Dispose);
 ChildPointer children[6];for(int i=0;i<6;++i){children[i].Data=Node(i);children[i].CountInfo=0;}
 for(int scenario=0;scenario<8;++scenario){ListFixture list;DefinitionFixture definition;activeList=&list;activeDefinition=&definition;list.VTable=loadTable;definition.__vftable=deleteTable;definition.IVRefCount=scenario==6?1:0;unsigned long initialRefs=definition.IVRefCount;definition.UpLimit=scenario==7?9:2;definition.DownLimit=scenario==7?3:5;definition.PositionOffset.X=-2.5f;definition.PositionOffset.Y=7.25f;
 unsigned long excluded[6]={1,3,1,99,4,5};unsigned long excludedCount=scenario==0?0:scenario==1?2:scenario==2?4:scenario==3?6:0;if(scenario==3)for(int i=0;i<6;++i)excluded[i]=i;definition.NonScrollingChildren.Start=excluded;definition.NonScrollingChildren.Finish=excluded+excludedCount;definition.NonScrollingChildren.Limit=excluded+6;
 list.Children.Start=children;list.Children.Finish=children+(scenario==4?0:6);list.Children.Limit=children+6;CComponent* initial[10];initial[0]=Node(6);SetWords(list.ScrollingChildren,initial,initial+1,initial+((scenario==5||scenario==7)?1:10));list.TopIndex=13;list.SelectionIndex=17;list.BInitialised=19;
 loads=releases=overflows=0;heapBuffer=0;shrink=scenario==5;
 list.GetDataFromDef();assert(loads==1&&releases==(initialRefs?0:1)&&definition.IVRefCount==initialRefs);assert(list.PositionOffset.X==-2.5f&&list.PositionOffset.Y==7.25f&&list.NumVisible==definition.DownLimit-definition.UpLimit);assert(list.TopIndex==13&&list.SelectionIndex==17&&list.BInitialised==19);
 unsigned long n=1,childCount=scenario==4?0:scenario==5?2:6;assert(list.ScrollingChildren[0]==Node(6));for(unsigned long i=0;i<childCount;++i){bool skip=false;for(unsigned long j=0;j<excludedCount;++j)if(excluded[j]==i)skip=true;if(!skip)assert(list.ScrollingChildren[n++]==Node(i));}assert(list.ScrollingChildren.size()==n);assert(overflows==((scenario==5||scenario==7)?1:0));
 SetWords(list.ScrollingChildren,0,0,0);delete[] heapBuffer;heapBuffer=0;
 }
 // The explicit adapter preserves the actual hidden-return address in EAX and one initialized lifetime.
 {ListFixture list;DefinitionFixture definition;activeList=&list;activeDefinition=&definition;list.VTable=loadTable;definition.__vftable=deleteTable;definition.IVRefCount=0;loads=releases=0;{DefinitionResultStorage storage;DefinitionResultStorage* returned=(list.*loadTable[108])(&storage);assert(returned==&storage&&storage.Data==&definition&&definition.IVRefCount==1);}assert(definition.IVRefCount==0&&releases==1);}
 puts("00541390_TEST PASS");return 0;
}
