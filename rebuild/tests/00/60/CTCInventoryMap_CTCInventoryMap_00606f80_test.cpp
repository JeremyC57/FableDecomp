// Behaviour test for CTCInventoryMap::~CTCInventoryMap (0x00606F80): the zooming-map and list pointers are dropped, the
// owned UI components are deleted (null slots skipped) and the vector cleared unless this is the inventory screen, then
// the members and bases are destroyed.
#include <cstdio>
#include <cstring>
#include <vector>
static char g_log[256];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct ComponentModel { char Tag; ComponentModel(char t) : Tag(t) { } virtual ~ComponentModel() { Log(Tag); } };
struct InventoryBaseModel {
    std::vector<ComponentModel*> Components;
    virtual ~InventoryBaseModel() { Log('B'); }
};
struct InventoryMapModel : InventoryBaseModel {
    void* pmap_real; void* MapList; bool InventoryScreen;
    std::vector<int> MapRegions;
    ~InventoryMapModel()
    {
        pmap_real = 0;
        MapList = 0;
        if (!InventoryScreen) {
            for (unsigned int i = 0; i < Components.size(); ++i) {
                if (Components[i]) delete Components[i];
            }
            Components.clear();
        }
        Log(Components.empty() ? 'e' : 'n');
    }
};
int main()
{
    int dummy = 0;
    InventoryMapModel* m = new InventoryMapModel; m->InventoryScreen = false; m->pmap_real = &dummy; m->MapList = &dummy;
    m->Components.push_back(new ComponentModel('1')); m->Components.push_back(0); m->Components.push_back(new ComponentModel('2'));
    g_log[0] = 0; InventoryBaseModel* b = m; delete b;
    if (std::strcmp(g_log, "12eB") != 0) { std::printf("00606f80_TEST FAIL %s\n", g_log); return 1; }
    InventoryMapModel* s = new InventoryMapModel; s->InventoryScreen = true; s->pmap_real = &dummy; s->MapList = &dummy;
    ComponentModel* kept = new ComponentModel('3'); s->Components.push_back(kept);
    g_log[0] = 0; b = s; delete b;
    if (std::strcmp(g_log, "nB") != 0) { std::printf("00606f80_TEST FAIL %s\n", g_log); return 1; }
    delete kept;
    std::printf("00606f80_TEST PASS\n");
    return 0;
}