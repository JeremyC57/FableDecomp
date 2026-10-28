// CTCContainer::AddItemToInventory(CThing* item) -- put an inventory item (interface 0x1a) into the hero's inventory component of the item's category; anything else is killed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ED090, 220 bytes (`ret 4`; the kill arm tail-jumps reusing the argument slot).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (two inlined lookups), the inventory item's category
// getter @ 0x005D9030 (manifest "CCategory::GetCategoryIdentifier", propagated; returns the interface type of the inventory
// that holds it), CTCBase::GetPlayerManager() @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature
// @ 0x00487DC0, puts @ 0x00BFEAF2 + abort @ 0x00BFEAEC (STLport's exception-free bitset range check, literal "bitset"
// @ 0x0122E068), CTCInventoryItem::AddToInventory(CTCInventoryBase*, bool, bool, long, bool, bool) @ 0x005D8D50, CThing::Kill(bool)
// @ 0x004C9B80.
// Layout: CThing UsingComponents bitset<274> +0x20 (type 0x1a = bit 26 of word 0), Components map +0x44.
// Shape: the item's component is HasTC + GetTC into ebx; the hero's inventory goes through the range-checked bitset::test
// and an unset result local (the absent arm reads the lookup key's slot, the dead `item` argument slot).
extern "C" int __cdecl puts(const char* text);          // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    CInterfaceMap Components;                           // +0x44
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    bool TestComponent(unsigned long position) const    // STLport bitset<274>::test (exception-free range check)
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (UsingComponents[position >> 5] & (1UL << (position & 31))) != 0;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    __forceinline CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;                                    // deliberately unset when absent (retail reads a dead slot)
        if (TestComponent(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
    void Kill(bool immediately);                        // 0x004C9B80
};
class CThingPlayerCreature : public CThing { };
struct CTCInventoryBase;
class CTCInventoryItem { public:
    long GetInventoryType();                            // 0x005D9030
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool e);   // 0x005D8D50
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                          // 0x00487DC0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCContainer : public CTCBase { public:
    CThing* ParentThing;                                // +0x04 (CTCBase)
    void AddItemToInventory(CThing* item);
};

void CTCContainer::AddItemToInventory(CThing* item)
{
    if (item->HasTC(0x1a)) {
        CTCInventoryItem* inventoryItem = (CTCInventoryItem*)item->GetTC(0x1a);
        long type = inventoryItem->GetInventoryType();
        CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        inventoryItem->AddToInventory((CTCInventoryBase*)hero->GetTCIfPresent(type), false, true, 0, false, false);
    } else
        item->Kill(false);
}