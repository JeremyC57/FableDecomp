// CTCContainer::AddItemsToInventory() -- create every pending container item (def index > 0) at the container's position, add it to the inventory, then clear the item list; false when there was nothing to add
// Grade: DIFFER(122v122) -- register swap only: retail keeps `this` in ebx and the loop index in ebp, ours `this` in ebp and the index in ebx (same instructions otherwise; early-return and reference-local forms tried)
//
// Retail 0x008ED170, 122 bytes (plain `ret`). Callees: CThing::GetPosition() const @ 0x004C73D0,
// CThingObject::Create(long, const C3DVector&, long, const CRightHandedSet*, unsigned long,
// const CCharString&) @ 0x00703210 (`__fastcall`), CTCContainer::AddItemToInventory(CThing*)
// @ 0x008ED090, vector<pair<long,CCharString>>::erase(iterator, iterator) @ 0x0074FB20 (out of line;
// manifest "std::vector_CopyAssignSegment").
// Layout: Ego_r.pdb CTCContainer: ContainerItems (vector<pair<long,CCharString>>, 8-byte elements) at
// +0x0c, the retail STLport {first, last, end}; ParentThing +0x04.
// Shape: an empty() early return (separate `xor al,al` exit); an unsigned index loop `i < size()`
// (`sar ecx,3`, recomputed) over `*(begin() + i)` (element address in esi); Create's arguments are
// evaluated right to left, so GetPosition runs after the constant pushes; the inline clear() is
// erase(begin(), end()) with the vector's address formed once in edi.
struct C3DVector;
struct CRightHandedSet;
struct CCharString { void* PStringData; };

struct CThing {
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

struct CThingObject;
CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long createFlags,
                                const CRightHandedSet* orientation, unsigned long uid,
                                const CCharString& scriptName);   // 0x00703210

struct SContainerItem {                             // pair<long, CCharString>
    long        first;                              // def index (count-like, > 0 when pending)
    CCharString second;
};

struct CContainerItemVector {                       // STLport vector<pair<long,CCharString>>
    SContainerItem* _M_start;
    SContainerItem* _M_finish;
    SContainerItem* _M_end_of_storage;
    SContainerItem* begin() { return _M_start; }
    SContainerItem* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    SContainerItem& operator[](unsigned long n) { return *(begin() + n); }
    SContainerItem* erase(SContainerItem* first, SContainerItem* last);   // 0x0074FB20
    void clear() { erase(begin(), end()); }
};

struct CTCContainer {
    virtual void v00();
    CThing*              ParentThing;               // +0x04 (CTCBase)
    unsigned char        _pad08[4];
    CContainerItemVector ContainerItems;            // +0x0c
    void AddItemToInventory(CThing* item);          // 0x008ED090
    bool AddItemsToInventory();
};

bool CTCContainer::AddItemsToInventory()
{
    if (!ContainerItems.empty())
    {
        for (unsigned long i = 0; i < ContainerItems.size(); ++i)
        {
            SContainerItem& item = ContainerItems[i];
            if (item.first > 0)
            {
                CThing* created = (CThing*)Create(item.first, ParentThing->GetPosition(), 5, 0, 0, item.second);
                if (created)
                    AddItemToInventory(created);
            }
        }
        ContainerItems.clear();
        return true;
    }
    return false;
}
