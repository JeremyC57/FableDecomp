// CGameScriptInterface::EntitySetAsScared(const CScriptThing& thing,bool value) const -- Forward the creature request under retail lifetime and type checks.
// Leaf: ?EntitySetAsScared@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: original PDB virtual const signature; direct mode/generator calls independently named.
#include "engine/CTCBase.h"
#include "engine/CTCTargeted.h"
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
#include "engine/CCountedPointer.h"
enum ETCInterfaceType {TC_TARGETED=9,TC_ENEMY=73};
#include "engine/CArray.h"
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
void CGameScriptInterface::EntitySetAsScared(const CScriptThing& thing,bool value)const{if(thing.IsAlive() && (thing.GetPThing()->ThingTypeFlags&8)){CTCCreatureModeManager* component;thing.GetPThing()->GetTC((ETCInterfaceType)49,&component);if(value){if(component->GetCurrentMode().GetType()!=15)component->AddMode(15);}else component->RemoveMode(15);}}