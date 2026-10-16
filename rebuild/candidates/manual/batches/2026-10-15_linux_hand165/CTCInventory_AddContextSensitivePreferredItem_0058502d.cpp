// CTCInventory::AddContextSensitivePreferredItem(const CCharString&, long, long) -- registers a thing def (by name) the hero carries as a context-sensitive preferred item, once
// Leaf: AddContextSensitivePreferredItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0058502D, 114 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 0Ch`.
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName(const
//          CCharString&) @ 0x009AD410, NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long)
//          @ 0x005BDF08, std::vector<CContextSensitiveItem>::push_back(const CContextSensitiveItem&) @ 0x00594782
//          (manifest "Vector_PushBack_Uint") on the static list @ 0x013B8B40.
// Shape: the name argument is pushed before the manager getter (arguments first); a def that is unknown (`<= 0`)
// or not carried returns false; the static list is walked with its begin/end in eax/edx (12-byte records) and an
// existing positive entry for the same def returns false; the new record is a named local built in the frame
// (ThingDefIndex [ebp-0Ch], ButtonIndex, Priority).
// Layout: PDB CTCInventory::CContextSensitiveItem {ThingDefIndex, ButtonIndex, Priority} (12 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;

struct CCharString {
    CCharStringData* PStringData;
};

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};

struct CContextSensitiveItem {
    long ThingDefIndex;                              // +0x00
    long ButtonIndex;                                // +0x04
    long Priority;                                   // +0x08
};

struct CContextSensitiveItemVector {                 // std::vector<CContextSensitiveItem>
    CContextSensitiveItem* _M_start;
    CContextSensitiveItem* _M_finish;
    CContextSensitiveItem* _M_end_of_storage;
    CContextSensitiveItem* begin() { return _M_start; }
    CContextSensitiveItem* end() { return _M_finish; }
    void push_back(const CContextSensitiveItem& item);   // 0x00594782
};

struct CTCInventory {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase

    static CContextSensitiveItemVector ContextSensitivePreferredItems;   // 0x013B8B40

    CDefinitionManager* GetDefinitionManager() const;    // 0x006869D0 (CTCBase)
    long GetNumberOfItemsOfTypeInInventory(long defIndex);   // 0x005BDF08 (CTCInventoryBase)
    bool AddContextSensitivePreferredItem(const CCharString& defName, long button, long priority);
};

bool CTCInventory::AddContextSensitivePreferredItem(const CCharString& defName, long button, long priority)
{
    long defIndex = GetDefinitionManager()->GetDefGlobalIndexFromName(defName);
    if (defIndex > 0 && GetNumberOfItemsOfTypeInInventory(defIndex) > 0) {
        for (CContextSensitiveItem* it = ContextSensitivePreferredItems.begin();
             it != ContextSensitivePreferredItems.end(); ++it) {
            if (it->ThingDefIndex > 0 && it->ThingDefIndex == defIndex)
                return false;
        }
        CContextSensitiveItem item;
        item.ThingDefIndex = defIndex;
        item.ButtonIndex = button;
        item.Priority = priority;
        ContextSensitivePreferredItems.push_back(item);
        return true;
    }
    return false;
}
