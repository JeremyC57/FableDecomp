// Grade: DIFFER(404v398) -- creation, item lookup, null-initialised inventory, STLport bitset range check (puts/abort), AddToInventory and quick-access calls match; retail lays the add-directly arm out first with the quick-access block as its fall-through and moves the hero-receive-items arm (0xcb) to the end with a jump back (sharing the 0x1a key slot); every if/else, else-if, goto and flag form the lane tried keeps source order (else arm inline, own key slot)
// CThingPlayerCreature::CreateAndAddObjectToInventory(long defIndex, long quickAccessSlot, bool addDirectly, const CCharString& scriptName) -- create an object of the def at the player's position; if it is an inventory item, add it either straight into the matching inventory component of this creature or through the hero's item-receiving component (0xcb), then, for a non-negative slot and a type-0x11 inventory, make it a quick-access item unless it already is one
// Leaf: ?CreateAndAddObjectToInventory@CThingPlayerCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AC200, length 398 bytes (+2 int3), `ret 10h`.
//   Name and signature from Ego_r.pdb (`?CreateAndAddObjectToInventory@CThingPlayerCreature@@QAEPAVCThingObject@@JJ_N
//   ABVCCharString@@@Z`).
// Callees: CThing::GetPosition() const @ 0x004C73D0, CreateThingObject @ 0x00703210 (`__fastcall`), LowerBound
//   @ 0x0040F020, CTCInventoryItem::GetCategoryIdentifier @ 0x005D9030, puts @ 0x00BFEAF2 / abort @ 0x00BFEAEC (the
//   STLport bitset::test range check, "bitset" @ 0x0122E068), CTCInventoryItem::AddToInventory @ 0x005D8D50, the
//   inventory's virtual slot 21 (+0x54), CTCInventory::GetQuickAccessItemButtonIndex @ 0x00581FD1,
//   CTCInventory::SetThingAsQuickAccessItem @ 0x00584157, CTCHeroReceiveItems::AddItemToInventory @ 0x007CAB30.
// Layout (retail): CThing bitset<274> UsingComponents +0x20 (0x1a = bit 26 of +0x20, 0xcb = bit 11 of +0x38), component
//   map +0x44, DefGlobalIndex +0x8c, PlayerNumber +0x90 (Ego_r.pdb +0x9c/+0xa0, CThing 0x10 lower).
extern "C" int puts(const char* text);                  // 0x00BFEAF2
extern "C" void abort();                                // 0x00BFEAEC (not declared noreturn: retail falls through)
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
struct CCharString { void* PStringData; };
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 bitset<274> UsingComponents
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short DefGlobalIndex;                      // +0x8c
    short          CurrentMapNumber;                    // +0x8e
    signed char    PlayerNumber;                        // +0x90
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(unsigned long type) const  // STLport bitset<274>::test
    {
        if (type >= 274) {
            puts("bitset");
            abort();
        }
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline CTCBase* GetTCIfPresent(long type)
    {
        CTCBase* component = 0;
        if (HasTC(type))
            component = GetTC(type);
        return component;
    }
};
struct CThingObject : CThing {};
CThingObject* __fastcall CreateThingObject(long defIndex, const C3DVector& position, long player, CRightHandedSet* orientation,
    unsigned long flags, const CCharString& scriptName);   // 0x00703210

struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20();
    virtual long GetInventoryType() const;              // slot 21 (+0x54)
};
struct CTCInventoryBase : CTCBase {};
struct CTCInventory : CTCInventoryBase {
    long GetQuickAccessItemButtonIndex(long defIndex);                  // 0x00581FD1
    void SetThingAsQuickAccessItem(long defIndex, long slot, bool b);   // 0x00584157
};
struct CTCInventoryItem : CTCBase {
    unsigned long GetCategoryIdentifier() const;        // 0x005D9030 (EInventoryCategoryIdentifier = the component type)
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool e);   // 0x005D8D50
};
struct CTCHeroReceiveItems : CTCBase {
    void AddItemToInventory(CThingObject* object, long flags);         // 0x007CAB30
};

struct CThingPlayerCreature : CThing {
    CThingObject* CreateAndAddObjectToInventory(long defIndex, long quickAccessSlot, bool addDirectly, const CCharString& scriptName);
};

CThingObject* CThingPlayerCreature::CreateAndAddObjectToInventory(long defIndex, long quickAccessSlot, bool addDirectly, const CCharString& scriptName)
{
    CThingObject* object = CreateThingObject(defIndex, GetPosition(), PlayerNumber, 0, 0, scriptName);
    if (object && object->HasTC(0x1a)) {
        CTCInventoryItem* item = (CTCInventoryItem*)object->GetTC(0x1a);
        if (item) {
            CTCInventoryBase* inventory = 0;
            unsigned long category = item->GetCategoryIdentifier();
            if (HasTC(category))
                inventory = (CTCInventoryBase*)GetTC(category);
            if (addDirectly) {
                if (!item->AddToInventory(inventory, false, quickAccessSlot != -1, 0, addDirectly, false))
                    return object;
            } else
                ((CTCHeroReceiveItems*)GetTCIfPresent(0xcb))->AddItemToInventory(object, 0);
            if (quickAccessSlot >= 0 && inventory->GetInventoryType() == 0x11) {
                CTCInventory* quick = (CTCInventory*)inventory;
                if (quick->GetQuickAccessItemButtonIndex(object->DefGlobalIndex) == -1)
                    quick->SetThingAsQuickAccessItem(object->DefGlobalIndex, quickAccessSlot, false);
            }
        }
    }
    return object;
}
