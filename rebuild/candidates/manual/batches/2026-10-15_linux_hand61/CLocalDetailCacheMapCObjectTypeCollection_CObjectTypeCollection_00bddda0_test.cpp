#include <cstdio>
// Behaviour test for the local-detail cache map collection teardown @ 0x00BDDDA0
#define FAIL { std::printf("00bddda0_TEST FAIL\n"); return 1; }
static int g_primitivesDeleted = 0, g_collectionsDeleted = 0, g_movableDtors = 0;
static long g_releasedPos = -1;
class CEngineStaticMapFileBlockManager;
static CEngineStaticMapFileBlockManager* g_releaseManager = 0;
class CEngineStaticMapFileBlockManager {
public:
    void ReleaseRequest(long fileBlockPos) { g_releasedPos = fileBlockPos; g_releaseManager = this; }
};
class CMovableResource { public: virtual ~CMovableResource() { ++g_movableDtors; } };
class CLocalDetailPrimitiveBase {
public:
    CLocalDetailPrimitiveBase() : Next(0) {}
    virtual ~CLocalDetailPrimitiveBase() { ++g_primitivesDeleted; }
    CLocalDetailPrimitiveBase* Next;
};
class CLocalDetailCacheMap {
public:
    bool LockedFromMemoryRelocation;
    CEngineStaticMapFileBlockManager* FileBlockManager;
    class CObjectTypeCollection : public CMovableResource {
    public:
        CObjectTypeCollection() : Primitives(0), Next(0) {}
        CLocalDetailPrimitiveBase* Primitives;
        CObjectTypeCollection* Next;
        virtual ~CObjectTypeCollection()
        {
            ++g_collectionsDeleted;
            while (Primitives) {
                CLocalDetailPrimitiveBase* next = Primitives->Next;
                delete Primitives;
                Primitives = next;
            }
        }
    };
    class CObjectCacheGroupCollection : public CMovableResource {
    public:
        CObjectCacheGroupCollection() : ObjectTypeCollections(0), Map(0), FileBlockPos(0), RenderUpdateNeeded(false), Loading(false) {}
        CObjectTypeCollection* ObjectTypeCollections;
        CLocalDetailCacheMap* Map;
        long FileBlockPos;
        bool RenderUpdateNeeded : 1;
        bool Loading : 1;
        void ClearLoadRequest(CEngineStaticMapFileBlockManager* manager)
        {
            if (Loading) { manager->ReleaseRequest(FileBlockPos); Loading = false; }
        }
        void UnloadContents(CEngineStaticMapFileBlockManager* manager)
        {
            ClearLoadRequest(manager);
            while (ObjectTypeCollections) {
                CObjectTypeCollection* next = ObjectTypeCollections->Next;
                delete ObjectTypeCollections;
                ObjectTypeCollections = next;
            }
        }
        virtual ~CObjectCacheGroupCollection()
        {
            ClearLoadRequest(Map->FileBlockManager);
            while (ObjectTypeCollections) {
                CObjectTypeCollection* next = ObjectTypeCollections->Next;
                delete ObjectTypeCollections;
                ObjectTypeCollections = next;
            }
        }
    };
};
static CLocalDetailPrimitiveBase* MakeChain(int n)
{
    CLocalDetailPrimitiveBase* head = 0;
    for (int i = 0; i < n; ++i) { CLocalDetailPrimitiveBase* p = new CLocalDetailPrimitiveBase; p->Next = head; head = p; }
    return head;
}
static CLocalDetailCacheMap::CObjectTypeCollection* MakeCollections(int n)
{
    CLocalDetailCacheMap::CObjectTypeCollection* head = 0;
    for (int i = 0; i < n; ++i) { CLocalDetailCacheMap::CObjectTypeCollection* c = new CLocalDetailCacheMap::CObjectTypeCollection; c->Next = head; head = c; }
    return head;
}
int main()
{

    {
        CLocalDetailCacheMap::CObjectTypeCollection* c = new CLocalDetailCacheMap::CObjectTypeCollection;
        c->Primitives = MakeChain(3);
        delete c;
    }
    if (g_primitivesDeleted != 3 || g_movableDtors != 1) FAIL;
    {
        CLocalDetailCacheMap::CObjectTypeCollection empty; empty.Primitives = 0;
    }
    if (g_primitivesDeleted != 3 || g_movableDtors != 2) FAIL;
    std::printf("00bddda0_TEST PASS\n");
    return 0;
}
