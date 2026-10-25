// Behaviour test for CResourceList::`scalar deleting destructor' (0x009FC700): the embedded Head resource asks its list
// to remove it only while it is listed.
#include <cstdio>

class CResourceList;
static int g_removed = 0; static const void* g_removedWhat = 0;

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() {}
    unsigned long IVRefCount;
};
class CResource : public CIVCountedPointeeBase {
public:
    virtual ~CResource();
    CResourceList* ResourceList;
    CResource*     PrevResource;
    CResource*     NextResource;
    unsigned long  ResourceSize;
    unsigned long  LastUsedFrame;
};
class CFailedAllocationHandler {
public:
    virtual ~CFailedAllocationHandler() {}
};
class CResourceList : public CFailedAllocationHandler {
public:
    virtual ~CResourceList();
    void RemoveResource(CResource& resource) { ++g_removed; g_removedWhat = &resource; resource.ResourceList = 0; }
    CResource     Head;
    unsigned long ResourceCount;
    long          AllocatedMemory;
    long          MaximumMemory;
    unsigned long CurrentFrame;
    unsigned long DebugStatsFrame;
    unsigned long UnloadDelay;
    unsigned long UnloadedThisFrame;
};
inline CResource::~CResource()
{
    if (ResourceList)
        ResourceList->RemoveResource(*this);
}
CResourceList::~CResourceList()
{
}

int main()
{
    CResourceList* listed = new CResourceList;
    listed->Head.ResourceList = listed;
    const void* head = &listed->Head;
    CFailedAllocationHandler* base = listed;
    delete base;
    if (g_removed != 1 || g_removedWhat != head) { std::printf("009fc700_TEST FAIL\n"); return 1; }
    CResourceList* unlisted = new CResourceList;
    unlisted->Head.ResourceList = 0;
    delete unlisted;
    if (g_removed != 1) { std::printf("009fc700_TEST FAIL\n"); return 1; }
    std::printf("009fc700_TEST PASS\n");
    return 0;
}