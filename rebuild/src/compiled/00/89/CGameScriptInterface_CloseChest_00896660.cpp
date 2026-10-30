// CGameScriptInterface::CloseChest(const CScriptThing& thing) -- Forward to the owned component after checking script lifetime.
// Leaf: ?CloseChest@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Identity: PDB exact virtual const signature; own interface kind60 and external callee007506c0 CTCChest::Close.
#include "engine/CTCBase.h"
#include "engine/CTCTargeted.h"
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
class CCPPointerInfo;
enum ETCInterfaceType {TC_TARGETED=9,TC_ENEMY=73};
namespace std {
template<class K,class V>struct pair {K first;V second;};template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A=allocator<T> >class vector {public:typedef T* iterator;T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;T* begin(){return _M_start;}T* end(){return _M_finish;}};
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess {public:bool operator()(const std::pair<K,V>& entry,const K& key)const{return entry.first<key;}};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap {public:typedef std::pair<K,V> value_type;typedef value_type* iterator;CArray<value_type> Data;Cmp Compare;bool Dirty;
__declspec(noinline) iterator LowerBound(const K& key){iterator first=Data.begin();int count=Data.end()-first;while(count>0){int half=count/2;iterator middle=first+half;if(Compare(*middle,key)){first=middle+1;count-=half+1;}else count=half;}return first;}
__forceinline V Get(K key){iterator it=LowerBound(key);iterator last=Data.end();return (it==last || key<it->first ? last:it)->second;}
};
// Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
class CThing {public:unsigned char Prefix[0x20];unsigned int UsingComponents[9];CVectorMap<ETCInterfaceType,CTCBase*> Components;template<class T>__forceinline bool GetTC(ETCInterfaceType kind,T** out){if(!HasTC(kind))return false;*out=(T*)Components.Get(kind);return true;} __forceinline bool HasTC(ETCInterfaceType kind)const{return (UsingComponents[(unsigned int)kind>>5]&(1u<<((unsigned int)kind&31)))!=0;}};
// Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
class CTCEnemy:public CTCBase {public:bool FriendsWithEverythingFlag,EnableFollowersEnemyProxy;void AddEnemyThing(const CThing*);void RemoveEnemyThing(const CThing*);void RemoveAllyThing(const CThing&);};
// Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Layout: only external method receivers; no active chest/mode-manager headers, no derived fields read. Base offsets agree with CTCBase.
class CTCChest:public CTCBase {public:void Close();void DisplayLockedMessage()const;};
class CTCCreatureModeManager:public CTCBase {public:void ResetModes();};
// Layout: shared script header flattened; genuine virtual API below, no owner fields touched.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::CloseChest(const CScriptThing& thing)const{if(thing.IsAlive()){CThing* p=thing.GetPThing();CTCChest* component;if(p->GetTC((ETCInterfaceType)60,&component))component->Close();}}