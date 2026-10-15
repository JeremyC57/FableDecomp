// Behaviour test for NInventory::CTCInventoryBase::~CTCInventoryBase (0x005C00A0): unregistered first, then the members in
// reverse (POD vectors freed only when allocated), the def released, finally the CTCBase part.
#include <cstdio>
#include <cstdlib>
static int g_order[16]; static int g_n; static int g_frees;
struct Tracked { int id; ~Tracked() { g_order[g_n++] = id; } };
struct PodVec { void* first; ~PodVec() { if (first) { std::free(first); ++g_frees; } } };
struct Def { unsigned int RefCount; int destroyed; void Release() { if (--RefCount == 0) ++destroyed; } };
struct DefPtr { Def* O; ~DefPtr() { if (O) { O->Release(); O = 0; } g_order[g_n++] = 70; } };
struct TCBase { ~TCBase() { g_order[g_n++] = 100; } void RemoveFromGlobalTCs() { g_order[g_n++] = 0; } };
struct Inventory : TCBase {
    PodVec Components; Tracked Categories; DefPtr PDef; Tracked SerialisedItems, SerialisedSelectedItems;
    PodVec MenuDisplayListItem, MenuDisplayListCategory; Tracked ConfiscatedItems;
    ~Inventory() { RemoveFromGlobalTCs(); }
};
int main()
{
    Def d; d.RefCount = 2; d.destroyed = 0;
    Inventory* inv = new Inventory;
    inv->Components.first = std::malloc(4); inv->MenuDisplayListItem.first = 0; inv->MenuDisplayListCategory.first = std::malloc(8);
    inv->Categories.id = 20; inv->SerialisedItems.id = 78; inv->SerialisedSelectedItems.id = 80; inv->ConfiscatedItems.id = 0xac;
    inv->PDef.O = &d;
    delete inv;
    const int want[7] = { 0, 0xac, 80, 78, 70, 20, 100 };
    for (int i = 0; i < 7; ++i) if (g_order[i] != want[i]) { std::printf("005c00a0_TEST FAIL\n"); return 1; }
    if (g_frees != 2 || d.RefCount != 1 || d.destroyed) { std::printf("005c00a0_TEST FAIL\n"); return 1; }
    std::printf("005c00a0_TEST PASS\n");
    return 0;
}