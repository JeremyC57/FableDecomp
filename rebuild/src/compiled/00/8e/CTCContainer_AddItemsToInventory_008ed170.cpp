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
// Name-fix (agentNF55, round 7): the local CTCContainer / CContainerItemVector / SContainerItem models replaced by the shared
//   engine/CTCContainer.h (retail CTCBase; ContainerItems the STLport vector<pair<long, CCharString> > with the per-TU
//   FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE, so clear() calls ?erase@?$vector@U?$pair@JVCCharString@@@std@@...@std@@QAEPAU?$pair@
//   JVCCharString@@@2@PAU32@0@Z 0x0074FB20), header opt-in FABLE_RETAIL_CTCCONTAINER_ADDITEMS for this method and the protected
//   AddItemToInventory(CThing&) (Ego_r.pdb ?AddItemToInventory@CTCContainer@@IAEXAAVCThing@@@Z).
//   The local CThingObject declaration is gone too: engine/CThingObject.h (CREATE opt-in) over the retail CThing chain.
class C3DVector;
class CRightHandedSet;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CTCCONTAINER_ADDITEMS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#include "engine/CThing.h"
#include "engine/CTCContainer.h"
#include "engine/CThingObject.h"   // Create 0x00703210 (Ego_r.pdb ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z)


bool CTCContainer::AddItemsToInventory()
{
    if (!ContainerItems.empty())
    {
        for (unsigned long i = 0; i < ContainerItems.size(); ++i)
        {
            std::pair<long, CCharString>& item = ContainerItems[i];
            if (item.first > 0)
            {
                CThing* created = CThingObject::Create(item.first, GetParentThing().GetPos(), 5, 0, 0, item.second);
                if (created)
                    AddItemToInventory(*created);
            }
        }
        ContainerItems.clear();
        return true;
    }
    return false;
}
