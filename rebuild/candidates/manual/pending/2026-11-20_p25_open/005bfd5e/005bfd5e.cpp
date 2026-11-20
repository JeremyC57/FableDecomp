// NInventory::CTCInventoryBase::OnSerialise(CPersistContext& context, bool in_game) -- saving: persist the non-empty item stacks, the def names of the selected items and the confiscated items; loading: transfer into fresh serialised item / selection vectors and the cleared confiscated list
// Leaf: ?OnSerialise@CTCInventoryBase@NInventory@@MAEXAAVCPersistContext@@_N@Z
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_SERIALISED
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_PDEF
#define FABLE_RETAIL_CTCINVENTORYBASE_GETPDEF_INLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#include "fable_string.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CPersistContext.h"
#include "engine/CWorld.h"

namespace NInventory {
void CTCInventoryBase::OnSerialise(CPersistContext& context, bool in_game)
{
    if (context.IsSaving()) {
        std::vector<CItem> items;
        GetAllNonEmptyInventoryItems(items, PeekWorld().SerialisingNonPersistentQuestItems);
        context.TransferVector("InventoryItems", items);
        std::vector<long> selected_items;
        GetAllSelectedItems(selected_items);
        unsigned long count = selected_items.size();
        std::vector<CCharString> selected_item_def_names;
        selected_item_def_names.reserve(count);
        for (unsigned long c1 = 0; c1 < count; ++c1)
            if (selected_items[c1] > 0)
                selected_item_def_names.push_back(GetDefinitionManager().GetDefNameFromGlobalIndex(selected_items[c1]));
        context.TransferVector("SelectedInventoryItems", selected_item_def_names);
        context.TransferVector("ConfiscatedItems2", ConfiscatedItems);
    } else {
        SerialisedItems.Reset(new std::vector<CItem>);
        context.TransferVector("InventoryItems", *SerialisedItems);
        SerialisedSelectedItems.Reset(new std::vector<CCharString>);
        context.TransferVector("SelectedInventoryItems", *SerialisedSelectedItems);
        ConfiscatedItems.clear();
        context.TransferVector("ConfiscatedItems2", ConfiscatedItems);
    }
}
}
