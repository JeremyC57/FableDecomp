#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
// 0064a2db NPlayerGui::CDrawPerceivers::AddPerceivingThing
// Flags: /O1 /Oy-
// Leaf: ?AddPerceivingThing@CDrawPerceivers@NPlayerGui@@QAEXAAVCThing@@@Z
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
class CBaseClass{public:virtual~CBaseClass();};
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

CBaseClass::~CBaseClass(){}
NPlayerGui::CDrawBase::~CDrawBase(){}
void NPlayerGui::CDrawBase::ClearPrimitives(){}
static CThing*objects[10];static int references[10];static int pushes,destructions;
static int indexOf(CBaseObject*p){for(int i=0;i<10;++i)if(objects[i]==p)return i;abort();return 0;}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{CBaseObject*p;memcpy(&p,PData,4);return p;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){CBaseObject*p=GetPItem();if(p)--references[indexOf(p)];++destructions;}
template<class T>CIntelligentPointer<T>::CIntelligentPointer(T*p){CBaseObject*base=const_cast<CBaseObject*>(static_cast<const CBaseObject*>(p));memcpy(PData,&base,4);if(base)++references[indexOf(base)];}
namespace std{
template<class T,class A>void list<T,A>::push_back(const T&value){
 typedef _List_node<T>Node;Node*n=static_cast<Node*>(::operator new(sizeof(Node)));
 new(&n->_M_data)T(static_cast<const CThing*>(value.GetPItem()));
 _List_node_base*head=this->_M_node._M_data;n->_M_next=head;n->_M_prev=head->_M_prev;head->_M_prev->_M_next=n;head->_M_prev=n;++pushes;
}
}
typedef CIntelligentPointer<const CThing>Item;
static int countNodes(std::_List_node_base&head){int n=0;for(std::_List_node_base*p=head._M_next;p!=&head;p=p->_M_next){if(++n>20)abort();if(p->_M_next->_M_prev!=p||p->_M_prev->_M_next!=p)abort();}return n;}
static void clearNodes(std::_List_node_base&head){std::_List_node_base*p=head._M_next;while(p!=&head){std::_List_node_base*next=p->_M_next;std::_List_node<Item>*n=static_cast<std::_List_node<Item>*>(p);n->_M_data.~Item();::operator delete(n);p=next;}head._M_next=head._M_prev=&head;}
int main(){
 CThing pool[10];for(int i=0;i<10;++i)objects[i]=&pool[i];
 int cases=0;
 for(int n=0;n<=8;++n)for(int query=0;query<=n;++query)for(int nullPrefix=0;nullPrefix<2;++nullPrefix){
  NPlayerGui::CDrawPerceivers drawer;std::_List_node_base head;head._M_next=head._M_prev=&head;drawer.Things._M_node._M_data=&head;
  for(int i=0;i<10;++i)references[i]=0;
  if(nullPrefix){Item empty(0);drawer.Things.push_back(empty);}
  for(int i=0;i<n;++i){Item value(&pool[i]);drawer.Things.push_back(value);}
  int before=countNodes(head);pushes=destructions=0;
  drawer.AddPerceivingThing(pool[query]);
  if(pushes!=(query==n?1:0)||destructions!=(query==n?1:0)||countNodes(head)!=before+(query==n?1:0))abort();
  for(int i=0;i<10;++i)if(references[i]!=(i<n||i==query?1:0))abort();
  int after=countNodes(head);pushes=destructions=0;drawer.AddPerceivingThing(pool[query]);if(pushes||destructions||countNodes(head)!=after)abort();
  clearNodes(head);for(int i=0;i<10;++i)if(references[i])abort();++cases;
 }
 if(cases!=90)abort();puts("0064a2db_TEST PASS");return 0;
}