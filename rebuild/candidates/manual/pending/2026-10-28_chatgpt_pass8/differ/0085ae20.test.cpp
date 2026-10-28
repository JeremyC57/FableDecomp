#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
class CCombatActionBase{public:class CStrikeEvent;};class CCPPointerInfo{public:long Count;void Release();};template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}CCountedPointer(const CCountedPointer& p):Data(p.Data),Info(p.Info){if(Info)++Info->Count;}~CCountedPointer(){if(Info)Info->Release();}};
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
typedef CCountedPointer<CCombatActionBase::CStrikeEvent> Element;
template void std::list<Element>::push_back(const Element&);
void CCPPointerInfo::Release(){--Count;}
int main(){CCPPointerInfo info;info.Count=1;Element value;value.Data=0;value.Info=&info;std::list<Element> list;std::_List_node_base root;root._M_next=root._M_prev=&root;list._M_node=&root;list.push_back(value);typedef std::_List_node<Element> Node;Node* first=(Node*)root._M_next;if(info.Count!=2 || first->_M_data.Info!=&info || first->_M_data.Data || first->_M_next!=&root || first->_M_prev!=&root)goto fail;value.Info=0;list.push_back(value);{Node* last=(Node*)root._M_prev;if(first->_M_next!=last || last->_M_prev!=first || last->_M_next!=&root || last->_M_data.Info || info.Count!=2)goto fail;first->_M_data.~Element();last->_M_data.~Element();free(first);free(last);}if(info.Count!=1)goto fail;puts("0085ae20_TEST PASS");return 0;fail:puts("0085ae20_TEST FAIL");return 1;}
