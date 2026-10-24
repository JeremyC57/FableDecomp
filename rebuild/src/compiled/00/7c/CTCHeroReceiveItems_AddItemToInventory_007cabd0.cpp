// CTCHeroReceiveItems::AddItemToInventory(CThing& thing, bool flag) -- move a received item into the hero's matching inventory: when the thing is an inventory item and the hero has the inventory its type names, add it there (passing the flag through); otherwise kill the thing
// Leaf: ?AddItemToInventory@CTCHeroReceiveItems
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CABD0, 217 bytes (+7 int3), thiscall `ret 8`.
// Identity: Ego_r.pdb name set ?AddItemToInventory@CTCHeroReceiveItems@@AAEXAAVCThing@@_N@Z (private, CThing&, bool);
//   the only caller is in CTCHeroReceiveItems (0x007CAD7E). The catalog label "Entity_OnItemSpawned" is descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (both inline lookups),
//   CTCInventoryItem::GetInventoryType @ 0x005D9030, puts @ 0x00BFEAF2 + abort @ 0x00BFEAEC (STLport's exception-free
//   bitset<274>::test range check, "bitset" @ 0x0122E068), CTCInventoryItem::AddToInventory @ 0x005D8D50,
//   CThing::Kill(bool) @ 0x004C9B80 (both as named by the landed 0x00849380).
// Layout: CTCBase ParentThing +4; retail CThing bitset<274> +0x20 (CTCInventoryItem 0x1a = bit 26 of word 0), interface
//   map +0x44.
// Shape: the item lookup is the plain inline GetInterface, the inventory lookup the range-checked bitset::test form
//   (the landed 0x00849380 GetCheckedInterface); both keys are homed in the dead `thing` argument slot. AddToInventory
//   gets (inventory, false, true, 0, false, flag); the two Kill(false) calls are written in both else arms (the shrink-wrapped
//   prologue: ebx and esi are pushed only on the item path).
extern "C" int __cdecl puts(const char* text);          // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC

class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 bitset<274> of the interface types present
    CInterfaceMap  m_interfaces;                        // +0x44
    void Kill(bool immediately);                        // 0x004C9B80
    bool TestComponent(unsigned long position) const    // STLport bitset<274>::test (exception-free range check)
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (m_presentBits[position >> 5] & (1UL << (position & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
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
    template<class T> __forceinline bool GetCheckedInterface(T** ppOut, long interfaceType)
    {
        if (TestComponent(interfaceType)) {
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

class CTCInventoryBase;
class CTCInventoryItem {                                // interface 0x1a
public:
    long GetInventoryType() const;                      // 0x005D9030
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long price, bool c, bool d);   // 0x005D8D50
};

class CTCHeroReceiveItems {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    void AddItemToInventory(CThing& thing, bool flag);
};

void CTCHeroReceiveItems::AddItemToInventory(CThing& thing, bool flag)
{
    CTCInventoryItem* item;
    if (thing.GetInterface(&item, 0x1A)) {
        CThing* parent = ParentThing;
        CTCInventoryBase* inventory;
        if (parent->GetCheckedInterface(&inventory, item->GetInventoryType()))
            item->AddToInventory(inventory, false, true, 0, false, flag);
        else
            thing.Kill(false);
    }
    else
        thing.Kill(false);
}