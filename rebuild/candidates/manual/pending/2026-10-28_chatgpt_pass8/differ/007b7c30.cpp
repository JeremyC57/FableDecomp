// CTCOverheadDisplay::AddDamageChange(float value) -- append a timed display change when its definition flag is enabled.
// Leaf: ?AddDamageChange@CTCOverheadDisplay@@QAEXM@Z
// Identity: proven; PDB own AddDamageChange(float), own PDef+24->DisplayDamage bool and DamageValues list+12; Renown caller CTCHeroStats::AddRenown0x57D977.
// Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
// Layout: shared COverheadDisplayDef data header reused unchanged. No component header; real CTCBase12, retail4-byte list sentinel pointers and PDef+24 agree with own OnCreate. Actual STLport list node insertion and pair construction are inline C++.
#include "engine/COverheadDisplayDef.h"
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
extern float g_OverheadValueExpiryFrames;
namespace std {
template<class A,class B> struct pair { A first; B second; pair(const A& a,const B& b):first(a),second(b) {} pair(const pair& p):first(p.first),second(p.second) {} };
class _List_node_base { public: _List_node_base* _M_next; _List_node_base* _M_prev; };
template<class T> class _List_node:public _List_node_base { public: T _M_data; };
template<class T> class allocator {};
template<class T,class Alloc=allocator<T> > class list { public: _List_node_base* _M_node; void push_back(const T& value) { insert(_M_node,value); } void insert(_List_node_base* position,const T& value) { _List_node<T>* node=(_List_node<T>*)malloc(sizeof(_List_node<T>)); new(&node->_M_data) T(value); _List_node_base* previous=position->_M_prev; node->_M_next=position; node->_M_prev=previous; previous->_M_next=node; position->_M_prev=node; } };
}
template<class T> class CDefPointer { public: T* Object; T* operator->() const { return Object; } };
class CTCBase { public: virtual ~CTCBase() {} void* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
class CTCOverheadDisplay:public CTCBase { public: std::list<std::pair<long,float> > DamageValues,RepairValues,RenownValues; CDefPointer<const COverheadDisplayDef> PDef; void AddDamageChange(float value); };
void CTCOverheadDisplay::AddDamageChange(float value) { if(!PDef->DisplayDamage) return; DamageValues.push_back(std::pair<long,float>((long)g_OverheadValueExpiryFrames,value)); }
