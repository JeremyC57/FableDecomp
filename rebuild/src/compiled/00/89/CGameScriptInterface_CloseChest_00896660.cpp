// CGameScriptInterface::CloseChest(const CScriptThing& thing) -- Forward to the owned component after checking script lifetime.
// Leaf: ?CloseChest@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Identity: PDB exact virtual const signature; own interface kind60 and external callee007506c0 CTCChest::Close.
#define CGameScriptInterface CGameScriptInterface_HeaderLayout
#include "engine/CGameScriptInterface.h"
#undef CGameScriptInterface
#define CScriptThing CScriptThing_HeaderLayout
#include "engine/CScriptThing.h"
#undef CScriptThing
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
class CScriptThing {public:
 virtual void UnusedSlot000();
 virtual void UnusedSlot001();
 virtual void UnusedSlot002();
 virtual void UnusedSlot003();
 virtual void UnusedSlot004();
 virtual void UnusedSlot005();
 virtual void UnusedSlot006();
 virtual void UnusedSlot007();
 virtual void UnusedSlot008();
 virtual void UnusedSlot009();
 virtual void UnusedSlot010();
 virtual CThing* GetPThing()const;
 virtual void UnusedSlot012();
 virtual void UnusedSlot013();
 virtual void UnusedSlot014();
 virtual void UnusedSlot015();
 virtual void UnusedSlot016();
 virtual void UnusedSlot017();
 virtual void UnusedSlot018();
 virtual void UnusedSlot019();
 virtual void UnusedSlot020();
 virtual void UnusedSlot021();
 virtual void UnusedSlot022();
 virtual void UnusedSlot023();
 virtual void UnusedSlot024();
 virtual void UnusedSlot025();
 virtual void UnusedSlot026();
 virtual void UnusedSlot027();
 virtual void UnusedSlot028();
 virtual void UnusedSlot029();
 virtual void UnusedSlot030();
 virtual void UnusedSlot031();
 virtual void UnusedSlot032();
 virtual void UnusedSlot033();
 virtual void UnusedSlot034();
 virtual void UnusedSlot035();
 virtual void UnusedSlot036();
 virtual void UnusedSlot037();
 virtual void UnusedSlot038();
 virtual void UnusedSlot039();
 virtual void UnusedSlot040();
 virtual void UnusedSlot041();
 virtual void UnusedSlot042();
 virtual void UnusedSlot043();
 virtual void UnusedSlot044();
 virtual void UnusedSlot045();
 virtual void UnusedSlot046();
 virtual void UnusedSlot047();
 virtual void UnusedSlot048();
 virtual void UnusedSlot049();
 virtual void UnusedSlot050();
 virtual void UnusedSlot051();
 virtual void UnusedSlot052();
 virtual void UnusedSlot053();
 virtual void UnusedSlot054();
 virtual void UnusedSlot055();
 virtual void UnusedSlot056();
 virtual void UnusedSlot057();
 virtual void UnusedSlot058();
 virtual void UnusedSlot059();
 virtual void UnusedSlot060();
 virtual void UnusedSlot061();
 virtual void UnusedSlot062();
 virtual void UnusedSlot063();
 virtual void UnusedSlot064();
 virtual void UnusedSlot065();
 virtual void UnusedSlot066();
 virtual void UnusedSlot067();
 virtual void UnusedSlot068();
 virtual void UnusedSlot069();
 virtual void UnusedSlot070();
 virtual void UnusedSlot071();
 virtual void UnusedSlot072();
 virtual void UnusedSlot073();
 virtual void UnusedSlot074();
 virtual bool IsAlive()const;
 CScriptThing* PImp_Data;CCPPointerInfo* PImp_Info;
};
// Layout: only external method receivers; no active chest/mode-manager headers, no derived fields read. Base offsets agree with CTCBase.
class CTCChest:public CTCBase {public:void Close();void DisplayLockedMessage()const;};
class CTCCreatureModeManager:public CTCBase {public:void ResetModes();};
// Layout: shared script header flattened; genuine virtual API below, no owner fields touched.
class CGameScriptInterface {public:virtual void CloseChest(const CScriptThing& thing)const;unsigned char OwnerFields[0x4c];};
void CGameScriptInterface::CloseChest(const CScriptThing& thing)const{if(thing.IsAlive()){CThing* p=thing.GetPThing();CTCChest* component;if(p->GetTC((ETCInterfaceType)60,&component))component->Close();}}