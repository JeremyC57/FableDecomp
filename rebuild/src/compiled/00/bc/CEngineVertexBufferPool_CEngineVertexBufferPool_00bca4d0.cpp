// CEngineVertexBufferPool::CEngineVertexBufferPool(long sizeGranularity) -- constructor of the engine's vertex-buffer pool: the dynamic resource manager base, then the allocation granularity
// Leaf: ??0CEngineVertexBufferPool@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCA4D0, 60 bytes, __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>" is
//   propagated. The final vtable store 0x012A69EC is RTTI .?AVCEngineVertexBufferPool@@; FableWin.pdb
//   ??0CEngineVertexBufferPool@@QAE@J@Z; Ego_r.pdb CEngineVertexBufferPool (28 bytes): the
//   CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32,3>> base (24 bytes, out-of-line twin
//   0x00BCA0D0) and SizeGranularity +0x18. No direct callers here (other managers inline it, e.g. landed 0x00BCA770).
// Callees: CEngineTreeFilerBase::CEngineTreeFilerBase() @ 0x00BF2880, CEngineTreeFilerBase::Initialise(long) @ 0x00BF3180.
// Shape: the base template constructor inline (base vtable, tree filer, zero counters, Initialise(3)), then
//   SizeGranularity from the argument; VC7.1 schedules the derived vtable store last.
class CNodeHeader;
class CVertexBufferWin32;
template <class T, long N> class CEngineDynamic3DResource;

class CEngineTreeFilerBase {
public:
    CNodeHeader* BaseNode;                              // +0x00
    long         KeySize;                               // +0x04
    CEngineTreeFilerBase();                             // 0x00BF2880 (BaseNode = 0)
    void Initialise(long keySize);                      // 0x00BF3180
};
template <class T> class CEngineTreeFiler : public CEngineTreeFilerBase {
public:
    CEngineTreeFiler() {}
};

template <class TResource> class CEngineDynamic3DResourceManager {
public:
    struct CListHeader;
    long AllocCount;                                    // +0x04
    long FreePoolMem;                                   // +0x08
    long TotalMemAllocated;                             // +0x0C
    CEngineTreeFiler<CListHeader> ResourceMappingTree;  // +0x10
    CEngineDynamic3DResourceManager()
    {
        AllocCount = 0;
        FreePoolMem = 0;
        TotalMemAllocated = 0;
        ResourceMappingTree.Initialise(3);
    }
    virtual long GetTotalBytesAllocated() const;        // vtable 0x012A69DC (three slots)
    virtual long GetTotalBytesFree() const;
private:
    virtual void FreeMemory();
};

class CEngineVertexBufferPool : public CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32, 3> > {
public:
    long SizeGranularity;                               // +0x18
    CEngineVertexBufferPool(long sizeGranularity);
    virtual long GetTotalBytesAllocated() const;        // vtable 0x012A69EC
};

CEngineVertexBufferPool::CEngineVertexBufferPool(long sizeGranularity)
    : SizeGranularity(sizeGranularity)
{
}