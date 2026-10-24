#include <cstdio>
#include <cstdlib>

// Model of CEngineSurfaceCompositionManager::AddSurface: first use creates the cache, keeps a counted copy in the
// cached-surface list and registers the resource; a request that already has a cache is left alone.
struct CPointerInfo { unsigned int RefCount; };
struct CCache { int owner; };
struct CCountedPointer {
    CCache* Data; CPointerInfo* Info;
    void Reset(CCache* p) { Data = p; Info = (CPointerInfo*)std::malloc(sizeof(CPointerInfo)); Info->RefCount = 1; }
};
struct Node { Node* next; Node* prev; CCountedPointer value; };
struct Manager {
    int id;
    Node head;
    CCache* resources[4]; int resourceCount;
    void AddResource(CCache* r) { resources[resourceCount++] = r; }
    void AddSurface(CCountedPointer& cache)
    {
        if (!cache.Data) {
            CCache* c = (CCache*)std::malloc(sizeof(CCache)); c->owner = id;
            cache.Reset(c);
            Node* n = (Node*)std::malloc(sizeof(Node));
            n->value = cache; if (n->value.Info) ++n->value.Info->RefCount;
            Node* p = head.prev; n->next = &head; n->prev = p; p->next = n; head.prev = n;
            AddResource(cache.Data);
        }
    }
};

int main()
{
    Manager m; m.id = 7; m.head.next = m.head.prev = &m.head; m.resourceCount = 0;
    CCountedPointer info; info.Data = 0; info.Info = 0;
    m.AddSurface(info);
    if (!info.Data || info.Data->owner != 7 || info.Info->RefCount != 2 || m.head.next == &m.head || m.head.next->value.Data != info.Data || m.resourceCount != 1 || m.resources[0] != info.Data) { std::printf("00b2c460_TEST FAIL\n"); return 1; }
    m.AddSurface(info);
    if (m.resourceCount != 1 || m.head.next->next != &m.head || info.Info->RefCount != 2) { std::printf("00b2c460_TEST FAIL\n"); return 1; }
    std::free(m.head.next); std::free(info.Info); std::free(info.Data);
    std::printf("00b2c460_TEST PASS\n");
    return 0;
}