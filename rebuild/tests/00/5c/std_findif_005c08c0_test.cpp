// Test for 0x005C08C0 std::find_if<const NInventory::CCategory*, NInventory::CIsInventoryCategory>: the row only forwards to the
// random-access __find_if (0x005C0653), so this test supplies that callee and checks the forwarded range and predicate.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CThing;
#include "engine/CCategory.h"
#include "stlport/fable_algorithm.h"
#include <cstdio>

static const NInventory::CCategory* g_first = 0;
static const NInventory::CCategory* g_last = 0;
static long g_index = 0;
static int g_calls = 0;
namespace std {
template<> const NInventory::CCategory* FABLE_FASTCALL __find_if(const NInventory::CCategory* first, const NInventory::CCategory* last,
    NInventory::CIsInventoryCategory pred, const random_access_iterator_tag&)
{
    g_first = first; g_last = last; g_index = pred.Index; ++g_calls;
    return last;
}
}
int main()
{
    static long storage[2 * 11];
    const NInventory::CCategory* first = reinterpret_cast<const NInventory::CCategory*>(storage);
    const NInventory::CCategory* last = first + 2;
    const NInventory::CCategory* found = std::find_if(first, last, NInventory::CIsInventoryCategory(7));
    if (g_calls != 1 || g_first != first || g_last != last || g_index != 7 || found != last) { std::printf("BAD\n"); return 1; }
    std::printf("UMOVE_005c08c0_OK\n");
    return 0;
}
