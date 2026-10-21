// Grade: DIFFER(465v484) -- retail homes the IS_THING_ALIVE temporary and both interface-lookup keys in the dead `item` argument slot [ebp+8] (frame: two push ecx) and keeps the object in edi; ours gives them frame slots (sub esp,10h) and `this` lands in esi; the plain `CThing*` argument form (3e26a, 482v484) keeps the object in memory instead
// CTCInventory::OnItemSelected(long item, bool* handled) -- the hero picked an inventory item: create its object out of
// limbo; for an item of category 1 use it through its interface-0x0E component when the hero is free to (or the
// component allows it mid-action; then *handled), else hand it back to the inventory (interface 0x1A); category 2
// likewise, re-highlighting the current item when the use consumed it; *handled false otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00583E26, 484 bytes, `ret 8` (frame pointer, `leave`).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
struct CPresenceBits { bool test(long kind); };             // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char m_flags91;                                // +0x91 bit 0: dead
    void SetInLimbo(bool limbo);                            // 0x004C8CF0
    __forceinline bool HasTC(long type) { return present.test(type); }
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc = 0;
        if (HasTC(interfaceType))
            tc = FindTC<T>(interfaceType);
        return tc;
    }
};
struct CThingObject : CThing { };
bool __fastcall IS_THING_ALIVE(CThing* const& thing);      // 0x0040DCE0 (instance shared by identical bodies)
struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction();                           // 0x006622A0
};
struct CTCUsable {                                          // interface type 0x0E
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); 
    virtual void Use(CThing* user);                         // slot 28 (+0x70)
    virtual void v29();
    virtual bool CanBeUsedDuringAction();                   // slot 30 (+0x78)
};
struct CTCInventoryItem {                                   // interface type 0x1A
    void ReturnToInventory();                               // 0x005D8E50
};
long __fastcall GetInventoryCategory(long item);            // 0x005D9710

struct CTCInventory {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); 
    virtual void HighlightItem(long item);                  // slot 36 (+0x90)
    CThing* ParentThing;                                    // +0x04 CTCBase
    CThingObject* CreateInventoryObject(long item);               // 0x005BE8F3 (CTCInventoryBase)
    long GetHighlightedItem();                              // 0x005BCAE3 (CTCInventoryBase)
    void OnItemSelected(long item, bool* handled);
};

void CTCInventory::OnItemSelected(long item, bool* handled)
{
    CThingCreatureBase* hero = (CThingCreatureBase*)ParentThing;
    switch (GetInventoryCategory(item)) {
    case 1: {
        CThingObject* object = CreateInventoryObject(item);
        if (IS_THING_ALIVE(object)) {
            object->SetInLimbo(true);
            CTCUsable* usable;
            if (object->HasTC(0x0e) && ((usable = object->FindTC<CTCUsable>(0x0e)), !hero->IsPerformingAnyAction() || usable->CanBeUsedDuringAction())) {
                *handled = true;
                usable->Use(ParentThing);
            } else
                object->GetTC<CTCInventoryItem>(0x1a)->ReturnToInventory();
        }
        break;
    }
    case 2: {
        CThingObject* object = CreateInventoryObject(item);
        if (IS_THING_ALIVE(object)) {
            object->SetInLimbo(true);
            CTCUsable* usable = object->GetTC<CTCUsable>(0x0e);
            if (!hero->IsPerformingAnyAction() || usable->CanBeUsedDuringAction()) {
                usable->Use(ParentThing);
                if (object->m_flags91 & 1)
                    HighlightItem(GetHighlightedItem());
            } else
                object->GetTC<CTCInventoryItem>(0x1a)->ReturnToInventory();
        }
        *handled = false;
        break;
    }
    }
}
