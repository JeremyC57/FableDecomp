// CAIStateGroup_TavernServer::FreeServingPoint() -- Return the reserved action point and clear its index.
// Leaf: ?FreeServingPoint@CAIStateGroup_TavernServer@@QAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Original PDB owner method and tracked/index fields; TC184 CTCTavernTable, helper007b9090 original FreeServingPoint(long). Own header offsets distinguish server from keeper; old FreeBarPoint labels propagated.
#include "engine/CTCBase.h"
#define CAIStateGroup_TavernServer CAIStateGroup_TavernServer_HeaderLayout
#include "engine/CAIStateGroup_TavernServer.h"
#undef CAIStateGroup_TavernServer
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include "engine/CArray.h"
enum ETCInterfaceType{TC_TABLE=184,TC_BAR=185};
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& entry,const K& key)const{return entry.first<key;}};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >class CVectorMap{public:typedef std::pair<K,V> value_type;typedef value_type* iterator;CArray<value_type> Data;CompareType Compare;bool Dirty;iterator LowerBound(const K& key);iterator end(){return Data.end();}__forceinline V& operator[](const K& key){K keyCopy=key;iterator p=LowerBound(keyCopy);if(p==end() || key<p->first)p=end();return p->second;}};
class CThing{public:unsigned char _unmodeled00[0x20];unsigned int Interfaces[9];CVectorMap<ETCInterfaceType,CTCBase*> Components;template<class T>__forceinline bool GetTC(T** out){if(Interfaces[T::InterfaceType/32]&(1U<<(T::InterfaceType%32))){*out=(T*)Components[(ETCInterfaceType)T::InterfaceType];return true;}return false;}};class CThingCreatureBase:public CThing{};
class CBaseObject;class CBaseObjectPointer;class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer(){}CBaseObjectPointer* PData;CBaseObject* GetPItem()const;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}};
class CThingObject:public CThing{};
class CTCTavernTable:public CTCBase{public:enum{InterfaceType=184};void FreeServingPoint(long);};
// Layout: active owner header retained. Original tracked PTable viewed as CIntelligentPointer<CThingObject>; component must exist for a reserved index. Missing-component output remains unset as retail.
class CAIStateGroup_TavernServer:public CAIStateGroup_TavernServer_HeaderLayout{public:void FreeServingPoint();};
void CAIStateGroup_TavernServer::FreeServingPoint(){if(ServingActionPointIndex!=-1){CTCTavernTable* component;reinterpret_cast<CIntelligentPointer<CThingObject>*>(PTable)->GetPItem()->GetTC(&component);component->FreeServingPoint(ServingActionPointIndex);ServingActionPointIndex=-1;}}