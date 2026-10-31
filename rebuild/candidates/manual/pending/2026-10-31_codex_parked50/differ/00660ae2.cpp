// 00660ae2 CTCVillage::RemoveThingFromTask<CVillageTaskTorch>
// Codex parked-50 best attempt: DIFFER(122v123); residue: 122/123 bytes; corrected Torch specialization still reads the returned iterator through EAX instead of its hidden-result stack slot.
// Flags: /O1 /Oy-
// Leaf: ??$RemoveThingFromTask@VCVillageTaskTorch@@@CTCVillage@@ABEXV?$CIntelligentPointer@VCVillageTaskBase@@@@ABVCThing@@AAV?$list@V?$CCountedPointer@VCVillageTaskTorch@@@@V?$allocator@V?$CCountedPointer@VCVillageTaskTorch@@@@@std@@@std@@@Z
#include <stddef.h>
#define CTCVillage Pass9VillageStorage
#include "engine/CTCVillage.h"
#undef CTCVillage
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
extern "C" void __cdecl free(void*);
class CThing;
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
enum EVillageTaskType{};
// Layout: RTTI CVillageTaskBase:CBaseObject, eight bytes. These are the original
// virtual slots 2..6, independently checked in the Torch vtable at0125a8ac.
class CVillageTaskBase:public CBaseObject{public:
 virtual EVillageTaskType GetTaskType()const=0;
 virtual bool IsValid()const=0;
 virtual bool IsTaskAvailable()const=0;
 virtual bool IsThingInvolvedWithTask(const CThing&)const=0;
 virtual void RemoveThingFromTask(const CThing&,bool&)=0;
};
class CVillageTaskTorch:public CVillageTaskBase{};
class CBaseIntelligentPointer{public:virtual~CBaseIntelligentPointer();unsigned char PData[4];CBaseObject*GetPItem()const;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:
 __forceinline T*operator->()const{return static_cast<T*>(GetPItem());}
 __forceinline T&operator*()const{return *static_cast<T*>(GetPItem());}
};
#include "engine/CCountedPointer.h"
class CIsTask{public:bool operator()(const CCountedPointer<CVillageTaskBase>&)const;CVillageTaskBase*Task;__forceinline CIsTask(const CVillageTaskBase&x):Task(const_cast<CVillageTaskBase*>(&x)){};};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{_List_node_base*_M_node;__forceinline _List_iterator_base(_List_node_base*p):_M_node(p){} __forceinline _List_iterator_base(){}};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
 typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 __forceinline _List_iterator(){}
 __forceinline _List_iterator(_List_node<T>*p):_List_iterator_base(p){}
 __forceinline _List_iterator(const iterator&x):_List_iterator_base(x._M_node){}
};
template<class T,class A>class _List_base{public:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{public:
 typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 __forceinline iterator begin(){return iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}
 __forceinline iterator end(){return iterator(this->_M_node._M_data);}
 __declspec(noinline) iterator erase(iterator position){
  _List_node_base*next=position._M_node->_M_next;
  _List_node_base*prev=position._M_node->_M_prev;
  _List_node<T>*node=static_cast<_List_node<T>*>(position._M_node);
  prev->_M_next=next;next->_M_prev=prev;
  node->_M_data.~T();free(node);
  return iterator(static_cast<_List_node<T>*>(next));
 }
};
struct input_iterator_tag{};
template<class I,class P>I __fastcall __find_if(I,I,P,const input_iterator_tag&);
template<class I,class P>__declspec(noinline) I __fastcall find_if(I first,I last,P pred){
 // Fidelity: visible input-reading algorithm context; only the caller is delivered.
 while(first._M_node!=last._M_node){
  if(pred(static_cast<_List_node<CCountedPointer<CVillageTaskTorch> >*>(first._M_node)->_M_data))break;
  first._M_node=first._M_node->_M_next;
 }
 return first;
}
}
class CTCBase:public CBaseClassNonCopyable{public:virtual~CTCBase();protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;};
// Layout: actual RTTI primary TCBase. No village fields are read by this member;
// the imported retail layout supplies the remaining opaque storage size.
class CTCVillage:public CTCBase{unsigned char pass9_unread_village[sizeof(Pass9VillageStorage)-sizeof(CTCBase)];private:
 template<class T>void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>,const CThing&,std::list<CCountedPointer<T> >&)const;
};
template<class T>void CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>task,const CThing&thing,std::list<CCountedPointer<T> >&tasks)const{
 if(task->IsThingInvolvedWithTask(thing)){
  bool remove_task;task->RemoveThingFromTask(thing,remove_task);
  if(remove_task){
   typename std::list<CCountedPointer<T> >::iterator found;found=std::find_if(tasks.begin(),tasks.end(),CIsTask(*task));
   tasks.erase(found);
  }
 }
}
template void CTCVillage::RemoveThingFromTask<CVillageTaskTorch>(CIntelligentPointer<CVillageTaskBase>,const CThing&,std::list<CCountedPointer<CVillageTaskTorch> >&)const;
