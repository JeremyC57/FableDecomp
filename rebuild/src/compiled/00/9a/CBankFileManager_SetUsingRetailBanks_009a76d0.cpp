// CBankFileManager::SetUsingRetailBanks() -- Store the requested input state.
// Leaf: ?SetUsingRetailBanks@CBankFileManager@@QAEX_N@Z
// Identity: Own sole direct caller00402875 uses global013ca79c, actual CBankFileManager identity/RTTI and PDB SetUsingRetailBanks(bool), UsingRetailBanks+20 after map12/list4. Prior physics-navigator label rejected.
// Layout: original class keys, actual bases and typed STLport containers/counting handles. CManager header renamed to restore actual inheritance/types; enum value spellings unspecified. Bank owner declaration is a prefix through UsingRetailBanks only. Unused virtual slots explicitly descriptive.
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
class CBaseClass{public:virtual ~CBaseClass(){}};class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable(){}~CBaseClassNonCopyable(){}private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);};
class CCharStringData;class CCharString{public:CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
namespace std{
template<class T>class allocator{};template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T,class A>class _List_base{protected:typedef _List_node<T> _Node;typedef allocator<_Node> _NodeAlloc;_STLP_alloc_proxy<_Node*,_Node,_NodeAlloc> _M_node;};
template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{};
template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:T* begin(){return this->_M_start;}T* end(){return this->_M_finish;}};
template<class Iter,class Compare>void __fastcall sort(Iter,Iter,Compare);
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}};
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;Cmp Compare;bool Dirty;__forceinline void Sort(){if(Dirty){std::sort(Data.begin(),Data.end(),Compare);Dirty=false;}}};

namespace std{
template<class T>struct less{};template<class T>struct _Select1st{};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class T,class A>class _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > > _M_header;};
template<class K,class V,class S,class C,class A>class _Rb_tree:protected _Rb_tree_base<V,A>{public:unsigned int _M_node_count;C _M_key_compare;};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{public:_Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,C,A> _M_t;};
}
namespace NBankFileManager{class CRetailBank;}class CBankFileManager:public CBaseClass{public:class CBankInfo;std::map<CCharString,CBankInfo> Banks;std::list<CCountedPointer<NBankFileManager::CRetailBank> > RetailBanks;bool UsingRetailBanks;void SetUsingRetailBanks(bool);};void CBankFileManager::SetUsingRetailBanks(bool value){UsingRetailBanks=value;}
#ifdef _MSC_VER
typedef char ExactFlagOffset[offsetof(CBankFileManager,UsingRetailBanks)==20?1:-1];
#endif