// Test for 0x005C0C86 std::find_if<const NInventory::CItem*, std::const_mem_fun_ref_t<bool, NInventory::CItem> >: the row only forwards to
// the random-access __find_if (0x005C0965), so this test supplies that callee and checks the forwarded range and predicate (NF92 rewrite of
// the old stand-in test; PassPattern kept).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#define FABLE_RETAIL_STLP_CONST_MEM_FUN_REF
#include "rebuild_abi.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "stlport/fable_algorithm.h"
#include <cstdio>

static const NInventory::CItem* g_first = 0;
static const NInventory::CItem* g_last = 0;
static int g_calls = 0;
static bool g_pred_on_first = false;
namespace std {
template<> const NInventory::CItem* FABLE_FASTCALL __find_if(const NInventory::CItem* first, const NInventory::CItem* last,
    const_mem_fun_ref_t<bool, NInventory::CItem> pred, const random_access_iterator_tag&)
{
    g_first = first; g_last = last; ++g_calls;
    g_pred_on_first = pred(*first);
    return last;
}
}
int main()
{
    static long storage[2 * 5];                      // two 0x14-byte NInventory::CItem slots, both empty (NumberOfItems 0)
    const NInventory::CItem* first = reinterpret_cast<const NInventory::CItem*>(storage);
    const NInventory::CItem* last = first + 2;
    const NInventory::CItem* found = std::find_if(first, last, std::mem_fun_ref(&NInventory::CItem::IsSlotEmpty));
    if (g_calls != 1 || g_first != first || g_last != last || !g_pred_on_first || found != last) { std::printf("BAD\n"); return 1; }
    std::printf("UMOVE_005c0c86_OK\n");
    return 0;
}
