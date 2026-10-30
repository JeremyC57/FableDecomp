// CGameScriptInterface::EntitySetAsDrunk(CScriptThing& thing,bool value) const -- Forward the creature request under retail lifetime and type checks.
// Leaf: ?EntitySetAsDrunk@CGameScriptInterface@@UBEXAAVCScriptThing@@_N@Z
// Identity: original PDB virtual const signature; direct mode/generator calls independently named.
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
class CThing {public:unsigned char Prefix[0x20];unsigned int UsingComponents[9];CVectorMap<ETCInterfaceType,CTCBase*> Components;unsigned char Pad54[0x18];unsigned int ThingTypeFlags;unsigned char Pad70[0x21];unsigned char Dead:1;unsigned char OtherFlags:7;template<class T>__forceinline bool GetTC(ETCInterfaceType kind,T** out){if(!HasTC(kind))return false;*out=(T*)Components.Get(kind);return true;} __forceinline bool HasTC(ETCInterfaceType kind)const{return (UsingComponents[(unsigned int)kind>>5]&(1u<<((unsigned int)kind&31)))!=0;}};
// Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
class CTCEnemy:public CTCBase {public:bool FriendsWithEverythingFlag,EnableFollowersEnemyProxy;};
// Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Layout: CThing ThingTypeFlags+0x6c, Dead bit0+0x91 from retail; intermediate fields are not used.
// Layout: no active mode-manager header; nested mode is polymorphic with GetType slot1, no other fields accessed.
class CTCCreatureModeManager:public CTCBase {public:class CModeBase{public:virtual ~CModeBase();virtual unsigned long GetType()const=0;};CModeBase& GetCurrentMode()const;bool AddMode(unsigned long);void RemoveMode(unsigned long);};
#define CTCCreatureGenerator CTCCreatureGenerator_HeaderLayout
#include "engine/CTCCreatureGenerator.h"
#undef CTCCreatureGenerator
class CTCCreatureGenerator:public CTCCreatureGenerator_HeaderLayout {public:bool IsDepleted()const;long GetNumGeneratedCreatures()const;bool AreAllCreaturesAlive()const;};
__forceinline bool IsThingAlive(const CScriptThing& script){CThing* p=script.GetPThing();return p && !p->Dead;}
// Layout: shared script header retained as evidence; genuine virtual declaration, owner fields untouched.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetAsDrunk(CScriptThing& thing,bool value)const{if(thing.IsAlive() && (thing.GetPThing()->ThingTypeFlags&8)){CTCCreatureModeManager* component;thing.GetPThing()->GetTC((ETCInterfaceType)49,&component);if(value){if(component->GetCurrentMode().GetType()!=44)component->AddMode(44);}else component->RemoveMode(44);}}