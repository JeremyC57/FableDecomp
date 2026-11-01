// Behaviour test: CResourceBank::~CResourceBank (0x009FC690) -- LoadingQueue then ResourceList die; each list's Head
// resource unhooks itself from its owning list (RemoveResource) only when it has one.
#include <cstdio>
static int g_order[8]; static int g_n = 0;
class CResourceList;
class CResource {
public:
    CResourceList* ResourceList;
    CResource() : ResourceList(0) { }
    virtual ~CResource();
};
class CResourceList {
public:
    int id;
    CResource Head;
    CResourceList(int i) : id(i) { }
    virtual ~CResourceList() { }
    void RemoveResource(CResource& r) { g_order[g_n++] = id; r.ResourceList = 0; }
};
CResource::~CResource() { if (ResourceList) ResourceList->RemoveResource(*this); }
class CResourceBank {
public:
    CResourceList ResourceList;
    CResourceList LoadingQueue;
    CResourceBank() : ResourceList(1), LoadingQueue(2) { }
    virtual ~CResourceBank() { }
};
int main()
{
    CResourceList owner(7), owner2(9);
    {
        CResourceBank bank;
        bank.ResourceList.Head.ResourceList = &owner;
        bank.LoadingQueue.Head.ResourceList = &owner2;
    }
    if (g_n != 2 || g_order[0] != 9 || g_order[1] != 7) { std::printf("009fc690_TEST FAIL\n"); return 1; }
    {
        CResourceBank bank;                      // heads not linked: no RemoveResource
    }
    if (g_n != 2) { std::printf("009fc690_TEST FAIL\n"); return 1; }
    std::printf("009fc690_TEST PASS\n");
    return 0;
}