// CScriptedMapBrush::GetThingsToMove(std::list<UID>& out) const -- append sorted brush thing IDs to the output list.
// Leaf: ?GetThingsToMove@CScriptedMapBrush@@QBEXAAV?$list@_KV?$allocator@_K@std@@@std@@@Z
// Identity: proven; own CMap::ScriptedMapBrushMoveThings00820840 caller and both PDB const list<unsigned __int64>& API; ThingsToMove actual retail sorted tree at0x20.
// Layout: unchanged shared CScriptedMapBrush header reused; typed read-only tree view over named opaque containers. ThingsToMove map iterator key at node+16 equals the set UID value view; mapped position/orientation unused. Actual UID unsigned __int64 under VC; portable tests use opaque two uint storage. STLport tree increment and list insertion are genuine C++.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
#ifdef _MSC_VER
typedef unsigned __int64 UID;
#else
struct UID{unsigned int Low,High;};
#endif
#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);
#else
#include <stdlib.h>
#endif

namespace std {
template<class A,class B> struct pair { A first; B second; pair(const A& a,const B& b):first(a),second(b) {} pair(const pair& p):first(p.first),second(p.second) {} };
class _List_node_base { public: _List_node_base* _M_next; _List_node_base* _M_prev; };
template<class T> class _List_node:public _List_node_base { public: T _M_data; };
template<class T> class allocator {};
template<class T,class Alloc=allocator<T> > class list { public: _List_node_base* _M_node; void push_back(const T& value) { insert(_M_node,value); } void insert(_List_node_base* position,const T& value) { _List_node<T>* node=(_List_node<T>*)malloc(sizeof(_List_node<T>)); new(&node->_M_data) T(value); _List_node_base* previous=position->_M_prev; node->_M_next=position; node->_M_prev=previous; previous->_M_next=node; position->_M_prev=node; } };
}
namespace std{class _Rb_tree_node_base{public:bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};template<class Dummy>class _Rb_global{public:static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* _M_node){if(_M_node->_M_right){_M_node=_M_node->_M_right;while(_M_node->_M_left)_M_node=_M_node->_M_left;}else{_Rb_tree_node_base* y=_M_node->_M_parent;while(_M_node==y->_M_right){_M_node=y;y=y->_M_parent;}if(_M_node->_M_right!=y)_M_node=y;}return _M_node;}};typedef _Rb_global<bool> _Rb_global_inst;template<class T>class _Rb_tree_node:public _Rb_tree_node_base{public:T _M_value_field;};template<class T>class _Rb_tree_iterator{public:_Rb_tree_node_base* _M_node;const T& operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}bool operator!=(const _Rb_tree_iterator& b)const{return _M_node!=b._M_node;}_Rb_tree_iterator& operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}};template<class T>class _Rb_tree_view{public:_Rb_tree_node_base* _M_header;unsigned int _M_node_count;void* _unused_allocator;_Rb_tree_iterator<T> begin()const{_Rb_tree_iterator<T> p;p._M_node=_M_header->_M_left;return p;}_Rb_tree_iterator<T> end()const{_Rb_tree_iterator<T> p;p._M_node=_M_header;return p;}};}
#define CScriptedMapBrush CScriptedMapBrush_Fields
#include "engine/CScriptedMapBrush.h"
#undef CScriptedMapBrush
class CScriptedMapBrush:public CScriptedMapBrush_Fields{public:void GetThingsToMove(std::list<UID>&)const;};
void CScriptedMapBrush::GetThingsToMove(std::list<UID>& out)const{const std::_Rb_tree_view<UID>& things=*(const std::_Rb_tree_view<UID>*)&ThingsToMove;for(std::_Rb_tree_iterator<UID> p=things.begin();p!=things.end();++p)out.push_back(*p);}
