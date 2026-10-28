// std::list<CCountedPointer<const CFileInstaller::CFileInfo>>::push_back(const value_type& value) -- append a shared counted-pointer node.
// Leaf: ?push_back@?$list@V?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@V?$allocator@V?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@@std@@@std@@QAEXABV?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@@Z
// Identity: proven; own CFileInstaller::AddFilteredFiles009c47a0 and countedtemporary009c6a70; exact PDB const CFileInfo pointer/list. Actual original STLport _List_base/rebound-node proxy, _List_iterator/usercopy and protected _M_insert(iterator,constT&) with original placement copy/link order. True countedpointer two words and original CCPPointerInfo12 RefCount/fastcallDeleteFunc/Data; no nonexistent CCPPointerInfo::Release or truncated count-only object. Outer classes naming scopes only, no layout/sizeof claims. Own storeddeleter00cdee00 consumesECX, matching actual fastcall pointer type.
// Layout: genuine original list/node storage and non-POD counted-pointer special members; node16/sentinel4 native.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void*p){return p;}
#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);
#else
#include <stdlib.h>
#endif
class CCPPointerInfo{public:unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void* Data;};template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}CCountedPointer(const CCountedPointer&p):Data(p.Data),Info(p.Info){if(Info)++Info->RefCount;}~CCountedPointer();CCountedPointer&operator=(const CCountedPointer&);};
class CFileInstaller{public:class CFileInfo;};namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class A,class B>struct pair{A first;B second;};template<class T>struct less{};template<class T>struct _Select1st{};template<class T>struct _Identity{};template<class T>struct _Nonconst_traits{typedef T& reference;typedef T* pointer;};template<class T>struct _Const_traits{typedef const T& reference;typedef const T* pointer;};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;void _M_hook(_List_node_base*position){_List_node_base* previous=position->_M_prev;_M_next=position;_M_prev=previous;previous->_M_next=this;position->_M_prev=this;}};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T,class Traits>struct _List_iterator{_List_node_base* _M_node;_List_iterator(){}_List_iterator(_List_node_base*p):_M_node(p){}_List_iterator(const _List_iterator&x):_M_node(x._M_node){}};
template<class T,class A>class _List_base{protected:typedef _List_node<T> _Node;typedef allocator<_Node> _NodeAlloc;_STLP_alloc_proxy<_Node*,_Node,_NodeAlloc> _M_node;};template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{public:typedef _List_iterator<T,_Nonconst_traits<T> > iterator;iterator end(){return iterator(this->_M_node._M_data);}void push_back(const T&value){_M_insert(end(),value);}protected:void _M_insert(iterator position,const T&value){_List_node<T>* node=(_List_node<T>*)malloc(sizeof(_List_node<T>));new((void*)&node->_M_data)T(value);node->_M_hook(position._M_node);}};
}
typedef CCountedPointer<const CFileInstaller::CFileInfo> Element;template void std::list<Element>::push_back(const Element&);