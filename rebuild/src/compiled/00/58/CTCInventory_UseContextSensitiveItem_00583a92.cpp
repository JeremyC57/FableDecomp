// CTCInventory::UseContextSensitiveItem(long quickSlot) -- for a positive quick-access def: if its context-sensitive sub-def is a type-6 item with a slot in [0, 6), remember the slot's previous item and put this one in it; then use the item held in the primary slot when it is alive and of that def, otherwise use the def itself
// Leaf: UseContextSensitiveItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00583A92, 323 bytes, `ret 4`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`leave`, `push 46h ; pop ebx`).
// Callees: std::bitset<>::test @ 0x00410DE0 and CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0
//   (inlined GetInterface for CTCCarrying, type 0x46), CTCBase::GetDefinitions @ 0x006869D0,
//   CDefinitionManager::GetDef<CThingDef> @ 0x0042AF3C, CDefClassBase::GetSubDef<CContextSensitiveItemDef> @ 0x005933C8
//   (landed), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, IS_THING_ALIVE<CThing*> @ 0x0040DCE0 (__fastcall), the
//   inventory's own virtual slots 33 (+0x84, use by def) and 34 (+0x88, use a thing), the defs' virtual slot 1.
// Layout: retail CTCInventory (engine/CTCInventory.h, STLport 4.6 12-byte vectors): ContextSensitiveItems begin +0x154, the slot item
//   arrays' begin words +0x1E4 / +0x1F0; CContextSensitiveItemDef +0x28 item type, +0x34 slot; retail CThing bitset
//   +0x20, interface map +0x44, def index +0x8C (unsigned short).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
struct CPresenceBits { bool test(long kind) const; };      // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;             // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                                // +0x8C
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);      // 0x0040DCE0

class CTCCarrying { public:
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
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
class CContextSensitiveItemDef : public CDefClassBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long ItemType;                                          // +0x28
    unsigned char _pad2c[0x34 - 0x2c];
    long Slot;                                              // +0x34
};
struct CThingDef : CDefClassBase {
    bool GetSubDef(CDefPointer<CContextSensitiveItemDef>* out);   // 0x005933C8
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingDef>* out) const;   // 0x0042AF3C
};
class CGameDefinitionManager : public CDefinitionManager
{
};

template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};

// Same-TU stand-ins (see 0x00801A50): with these bodies visible VC7.1 sees that the key, def-pointer and thing
// addresses do not escape, and packs the frame as retail did (key in the carrying word, the item def and the held
// thing in the dead argument slot).
CDefClassBase* LookUpDef(long index);
CDefClassBase* LookUpSubDef(const CDefClassBase* def);
bool IsAlive(const CThing* thing);
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* lo = first;
    SEntry* hi = last;
    while (lo < hi) {
        SEntry* mid = lo + (hi - lo) / 2;
        if (mid->type < *type)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}
bool CDefinitionManager::GetDef(long index, CDefPointer<CThingDef>* out) const
{
    CThingDef* def = (CThingDef*)LookUpDef(index);
    out->Object = def;
    return def != 0;
}
bool CThingDef::GetSubDef(CDefPointer<CContextSensitiveItemDef>* out)
{
    CContextSensitiveItemDef* def = (CContextSensitiveItemDef*)LookUpSubDef(this);
    out->Object = def;
    return def != 0;
}
bool __fastcall IS_THING_ALIVE(CThing* const& thing)
{
    return thing != 0 && IsAlive(thing);
}
#pragma auto_inline(on)

class CTCBase
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void UseItemDef(long defIndex, bool& used);     // slot 33 (+0x84)
    virtual void UseItemThing(CThing* thing, bool& used);   // slot 34 (+0x88)
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventory : public NInventory::CTCInventoryBase { public:

    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad008[0x154 - 0x08];
    CRetailVector<long> ContextSensitiveItems;                   // +0x154
    unsigned char _pad160[0x1e4 - 0x160];
    CRetailVector<long> SlotItems;                          // +0x1E4
    CRetailVector<long> PreviousSlotItems;                  // +0x1F0

    void UseContextSensitiveItem(long quickSlot);
};

void CTCInventory::UseContextSensitiveItem(long quickSlot)
{
    CTCCarrying* carrying = 0;
    ParentThing->GetInterface(&carrying, 0x46);
    if (ContextSensitiveItems[quickSlot] > 0) {
        CDefPointer<CThingDef> thingDef;
        if (GetDefinitionManager().GetDef(ContextSensitiveItems[quickSlot], &thingDef)) {
            CDefPointer<CContextSensitiveItemDef> itemDef;
            if (thingDef->GetSubDef(&itemDef) && itemDef->ItemType == 6 && itemDef->Slot >= 0 && itemDef->Slot < 6) {
                PreviousSlotItems[itemDef->Slot] = SlotItems[itemDef->Slot];
                SlotItems[itemDef->Slot] = ContextSensitiveItems[quickSlot];
            }
        }
        CThing* held = carrying->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(held) && held->DefIndex == ContextSensitiveItems[quickSlot]) {
            bool used;
            UseItemThing(held, used);
        } else {
            bool used;
            UseItemDef(ContextSensitiveItems[quickSlot], used);
        }
    }
}