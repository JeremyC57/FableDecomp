// Behaviour test for CWorld::CWorld (0x004A67D0): init block copied, managers null, flags at their defaults,
// atmos list sentinel self-linked, start time stamped.
#include <cstdio>
#include <cstdlib>
struct CWorldInit { void* Component; void* Reserved; void* PlayerManager; void* DefinitionManager; };
static double GFGetTime() { return 123.5; }
template <class T> struct scoped_ptr { T* ptr; scoped_ptr() : ptr(0) {} };
template <class T> struct CCountedPointer { T* Data; void* Info; CCountedPointer() : Data(0), Info(0) {} };
struct SAtmosNode { SAtmosNode* next; SAtmosNode* prev; long data[3]; };
struct CAtmosCopyList {
    SAtmosNode* node;
    CAtmosCopyList() : node(0) { SAtmosNode* n = (SAtmosNode*)malloc(sizeof(SAtmosNode)); n->next = n; n->prev = n; node = n; }
    ~CAtmosCopyList() { free(node); }
};
struct CWorldModel {
    void* Component; void* PlayerManager; void* DefinitionManager;
    scoped_ptr<int> PWorldMap; CCountedPointer<int> PAtmosProcessor; int* PCurrentGameCamera;
    bool JustLoaded; double LastUpdateTimeLength; double LastUpdateTime;
    bool CreatureGenerationEnabled, DontPopulateNextLoadedRegion, WaitingForInventoryTutorialToFinish;
    long FrameCachedLODCentre; bool SerialisingNonPersistentQuestItems; bool DisplayMiniMapOnRegionLoad;
    CAtmosCopyList AtmosBanksWaitingToCopy; double StartTime; double TimePlayed; bool HasInitialisedStartTime;
    CWorldModel(const CWorldInit& init)
        : Component(init.Component), PlayerManager(init.PlayerManager), DefinitionManager(init.DefinitionManager),
          PCurrentGameCamera(0), JustLoaded(false), LastUpdateTimeLength(0.05f), LastUpdateTime(0.0),
          CreatureGenerationEnabled(true), DontPopulateNextLoadedRegion(false), WaitingForInventoryTutorialToFinish(false),
          FrameCachedLODCentre(-1), SerialisingNonPersistentQuestItems(true), DisplayMiniMapOnRegionLoad(true),
          HasInitialisedStartTime(false)
    {
        StartTime = GFGetTime();
        TimePlayed = 0.0;
    }
};
int main()
{
    int a, b, c, d;
    CWorldInit init = { &a, &b, &c, &d };
    CWorldModel w(init);
    int ok = w.Component == &a && w.PlayerManager == &c && w.DefinitionManager == &d;
    ok = ok && w.PWorldMap.ptr == 0 && w.PAtmosProcessor.Data == 0 && w.PAtmosProcessor.Info == 0 && w.PCurrentGameCamera == 0;
    ok = ok && w.LastUpdateTimeLength == (double)0.05f && w.LastUpdateTime == 0.0 && w.FrameCachedLODCentre == -1;
    ok = ok && w.CreatureGenerationEnabled && !w.DontPopulateNextLoadedRegion && w.SerialisingNonPersistentQuestItems;
    ok = ok && w.AtmosBanksWaitingToCopy.node->next == w.AtmosBanksWaitingToCopy.node && w.AtmosBanksWaitingToCopy.node->prev == w.AtmosBanksWaitingToCopy.node;
    ok = ok && w.StartTime == 123.5 && w.TimePlayed == 0.0 && !w.HasInitialisedStartTime;
    if (!ok) { std::printf("004a67d0_TEST FAIL\n"); return 1; }
    std::printf("004a67d0_TEST PASS\n");
    return 0;
}
