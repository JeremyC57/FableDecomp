// Behaviour test for CLocalDetailCacheMap::CQuadTreeElement @ 0x00BDE020
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
    CEngineStaticMapFileBlockManager manager;
    CQuadTreeElement* e = new CQuadTreeElement;
    e->Loading = true; e->StaticContentsLoaded = true; e->HasDynamicAreasFlag = true; e->FileBlockPos = 1234;
    e->Children[0] = new CQuadTreeElement; e->Children[2] = new CQuadTreeElement;
    e->ObjectCacheGroupCollections = new CObjectCacheGroupCollection(new CObjectCacheGroupCollection(0));
    e->UnloadStaticContents(&manager);
    if (g_releasedPos != 1234 || e->Loading || e->StaticContentsLoaded || !e->HasDynamicAreasFlag) { std::printf("00bde020_TEST FAIL\n"); return 1; }
    if (g_childrenDeleted != 2 || g_collectionsDeleted != 2 || e->Children[0] != 0 || e->Children[2] != 0 || e->ObjectCacheGroupCollections != 0) { std::printf("00bde020_TEST FAIL\n"); return 1; }
    // nothing loaded: nothing released, nothing deleted
    g_releasedPos = -1;
    e->UnloadStaticContents(&manager);
    if (g_releasedPos != -1 || g_childrenDeleted != 2) { std::printf("00bde020_TEST FAIL\n"); return 1; }
    delete e;
    std::printf("00bde020_TEST PASS\n");
    return 0;
}