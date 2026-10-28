#include <stdio.h>
#include <stdlib.h>
class COverheadDisplayDef { public: unsigned char _unmodeled00[37]; bool DisplayDamage,DisplayRepair,DisplayRenown; };
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
float g_OverheadValueExpiryFrames=18.75f;
int main() { CTCOverheadDisplay object; COverheadDisplayDef def; object.PDef.Object=&def; def.DisplayDamage=def.DisplayRepair=def.DisplayRenown=false; std::_List_node_base roots[3]; for(int i=0;i<3;++i) roots[i]._M_next=roots[i]._M_prev=&roots[i]; object.DamageValues._M_node=&roots[0]; object.RepairValues._M_node=&roots[1]; object.RenownValues._M_node=&roots[2];
 object.AddDamageChange(3.5f); if(object.DamageValues._M_node->_M_next!=object.DamageValues._M_node) goto fail; def.DisplayDamage=true; object.AddDamageChange(3.5f); { std::_List_node_base* root=object.DamageValues._M_node; std::_List_node<std::pair<long,float> >* first=(std::_List_node<std::pair<long,float> >*)root->_M_next; if(first==root || first->_M_next!=root || first->_M_prev!=root || root->_M_prev!=first || first->_M_data.first!=18 || first->_M_data.second!=3.5f) goto fail;
 g_OverheadValueExpiryFrames=-2.75f; object.AddDamageChange(-7.5f); std::_List_node<std::pair<long,float> >* last=(std::_List_node<std::pair<long,float> >*)root->_M_prev; if(last==first || last->_M_next!=root || last->_M_prev!=first || first->_M_next!=last || last->_M_data.first!=-2 || last->_M_data.second!=-7.5f) goto fail; free(last); free(first); }
 puts("007b7c30_TEST PASS"); return 0; fail: puts("007b7c30_TEST FAIL"); return 1; }
