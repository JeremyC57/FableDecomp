#include <cstdio>

class CResourceList;

class CResource { public:
    long RefCount; CResourceList* ResourceList; CResource* PrevResource; CResource* NextResource;
    unsigned long ResourceSize; unsigned long LastUsedFrame; int resets;
    CResource() : RefCount(1), ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0), resets(0) { }
    virtual ~CResource() { }
    virtual unsigned long GetUnloadableMemory() { return ResourceSize; }
    virtual void ResetDebugMemoryInUse() { ++resets; }
    void ListInsert(CResource* prev, CResource* next)
    {
        PrevResource = prev; NextResource = next; prev->NextResource = this; NextResource->PrevResource = this;
    }
    void ListRemove()
    {
        if (NextResource) {
            if (PrevResource->NextResource == this && NextResource->PrevResource == this) {
                PrevResource->NextResource = NextResource;
                NextResource->PrevResource = PrevResource;
            }
            PrevResource = 0;
            NextResource = 0;
        }
    }
};

class CResourceList { public:
    CResource Head; unsigned long ResourceCount; long AllocatedMemory; long MaximumMemory; unsigned long CurrentFrame;
    unsigned long DebugStatsFrame; unsigned long UnloadDelay; unsigned long UnloadedThisFrame;
    CResourceList()
    {
        Head.PrevResource = &Head; Head.NextResource = &Head;
        ResourceCount = 0; CurrentFrame = 0; DebugStatsFrame = 0; AllocatedMemory = 0; MaximumMemory = 0x7fffffff;
        UnloadDelay = 1; UnloadedThisFrame = 0;
    }
    CResource* GetHead() { return &Head; }
    void AddResource(CResource* resource)
    {
        resource->ResourceList = this;
        resource->LastUsedFrame = CurrentFrame;
        resource->PrevResource = GetHead()->PrevResource;
        resource->NextResource = GetHead();
        resource->PrevResource->NextResource = resource;
        resource->NextResource->PrevResource = resource;
        ResourceCount++;
        AllocatedMemory += resource->ResourceSize;
    }
    void RemoveResource(CResource* resource)
    {
        resource->ResourceList = 0;
        resource->ListRemove();
        ResourceCount--;
        AllocatedMemory -= resource->ResourceSize;
    }
    unsigned long GetUnloadableMemory(unsigned long minUnusedFrames)
    {
        unsigned long total = 0;
        for (CResource* resource = Head.NextResource; resource != &Head; resource = resource->NextResource) {
            if (CurrentFrame - resource->LastUsedFrame < minUnusedFrames)
                break;
            total += resource->GetUnloadableMemory();
        }
        return total;
    }
    void ResetDebugStats()
    {
        for (CResource* resource = Head.NextResource; resource != &Head; resource = resource->NextResource)
            resource->ResetDebugMemoryInUse();
        DebugStatsFrame = CurrentFrame;
    }
};

// Walks the list from the head and checks it holds exactly `n` resources in this order, linked both ways.
static bool Order(CResourceList& l, CResource** want, int n)
{
    CResource* p = &l.Head;
    for (int i = 0; i < n; ++i) {
        CResource* q = p->NextResource;
        if (q != want[i] || q->PrevResource != p) return false;
        p = q;
    }
    return p->NextResource == &l.Head && l.Head.PrevResource == p;
}

class CResourceBank { public:
    CResourceList ResourceList; CResourceList LoadingQueue;
    void AddResourceToLoadingQueue(CResource* resource) { LoadingQueue.AddResource(resource); }
};

int main()
{
    CResourceBank bank; CResource a, b; a.ResourceSize = 3; b.ResourceSize = 9; bank.LoadingQueue.CurrentFrame = 4;
    bank.AddResourceToLoadingQueue(&a); bank.AddResourceToLoadingQueue(&b);
    CResource* want[2] = { &a, &b };
    if (!Order(bank.LoadingQueue, want, 2) || bank.LoadingQueue.ResourceCount != 2 || bank.LoadingQueue.AllocatedMemory != 12
        || b.ResourceList != &bank.LoadingQueue || b.LastUsedFrame != 4 || bank.ResourceList.ResourceCount
        || bank.ResourceList.Head.NextResource != &bank.ResourceList.Head)
    { std::printf("009fc3d0_TEST FAIL\n"); return 1; }
    std::printf("009fc3d0_TEST PASS\n");
    return 0;
}