// CEngineDynamic3DResourceManager::CEngineDynamic3DResourceManager() -- constructor of the dynamic vertex-buffer resource manager: own vtable, empty resource tree, zero counters, tree keyed on three words
// Leaf: ??0?$CEngineDynamic3DResourceManager@V?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCA0D0, 45 bytes, __fastcall `ret`, returns `this`.
// Identity (label correction): the catalog's "pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>" is
//   propagated. The vtable store 0x012A69DC is RTTI .?AV?$CEngineDynamic3DResourceManager@V?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@@@;
//   FableWin.pdb lists its ??0 and the CEngineTreeFiler<CListHeader> member; Ego_r.pdb layout: vptr, AllocCount +4,
//   FreePoolMem +8, TotalMemAllocated +0xC, ResourceMappingTree +0x10 (24 bytes). The same body is inlined in the
//   derived CEngineVertexBufferPool constructor 0x00BCA4D0 and in the landed SpriteTrails manager 0x00BCA770.
//   No direct callers (an emitted COMDAT of the template).
// Callees: CEngineTreeFilerBase::CEngineTreeFilerBase() @ 0x00BF2880, CEngineTreeFilerBase::Initialise(long) @ 0x00BF3180.
// Shape: vtable store, the tree filer member constructed (its template constructor inline over the out-of-line base),
//   then the body zeroes the three counters from one register and calls Initialise(3) (the zero stores land between
//   `push 3` and the call). The vtable's three slots are the PDB's three virtuals (order not provable; masked).
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

template <class TResource> struct CEngineDynamic3DResourceManager {
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

template CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32, 3> >::CEngineDynamic3DResourceManager();