// 0064a2db NPlayerGui::CDrawPerceivers::AddPerceivingThing
// Flags: /O1 /Oy-
// Leaf: ?AddPerceivingThing@CDrawPerceivers@NPlayerGui@@QAEXAAVCThing@@@Z
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CBaseObject:public CBaseClass{unsigned char pass9_unread_pointer[4];};
class CClass:public CBaseObject{unsigned char pass9_unread_class[12];};
class CThing:public CClass{};
class CBaseIntelligentPointer{public:virtual~CBaseIntelligentPointer();unsigned char PData[4];CBaseObject*GetPItem()const;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer(T*);};
template<class T>__forceinline bool __fastcall operator==(const CIntelligentPointer<T>&a,const CBaseObject*b){return a.GetPItem()==b;}
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{_List_node_base*_M_node;__forceinline _List_iterator_base(_List_node_base*n):_M_node(n){}void _M_incr(){_M_node=_M_node->_M_next;}};
template<class T>struct _Nonconst_traits{typedef T&reference;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
 __forceinline _List_iterator(_List_node_base*n):_List_iterator_base(n){}
 __forceinline _List_iterator(const _List_iterator&x):_List_iterator_base(x._M_node){}
 T&operator*()const{return static_cast<_List_node<T>*>(this->_M_node)->_M_data;}
 _List_iterator&operator++(){this->_M_incr();return *this;}
 bool operator==(const _List_iterator&x)const{return this->_M_node==x._M_node;}
 bool operator!=(const _List_iterator&x)const{return this->_M_node!=x._M_node;}
};
template<class T,class A>class _List_base{public:_STLP_alloc_proxy<_List_node_base*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{public:
 typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 __forceinline iterator begin(){return iterator(this->_M_node._M_data->_M_next);}
 __forceinline iterator end(){return iterator(this->_M_node._M_data);}
 void push_back(const T&);
};
// Original forward-iterator find body, visible and noninlined for escape analysis.
template<class I,class T>__declspec(noinline) I __fastcall find(I first,I last,const T&value){while(first!=last&&!(*first==value))++first;return first;}
}
namespace NPlayerGui{
// Layout: original nonvirt destructor/polymorphic CDrawBase40; later unused
// owner data stays opaque. RTTI gives solebase at0.
class CDrawBase{public:virtual void ClearPrimitives();~CDrawBase();unsigned char pass9_unread_base[36];};
// Layout: five unused state words afterbase40, then Things60; size64retail.
class CDrawPerceivers:public CDrawBase{unsigned char pass9_unread_state[20];public:
 std::list<CIntelligentPointer<const CThing> >Things;
 void AddPerceivingThing(CThing&);
};
void CDrawPerceivers::AddPerceivingThing(CThing&thing){
 if(std::find(Things.begin(),Things.end(),&thing)==Things.end()){
  CIntelligentPointer<const CThing>pointer(&thing);
  Things.push_back(pointer);
 }
}
}