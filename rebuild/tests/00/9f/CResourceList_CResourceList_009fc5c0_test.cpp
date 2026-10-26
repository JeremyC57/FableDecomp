// behaviour test for 009fc5c0: ~CResourceList -- the embedded head resource is handed to its owning
// list's RemoveResource (which uncounts it) only when it is attached, then the bases unwind.
#include <cstdio>
class CResourceList;
static int g_removed = 0;
class CResource { public:
    unsigned long IVRefCount;
    CResourceList* ResourceList;
    CResource* PrevResource;
    CResource* NextResource;
    unsigned long ResourceSize;
    unsigned long LastUsedFrame;
    inline ~CResource();
};
class CResourceList { public:
    CResource Head;
    unsigned long ResourceCount;
    long AllocatedMemory;
    void RemoveResource(CResource* resource)
    {
        ++g_removed;
        resource->ResourceList = 0;
        ResourceCount--;
        AllocatedMemory -= resource->ResourceSize;
    }
    ~CResourceList() { }
};
inline CResource::~CResource()
{
    if (ResourceList)
        ResourceList->RemoveResource(this);
}
int main()
{
    int ok = 1;
    CResourceList owner;
    owner.Head.ResourceList = 0;
    owner.ResourceCount = 3;
    owner.AllocatedMemory = 100;
    {
        CResourceList list;
        list.Head.ResourceList = &owner;
        list.Head.ResourceSize = 40;
    }
    if (g_removed != 1 || owner.ResourceCount != 2 || owner.AllocatedMemory != 60) ok = 0;
    {
        CResourceList list;
        list.Head.ResourceList = 0;
    }
    if (g_removed != 1) ok = 0;
    if (!ok) { std::printf("009fc5c0_TEST FAIL\n"); return 1; }
    std::printf("009fc5c0_TEST PASS\n");
    return 0;
}