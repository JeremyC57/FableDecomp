#pragma optimize("s", on)
// CTCInventory::RemoveContextSensitivePreferredItem(const CCharString& itemName) -- erases the named item's record from the static context-sensitive preferred-item list; true when one was removed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005829CD, length 73 bytes; size-optimised TU ("s").
// Callees: CTCBase::GetDefinitions() 0x006869D0 (the game's definition manager via ParentThing; landed name),
//          CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) 0x009AD410,
//          vector<CPreferredItem>::erase(iterator) 0x0059310D on the static list 0x013B8B40.
// Ego_r PDB: ?RemoveContextSensitivePreferredItem@CTCInventory@@QAE_NABVCCharString@@@Z.
// The static list holds 12-byte records whose first dword is the item type (the other two words are not read
// here; CTCInventory 0x00581BDE reads the third as the button index). Its {first,last,end} words sit at
// 0x013B8B40/44/48.
// Shape: the name argument is pushed before the definition-manager getter runs (VC evaluates the object after
// the arguments); `item > 0` guards the walk; first/last are both read once (nothing in the loop writes
// memory); the record test keeps its own `> 0` before the equality; erase's iterator result is dropped.
struct CCharString;

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};

struct CPreferredItem {
    long ItemType;                                   // +0x00
    long Reserved;                                   // +0x04
    long ButtonIndex;                                // +0x08
};

struct CPreferredItemVector {
    CPreferredItem* first;                           // +0x00
    CPreferredItem* last;                            // +0x04
    CPreferredItem* end_of_storage;                  // +0x08
    CPreferredItem* begin() { return first; }
    CPreferredItem* end() { return last; }
    CPreferredItem* erase(CPreferredItem* where);    // 0x0059310D
};

struct CTCInventory {
    static CPreferredItemVector ContextSensitivePreferredItems;   // 0x013B8B40
    CDefinitionManager* GetDefinitions();            // CTCBase, 0x006869D0
    bool RemoveContextSensitivePreferredItem(const CCharString& itemName);
};

bool CTCInventory::RemoveContextSensitivePreferredItem(const CCharString& itemName)
{
    long item = GetDefinitions()->GetDefGlobalIndexFromName(itemName);
    if (item > 0) {
        for (CPreferredItem* it = ContextSensitivePreferredItems.begin(); it != ContextSensitivePreferredItems.end(); ++it) {
            if (it->ItemType > 0 && it->ItemType == item) {
                ContextSensitivePreferredItems.erase(it);
                return true;
            }
        }
    }
    return false;
}
