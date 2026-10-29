// CTCInventory::RemoveItemFromInventory(long defIndex, long count) -- remove through the base, then clear every quick-access and context-sensitive entry of that def that is no longer held (repeating the quick-access pass while it clears), and switch the owner's light off when the def has a light sub-def
// Leaf: RemoveItemFromInventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00583CD4, 338 bytes, `ret 8`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (no frame words: the flag and both def pointers live in the argument slots).
// Callees: CTCInventoryBase::RemoveItemFromInventory(long, long) @ 0x005BF3C6 (landed),
//   CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08, CTCBase::GetDefinitions @ 0x006869D0,
//   CDefinitionManager::GetDef<CThingDef> @ 0x0042AF3C, CDefClassBase::GetSubDef<CLightDef> @ 0x0059346A (landed),
//   std::bitset<>::test @ 0x00410DE0 and CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined
//   GetInterface for CTCLight, type 0x27), CTCLight::SetActive(bool) @ 0x006412B0, the defs' virtual slot 1.
// Shape: same-TU `auto_inline(off)` stand-ins for GetDef / GetSubDef (as 0x00801A50) keep the thing def in ebx and let
//   VC7.1 home both def pointers and the lookup key in the dead argument slots.
// Layout: retail CTCInventory (engine/CTCInventory.h, STLport 4.6 12-byte vectors): QuickAccessItems +0x148,
//   ContextSensitiveItems +0x154, ContextSensitiveUpdate +0x1D4 (as the landed GetQuickAccessItem 0x00581E33 / OnItemAdded); retail CThing bitset +0x20, interface map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
struct CPresenceBits { bool test(long kind); };            // bitset<>::test 0x00410DE0
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CTCLight { public:
    void SetActive(bool active);                            // 0x006412B0
};

class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() { Object = 0; }
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CLightDef : public CDefClassBase { public: };
struct CThingDef : CDefClassBase {
    bool GetSubDef(CDefPointer<CLightDef>* out);            // 0x0059346A
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingDef>* out) const;   // 0x0042AF3C
};
class CGameDefinitionManager : public CDefinitionManager
{
};

// Same-TU stand-ins (see the Shape note).
CDefClassBase* LookUpDef(long index);
CDefClassBase* LookUpSubDef(const CDefClassBase* def);
#pragma auto_inline(off)
bool CDefinitionManager::GetDef(long index, CDefPointer<CThingDef>* out) const
{
    CThingDef* def = (CThingDef*)LookUpDef(index);
    out->Object = def;
    return def != 0;
}
bool CThingDef::GetSubDef(CDefPointer<CLightDef>* out)
{
    CLightDef* def = (CLightDef*)LookUpSubDef(this);
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
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

struct CTCInventoryBase : public CTCBase {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 CTCBase
    long GetNumberOfItemsOfTypeInInventory(long defIndex);  // 0x005BDF08
    void RemoveItemFromInventory(long defIndex, long count);   // 0x005BF3C6
};

class CTCInventory : public CTCInventoryBase { public:
    unsigned char _pad008[0x148 - 0x08];
    CRetailVector<long> QuickAccessItems;                          // +0x148
    CRetailVector<long> ContextSensitiveItems;                   // +0x154
    unsigned char _pad160[0x1d4 - 0x160];
    bool          ContextSensitiveUpdate;                             // +0x1D4

    void RemoveItemFromInventory(long defIndex, long count);
};

void CTCInventory::RemoveItemFromInventory(long defIndex, long count)
{
    CTCInventoryBase::RemoveItemFromInventory(defIndex, count);
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
    CDefPointer<CThingDef> thingDef;
    GetDefinitionManager().GetDef(defIndex, &thingDef);
    CDefPointer<CLightDef> lightDef;
    if (thingDef->GetSubDef(&lightDef)) {
        CTCLight* light;
        if (ParentThing->GetInterface(&light, 0x27))
            light->SetActive(false);
    }
}