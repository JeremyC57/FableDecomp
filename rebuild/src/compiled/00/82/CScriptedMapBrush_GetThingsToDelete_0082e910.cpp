// CScriptedMapBrush::GetThingsToDelete(std::list<UID>& out) const -- append sorted original brush thing IDs.
// Leaf: ?GetThingsToDelete@CScriptedMapBrush@@QBEXAAV?$list@_KV?$allocator@_K@std@@@std@@@Z
// Identity: proven; Own CMap caller008209c0 and both original PDB const list<unsigned__int64>& API. OwnRTTI CScriptedMapBrush:CBaseObject:CBaseClass and original full namedfields84; shared aliases preserve unchanged flattened layout evidence. Full original STLport list/base/rebound-nodeproxy and set/tree/base/rebound-nodeproxy, boolcolor, _Rb_global<bool>class and _Rb_tree_iterator->_Rb_tree_base_iterator with traits/usercopy constructors. No artificial read-only tree view, opaque allocator word, directlist storage or flattened engine base. Original setkeys UID64; unused original nested cell/mapvalue types forward-only, no invented fields or element-sizeclaim.
// Layout: correct original complete engine/container hierarchies and original class keys, fixed-size data fields.
#define CScriptedMapBrush CScriptedMapBrush_HeaderEvidence
#include "engine/CScriptedMapBrush.h"
#undef CScriptedMapBrush
#define CBaseObject CBaseObject_HeaderEvidence
#include "engine/CBaseObject.h"
#undef CBaseObject
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
struct UID{unsigned int Low,High;};
#else
typedef unsigned int PlacementSize;typedef unsigned __int64 UID;
#endif
inline void* operator new(PlacementSize,void*p){return p;}
#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);
#else
#include <stdlib.h>
#endif
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class A,class B>struct pair{A first;B second;};template<class T>struct less{};template<class T>struct _Select1st{};template<class T>struct _Identity{};template<class T>struct _Nonconst_traits{typedef T& reference;typedef T* pointer;};template<class T>struct _Const_traits{typedef const T& reference;typedef const T* pointer;};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;void _M_hook(_List_node_base*position){_List_node_base* previous=position->_M_prev;_M_next=position;_M_prev=previous;previous->_M_next=this;position->_M_prev=this;}};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T,class Traits>struct _List_iterator{_List_node_base* _M_node;_List_iterator(){}_List_iterator(_List_node_base*p):_M_node(p){}_List_iterator(const _List_iterator&x):_M_node(x._M_node){}};
template<class T,class A>class _List_base{protected:typedef _List_node<T> _Node;typedef allocator<_Node> _NodeAlloc;_STLP_alloc_proxy<_Node*,_Node,_NodeAlloc> _M_node;};template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{public:typedef _List_iterator<T,_Nonconst_traits<T> > iterator;iterator end(){return iterator(this->_M_node._M_data);}void push_back(const T&value){_M_insert(end(),value);}protected:void _M_insert(iterator position,const T&value){_List_node<T>* node=(_List_node<T>*)malloc(sizeof(_List_node<T>));new((void*)&node->_M_data)T(value);node->_M_hook(position._M_node);}};
template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};template<class Dummy>class _Rb_global{public:static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base*node){if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}else{_Rb_tree_node_base*y=node->_M_parent;while(node==y->_M_right){node=y;y=y->_M_parent;}if(node->_M_right!=y)node=y;}return node;}};
struct _Rb_tree_base_iterator{_Rb_tree_node_base* _M_node;};template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;typedef typename Traits::reference reference;typedef _Rb_tree_node<V>* _Link_type;_Rb_tree_iterator(){}_Rb_tree_iterator(_Link_type n){_M_node=n;}_Rb_tree_iterator(const iterator&i){_M_node=i._M_node;}reference operator*()const{return ((_Rb_tree_node<V>*)_M_node)->_M_value_field;}_Rb_tree_iterator&operator++(){_M_node=_Rb_global<bool>::_M_increment(_M_node);return *this;}bool operator!=(const _Rb_tree_iterator&x)const{return _M_node!=x._M_node;}};
template<class V,class A>class _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header;};template<class K,class V,class Select,class Compare,class A>class _Rb_tree:protected _Rb_tree_base<V,A>{public:typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;unsigned int _M_node_count;Compare _M_key_compare;const_iterator begin()const{return const_iterator((_Rb_tree_node<V>*)this->_M_header._M_data->_M_left);}const_iterator end()const{return const_iterator(this->_M_header._M_data);}};
template<class K,class Compare=less<K>,class A=allocator<K> >class set{private:typedef _Rb_tree<K,K,_Identity<K>,Compare,A> tree_type;tree_type _M_t;public:typedef typename tree_type::const_iterator const_iterator;const_iterator begin()const{return _M_t.begin();}const_iterator end()const{return _M_t.end();}};template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >class map{private:_Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,Compare,A> _M_t;};}
struct CBaseObjectPointer;class CBaseClass{public:CBaseClass();virtual ~CBaseClass();};class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);~CCharString();};class C2DBoxI{public:long TLX,TLY,BRX,BRY;};class CScriptedMapBrush:public CBaseObject{public:class CScriptedMapCell;class CThingPosAndOrientation;std::vector<CScriptedMapCell> Cells;std::set<UID> ThingsToDelete;std::map<UID,CThingPosAndOrientation> ThingsToMove;std::set<UID> ThingsToAdd;C2DBoxI GameArea;CCharString BrushName;float CreationTimeSeconds;long CreationPattern;void GetThingsToDelete(std::list<UID>&)const;};
void CScriptedMapBrush::GetThingsToDelete(std::list<UID>& out)const{for(std::set<UID>::const_iterator i=ThingsToDelete.begin();i!=ThingsToDelete.end();++i)out.push_back(*i);}
#ifdef _MSC_VER
typedef char OriginalBrushSize[sizeof(CScriptedMapBrush)==84?1:-1];typedef char OriginalNodeBaseSize[sizeof(std::_Rb_tree_node_base)==16?1:-1];
#endif