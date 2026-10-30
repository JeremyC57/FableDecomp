// std::list::push_back(const value_type& value) -- allocate and append a shared counted-pointer node.
// Grade: RELOCATION_MATCH -- shared engine/CCountedPointer.h (the parked local CCountedPointer/CCPPointerInfo removed) + the landed STLport list model of 0x007B7C80 (_List_base, _M_insert, _List_node_base::_M_hook); graded over the true 59-byte extent (BOUNDS_FIX.tsv). The parked minimal list shrink-wrapped the edi push
// Leaf: ?push_back@?$list@V?$CCountedPointer@VCStrikeEvent@CCombatActionBase@@@@V?$allocator@V?$CCountedPointer@VCStrikeEvent@CCombatActionBase@@@@@std@@@std@@QAEXABV?$CCountedPointer@VCStrikeEvent@CCombatActionBase@@@@@Z
// Identity: proven; SetupCombatStrikeEvents00859A70 calls on StrikeEvents+0xBC, PDB list<CCountedPointer<CStrikeEvent>>.
// Layout: actual outer classes provide nested naming scopes only; generated CCombatActionBase/CFileInstaller headers lack these nested definitions. Two-word CCountedPointer holds Data+CCPPointerInfo, Count at info+0; retail list sentinel4/node16. Genuine inline allocator/placement copy/link operations.
#include "engine/CCountedPointer.h"
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
extern "C" void* __cdecl malloc(unsigned int);
#else
#include <stdlib.h>
#endif
class CCombatActionBase { public: class CStrikeEvent; };
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class A,class B>struct pair{A first;B second;pair():first(),second(){}pair(const A&a,const B&b):first(a),second(b){}pair(const pair&x):first(x.first),second(x.second){}};template<class T>struct less{};template<class T>struct _Select1st{};template<class T>struct _Identity{};template<class T>struct _Nonconst_traits{typedef T& reference;typedef T* pointer;};template<class T>struct _Const_traits{typedef const T& reference;typedef const T* pointer;};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;void _M_hook(_List_node_base*position){_List_node_base* previous=position->_M_prev;_M_next=position;_M_prev=previous;previous->_M_next=this;position->_M_prev=this;}};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T,class Traits>struct _List_iterator{_List_node_base* _M_node;_List_iterator(){}_List_iterator(_List_node_base*p):_M_node(p){}_List_iterator(const _List_iterator&x):_M_node(x._M_node){}};
template<class T,class A>class _List_base{protected:typedef _List_node<T> _Node;typedef allocator<_Node> _NodeAlloc;_STLP_alloc_proxy<_Node*,_Node,_NodeAlloc> _M_node;};template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{public:typedef _List_iterator<T,_Nonconst_traits<T> > iterator;iterator end(){return iterator(this->_M_node._M_data);}void push_back(const T&value){_M_insert(end(),value);}protected:void _M_insert(iterator position,const T&value){_List_node<T>* node=(_List_node<T>*)malloc(sizeof(_List_node<T>));new((void*)&node->_M_data)T(value);node->_M_hook(position._M_node);}};
}
typedef CCountedPointer<CCombatActionBase::CStrikeEvent> Element;
template void std::list<Element>::push_back(const Element&);