// Behaviour test for CCountedPointer<NAnimationEvents::CManager>::DeleteData (0x006FD400): null-safe delete that destroys the extra-events entries, the map storage and the symbol map.
#include <cstdio>
#include <cstdlib>
static int g_entries = 0, g_symbolMaps = 0, g_frees = 0;
struct Entry { unsigned int key; ~Entry() { ++g_entries; } };
class CSymbolMap { public: int body[9]; ~CSymbolMap() { ++g_symbolMaps; } };
struct ExtraEventsMap {
    Entry* Start; Entry* Finish; Entry* Eos; bool Dirty;
    ~ExtraEventsMap() { for (Entry* e = Start; e != Finish; ++e) e->~Entry(); if (Start) { std::free(Start); ++g_frees; } }
};
struct CManager { unsigned int Word0; void* MeshDataBank; CSymbolMap EventSymbolMap; ExtraEventsMap AnimToExtraEventsMap; };
static void DeleteData(void* data) { delete (CManager*)data; }
int main()
{
    DeleteData(0);
    if (g_entries || g_symbolMaps || g_frees) { std::printf("006fd400_TEST FAIL\n"); return 1; }
    CManager* m = new CManager;
    m->AnimToExtraEventsMap.Start = (Entry*)std::malloc(3 * sizeof(Entry));
    m->AnimToExtraEventsMap.Finish = m->AnimToExtraEventsMap.Start + 3;
    DeleteData(m);
    if (g_entries != 3 || g_symbolMaps != 1 || g_frees != 1) { std::printf("006fd400_TEST FAIL\n"); return 1; }
    std::printf("006fd400_TEST PASS\n");
    return 0;
}