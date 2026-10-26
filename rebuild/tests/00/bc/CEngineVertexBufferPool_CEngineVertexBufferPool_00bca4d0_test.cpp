// Behaviour test for 0x00BCA4D0 CEngineVertexBufferPool::CEngineVertexBufferPool(long sizeGranularity).
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
class CEngineVertexBufferPool : public CEngineDynamic3DResourceManager<CVertexBufferResource> {
public:
    long SizeGranularity;
    CEngineVertexBufferPool(long sizeGranularity) : SizeGranularity(sizeGranularity) {}
    virtual long GetTotalBytesAllocated() const { return TotalMemAllocated + 1; }
};

int main()
{
    CEngineVertexBufferPool p(0x10000);
    CEngineDynamic3DResourceManager<CVertexBufferResource>* base = &p;
    if (!(p.SizeGranularity == 0x10000 && p.AllocCount == 0 && p.ResourceMappingTree.KeySize == 3 && base->GetTotalBytesAllocated() == 1)) {
        std::printf("00bca4d0_TEST FAIL\n");
        return 1;
    }
    std::printf("00bca4d0_TEST PASS\n");
    return 0;
}