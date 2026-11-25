// 005bfd5e NInventory::CTCInventoryBase::OnSerialise
// Flags:
// Leaf: ?OnSerialise@CTCInventoryBase@NInventory@@MAEXAAVCPersistContext@@_N@Z
#define FABLE_RETAIL_STLP_INVENTORY_LONG_OWNERS_PASS27
#define FABLE_RETAIL_CWORLD_INVENTORY_SERIALISE_PASS27
#define FABLE_RETAIL_STLP_VECTOR_INDEX_START
#define FABLE_RETAIL_CPERSISTCONTEXT_ISSAVING_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_GETHIGHLIGHTEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_SERIALISED_PASS27
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

namespace std {
template<> vector<NInventory::CItem>::~vector();
template<> vector<CCharString>::~vector();

template<> void vector<CCharString>::push_back(const CCharString&);
template<> vector<NInventory::CItem>::iterator vector<NInventory::CItem>::erase(iterator,iterator);

}

#ifdef _MSC_VER
#pragma optimize("s",on)
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#endif
namespace NInventory {

void CTCInventoryBase::GetAllSelectedItems(std::vector<long>& items) const
{
    items.clear();
    for (unsigned long i = 0; i < Categories.size(); ++i)
        items.push_back(Categories[i].GetHighlightedItem());
}

void CTCInventoryBase::OnSerialise(CPersistContext& context, bool in_game)
{
    if (context.IsSaving()) {
        std::vector<CItem> items;
        GetAllNonEmptyInventoryItems(items, PeekWorld().IsSerialisingNonPersistentQuestItems());
        context.TransferVector("InventoryItems", items);
        std::vector<long> selected_items;
        GetAllSelectedItems(selected_items);
        std::vector<CCharString> selected_item_def_names;
        selected_item_def_names.reserve(selected_items.size());
        for (long c1 = 0; c1 < selected_items.size(); ++c1)
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