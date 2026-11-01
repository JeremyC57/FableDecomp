// Behaviour test: CThingManager::~CThingManager (0x00523440) -- DeleteAllThings runs first, then the members die in
// reverse declaration order (CurrentSectionName, NPCNameMap, EntityRuntimePersistence, UpdateLists, UIDToThingMap,
// DeadThingList, ThingTypeInfo), then the CClassFactory base.
#include <cstdio>
static int g_order[12]; static int g_n = 0;
struct Member { int id; Member(int i) : id(i) { } ~Member() { g_order[g_n++] = id; } };
class CClassFactory { public: virtual ~CClassFactory() { g_order[g_n++] = 0; } };
class CThingManager : public CClassFactory { public:
    Member ThingTypeInfo, DeadThingList, UIDToThingMap, UpdateLists, EntityRuntimePersistence, NPCNameMap, CurrentSectionName;
    int things;
    CThingManager() : ThingTypeInfo(1), DeadThingList(2), UIDToThingMap(3), UpdateLists(4), EntityRuntimePersistence(5),
                      NPCNameMap(6), CurrentSectionName(7), things(3) { }
    void DeleteAllThings() { things = 0; g_order[g_n++] = 99; }
    virtual ~CThingManager() { DeleteAllThings(); }
};
int main()
{
    CClassFactory* f = new CThingManager;
    delete f;
    static const int want[9] = { 99, 7, 6, 5, 4, 3, 2, 1, 0 };
    if (g_n != 9) { std::printf("00523440_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (g_order[i] != want[i]) { std::printf("00523440_TEST FAIL\n"); return 1; }
    std::printf("00523440_TEST PASS\n");
    return 0;
}