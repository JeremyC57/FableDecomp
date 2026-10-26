// CEngineLightingManager::CManagerLocalLight::Delete() -- `delete this`: the trivially destructible light record goes back on its class heap (the static operator delete, CDynamicObjectHeap::Free, inline)
// Leaf: ?Delete@CManagerLocalLight@CEngineLightingManager@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B49A40, 41 bytes (+ int3 padding to 48), ecx = the block, plain `ret` or a
//   tail jump to CDynamicObjectHeapBase::RefCountedRelease.
// Identity (proven): CEngineLightingManager::CManagerLocalLight::Delete(); evidence: the address is slot 0, the only slot, of vtable 0x012A2228 (RTTI .?AUCManagerLocalLight@CEngineLightingManager@@, no base class); the PDBs give the struct exactly one virtual, ?Delete@CManagerLocalLight@CEngineLightingManager@@UAEXXZ, beside its static ??3/??2 pool operators; the body frees `this` into CDynamicObjectHeap<72, 16> (RTTI of the stored heap vtable .?AV?$CDynamicObjectHeap@$0EI@$0BA@@@), whose object size 72 equals Ego_r.pdb sizeof CEngineLightingManager::CManagerLocalLight = 72
// Label: the manifest's "CEngineLightingManager::CManagerLocalLight::vfunc_0" is propagated (family J025: six pool frees of one shape, each on its own static heap;
//   retail has no identical-code folding, so each address is one class's function).
// Callees: CDynamicObjectHeapBase::RefCountedRelease() 0x00F36CA0 (?RefCountedRelease@CDynamicObjectHeapBase@@QAEXXZ, FableWin.pdb + Ego_r.pdb; landed).
// Layout: CDynamicObjectHeapBase (Ego_r.pdb, 0x14): vptr, AllocCount +4, TotalAllocatorBlocks +8, RefCountReleaseEnabled +0xC,
//   HeapNext +0x10; CDynamicObjectHeap<N,M>::FreeList +0x14 (the landed heap constructors 0x00AF8700 / 0x00AF8B40). The heap is
//   a static object at 0x01436FB0 (CDynamicObjectHeap<72, 16>, built by the static initialiser 0x01224730 that stores vtable 0x012A20F0); no PDB
//   data symbol names it, so its identifier `LocalLightHeap` is descriptive.
// Shape: CDynamicObjectHeap::Free (FableWin.pdb ?Free@?$CDynamicObjectHeap@...@@QAEXPAX@Z) inline: AllocCount decremented, the
//   block pushed on the free list, RefCountedRelease tail-called when the count reaches 0 and release is enabled; no destructor runs (the struct is trivially destructible) and `delete this` has no null test.
struct CObjectEntry { CObjectEntry* NextEntry; };

class CDynamicObjectHeapBase {                                  // retail 0x14 bytes, RTTI .?AVCDynamicObjectHeapBase@@
public:
    virtual ~CDynamicObjectHeapBase();                          // 0x00F36C90
    long                    AllocCount;                         // +0x04
    long                    TotalAllocatorBlocks;               // +0x08
    bool                    RefCountReleaseEnabled;             // +0x0c
    CDynamicObjectHeapBase* HeapNext;                           // +0x10
    void RefCountedRelease();                                   // 0x00F36CA0
};

template <long ObjectSize, long ObjectsPerBlock> class CDynamicObjectHeap : public CDynamicObjectHeapBase {
public:
    CObjectEntry* FreeList;                                     // +0x14
    void Free(void* object)
    {
        --AllocCount;
        ((CObjectEntry*)object)->NextEntry = FreeList;
        FreeList = (CObjectEntry*)object;
        if (AllocCount == 0 && RefCountReleaseEnabled)
            RefCountedRelease();
    }
};

extern CDynamicObjectHeap<72, 16> LocalLightHeap;                     // 0x01436FB0

class CEngineLightingManager {
public:
    struct CManagerLocalLight {                                  // RTTI .?AUCManagerLocalLight@CEngineLightingManager@@, vtable 0x012A2228 (one slot)
        virtual void Delete();                                  // slot 0
        static void __fastcall operator delete(void* object) { LocalLightHeap.Free(object); }   // ??3CManagerLocalLight@CEngineLightingManager@@SIXPAX@Z
    };
};

void CEngineLightingManager::CManagerLocalLight::Delete()
{
    delete this;
}