#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
// CGameTimeManager original typed clock family.
// Leaf: ?IsTimeBetween@CGameTimeManager@@ABE_NMMM@Z
// Flags: 
#include <stddef.h>
#include "rebuild_abi.h"
class CWorld;class CMainGameComponent;class CCharStringData;class CCharString {public:CCharStringData*PStringData;~CCharString();};enum EGenericVarType {};class CGenericVar {public:virtual~CGenericVar();EGenericVarType Type;CCharString ValString;long ValSLONG;unsigned long ValULONG;bool ValBool;float ValFloat;};
namespace std {
template<class T>class allocator {};template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};template<class T>struct less:public binary_function<T,T,bool>{};template<class T>struct _Identity:public unary_function<T,T>{};
struct _Rb_tree_node_base {bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};template<class T>struct _Rb_tree_node:public _Rb_tree_node_base {T _M_value_field;};
template<class T,class A>struct _Rb_tree_base {protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > >_M_header;};template<class K,class V,class S,class C,class A>class _Rb_tree:public _Rb_tree_base<V,A> {protected:size_t _M_node_count;C _M_key_compare;};template<class K,class C=less<K>,class A=allocator<K> >class set {private:_Rb_tree<K,K,_Identity<K>,C,A>_M_t;};
template<class T,class A>class _Vector_base {protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:typedef size_t size_type;typedef const T&const_reference;const_reference operator[](size_type n)const{return *(this->_M_start+n);}};
}
class CGameTimeManager {public:CWorld*World;CMainGameComponent*GameComponent;float Time,PrevTime,TimeStep;long DayCount;bool UsingVillageTime;float VillageTime;bool VillageDay,FastForwardingToTime;float TimeFastForwardingTo,SecondsLeftToFastForwardTime;std::set<long>StopTimeIndexes;long NextStopTimeIndex;float GetTimeFrom24HourClock(long clock)const{float minutes=(float)(clock%100)*(1.0f/60.0f);return minutes*(1.0f/24.0f)+(float)(clock/100)*(1.0f/24.0f);}
bool IsTimeBetween(long,long)const;void SetTimeOfDay(float);
private:bool IsTimeBetween(float,float,float)const;static void __fastcall ConsoleSetTimeOfDay(CGameTimeManager&,const std::vector<CGenericVar>&);};
FABLE_STATIC_ASSERT(sizeof(CGameTimeManager)==60);FABLE_STATIC_ASSERT(sizeof(CGenericVar)==28);FABLE_STATIC_ASSERT(sizeof(std::vector<CGenericVar>)==12);
bool CGameTimeManager::IsTimeBetween(float time,float start,float end)const{if(start<end){if(time>=start&&time<end)return true;}else if(time>=start||time<end)return true;return false;}

// Fixture-only definition of the existing public overload as an access bridge.
// It calls the exact private candidate body with quarter-day literal inputs.
// This harness method is not a reconstruction claim for the public overload.
bool CGameTimeManager::IsTimeBetween(long start,long end)const{return IsTimeBetween(Time,(float)start/4.0f,(float)end/4.0f);}
struct Case{float time;long begin,end;bool expected;};static const Case cases[]={{0.5f,1,3,true},{0.25f,1,3,true},{0.75f,1,3,false},{0.0f,3,1,true},{0.875f,3,1,true},{0.5f,3,1,false},{0.25f,1,1,true}};
int main(){union Store{double align;unsigned char bytes[60];}storage;memset(&storage,0xa5,sizeof storage);CGameTimeManager*p=reinterpret_cast<CGameTimeManager*>(storage.bytes);for(unsigned i=0;i<sizeof cases/sizeof cases[0];++i){p->Time=cases[i].time;if(p->IsTimeBetween(cases[i].begin,cases[i].end)!=cases[i].expected)return i+1;}unsigned long nanbits=0x7fc00000;memcpy(&p->Time,&nanbits,4);if(p->IsTimeBetween(1L,3L)||p->IsTimeBetween(3L,1L))return 20;puts("006bb5b0_TEST PASS");return 0;}