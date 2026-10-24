// behaviour test for 0x00BD9FD0 CEngineLandscapeMemoryManager::CEngineLandscapeMemoryManager(const CEngineMemoryInit&)
#include <cstdio>
#include <cstdlib>

struct NodeBase { NodeBase* Next; NodeBase* Prev; };
struct Node : NodeBase { void* Data; };

struct PtrList {
    Node* Sentinel;
    PtrList() : Sentinel(0)
    {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->Next = n;
        n->Prev = n;
        Sentinel = n;
    }
    ~PtrList() { std::free(Sentinel); }
    bool empty() const { return Sentinel->Next == Sentinel; }
};

struct MemoryInit { int Unused; };

struct FailedAllocationHandler { virtual ~FailedAllocationHandler() { } virtual bool HandleFailedAllocation(unsigned int) = 0; };
struct MovableResource { MovableResource() { } virtual ~MovableResource() { } virtual void RelocateData() = 0; };

struct LandscapeMemoryManager : FailedAllocationHandler, MovableResource {
    void* VirtualMemPool;
    void* PhysicalMemPool;
    PtrList TextureDisposalList[3];
    PtrList VertexBufferDisposalList[3];
    PtrList IndexBufferDisposalList[3];
    int CurrentDisposalListIndex;
    bool InsideDefragment;
    bool PoolAllocationEnabled;
    int AllocationCount;
    bool EnableDelayedResourceDestruction;
    LandscapeMemoryManager(const MemoryInit&)
    {
        InsideDefragment = false;
        PoolAllocationEnabled = false;
        AllocationCount = 0;
        EnableDelayedResourceDestruction = false;
        CurrentDisposalListIndex = 0;
    }
    bool HandleFailedAllocation(unsigned int) { return false; }
    void RelocateData() { }
};

int main()
{
    MemoryInit init; init.Unused = 7;
    LandscapeMemoryManager* m = new LandscapeMemoryManager(init);
    for (int i = 0; i < 3; ++i) {
        if (!m->TextureDisposalList[i].empty() || !m->VertexBufferDisposalList[i].empty() || !m->IndexBufferDisposalList[i].empty()) {
            std::printf("00bd9fd0_TEST FAIL\n"); return 1;
        }
        if (m->TextureDisposalList[i].Sentinel->Prev != m->TextureDisposalList[i].Sentinel) { std::printf("00bd9fd0_TEST FAIL\n"); return 1; }
    }
    if (m->TextureDisposalList[0].Sentinel == m->TextureDisposalList[1].Sentinel) { std::printf("00bd9fd0_TEST FAIL\n"); return 1; }
    if (m->CurrentDisposalListIndex != 0 || m->InsideDefragment || m->PoolAllocationEnabled || m->AllocationCount != 0 ||
        m->EnableDelayedResourceDestruction) { std::printf("00bd9fd0_TEST FAIL\n"); return 1; }
    FailedAllocationHandler* h = m;
    MovableResource* r = m;
    if ((void*)h != (void*)m || (void*)r == (void*)m) { std::printf("00bd9fd0_TEST FAIL\n"); return 1; }
    delete m;
    std::printf("00bd9fd0_TEST PASS\n");
    return 0;
}