// Test for 0x005C08D3 std::find_if<NInventory::CCategory*, NInventory::CIsInventoryCategory>: the row only forwards to the
// random-access __find_if (0x005C06C3), so this test supplies that callee and checks the forwarded range and predicate.
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

static NInventory::CCategory* g_first = 0;
static NInventory::CCategory* g_last = 0;
static long g_index = 0;
static int g_calls = 0;
namespace std {
template<> NInventory::CCategory* FABLE_FASTCALL __find_if(NInventory::CCategory* first, NInventory::CCategory* last,
    NInventory::CIsInventoryCategory pred, const random_access_iterator_tag&)
{
    g_first = first; g_last = last; g_index = pred.Index; ++g_calls;
    return last;
}
}
int main()
{
    static long storage[2 * 11];
    NInventory::CCategory* first = reinterpret_cast<NInventory::CCategory*>(storage);
    NInventory::CCategory* last = first + 2;
    NInventory::CCategory* found = std::find_if(first, last, NInventory::CIsInventoryCategory(7));
    if (g_calls != 1 || g_first != first || g_last != last || g_index != 7 || found != last) { std::printf("BAD\n"); return 1; }
    std::printf("UMOVE_005c08d3_OK\n");
    return 0;
}
