// CCreatureAction_DropWeapon::PerformAction() -- when the creature has a carrying component: drop the thing in its primary slot (if any) through DropWeapon, then, when it has a creature mode manager, remove mode 4
// Leaf: ?PerformAction@CCreatureAction_DropWeapon@@UAEXXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845FE0, 164 bytes, thiscall, `ret`. Vtable 0x01271274 slot 3.
// Identity (proven): RTTI vtable 0x01271274 (.?AVCCreatureAction_DropWeapon@@) slot 3 (PerformAction); Ego_r.pdb / FableWin.pdb
//   hold ?PerformAction@CCreatureAction_DropWeapon@@UAEXXZ (donor 0x00520F30, right after DropWeapon 0x00520BC0, as retail
//   0x00845FE0 follows the landed DropWeapon 0x00845CE0).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (x3), CVectorMap<ETCInterfaceType, CTCBase*,
//   CKeyPairCompareLess<..> >::LowerBound(const ETCInterfaceType&) 0x0040F020 (Ego_r template spelling, L083),
//   CTCCarrying::GetThingInPrimarySlot() const 0x006A1440 (PDB QBE), CCreatureAction_DropWeapon::DropWeapon(CThing&,
//   CTCCarrying&, CThing&) 0x00845CE0 (public static __fastcall, PDB), CTCCreatureModeManager::RemoveMode(unsigned long)
//   0x0073ABA0 (PDB).
// Layout: retail CThing UsingComponents (bitset<274>) +0x20 and Components (CVectorMap, 0x10) +0x44 (the landed GetTC model
//   0x004ACAB0); interface 0x46 = CTCCarrying (word 2 bit 6), 0x31 = CTCCreatureModeManager (word 1 bit 17; the landed
//   DropWeapon 0x00845CE0 removes mode 4 through the same 0x31 component). PCreature +0x08 (engine/CCreatureActionBase.h,
//   flat, so the polymorphic class is modelled locally).
// Shape: CThing::GetTC<T>(T**) inline in each if-condition (the out-pointer form: the component reaches ecx through
//   `mov eax,[eax+4] ; mov ecx,eax`); the key copy lives in the prologue's `push ecx` slot.
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CThingCreatureBase CThingCreatureBasePdb
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"


enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed
class CTCBase;
class CBaseObject;
class CThing;
class CTCCarrying {
public:
    enum { INTERFACE_TYPE = 0x46 };
    CThing* GetThingInPrimarySlot() const;                   // 0x006A1440
};
class CTCCreatureModeManager {
public:
    enum { INTERFACE_TYPE = 0x31 };
    void RemoveMode(unsigned long mode);                     // 0x0073ABA0
};
class CThing {                                               // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];
    std::bitset<274>   UsingComponents;                      // +0x20
    TCMap              Components;                           // +0x44
    template <class T> bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || T::INTERFACE_TYPE < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { };
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    virtual void PerformAction() = 0;
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
class CCreatureAction_DropWeapon : public CCreatureActionBase {
public:
    virtual void PerformAction();
    static void __fastcall DropWeapon(CThing& weapon, CTCCarrying& carrying, CThing& creature);   // 0x00845CE0
};

void CCreatureAction_DropWeapon::PerformAction()
{
    CTCCarrying* carrying;
    if (PCreature->GetTC(&carrying)) {
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon)
            DropWeapon(*weapon, *carrying, *PCreature);
        CTCCreatureModeManager* modes;
        if (PCreature->GetTC(&modes))
            modes->RemoveMode(4);
    }
}