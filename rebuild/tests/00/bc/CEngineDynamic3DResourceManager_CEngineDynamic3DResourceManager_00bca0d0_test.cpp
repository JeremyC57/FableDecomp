// Behaviour test for 0x00BCA0D0 CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32,3> >::CEngineDynamic3DResourceManager().
#include <cstdio>
#include <cstring>

class CEngineTreeFilerBase {
public:
    void* BaseNode;
    long  KeySize;
    CEngineTreeFilerBase() : BaseNode(0), KeySize(-1) {}
    void Initialise(long keySize) { KeySize = keySize; }
};
template <class T> class CEngineTreeFiler : public CEngineTreeFilerBase { };
template <class TResource> struct CEngineDynamic3DResourceManager {
public:
    struct CListHeader { };
    long AllocCount;
    long FreePoolMem;
    long TotalMemAllocated;
    CEngineTreeFiler<CListHeader> ResourceMappingTree;
    CEngineDynamic3DResourceManager()
    {
        AllocCount = 0; FreePoolMem = 0; TotalMemAllocated = 0;
        ResourceMappingTree.Initialise(3);
    }
    virtual ~CEngineDynamic3DResourceManager() {}
    virtual long GetTotalBytesAllocated() const { return TotalMemAllocated; }
    virtual long GetTotalBytesFree() const { return FreePoolMem; }
};
struct CVertexBufferResource { };

int main()
{
    CEngineDynamic3DResourceManager<CVertexBufferResource> m;
    if (!(m.AllocCount == 0 && m.FreePoolMem == 0 && m.TotalMemAllocated == 0 && m.ResourceMappingTree.BaseNode == 0 && m.ResourceMappingTree.KeySize == 3 && m.GetTotalBytesAllocated() == 0)) {
        std::printf("00bca0d0_TEST FAIL\n");
        return 1;
    }
    std::printf("00bca0d0_TEST PASS\n");
    return 0;
}