// CTCContainer::AddItemsToInventory() -- create every pending container item (def index > 0) at the container's position, add it to the inventory, then clear the item list; false when there was nothing to add
// Leaf: ?AddItemsToInventory@CTCContainer@@QAE_NXZ
// Retry 10b (accessor sweep, FIXES B-4/B-5): the parent thing is read through an inline accessor; the plain member read gives the parked register difference.
//
// Retail 0x008ED170, 122 bytes (plain `ret`). Callees: CThing::GetPos() const @ 0x004C73D0,
// CThingObject::Create(long, const C3DVector&, long, const CRightHandedSet*, unsigned long,
// const CCharString&) @ 0x00703210 (`__fastcall`), CTCContainer::AddItemToInventory(CThing*)
// @ 0x008ED090, vector<pair<long,CCharString>>::erase(iterator, iterator) @ 0x0074FB20 (out of line;
// manifest "std::vector_CopyAssignSegment").
// Layout: Ego_r.pdb CTCContainer: ContainerItems (vector<pair<long,CCharString>>, 8-byte elements) at
// +0x0c, the retail STLport {first, last, end}; ParentThing +0x04.
// Shape: an empty() early return (separate `xor al,al` exit); an unsigned index loop `i < size()`
// (`sar ecx,3`, recomputed) over `*(begin() + i)` (element address in esi); Create's arguments are
// evaluated right to left, so GetPos runs after the constant pushes; the inline clear() is
// erase(begin(), end()) with the vector's address formed once in edi.
class C3DVector;
class CRightHandedSet;
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"

class CThingObject;
class CThingObject { public:
    static CThingObject* __fastcall Create(long a0, C3DVector const&a1, long a2, CRightHandedSet const*a3, unsigned long a4, CCharString const&a5);   // 0x00703210 (Ego_r.pdb ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z)
};

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

class CTCContainer { public:
    virtual void v00();
    CThing*              ParentThing;               // +0x04 (CTCBase)
    CThing* GetParentThing() const { return ParentThing; }
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
                CThing* created = (CThing*)CThingObject::Create(item.first, GetParentThing()->GetPos(), 5, 0, 0, item.second);
                if (created)
                    AddItemToInventory(created);
            }
        }
        ContainerItems.clear();
        return true;
    }
    return false;
}