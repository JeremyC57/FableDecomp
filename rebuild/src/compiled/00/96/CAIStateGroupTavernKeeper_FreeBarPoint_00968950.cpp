// CAIStateGroup_TavernKeeper::FreeBarPoint() -- Return the reserved action point and clear its index.
// Leaf: ?FreeBarPoint@CAIStateGroup_TavernKeeper@@QAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Original PDB owner method and tracked/index fields; TC185 CTCTavernBar, helper007ba4f0 original FreeSlot(long). Own header offsets distinguish server from keeper; old FreeBarPoint labels propagated.
#include "engine/CTCBase.h"
#define CAIStateGroup_TavernKeeper CAIStateGroup_TavernKeeper_HeaderLayout
#include "engine/CAIStateGroup_TavernKeeper.h"
#undef CAIStateGroup_TavernKeeper
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
class CTCTavernBar:public CTCBase{public:enum{InterfaceType=185};void FreeSlot(long);};
// Layout: active owner header retained. Original tracked PBar viewed as CIntelligentPointer<CThingObject>; component must exist for a reserved index. Missing-component output remains unset as retail.
class CAIStateGroup_TavernKeeper:public CAIStateGroup_TavernKeeper_HeaderLayout{public:void FreeBarPoint();};
void CAIStateGroup_TavernKeeper::FreeBarPoint(){if(JugActionPointIndex!=-1){CTCTavernBar* component;reinterpret_cast<CIntelligentPointer<CThingObject>*>(PBar)->GetPItem()->GetTC(&component);component->FreeSlot(JugActionPointIndex);JugActionPointIndex=-1;}}