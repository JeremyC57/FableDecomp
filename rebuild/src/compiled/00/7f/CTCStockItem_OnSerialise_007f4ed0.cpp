// CTCStockItem::OnSerialise(CPersistContext& context, bool) -- transfer the for-sale and stealable flags and the price, then mark the item initialised
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F4ED0, 100 bytes (`ret 8`). Vtable 0x01240C1C (CTCStockItem, rtti-vtables.tsv) slot 1.
// Callees: CPersistContext::Transfer<bool> @ 0x004045C0 and Transfer<long> @ 0x00410BE0 (manifest "Transfer<signed char>" labels
// are propagated; as the landed CThingTrackNode::SerialiseGameState 0x00837CE0). Names 0x01271DE0 / 0x01271DD4 / 0x01271DCC.
// Layout: engine/CTCStockItem.h (Ego_r.pdb): ForSale +0x0c, Stealable +0x0d, Initialised +0x0e, Price +0x10.
// Shape: each default is a temporary bound to the const-ref parameter: the first in the function's `push ecx` word, the next two
// in the dead context argument slot.
#include "engine/CTCStockItem.h"

struct CPersistContext {
    void Transfer(const char* name, long& value, const long& defaultValue);   // 0x00410BE0
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
};

struct CTCStockItem_Methods : CTCStockItem {
    void OnSerialise(CPersistContext& context, bool loading);
};

void CTCStockItem_Methods::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("ForSale", ForSale, false);
    context.Transfer("Stealable", Stealable, false);
    context.Transfer("Price", Price, 0);
    Initialised = true;
}