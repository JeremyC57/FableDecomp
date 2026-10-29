#include <cstdio>
#include <cstdlib>
#include <cstring>
#define PASS9_TEST
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
#endif
// 00619a10 std::_Rb_tree::_M_copy
// Leaf: ?_M_copy@?$_Rb_tree@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$vector@VCHeroLogBookEntry@CHeroLogBook@@V?$allocator@VCHeroLogBookEntry@CHeroLogBook@@@std@@@std@@@std@@U?$_Select1st@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$vector@VCHeroLogBookEntry@CHeroLogBook@@V?$allocator@VCHeroLogBookEntry@CHeroLogBook@@@std@@@std@@@std@@@5@U?$less@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@5@V?$allocator@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$vector@VCHeroLogBookEntry@CHeroLogBook@@V?$allocator@VCHeroLogBookEntry@CHeroLogBook@@@std@@@std@@@std@@@5@@std@@AAEPAU?$_Rb_tree_node@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$vector@VCHeroLogBookEntry@CHeroLogBook@@V?$allocator@VCHeroLogBookEntry@CHeroLogBook@@@std@@@std@@@std@@@2@PAU32@0@Z
// Flags:
// Layout: original nested logbook Entry has three CWideString fields at0/4/8; retail node32 holds enumkey4 and vector12.
#include <stddef.h>
#include <new>
#include "rebuild_abi.h"
extern "C" void*__cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
#ifdef PASS9_TEST
int main();
#endif
class CWideStringData;class CWideString {public:CWideStringData*PStringData;CWideString(const CWideString&);~CWideString();};
class CHeroLogBook {public:class CHeroLogBookEntry {public:enum ECategory {};CWideString Name,AbbreviatedName,Content;};};
namespace std{
typedef unsigned int size_t;typedef int ptrdiff_t;
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>&p):first(p.first),second(p.second){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class B,class C>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef C result_type;};
template<class A,class B>struct unary_function{typedef A argument_type;typedef B result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{public:typedef T value_type;template<class U>struct rebind{typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}T*allocate(size_t n,const void* =0){return n?static_cast<T*>(malloc(n*sizeof(T))):0;}void deallocate(T*p,size_t){free(p);}};
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
 _Rb_tree_base(const allocator_type&);~_Rb_tree_base();
protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;
};
struct __false_type {__false_type(){}};
template<class T,class U>inline void __fastcall _Construct(T*p,const U&x){::new(static_cast<void*>(p))T(x);}
template<class I,class O>O __fastcall __uninitialized_copy(I,I,O,const __false_type&);

typedef CHeroLogBook::CHeroLogBookEntry Entry;
template<>__declspec(noinline) Entry*__fastcall __uninitialized_copy<const Entry*,Entry*>(const Entry*first,const Entry*last,Entry*result,const __false_type&){Entry*cur=result;for(;first!=last;++first,++cur)_Construct(&*cur,*first);return cur;}

template<class T>struct __type_traits {typedef __false_type has_trivial_assignment_operator;};
template<class T,class A>class _Vector_base {public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;_Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,allocator_type>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:typedef A allocator_type;typedef unsigned int size_type;typedef const T*const_iterator;typedef const T*const_pointer;typedef typename __type_traits<T>::has_trivial_assignment_operator _IsPODType;size_type size()const{return this->_M_finish-this->_M_start;}allocator_type get_allocator()const{return allocator_type();}const_iterator begin()const{return this->_M_start;}const_iterator end()const{return this->_M_finish;}vector(const vector&x):_Vector_base<T,A>(x.size(),x.get_allocator()){this->_M_finish=__uninitialized_copy((const_pointer)x._M_start,(const_pointer)x._M_finish,this->_M_start,_IsPODType());}};
template<class K,class V,class Key,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A> {typedef _Rb_tree_base<V,A>_Base;public:typedef V value_type;typedef _Rb_tree_node<V>*_Link_type;typedef unsigned int size_type;protected:size_type _M_node_count;Compare _M_key_compare;
_Link_type _M_create_node(const value_type&x){_Link_type tmp=this->_M_header.allocate(1);_Construct(&tmp->_M_value_field,x);return tmp;}
_Link_type _M_clone_node(_Link_type x){_Link_type tmp=_M_create_node(x->_M_value_field);tmp->_M_color=x->_M_color;tmp->_M_left=0;tmp->_M_right=0;return tmp;}
static _Link_type&__cdecl _S_left(_Link_type x){return (_Link_type&)x->_M_left;}static _Link_type&__cdecl _S_right(_Link_type x){return (_Link_type&)x->_M_right;}
private:_Link_type _M_copy(_Link_type,_Link_type);
#ifdef PASS9_TEST
friend int ::main();
#endif
};
template<class K,class V,class Key,class C,class A>typename _Rb_tree<K,V,Key,C,A>::_Link_type _Rb_tree<K,V,Key,C,A>::_M_copy(_Link_type x,_Link_type p){_Link_type top=_M_clone_node(x);top->_M_parent=p;if(x->_M_right)top->_M_right=_M_copy(_S_right(x),top);p=top;x=_S_left(x);while(x!=0){_Link_type y=_M_clone_node(x);p->_M_left=y;y->_M_parent=p;if(x->_M_right)y->_M_right=_M_copy(_S_right(x),y);p=y;x=_S_left(x);}return top;}
typedef CHeroLogBook::CHeroLogBookEntry Entry;
typedef Entry::ECategory Category;
typedef pair<const Category,vector<Entry> >Value;
template _Rb_tree<Category,Value,_Select1st<Value>,less<Category>,allocator<Value> >::_Link_type _Rb_tree<Category,Value,_Select1st<Value>,less<Category>,allocator<Value> >::_M_copy(_Link_type,_Link_type);
}
FABLE_STATIC_ASSERT(sizeof(CHeroLogBook::CHeroLogBookEntry)==12);

static unsigned stringCopies;
CWideString::CWideString(const CWideString&x):PStringData(x.PStringData){++stringCopies;}
CWideString::~CWideString(){}
namespace std {template<class I,class O>O __fastcall __uninitialized_copy(I first,I last,O out,const __false_type&){for(;first!=last;++first,++out)::new(static_cast<void*>(out))CHeroLogBook::CHeroLogBookEntry(*first);return out;}}
typedef CHeroLogBook::CHeroLogBookEntry E;
typedef E::ECategory K;
typedef std::pair<const K,std::vector<E> >V;
typedef std::_Rb_tree<K,V,std::_Select1st<V>,std::less<K>,std::allocator<V> >Tree;
typedef std::_Rb_tree_node<V>N;
static void check(bool p){if(!p)std::abort();}
static unsigned char tokens[12];
static N*makeNode(int key,unsigned n){N*p=static_cast<N*>(malloc(sizeof(N)));std::memset(p,0,sizeof(N));K k=static_cast<K>(key);std::memcpy(const_cast<K*>(&p->_M_value_field.first),&k,sizeof(k));E*entries=n?static_cast<E*>(malloc(n*sizeof(E))):0;for(unsigned i=0;i<n;++i){CWideStringData*ptrs[3]={reinterpret_cast<CWideStringData*>(tokens+i*3),reinterpret_cast<CWideStringData*>(tokens+i*3+1),reinterpret_cast<CWideStringData*>(tokens+i*3+2)};std::memcpy(entries+i,ptrs,sizeof ptrs);}E*ptrs[3]={entries,entries+n,entries+n};std::memcpy(&p->_M_value_field.second,ptrs,sizeof ptrs);p->_M_color=(key&1)!=0;return p;}
static unsigned validate(N*source,N*copy,N*parent){check(source!=copy&&source->_M_color==copy->_M_color&&copy->_M_parent==parent&&source->_M_value_field.first==copy->_M_value_field.first);const std::vector<E>&a=source->_M_value_field.second;const std::vector<E>&b=copy->_M_value_field.second;check(a.size()==b.size());if(a.size())check(a.begin()!=b.begin());for(unsigned i=0;i<a.size();++i){check(a.begin()[i].Name.PStringData==b.begin()[i].Name.PStringData&&a.begin()[i].AbbreviatedName.PStringData==b.begin()[i].AbbreviatedName.PStringData&&a.begin()[i].Content.PStringData==b.begin()[i].Content.PStringData);}unsigned total=a.size();if(source->_M_left)total+=validate(static_cast<N*>(source->_M_left),static_cast<N*>(copy->_M_left),copy);else check(copy->_M_left==0);if(source->_M_right)total+=validate(static_cast<N*>(source->_M_right),static_cast<N*>(copy->_M_right),copy);else check(copy->_M_right==0);return total;}
static void cleanup(N*p){if(!p)return;cleanup(static_cast<N*>(p->_M_left));cleanup(static_cast<N*>(p->_M_right));free(const_cast<E*>(p->_M_value_field.second.begin()));free(p);}
int main(){check(sizeof(N)==32&&sizeof(Tree)==12);union Storage{double align;unsigned char bytes[sizeof(Tree)];}storage;std::memset(&storage,0,sizeof storage);Tree*t=reinterpret_cast<Tree*>(storage.bytes);N*sentinel=makeNode(0,0);for(unsigned n=0;n<=2;++n){N*source=makeNode(1,n);stringCopies=0;N*copy=t->_M_copy(source,sentinel);check(validate(source,copy,sentinel)==n&&stringCopies==n*3);if(n){const_cast<E*>(copy->_M_value_field.second.begin())->Name.PStringData=0;check(source->_M_value_field.second.begin()->Name.PStringData!=0);}cleanup(source);cleanup(copy);}N*root=makeNode(3,2);N*l=makeNode(1,1);N*r=makeNode(5,0);N*lr=makeNode(2,2);N*rl=makeNode(4,1);root->_M_left=l;root->_M_right=r;l->_M_parent=root;r->_M_parent=root;l->_M_right=lr;lr->_M_parent=l;r->_M_left=rl;rl->_M_parent=r;stringCopies=0;N*copy=t->_M_copy(root,sentinel);check(validate(root,copy,sentinel)==6&&stringCopies==18);cleanup(root);cleanup(copy);cleanup(sentinel);std::puts("00619a10_TEST PASS");return 0;}