// CEngineDynamic3DResourceManager<class CEngineDynamic3DResource<class CVertexBufferWin32,3>>::FreeMemory() -- the vertex-buffer manager has no memory of its own to free
// Leaf: ?FreeMemory@?$CEngineDynamic3DResourceManager@V?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCA120, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 2 of CEngineVertexBufferPool's vtable 0x012A69EC (the slot holds this body in 2 vtable(s), owner CEngineDynamic3DResourceManager<class CEngineDynamic3DResource<class CVertexBufferWin32,3>>); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts FreeMemory(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?FreeMemory@?$CEngineDynamic3DResourceManager@V?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@@@EAEXXZ; `ret` matches the signature.
// Was: CEngineDynamic3DResourceManager::vfunc_2 (rebuild/src/compiled/00/bc/gapscan_sub00bca120_00bca120.cpp).
// No shared header for CEngineDynamic3DResourceManager<T> / CEngineDynamic3DResource<T, N>: minimal local definitions (RTTI: the manager
// has no base; Ego_r.pdb members AllocCount +0x04, FreePoolMem +0x08, TotalMemAllocated +0x0C, ResourceMappingTree +0x10, 0x18 bytes).
class CVertexBufferWin32;
template <class T, int N> class CEngineDynamic3DResource;
template <class TResource> class CEngineDynamic3DResourceManager {
public:
    virtual long GetTotalBytesFree() const;                  // slot 1
private:
    virtual void FreeMemory();                               // slot 2
public:
    long AllocCount;                                         // +0x04
    long FreePoolMem;                                        // +0x08
    long TotalMemAllocated;                                  // +0x0C
    unsigned char ResourceMappingTree[8];                    // +0x10 CEngineTreeFiler<CListHeader>
};
template <class TResource> void CEngineDynamic3DResourceManager<TResource>::FreeMemory()
{
}
template void CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32, 3> >::FreeMemory();
