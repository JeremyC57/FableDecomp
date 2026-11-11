// CTCInventory::RemoveItemFromInventory(long defIndex, long count) -- remove through the base, then clear every quick-access and context-sensitive entry of that def that is no longer held (repeating the quick-access pass while it clears), and switch the owner's light off when the def has a light sub-def
// Leaf: RemoveItemFromInventory
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x44 - 0x21] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00583CD4, 338 bytes, `ret 8`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (no frame words: the flag and both def pointers live in the argument slots).
// Callees: CTCInventoryBase::RemoveItemFromInventory(long, long) @ 0x005BF3C6 (landed),
//   NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) const @ 0x005BDF08, CTCBase::GetDefinitionManager @ 0x006869D0,
//   CDefinitionManager::GetDef<CThingBaseDef> const @ 0x0042AF3C, CDefClassBase::GetSubDef<CLightDef> const @ 0x0059346A (landed),
//   std::bitset<>::test @ 0x00410DE0 and CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined
//   GetInterface for CTCLight, type 0x27), CTCLight::SetActive(bool) @ 0x006412B0, the defs' virtual slot 1.
// Shape: same-TU `auto_inline(off)` stand-ins for GetDef / GetSubDef (as 0x00801A50) keep the thing def in ebx and let
//   VC7.1 home both def pointers and the lookup key in the dead argument slots.
// Layout: retail CTCInventory (engine/CTCInventory.h, STLport 4.6 12-byte vectors): QuickAccessItems +0x148,
//   ContextSensitiveItems +0x154, ContextSensitiveUpdate +0x1D4 (as the landed GetQuickAccessItem 0x00581E33 / OnItemAdded); retail CThing bitset +0x20, interface map +0x44.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> present;                                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CTCLight { public:
    void SetActive(bool active);                            // 0x006412B0
};

template <class T> class CDefPointer;
class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0059346A GetSubDef<CLightDef> (Ego_r / FableWin template)
};
template <class T> class CDefPointer { public:
    CDefClassBase* Object;                                  // the pointee as its non-const def base (engine/CDefPointer.h)
    CDefPointer() { Object = 0; }
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return static_cast<T*>(Object); }
};
class CLightDef : public CDefClassBase { public: };
class CThingBaseDef : public CDefClassBase { public: };
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef> (FableWin.pdb template)
};
class CGameDefinitionManager : public CDefinitionManager
{
};

// Same-TU stand-ins (see the Shape note).
CDefClassBase* LookUpDef(long index);
CDefClassBase* LookUpSubDef(const CDefClassBase* def);
#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    CDefClassBase* def = LookUpDef(index);
    out->Object = def;
    return def != 0;
}
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    CDefClassBase* def = LookUpSubDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)

template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return _M_start[n]; }
};

class CTCBase
{
public:
    virtual ~CTCBase();
    CThing*       ParentThing;                              // +0x04
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

namespace NInventory {
class CTCInventoryBase : public CTCBase { public:
    long GetNumberOfItemsOfTypeInInventory(long defIndex) const;   // 0x005BDF08 (Ego_r.pdb QBE)
    virtual void RemoveItemFromInventory(long defIndex, long count);   // 0x005BF3C6 (Ego_r.pdb UAE; called qualified)
};
}

class CTCInventory : public NInventory::CTCInventoryBase { public:
    unsigned char _pad008[0x148 - 0x08];
    CRetailVector<long> QuickAccessItems;                          // +0x148
    CRetailVector<long> ContextSensitiveItems;                   // +0x154
    unsigned char _pad160[0x1d4 - 0x160];
    bool          ContextSensitiveUpdate;                             // +0x1D4

    virtual void RemoveItemFromInventory(long defIndex, long count);   // Ego_r.pdb ?RemoveItemFromInventory@CTCInventory@@UAEXJJ@Z
};

void CTCInventory::RemoveItemFromInventory(long defIndex, long count)
{
    NInventory::CTCInventoryBase::RemoveItemFromInventory(defIndex, count);
    bool removed;
    do {
        removed = false;
        for (unsigned long i = 0; i < QuickAccessItems.size(); ++i) {
            if (QuickAccessItems[i] == defIndex && GetNumberOfItemsOfTypeInInventory(defIndex) == 0) {
                QuickAccessItems[i] = 0;
                removed = true;
                ContextSensitiveUpdate = true;
            }
        }
    } while (removed);
    for (unsigned long i = 0; i < ContextSensitiveItems.size(); ++i) {
        if (ContextSensitiveItems[i] == defIndex && GetNumberOfItemsOfTypeInInventory(defIndex) == 0) {
            ContextSensitiveItems[i] = 0;
            ContextSensitiveUpdate = true;
        }
    }
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(defIndex, &thingDef);
    CDefPointer<const CLightDef> lightDef;
    if (thingDef->GetSubDef(&lightDef)) {
        CTCLight* light;
        if (ParentThing->GetInterface(&light, 0x27))
            light->SetActive(false);
    }
}