// Behaviour test for CLocalDetailCacheMap::CQuadTreeElement @ 0x00BDDCB0
#include <cstdio>

static int g_collectionsDeleted = 0;
static int g_childrenDeleted = 0;
static int g_rebinds = 0;
static void* g_rebindBlocks[16];
static long g_releasedPos = -1;

struct CObjectCacheGroupCollection {
    CObjectCacheGroupCollection* Next;
    CObjectCacheGroupCollection(CObjectCacheGroupCollection* next) : Next(next) { }
    virtual ~CObjectCacheGroupCollection() { ++g_collectionsDeleted; }
};

struct CEngineStaticMapFileBlockManager {
    void ReleaseRequest(long fileBlockPos) { g_releasedPos = fileBlockPos; }
};

struct CEngineLandscapeMemoryManager {
    void RebindBlockOwner(void* block, void* owner) { (void)owner; if (g_rebinds < 16) g_rebindBlocks[g_rebinds] = block; ++g_rebinds; }
};
static CEngineLandscapeMemoryManager g_landscapeMemory;
struct SEngine { CEngineLandscapeMemoryManager* m_pLandscapeMemory; };
static SEngine g_engine = { &g_landscapeMemory };
static SEngine* g_pEngine_01436ea8 = &g_engine;

struct CQuadTreeElement {
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;
    CQuadTreeElement*  Children[4];
    long               FileBlockPos;
    bool               HasDynamicAreasFlag : 1;
    bool               RenderUpdateNeeded : 1;
    bool               StaticContentsLoaded : 1;
    bool               Loading : 1;
    CQuadTreeElement() : ObjectCacheGroupCollections(0), FileBlockPos(0)
    {
        Children[0] = Children[1] = Children[2] = Children[3] = 0;
        HasDynamicAreasFlag = false; RenderUpdateNeeded = false; StaticContentsLoaded = false; Loading = false;
    }
    virtual ~CQuadTreeElement() { ++g_childrenDeleted; }
    void RemoveObjectCollections()
    {
        while (ObjectCacheGroupCollections) {
            CObjectCacheGroupCollection* next = ObjectCacheGroupCollections->Next;
            delete ObjectCacheGroupCollections;
            ObjectCacheGroupCollections = next;
        }
    }
    void OnPostRelocation()
    {
        CEngineLandscapeMemoryManager* manager = g_pEngine_01436ea8->m_pLandscapeMemory;
        for (long i = 0; i < 4; ++i) {
            if (Children[i])
                manager->RebindBlockOwner(Children[i], this);
        }
        for (CObjectCacheGroupCollection* collection = ObjectCacheGroupCollections; collection; collection = collection->Next)
            manager->RebindBlockOwner(collection, this);
    }
    void UnloadStaticContents(CEngineStaticMapFileBlockManager* manager)
    {
        if (Loading) {
            manager->ReleaseRequest(FileBlockPos);
            Loading = false;
        }
        if (StaticContentsLoaded) {
            RemoveObjectCollections();
            for (long i = 0; i < 4; ++i) {
                delete Children[i];
                Children[i] = 0;
            }
            StaticContentsLoaded = false;
        }
    }
};

int main()
{
    CQuadTreeElement e, c1, c3;
    e.Children[1] = &c1; e.Children[3] = &c3;
    CObjectCacheGroupCollection last(0), first(&last);
    e.ObjectCacheGroupCollections = &first;
    e.OnPostRelocation();
    if (g_rebinds != 4 || g_rebindBlocks[0] != &c1 || g_rebindBlocks[1] != &c3 || g_rebindBlocks[2] != &first || g_rebindBlocks[3] != &last) { std::printf("00bddcb0_TEST FAIL\n"); return 1; }
    std::printf("00bddcb0_TEST PASS\n");
    return 0;
}