#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
#define __forceinline inline
#endif
#include <stdio.h>
#include <string.h>
// Original persistence section clearing, real nested values and STLport iterator erase.
// Leaf: ?ClearPersistenceDataForSection@CEntityRuntimePersistence@NThingManager@@QAEXABVCCharString@@@Z
// Flags:
// Complete original CThingManager storage/hierarchy/four-slot interface.
// Field visibility is a data projection; method access and exact keys are original.
#include <stddef.h>
#include <new>
#include "rebuild_abi.h"
extern "C" void*__cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
class CWideStringData;
#ifdef _MSC_VER
#define FABLE_MS_BITFIELDS
#else
#define FABLE_MS_BITFIELDS __attribute__((ms_struct))
#endif
#pragma pack(push,1)
template<class T>class FABLE_MS_BITFIELDS CBasicString {public:T*PData;unsigned long StringLength;unsigned long DataLength:31;bool UseFastExtend:1;unsigned long GetLength()const{return StringLength;}};
class CCharStringData {public:CBasicString<char>Data;long NoRefs;};
#pragma pack(pop)
extern const unsigned long CUnprovenGlobal_0129a168[256];
class CCharString {public:CCharStringData*PStringData;CCharString();CCharString(const CCharString&);~CCharString();const CCharString&operator=(const char*);long GetLength()const{if(!PStringData)return 0;return PStringData->Data.GetLength();}operator const char*()const{if(!PStringData)return "";return PStringData->Data.PData;}__forceinline unsigned long GetCRC()const{unsigned long length=GetLength();const char*data=*this;unsigned long crc=0;if(!length)return 0;for(unsigned long i=0;i<length;++i)crc=CUnprovenGlobal_0129a168[static_cast<unsigned char>(data[i])^(crc&255)]^(crc>>8);return crc;}};
class CWideString {public:CWideStringData*PStringData;CWideString();CWideString(const CWideString&);~CWideString();const CWideString&operator=(const CWideString&);};
#ifdef _MSC_VER
typedef unsigned __int64 ThingIdentifier;
#else
typedef unsigned int ThingIdentifier __attribute__((mode(DI),aligned(8)));
#endif
namespace std {
typedef ::size_t size_t;typedef ::ptrdiff_t ptrdiff_t;
struct __true_type {};struct __false_type {};
template<class T>struct __type_traits {typedef __true_type this_dummy_member_must_be_first;typedef __false_type has_trivial_default_constructor;typedef __false_type has_trivial_copy_constructor;typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;typedef __false_type is_POD_type;};

template<int IsPOD>struct __type_traits_aux {typedef __false_type has_trivial_default_constructor;typedef __false_type has_trivial_copy_constructor;typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;typedef __false_type is_POD_type;};
template<>struct __type_traits_aux<0> {typedef __false_type has_trivial_default_constructor;typedef __false_type has_trivial_copy_constructor;typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;typedef __false_type is_POD_type;};
template<>struct __type_traits_aux<1> {typedef __true_type has_trivial_default_constructor;typedef __true_type has_trivial_copy_constructor;typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;typedef __true_type is_POD_type;};
template<class T>struct __type_traits<T*>:public __type_traits_aux<1> {};
template<>struct __type_traits<long>:public __type_traits_aux<1> {};
template<>struct __type_traits<unsigned char>:public __type_traits_aux<1> {};
template<class T>class allocator {public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T&reference;typedef const T&const_reference;typedef size_t size_type;typedef ptrdiff_t difference_type;template<class U>struct rebind {typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}~allocator(){}T*allocate(size_type n,const void* =0){return n?reinterpret_cast<T*>(malloc(n*sizeof(T))):0;}void deallocate(pointer p,size_type){if(p)free(p);}};
template<class T,class A>struct _Alloc_traits {typedef A _Orig;typedef A allocator_type;};
template<class T,class U>struct _Alloc_traits<T,allocator<U> > {typedef allocator<U>_Orig;typedef allocator<T>allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {typedef A _Base;typedef _STLP_alloc_proxy<V,T,A>_Self;public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}T*allocate(size_t n){return static_cast<_Base&>(*this).allocate(n,0);}void deallocate(T*p,size_t n){static_cast<_Base&>(*this).deallocate(p,n);}};
template<class T>inline void __fastcall _Destroy(T*p){p->~T();}
template<class F>inline void __fastcall __destroy_aux(F first,F last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}
template<class F>inline void __fastcall __destroy_aux(F,F,const __true_type&){}
template<class F,class T>inline void __fastcall __destroy(F first,F last,T*){typedef typename __type_traits<T>::has_trivial_destructor _Trivial_destructor;__destroy_aux(first,last,_Trivial_destructor());}
template<class T>inline void __fastcall _Destroy(T*first,T*last){__destroy(first,last,first);}
template<class T,class U>inline void __fastcall _Construct(T*p,const U&v){::new(static_cast<void*>(p))T(v);}
#pragma auto_inline(off)
template<class I,class F>F __fastcall __uninitialized_copy(I first,I last,F result,const __false_type&){F cur=result;for(;first!=last;++first,++cur)_Construct(&*cur,*first);return cur;}
#pragma auto_inline(on)

struct input_iterator_tag {};struct forward_iterator_tag:public input_iterator_tag {};struct bidirectional_iterator_tag:public forward_iterator_tag {};
struct _List_node_base {_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base {T _M_data;};
struct _List_iterator_base {_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}_List_iterator_base(){}void _M_incr(){_M_node=_M_node->_M_next;}void _M_decr(){_M_node=_M_node->_M_prev;}bool operator==(const _List_iterator_base&x)const{return _M_node==x._M_node;}bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits {typedef T value_type;typedef T*pointer;typedef T&reference;};template<class T>struct _Const_traits {typedef T value_type;typedef const T*pointer;typedef const T&reference;typedef _Nonconst_traits<T>_Non_const_traits;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base {typedef T value_type;typedef typename Traits::pointer pointer;typedef typename Traits::reference reference;typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,_Const_traits<T> >const_iterator;typedef _List_iterator<T,Traits>_Self;typedef bidirectional_iterator_tag iterator_category;typedef _List_node<T>_Node;typedef size_t size_type;typedef ptrdiff_t difference_type;_List_iterator(_Node*n):_List_iterator_base(n){}_List_iterator(){}_List_iterator(const iterator&x):_List_iterator_base(x._M_node){}reference operator*()const{return static_cast<_Node*>(_M_node)->_M_data;}_Self&operator++(){this->_M_incr();return *this;}};
template<class T>struct _Is_integer {typedef __false_type _Integral;};
template<class T,class A>class _List_base {protected:typedef _List_node<T>_Node;typedef typename _Alloc_traits<_Node,A>::allocator_type _Node_allocator_type;public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;allocator_type get_allocator()const{return allocator_type(static_cast<const _Node_allocator_type&>(_M_node));}_List_base(const allocator_type&a):_M_node(_Node_allocator_type(a),static_cast<_Node*>(0)){_Node*n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;}~_List_base(){clear();_M_node.deallocate(_M_node._M_data,1);}void clear(){_Node*cur=static_cast<_Node*>(_M_node._M_data->_M_next);while(cur!=_M_node._M_data){_Node*tmp=cur;cur=static_cast<_Node*>(cur->_M_next);_Destroy(&tmp->_M_data);_M_node.deallocate(tmp,1);}_M_node._M_data->_M_next=_M_node._M_data;_M_node._M_data->_M_prev=_M_node._M_data;}public:_STLP_alloc_proxy<_Node*,_Node,_Node_allocator_type>_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A> {typedef _List_base<T,A>_Base;typedef list<T,A>_Self;public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T&reference;typedef const T&const_reference;typedef _List_node<T>_Node;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef typename _Base::allocator_type allocator_type;typedef _List_iterator<T,_Nonconst_traits<T> >iterator;typedef _List_iterator<T,_Const_traits<T> >const_iterator;protected:_Node*_M_create_node(const T&x){_Node*p=this->_M_node.allocate(1);_Construct(&p->_M_data,x);return p;}public:explicit list(const allocator_type&a=allocator_type()):_List_base<T,A>(a){}list(const list&x):_List_base<T,A>(x.get_allocator()){insert(begin(),x.begin(),x.end());}~list(){}iterator begin(){return iterator(static_cast<_Node*>(this->_M_node._M_data->_M_next));}const_iterator begin()const{return const_iterator(static_cast<_Node*>(this->_M_node._M_data->_M_next));}iterator end(){return iterator(this->_M_node._M_data);}const_iterator end()const{return const_iterator(this->_M_node._M_data);}bool empty()const{return this->_M_node._M_data->_M_next==this->_M_node._M_data;}iterator insert(iterator position,const T&x){_Node*tmp=_M_create_node(x);_List_node_base*n=position._M_node,*p=n->_M_prev;tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;return tmp;}template<class I>void insert(iterator pos,I first,I last){typedef typename _Is_integer<I>::_Integral _Integral;_M_insert_dispatch(pos,first,last,_Integral());}template<class I>void _M_insert_dispatch(iterator pos,I first,I last,const __false_type&){for(;first!=last;++first)insert(pos,*first);}void push_back(const T&x){insert(end(),x);}};

template<class F,class S,class T>inline F __fastcall fill_n(F first,S n,const T&x){for(;n>0;--n,++first)*first=x;return first;}
template<class F,class S,class T>inline F __fastcall __uninitialized_fill_n(F first,S n,const T&x,const __true_type&){return fill_n(first,n,x);}
template<class F,class S,class T>inline F __fastcall __uninitialized_fill_n(F first,S n,const T&x,const __false_type&){F cur=first;for(;n>0;--n,++cur)_Construct(&*cur,x);return cur;}
template<class F,class S,class T>inline F __fastcall uninitialized_fill_n(F first,S n,const T&x){return __uninitialized_fill_n(first,n,x,typename __type_traits<T>::is_POD_type());}

template<class T,class A>class _Vector_base {public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}_Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,allocator_type>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {typedef _Vector_base<T,A>_Base;public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T*iterator;typedef const T*const_iterator;typedef T&reference;typedef const T&const_reference;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef typename _Vector_base<T,A>::allocator_type allocator_type;protected:typedef typename __type_traits<T>::has_trivial_assignment_operator _TrivialAss;typedef typename __type_traits<T>::has_trivial_assignment_operator _IsPODType;public:allocator_type get_allocator()const{return static_cast<const allocator_type&>(this->_M_end_of_storage);}size_type size()const{return size_type(this->_M_finish-this->_M_start);}iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}reference operator[](size_type n){return *(begin()+n);}const_reference operator[](size_type n)const{return *(begin()+n);}iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}explicit vector(const allocator_type&a=allocator_type()):_Vector_base<T,A>(a){}vector(size_type n,const T&value,const allocator_type&a=allocator_type()):_Vector_base<T,A>(n,a){this->_M_finish=uninitialized_fill_n(this->_M_start,n,value);}vector(const vector&x):_Vector_base<T,A>(x.size(),x.get_allocator()){this->_M_finish=__uninitialized_copy(static_cast<const_pointer>(x._M_start),static_cast<const_pointer>(x._M_finish),this->_M_start,_IsPODType());}~vector(){_Destroy(this->_M_start,this->_M_finish);}};
}
namespace std {
template<class A,class B>struct pair {typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}};
template<class A,class B,class T>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef T result_type;};template<class A,class T>struct unary_function {typedef A argument_type;typedef T result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
struct _Rb_tree_node_base {typedef _Rb_tree_node_base*_Base_ptr;bool _M_color;_Base_ptr _M_parent,_M_left,_M_right;static _Base_ptr __cdecl _S_minimum(_Base_ptr x){while(x->_M_left!=0)x=x->_M_left;return x;}static _Base_ptr __cdecl _S_maximum(_Base_ptr x){while(x->_M_right!=0)x=x->_M_right;return x;}};template<class T>struct _Rb_tree_node:public _Rb_tree_node_base {T _M_value_field;};
template<class T,class A>struct _Rb_tree_base {typedef _Rb_tree_node<T>_Node;typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;_Rb_tree_base(const allocator_type&);~_Rb_tree_base();protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;};
template<class Dummy>class _Rb_global {public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr,_Base_ptr&,_Base_ptr&,_Base_ptr&);static void __cdecl _Rotate_left(_Base_ptr,_Base_ptr&);static void __cdecl _Rotate_right(_Base_ptr,_Base_ptr&);static _Base_ptr __cdecl _M_increment(_Base_ptr n){if(n->_M_right!=0){n=n->_M_right;while(n->_M_left!=0)n=n->_M_left;}else{_Base_ptr y=n->_M_parent;while(n==y->_M_right){n=y;y=y->_M_parent;}if(n->_M_right!=y)n=y;}return n;}};typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator {typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator {typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&x){_M_node=x._M_node;}reference operator*()const{return static_cast<_Link_type>(_M_node)->_M_value_field;}pointer operator->()const{return &(operator*());}_Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}_Self operator++(int){_Self tmp=*this;_M_node=_Rb_global_inst::_M_increment(_M_node);return tmp;}};
template<class K,class V,class S,class C,class A>class _Rb_tree:public _Rb_tree_base<V,A> {typedef _Rb_tree_base<V,A>_Base;protected:typedef _Rb_tree_node_base*_Base_ptr;typedef _Rb_tree_node<V>_Node;public:typedef K key_type;typedef V value_type;typedef V&reference;typedef _Rb_tree_node<V>*_Link_type;typedef size_t size_type;typedef typename _Base::allocator_type allocator_type;typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
protected:size_t _M_node_count;C _M_key_compare;_Link_type&_M_root()const{return reinterpret_cast<_Link_type&>(this->_M_header._M_data->_M_parent);}static _Link_type&__cdecl _S_left(_Link_type x){return reinterpret_cast<_Link_type&>(x->_M_left);}static _Link_type&__cdecl _S_right(_Link_type x){return reinterpret_cast<_Link_type&>(x->_M_right);}static reference __cdecl _S_value(_Link_type x){return x->_M_value_field;}static const K&__cdecl _S_key(_Link_type x){return S()(_S_value(x));}
public:_Rb_tree();_Rb_tree(const _Rb_tree&);~_Rb_tree();iterator begin(){return iterator(static_cast<_Link_type>(this->_M_header._M_data->_M_left));}const_iterator begin()const{return const_iterator(static_cast<_Link_type>(this->_M_header._M_data->_M_left));}void erase(iterator position){_Link_type y=static_cast<_Link_type>(_Rb_global_inst::_Rebalance_for_erase(position._M_node,this->_M_header._M_data->_M_parent,this->_M_header._M_data->_M_left,this->_M_header._M_data->_M_right));_Destroy(&y->_M_value_field);this->_M_header.deallocate(y,1);--_M_node_count;}iterator end(){return iterator(this->_M_header._M_data);}const_iterator end()const{return const_iterator(this->_M_header._M_data);}template<class KT>iterator find(const KT&x){return iterator(_M_find(x));}template<class KT>const_iterator find(const KT&x)const{return const_iterator(_M_find(x));}
private:template<class KT>_Rb_tree_node<V>*_M_find(const KT&k)const{_Link_type y=this->_M_header._M_data;_Link_type x=_M_root();while(x!=0)if(!_M_key_compare(_S_key(x),k))y=x,x=_S_left(x);else x=_S_right(x);if(y==this->_M_header._M_data||_M_key_compare(k,_S_key(y)))y=this->_M_header._M_data;return y;}
};
template<class K,class T,class C=less<K>,class A=allocator<pair<const K,T> > >class map {private:typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,C,A>_Rep_type;_Rep_type _M_t;public:typedef K key_type;typedef T mapped_type;typedef pair<const K,T>value_type;typedef typename _Rep_type::iterator iterator;typedef typename _Rep_type::const_iterator const_iterator;map();map(const map&);~map();iterator begin(){return _M_t.begin();}const_iterator begin()const{return _M_t.begin();}void erase(iterator position){_M_t.erase(position);}iterator find(const key_type&x){return _M_t.find(x);}const_iterator find(const key_type&x)const{return _M_t.find(x);}iterator end(){return _M_t.end();}const_iterator end()const{return _M_t.end();}T&operator[](const key_type&);};
}

#ifdef _MSC_VER
typedef unsigned __int64 PersistenceIdentifier;
#else
typedef unsigned int PersistenceIdentifier __attribute__((mode(DI),aligned(8)));
#endif
template<class T>class CArray:public std::vector<T> {};
namespace NThingManager {class CEntityRuntimePersistence {public:class CEntityScriptInfo {public:CCharString Script;CArray<unsigned char>SaveData;long UncompressedSize;bool Valid;};class CPersistedDeadEntity {public:PersistenceIdentifier UID;bool GamePersistent;unsigned long SectionNameCRC;};CArray<CEntityScriptInfo>LevelPersistentEntityScripts,GamePersistentEntityScripts;std::map<PersistenceIdentifier,CPersistedDeadEntity>PersistedDeadEntities;long SaveFormatVersion;void ClearPersistenceDataForSection(const CCharString&);private:void ClearPersistenceDataForSectionInEntityScript(const CCharString&,CEntityScriptInfo&);};}
FABLE_STATIC_ASSERT(sizeof(CBasicString<char>)==13);FABLE_STATIC_ASSERT(sizeof(CCharStringData)==17);FABLE_STATIC_ASSERT(sizeof(NThingManager::CEntityRuntimePersistence)==40);FABLE_STATIC_ASSERT(sizeof(NThingManager::CEntityRuntimePersistence::CEntityScriptInfo)==24);FABLE_STATIC_ASSERT(sizeof(NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity)==16);
void NThingManager::CEntityRuntimePersistence::ClearPersistenceDataForSection(const CCharString&section){for(unsigned long i=0;i<LevelPersistentEntityScripts.size();++i)if(LevelPersistentEntityScripts[i].Valid)ClearPersistenceDataForSectionInEntityScript(section,LevelPersistentEntityScripts[i]);for(unsigned long i=0;i<GamePersistentEntityScripts.size();++i)if(GamePersistentEntityScripts[i].Valid)ClearPersistenceDataForSectionInEntityScript(section,GamePersistentEntityScripts[i]);unsigned long crc=section.GetCRC();for(std::map<PersistenceIdentifier,CPersistedDeadEntity>::iterator i=PersistedDeadEntities.begin();i!=PersistedDeadEntities.end();)if(i->second.SectionNameCRC==crc)PersistedDeadEntities.erase(i++);else ++i;}

const unsigned long CUnprovenGlobal_0129a168[256]={0x00000000UL,0x77073096UL,0xee0e612cUL,0x990951baUL,0x076dc419UL,0x706af48fUL,0xe963a535UL,0x9e6495a3UL,0x0edb8832UL,0x79dcb8a4UL,0xe0d5e91eUL,0x97d2d988UL,0x09b64c2bUL,0x7eb17cbdUL,0xe7b82d07UL,0x90bf1d91UL,0x1db71064UL,0x6ab020f2UL,0xf3b97148UL,0x84be41deUL,0x1adad47dUL,0x6ddde4ebUL,0xf4d4b551UL,0x83d385c7UL,0x136c9856UL,0x646ba8c0UL,0xfd62f97aUL,0x8a65c9ecUL,0x14015c4fUL,0x63066cd9UL,0xfa0f3d63UL,0x8d080df5UL,0x3b6e20c8UL,0x4c69105eUL,0xd56041e4UL,0xa2677172UL,0x3c03e4d1UL,0x4b04d447UL,0xd20d85fdUL,0xa50ab56bUL,0x35b5a8faUL,0x42b2986cUL,0xdbbbc9d6UL,0xacbcf940UL,0x32d86ce3UL,0x45df5c75UL,0xdcd60dcfUL,0xabd13d59UL,0x26d930acUL,0x51de003aUL,0xc8d75180UL,0xbfd06116UL,0x21b4f4b5UL,0x56b3c423UL,0xcfba9599UL,0xb8bda50fUL,0x2802b89eUL,0x5f058808UL,0xc60cd9b2UL,0xb10be924UL,0x2f6f7c87UL,0x58684c11UL,0xc1611dabUL,0xb6662d3dUL,0x76dc4190UL,0x01db7106UL,0x98d220bcUL,0xefd5102aUL,0x71b18589UL,0x06b6b51fUL,0x9fbfe4a5UL,0xe8b8d433UL,0x7807c9a2UL,0x0f00f934UL,0x9609a88eUL,0xe10e9818UL,0x7f6a0dbbUL,0x086d3d2dUL,0x91646c97UL,0xe6635c01UL,0x6b6b51f4UL,0x1c6c6162UL,0x856530d8UL,0xf262004eUL,0x6c0695edUL,0x1b01a57bUL,0x8208f4c1UL,0xf50fc457UL,0x65b0d9c6UL,0x12b7e950UL,0x8bbeb8eaUL,0xfcb9887cUL,0x62dd1ddfUL,0x15da2d49UL,0x8cd37cf3UL,0xfbd44c65UL,0x4db26158UL,0x3ab551ceUL,0xa3bc0074UL,0xd4bb30e2UL,0x4adfa541UL,0x3dd895d7UL,0xa4d1c46dUL,0xd3d6f4fbUL,0x4369e96aUL,0x346ed9fcUL,0xad678846UL,0xda60b8d0UL,0x44042d73UL,0x33031de5UL,0xaa0a4c5fUL,0xdd0d7cc9UL,0x5005713cUL,0x270241aaUL,0xbe0b1010UL,0xc90c2086UL,0x5768b525UL,0x206f85b3UL,0xb966d409UL,0xce61e49fUL,0x5edef90eUL,0x29d9c998UL,0xb0d09822UL,0xc7d7a8b4UL,0x59b33d17UL,0x2eb40d81UL,0xb7bd5c3bUL,0xc0ba6cadUL,0xedb88320UL,0x9abfb3b6UL,0x03b6e20cUL,0x74b1d29aUL,0xead54739UL,0x9dd277afUL,0x04db2615UL,0x73dc1683UL,0xe3630b12UL,0x94643b84UL,0x0d6d6a3eUL,0x7a6a5aa8UL,0xe40ecf0bUL,0x9309ff9dUL,0x0a00ae27UL,0x7d079eb1UL,0xf00f9344UL,0x8708a3d2UL,0x1e01f268UL,0x6906c2feUL,0xf762575dUL,0x806567cbUL,0x196c3671UL,0x6e6b06e7UL,0xfed41b76UL,0x89d32be0UL,0x10da7a5aUL,0x67dd4accUL,0xf9b9df6fUL,0x8ebeeff9UL,0x17b7be43UL,0x60b08ed5UL,0xd6d6a3e8UL,0xa1d1937eUL,0x38d8c2c4UL,0x4fdff252UL,0xd1bb67f1UL,0xa6bc5767UL,0x3fb506ddUL,0x48b2364bUL,0xd80d2bdaUL,0xaf0a1b4cUL,0x36034af6UL,0x41047a60UL,0xdf60efc3UL,0xa867df55UL,0x316e8eefUL,0x4669be79UL,0xcb61b38cUL,0xbc66831aUL,0x256fd2a0UL,0x5268e236UL,0xcc0c7795UL,0xbb0b4703UL,0x220216b9UL,0x5505262fUL,0xc5ba3bbeUL,0xb2bd0b28UL,0x2bb45a92UL,0x5cb36a04UL,0xc2d7ffa7UL,0xb5d0cf31UL,0x2cd99e8bUL,0x5bdeae1dUL,0x9b64c2b0UL,0xec63f226UL,0x756aa39cUL,0x026d930aUL,0x9c0906a9UL,0xeb0e363fUL,0x72076785UL,0x05005713UL,0x95bf4a82UL,0xe2b87a14UL,0x7bb12baeUL,0x0cb61b38UL,0x92d28e9bUL,0xe5d5be0dUL,0x7cdcefb7UL,0x0bdbdf21UL,0x86d3d2d4UL,0xf1d4e242UL,0x68ddb3f8UL,0x1fda836eUL,0x81be16cdUL,0xf6b9265bUL,0x6fb077e1UL,0x18b74777UL,0x88085ae6UL,0xff0f6a70UL,0x66063bcaUL,0x11010b5cUL,0x8f659effUL,0xf862ae69UL,0x616bffd3UL,0x166ccf45UL,0xa00ae278UL,0xd70dd2eeUL,0x4e048354UL,0x3903b3c2UL,0xa7672661UL,0xd06016f7UL,0x4969474dUL,0x3e6e77dbUL,0xaed16a4aUL,0xd9d65adcUL,0x40df0b66UL,0x37d83bf0UL,0xa9bcae53UL,0xdebb9ec5UL,0x47b2cf7fUL,0x30b5ffe9UL,0xbdbdf21cUL,0xcabac28aUL,0x53b39330UL,0x24b4a3a6UL,0xbad03605UL,0xcdd70693UL,0x54de5729UL,0x23d967bfUL,0xb3667a2eUL,0xc4614ab8UL,0x5d681b02UL,0x2a6f2b94UL,0xb40bbe37UL,0xc30c8ea1UL,0x5a05df1bUL,0x2d02ef8dUL};
#define _STLP_CALL __cdecl
#define _STLP_STD std
#define _S_rb_tree_red false
#define _S_rb_tree_black true
namespace std {
template<class T>void __fastcall swap(T&a,T&b){T tmp=a;a=b;b=tmp;}
template <class _Dummy> void _STLP_CALL
_Rb_global<_Dummy>::_Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base* __y = __x->_M_right;
  __x->_M_right = __y->_M_left;
  if (__y->_M_left !=0)
    __y->_M_left->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_left)
    __x->_M_parent->_M_left = __y;
  else
    __x->_M_parent->_M_right = __y;
  __y->_M_left = __x;
  __x->_M_parent = __y;
}

template <class _Dummy> void _STLP_CALL 
_Rb_global<_Dummy>::_Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base* __y = __x->_M_left;
  __x->_M_left = __y->_M_right;
  if (__y->_M_right != 0)
    __y->_M_right->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_right)
    __x->_M_parent->_M_right = __y;
  else
    __x->_M_parent->_M_left = __y;
  __y->_M_right = __x;
  __x->_M_parent = __y;
}

template <class _Dummy> _Rb_tree_node_base* _STLP_CALL
_Rb_global<_Dummy>::_Rebalance_for_erase(_Rb_tree_node_base* __z,
					 _Rb_tree_node_base*& __root,
					 _Rb_tree_node_base*& __leftmost,
					 _Rb_tree_node_base*& __rightmost)
{
  _Rb_tree_node_base* __y = __z;
  _Rb_tree_node_base* __x = 0;
  _Rb_tree_node_base* __x_parent = 0;
  if (__y->_M_left == 0)     // __z has at most one non-null child. y == z.
    __x = __y->_M_right;     // __x might be null.
  else
    if (__y->_M_right == 0)  // __z has exactly one non-null child. y == z.
      __x = __y->_M_left;    // __x is not null.
    else {                   // __z has two non-null children.  Set __y to
      __y = __y->_M_right;   //   __z's successor.  __x might be null.
      while (__y->_M_left != 0)
        __y = __y->_M_left;
      __x = __y->_M_right;
    }
  if (__y != __z) {          // relink y in place of z.  y is z's successor
    __z->_M_left->_M_parent = __y; 
    __y->_M_left = __z->_M_left;
    if (__y != __z->_M_right) {
      __x_parent = __y->_M_parent;
      if (__x) __x->_M_parent = __y->_M_parent;
      __y->_M_parent->_M_left = __x;      // __y must be a child of _M_left
      __y->_M_right = __z->_M_right;
      __z->_M_right->_M_parent = __y;
    }
    else
      __x_parent = __y;  
    if (__root == __z)
      __root = __y;
    else if (__z->_M_parent->_M_left == __z)
      __z->_M_parent->_M_left = __y;
    else 
      __z->_M_parent->_M_right = __y;
    __y->_M_parent = __z->_M_parent;
    _STLP_STD::swap(__y->_M_color, __z->_M_color);
    __y = __z;
    // __y now points to node to be actually deleted
  }
  else {                        // __y == __z
    __x_parent = __y->_M_parent;
    if (__x) __x->_M_parent = __y->_M_parent;   
    if (__root == __z)
      __root = __x;
    else 
      if (__z->_M_parent->_M_left == __z)
        __z->_M_parent->_M_left = __x;
      else
        __z->_M_parent->_M_right = __x;
    if (__leftmost == __z) 
      if (__z->_M_right == 0)        // __z->_M_left must be null also
        __leftmost = __z->_M_parent;
    // makes __leftmost == _M_header if __z == __root
      else
        __leftmost = _Rb_tree_node_base::_S_minimum(__x);
    if (__rightmost == __z)  
      if (__z->_M_left == 0)         // __z->_M_right must be null also
        __rightmost = __z->_M_parent;  
    // makes __rightmost == _M_header if __z == __root
      else                      // __x == __z->_M_left
        __rightmost = _Rb_tree_node_base::_S_maximum(__x);
  }
  if (__y->_M_color != _S_rb_tree_red) { 
    while (__x != __root && (__x == 0 || __x->_M_color == _S_rb_tree_black))
      if (__x == __x_parent->_M_left) {
        _Rb_tree_node_base* __w = __x_parent->_M_right;
        if (__w->_M_color == _S_rb_tree_red) {
          __w->_M_color = _S_rb_tree_black;
          __x_parent->_M_color = _S_rb_tree_red;
          _Rotate_left(__x_parent, __root);
          __w = __x_parent->_M_right;
        }
        if ((__w->_M_left == 0 || 
             __w->_M_left->_M_color == _S_rb_tree_black) && (__w->_M_right == 0 || 
             __w->_M_right->_M_color == _S_rb_tree_black)) {
          __w->_M_color = _S_rb_tree_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__w->_M_right == 0 || 
              __w->_M_right->_M_color == _S_rb_tree_black) {
            if (__w->_M_left) __w->_M_left->_M_color = _S_rb_tree_black;
            __w->_M_color = _S_rb_tree_red;
            _Rotate_right(__w, __root);
            __w = __x_parent->_M_right;
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_rb_tree_black;
          if (__w->_M_right) __w->_M_right->_M_color = _S_rb_tree_black;
          _Rotate_left(__x_parent, __root);
          break;
        }
      } else {                  // same as above, with _M_right <-> _M_left.
        _Rb_tree_node_base* __w = __x_parent->_M_left;
        if (__w->_M_color == _S_rb_tree_red) {
          __w->_M_color = _S_rb_tree_black;
          __x_parent->_M_color = _S_rb_tree_red;
          _Rotate_right(__x_parent, __root);
          __w = __x_parent->_M_left;
        }
        if ((__w->_M_right == 0 || 
             __w->_M_right->_M_color == _S_rb_tree_black) && (__w->_M_left == 0 || 
             __w->_M_left->_M_color == _S_rb_tree_black)) {
          __w->_M_color = _S_rb_tree_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__w->_M_left == 0 || 
              __w->_M_left->_M_color == _S_rb_tree_black) {
            if (__w->_M_right) __w->_M_right->_M_color = _S_rb_tree_black;
            __w->_M_color = _S_rb_tree_red;
            _Rotate_left(__w, __root);
            __w = __x_parent->_M_left;
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_rb_tree_black;
          if (__w->_M_left) __w->_M_left->_M_color = _S_rb_tree_black;
          _Rotate_right(__x_parent, __root);
          break;
        }
      }
    if (__x) __x->_M_color = _S_rb_tree_black;
  }
  return __y;
}

}
using NThingManager::CEntityRuntimePersistence;
typedef CEntityRuntimePersistence::CEntityScriptInfo ScriptInfo;typedef CEntityRuntimePersistence::CPersistedDeadEntity DeadInfo;typedef std::pair<const PersistenceIdentifier,DeadInfo>DeadValue;typedef std::_Rb_tree_node<DeadValue>DeadNode;
static CEntityRuntimePersistence*expectedOwner;static const CCharString*expectedSection;static ScriptInfo*expectedScripts[8];static unsigned expectedCount,calls;static bool bad;
void CEntityRuntimePersistence::ClearPersistenceDataForSectionInEntityScript(const CCharString&section,ScriptInfo&info){if(this!=expectedOwner||&section!=expectedSection||calls>=expectedCount||&info!=expectedScripts[calls]||!info.Valid)bad=true;++calls;}
static void vectorHeader(void*p,void*first,void*last){void*words[3]={first,last,last};memcpy(p,words,sizeof words);}
static void node(DeadNode*n,PersistenceIdentifier key,unsigned long crc){n->_M_color=true;n->_M_parent=n->_M_left=n->_M_right=0;DeadInfo v;v.UID=key;v.GamePersistent=false;v.SectionNameCRC=crc;::new(static_cast<void*>(&n->_M_value_field))DeadValue(key,v);}
int main(){union Owner{PersistenceIdentifier align;unsigned char bytes[40];}o;memset(&o,0,sizeof o);expectedOwner=reinterpret_cast<CEntityRuntimePersistence*>(o.bytes);union Scripts{PersistenceIdentifier align;unsigned char bytes[24*5];}scriptStorage;memset(&scriptStorage,0,sizeof scriptStorage);ScriptInfo*scripts=reinterpret_cast<ScriptInfo*>(scriptStorage.bytes);for(unsigned i=0;i<5;++i)scripts[i].Valid=(i!=1&&i!=3);vectorHeader(&expectedOwner->LevelPersistentEntityScripts,scripts,scripts+3);vectorHeader(&expectedOwner->GamePersistentEntityScripts,scripts+3,scripts+5);expectedScripts[0]=scripts;expectedScripts[1]=scripts+2;expectedScripts[2]=scripts+4;expectedCount=3;
 char text[]="123456789";CCharStringData data;memset(&data,0,sizeof data);data.Data.PData=text;data.Data.StringLength=9;union StringStorage{void*align;unsigned char bytes[4];}str;CCharString*section=reinterpret_cast<CCharString*>(str.bytes);section->PStringData=&data;expectedSection=section;if(section->GetCRC()!=0x2dfd2d88UL)return 1;
 DeadNode head;head._M_color=false;DeadNode*left=static_cast<DeadNode*>(malloc(sizeof(DeadNode)));DeadNode*middle=static_cast<DeadNode*>(malloc(sizeof(DeadNode)));DeadNode*right=static_cast<DeadNode*>(malloc(sizeof(DeadNode)));node(left,7,0x2dfd2d88UL);node(middle,8,0x31415926);node(right,9,0x2dfd2d88UL);middle->_M_parent=&head;middle->_M_left=left;middle->_M_right=right;left->_M_parent=middle;right->_M_parent=middle;head._M_parent=middle;head._M_left=left;head._M_right=right;DeadNode*h=&head;memcpy(&expectedOwner->PersistedDeadEntities,&h,sizeof h);unsigned count=3;memcpy(reinterpret_cast<unsigned char*>(&expectedOwner->PersistedDeadEntities)+4,&count,4);
 expectedOwner->ClearPersistenceDataForSection(*section);memcpy(&count,reinterpret_cast<unsigned char*>(&expectedOwner->PersistedDeadEntities)+4,4);if(bad||calls!=3||count!=1||head._M_parent!=middle||head._M_left!=middle||head._M_right!=middle||middle->_M_value_field.second.SectionNameCRC!=0x31415926)return 2;free(middle);
 head._M_parent=0;head._M_left=head._M_right=&head;count=0;memcpy(reinterpret_cast<unsigned char*>(&expectedOwner->PersistedDeadEntities)+4,&count,4);calls=0;section->PStringData=0;expectedOwner->ClearPersistenceDataForSection(*section);if(bad||calls!=3||section->GetCRC()!=0)return 3;
 vectorHeader(&expectedOwner->LevelPersistentEntityScripts,scripts,scripts);vectorHeader(&expectedOwner->GamePersistentEntityScripts,scripts,scripts);calls=expectedCount=0;expectedOwner->ClearPersistenceDataForSection(*section);if(bad||calls)return 4;puts("00520390_TEST PASS");return 0;}