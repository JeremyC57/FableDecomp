// Self-contained target, visible copy/destruction chain, real polymorphic observer fixtures.
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
#endif
#define private public
#define protected public
#define malloc fixture_malloc
#define free fixture_free
// 0055cb10 NUISystem::CObservable::SetEvent
// Leaf: ?SetEvent@CObservable@NUISystem@@UAEXW4EEvent@2@@Z
// Flags:
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
namespace std{
typedef ::size_t size_t;typedef ::ptrdiff_t ptrdiff_t;
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>&p):first(p.first),second(p.second){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class B,class C>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef C result_type;};
template<class A,class B>struct unary_function{typedef A argument_type;typedef B result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{public:typedef T value_type;template<class U>struct rebind{typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}};
template<class T,class A>struct _Alloc_traits{typedef typename A::template rebind<T>::other allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base{typedef _Rb_tree_node_base*_Base_ptr;_Rb_tree_Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);static _Base_ptr __cdecl _M_decrement(_Base_ptr);};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;
 _Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&p){_M_node=p._M_node;}
 reference operator*()const{return _Link_type(_M_node)->_M_value_field;}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}_Self&operator--(){_M_node=_Rb_global_inst::_M_decrement(_M_node);return*this;}
};
template<class V,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<V>_Node;typedef typename _Alloc_traits<V,A>::allocator_type allocator_type;
 _Rb_tree_base(const allocator_type& = allocator_type());~_Rb_tree_base();
protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;
};
template<class K,class V,class Key,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A>{
 typedef _Rb_tree_base<V,A>_Base;
protected:typedef _Rb_tree_node_base*_Base_ptr;typedef _Rb_tree_node<V>_Node;typedef _Rb_tree_Color_type _Color_type;
public:typedef K key_type;typedef V value_type;typedef V*pointer;typedef const V*const_pointer;typedef V&reference;typedef const V&const_reference;typedef _Rb_tree_node<V>*_Link_type;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag _Iterator_category;typedef typename _Base::allocator_type allocator_type;
protected:size_type _M_node_count;Compare _M_key_compare;
 _Link_type&_M_rightmost()const{return(_Link_type&)this->_M_header._M_data->_M_right;}
 static _Link_type&__cdecl _S_right(_Link_type p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Link_type p){return p->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type p){return Key()(_S_value(p));}
 static _Link_type&__cdecl _S_right(_Base_ptr p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Base_ptr p){return((_Link_type)p)->_M_value_field;}
 static const K&__cdecl _S_key(_Base_ptr p){return Key()(_S_value(_Link_type(p)));}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
private:iterator _M_insert(_Base_ptr,_Base_ptr,const value_type&,_Base_ptr=0);
public:size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};

template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{
public:typedef pair<const K,V>value_type;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;
};
template<class K,class C=less<K>,class A=allocator<K> >class set{
private:typedef _Rb_tree<K,K,_Identity<K>,C,A>_Rep_type;_Rep_type _M_t;
};
}


#define CObserver CObserverFields
// Begin shared header engine/CObserver.h
#ifndef FABLE_ENGINE_COBSERVER_H
#define FABLE_ENGINE_COBSERVER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CObserver = 0x18.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CObserver = 0x14.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
struct CObserver {
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char EventsToObserve[0xc];                      // +0x04 set<NUISystem::EEvent,std::less<NUISystem::EEvent>,std::allocator<NUISystem::EEvent>_>
    bool          PreventObservation;                        // +0x10
    unsigned char _pad_0x11[0x3];                            // +0x11
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CObserver) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CObserver, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CObserver, EventsToObserve) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CObserver, PreventObservation) == 0x10);
#endif
#endif

#endif // FABLE_ENGINE_COBSERVER_H

// End shared header engine/CObserver.h
#undef CObserver
namespace std{
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag iterator_category;_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}_List_iterator_base(){}void _M_incr(){_M_node=_M_node->_M_next;}void _M_decr(){_M_node=_M_node->_M_prev;}bool operator==(const _List_iterator_base&x)const{return _M_node==x._M_node;}bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Const_traits{typedef T value_type;typedef const T*pointer;typedef const T&reference;typedef _Nonconst_traits<T>_Non_const_traits;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{typedef T value_type;typedef typename Traits::pointer pointer;typedef typename Traits::reference reference;typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,_Const_traits<T> >const_iterator;typedef _List_iterator<T,Traits>_Self;typedef bidirectional_iterator_tag iterator_category;typedef _List_node<T>_Node;typedef size_t size_type;typedef ptrdiff_t difference_type;
 _List_iterator(_Node*n):_List_iterator_base(n){}_List_iterator(){}_List_iterator(const iterator&i):_List_iterator_base(i._M_node){}reference operator*()const{return static_cast<_Node*>(_M_node)->_M_data;}_Self&operator++(){this->_M_incr();return*this;}
};
template<class T,class A>class _List_base{protected:typedef _List_node<T>_Node;typedef typename _Alloc_traits<_Node,A>::allocator_type _Node_allocator_type;
public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;allocator_type get_allocator()const{return allocator_type(static_cast<const _Node_allocator_type&>(_M_node));}_List_base(const allocator_type&);~_List_base();void clear();_STLP_alloc_proxy<_Node*,_Node,_Node_allocator_type>_M_node;
};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{typedef _List_base<T,A>_Base;
public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T&reference;typedef const T&const_reference;typedef _List_node<T>_Node;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef typename _Base::allocator_type allocator_type;typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,_Const_traits<T> >const_iterator;
 explicit list(const allocator_type&a=allocator_type());list(const list&);~list();
 iterator begin(){return iterator(static_cast<_Node*>(this->_M_node._M_data->_M_next));}const_iterator begin()const{return const_iterator(static_cast<_Node*>(this->_M_node._M_data->_M_next));}iterator end(){return iterator(this->_M_node._M_data);}const_iterator end()const{return const_iterator(this->_M_node._M_data);}bool empty()const{return this->_M_node._M_data->_M_next==this->_M_node._M_data;}
};
}
extern "C" void*__cdecl malloc(unsigned int);
namespace std{
template<class T,class A>_List_base<T,A>::_List_base(const allocator_type&a):_M_node(_Node_allocator_type(a),(_Node*)0){
 _Node*n=static_cast<_Node*>(malloc(sizeof(_Node)));n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
}
template<class T,class A>__declspec(noinline)list<T,A>::list(const list<T,A>&other):_Base(other.get_allocator()){
 for(const_iterator i=other.begin();i!=other.end();++i){
  _Node*n=static_cast<_Node*>(malloc(sizeof(_Node)));new(&n->_M_data)T(*i);
  _List_node_base*last=this->_M_node._M_data;n->_M_next=last;n->_M_prev=last->_M_prev;last->_M_prev->_M_next=n;last->_M_prev=n;
 }
}
}
extern "C" void __cdecl free(void*);
namespace std{
template<class T,class A>_List_base<T,A>::~_List_base(){_List_node_base*sentinel=this->_M_node._M_data;_List_node_base*p=sentinel->_M_next;while(p!=sentinel){_List_node_base*next=p->_M_next;free(p);p=next;}free(sentinel);}
template<class T,class A>__declspec(noinline)list<T,A>::~list(){}
}
namespace NUISystem{
enum EEvent{};
class CObserver{public:virtual~CObserver();virtual void ProcessEvent(EEvent)=0;virtual bool IsObservingEvent(EEvent);virtual void ObserveEvent(EEvent);virtual void IgnoreEvent(EEvent);virtual void IgnoreAllEvents();virtual void ObserveAllEvents();virtual void SetPreventObserving(bool);protected:std::set<EEvent>EventsToObserve;bool PreventObservation;};
// Exactoriginaldata16 and firstvirtualAPIprefix0; unusedvirtualtailAPIs not
// redeclared, no claim about thoseunobserved functions isintroduced.
class CObservable{public:virtual void SetEvent(EEvent);~CObservable();protected:std::list<CObserver*>Observers;CObserver*m_pExclusiveObserver;std::list<CObserver*>ConcurrentExclusiveObservers;};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CObserver)==sizeof(CObserverFields));
#endif
void CObservable::SetEvent(EEvent event){
 if(m_pExclusiveObserver){if(m_pExclusiveObserver->IsObservingEvent(event))m_pExclusiveObserver->ProcessEvent(event);}
 else if(!ConcurrentExclusiveObservers.empty()){
  std::list<CObserver*>observers(ConcurrentExclusiveObservers);
  std::list<CObserver*>::iterator it=observers.begin();std::list<CObserver*>::iterator end=observers.end();
  for(;it!=end;++it)if((*it)->IsObservingEvent(event))(*it)->ProcessEvent(event);
 }else{
  std::list<CObserver*>observers(Observers);
  std::list<CObserver*>::iterator it=observers.begin();std::list<CObserver*>::iterator end=observers.end();
  for(;it!=end;++it)if((*it)->IsObservingEvent(event))(*it)->ProcessEvent(event);
 }
}}

#undef malloc
#undef free
#undef private
#undef protected
static unsigned allocated,freed;
extern "C" void*__cdecl fixture_malloc(unsigned int bytes){void*p=malloc(bytes);if(!p)abort();++allocated;return p;}
extern "C" void __cdecl fixture_free(void*p){if(p){++freed;free(p);}}
namespace std{
template<class T,class A>list<T,A>::list(const allocator_type&a):_Base(a){}
template<class T,class A>void _List_base<T,A>::clear(){_List_node_base*head=this->_M_node._M_data;_List_node_base*p=head->_M_next;while(p!=head){_List_node_base*next=p->_M_next;fixture_free(p);p=next;}head->_M_next=head->_M_prev=head;}
template<class T,class A>_Rb_tree_base<T,A>::_Rb_tree_base(const allocator_type&a):_M_header(_M_node_allocator_type(a),(_Node*)0){}
template<class T,class A>_Rb_tree_base<T,A>::~_Rb_tree_base(){}
}
namespace NUISystem{
CObserver::~CObserver(){}bool CObserver::IsObservingEvent(EEvent){return false;}void CObserver::ObserveEvent(EEvent){}void CObserver::IgnoreEvent(EEvent){}void CObserver::IgnoreAllEvents(){}void CObserver::ObserveAllEvents(){}void CObserver::SetPreventObserving(bool){}CObservable::~CObservable(){}
}
typedef std::list<NUISystem::CObserver*>TestList;
typedef std::_List_node<NUISystem::CObserver*>TestNode;
class TestObserver:public NUISystem::CObserver{
public:unsigned id,queries,handled;bool interested,mutate;NUISystem::CObservable*owner;
 TestObserver():id(0),queries(0),handled(0),interested(false),mutate(false),owner(0){}
 virtual bool IsObservingEvent(NUISystem::EEvent e){if((int)e!=25)abort();++queries;return interested;}
 virtual void ProcessEvent(NUISystem::EEvent e){if((int)e!=25)abort();++handled;if(mutate){owner->Observers.clear();owner->ConcurrentExclusiveObservers.clear();}}
};
static void append(TestList&list,NUISystem::CObserver*p){TestNode*head=list._M_node._M_data;TestNode*n=static_cast<TestNode*>(fixture_malloc(sizeof(TestNode)));n->_M_data=p;n->_M_prev=head->_M_prev;n->_M_next=head;head->_M_prev->_M_next=n;head->_M_prev=n;}
int main(){
 unsigned cases=0;
 for(unsigned regular=0;regular<=5;++regular)for(unsigned concurrent=0;concurrent<=3;++concurrent)for(int exclusive=-1;exclusive<2;++exclusive)for(unsigned mask=0;mask<32;++mask)for(unsigned mutate=0;mutate<2;++mutate){
  {
   NUISystem::CObservable owner;TestObserver observers[5];
   for(unsigned i=0;i<5;++i){observers[i].id=i;observers[i].owner=&owner;observers[i].interested=(mask&(1u<<i))!=0;observers[i].mutate=mutate!=0;}
   for(unsigned i=0;i<regular;++i)append(owner.Observers,&observers[i]);
   for(unsigned i=0;i<concurrent;++i)append(owner.ConcurrentExclusiveObservers,&observers[i]);
   owner.m_pExclusiveObserver=exclusive<0?0:&observers[exclusive];
   unsigned before=allocated;
   owner.SetEvent(static_cast<NUISystem::EEvent>(25));
   unsigned active=concurrent?concurrent:regular;
   if(allocated-before!=(exclusive<0?active+1:0))return 1;
   for(unsigned i=0;i<5;++i){unsigned expected=exclusive<0?(i<active?1:0):(i==(unsigned)exclusive?1:0);if(observers[i].queries!=expected||observers[i].handled!=(expected&&observers[i].interested?1:0))return 2;}
  }
  if(allocated!=freed)return 3;++cases;
 }
 if(cases!=4608)return 4;
 puts("0055cb10_TEST PASS");return 0;
}