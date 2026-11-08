// CGameScriptInterface::EntitySetThingAsEnemyOfThing(const CScriptThing& thing,const CScriptThing& other) -- Forward to the owned component after checking script lifetime.
// Leaf: ?EntitySetThingAsEnemyOfThing@CGameScriptInterface@@UBEXABVCScriptThing@@0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: PDB exact virtual const signature; own interface kind73 and external callee0076d1b0 CTCEnemy::AddEnemyThing.
#include "fable_compiler.h"
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
FABLE_FORCEINLINE V Get(K key){iterator it=LowerBound(key);iterator last=Data.end();return (it==last || key<it->first ? last:it)->second;}
};
// Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
class CThing {public:unsigned char Prefix[0x20];unsigned int UsingComponents[9];CVectorMap<ETCInterfaceType,CTCBase*> Components;template<class T>FABLE_FORCEINLINE bool GetTC(ETCInterfaceType kind,T** out){if(!HasTC(kind))return false;*out=(T*)Components.Get(kind);return true;} FABLE_FORCEINLINE bool HasTC(ETCInterfaceType kind)const{return (UsingComponents[(unsigned int)kind>>5]&(1u<<((unsigned int)kind&31)))!=0;}};
// Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
class CTCEnemy:public CTCBase {public:bool FriendsWithEverythingFlag,EnableFollowersEnemyProxy;void AddEnemyThing(const CThing*);void RemoveEnemyThing(const CThing*);void RemoveAllyThing(const CThing&);};
// Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
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
void CGameScriptInterface::EntitySetThingAsEnemyOfThing(const CScriptThing& thing,const CScriptThing& other)const{if(thing.IsAlive() && other.IsAlive()){CThing* p=thing.GetPThing();CTCEnemy* component;if(p->GetTC((ETCInterfaceType)73,&component))component->AddEnemyThing(other.GetPThing());}}