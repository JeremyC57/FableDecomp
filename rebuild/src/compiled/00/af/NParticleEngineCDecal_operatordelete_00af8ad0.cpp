// NParticleEngine::CDecal::operator delete(void* object) -- the class's static pool free: the block goes back on the Decal heap's free list (CDynamicObjectHeap::Free inline)
// Leaf: ??3CDecal@NParticleEngine@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8AD0, 41 bytes (+ int3 padding to 48), ecx = the block, plain `ret` or a
//   tail jump to CDynamicObjectHeapBase::RefCountedRelease.
// Identity (proven): NParticleEngine::CDecal::operator delete(void*); evidence: FableWin.pdb names ??3CDecal@NParticleEngine@@SIXPAX@Z (static __fastcall operator delete) and its sibling ??2 operator new; its callers (0x00AF5690, 0x00AF5770, 0x00AF7C80, 0x00AF7CA0) include 0x00AF7CA0, slot 0 of vtable 0x0129F4FC (RTTI .?AVCDecal@NParticleEngine@@), which frees `this` through this address; the heap is CDynamicObjectHeap<116, 529> (RTTI of the stored vtable .?AV?$CDynamicObjectHeap@$0HE@$0CBB@@@), whose object size 116 is the class's retail size (Ego_r.pdb sizeof CDecal = 124); the inline operator new on the same heap is 0x00AF8B00
// Label: the manifest's "operator_delete" is propagated (family J025: six pool frees of one shape, each on its own static heap;
//   retail has no identical-code folding, so each address is one class's function).
// Callees: CDynamicObjectHeapBase::RefCountedRelease() 0x00F36CA0 (?RefCountedRelease@CDynamicObjectHeapBase@@QAEXXZ, FableWin.pdb + Ego_r.pdb; landed).
// Layout: CDynamicObjectHeapBase (Ego_r.pdb, 0x14): vptr, AllocCount +4, TotalAllocatorBlocks +8, RefCountReleaseEnabled +0xC,
//   HeapNext +0x10; CDynamicObjectHeap<N,M>::FreeList +0x14 (the landed heap constructors 0x00AF8700 / 0x00AF8B40). The heap is
//   a static object at 0x01427E20 (CDynamicObjectHeap<116, 529>, built by the static initialiser 0x01224590 that stores vtable 0x0129F650); no PDB
//   data symbol names it, so its identifier `DecalHeap` is descriptive.
// Shape: CDynamicObjectHeap::Free (FableWin.pdb ?Free@?$CDynamicObjectHeap@...@@QAEXPAX@Z) inline: AllocCount decremented, the
//   block pushed on the free list, RefCountedRelease tail-called when the count reaches 0 and release is enabled; the out-of-line static `__fastcall` operator delete takes the block in ecx (the deleting destructor 0x00AF7CA0 calls it with `mov ecx, esi`).
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

extern CDynamicObjectHeap<116, 529> DecalHeap;                     // 0x01427E20

namespace NParticleEngine {

class CDecal {                     // RTTI .?AVCDecal@NParticleEngine@@ (vtable 0x0129F4FC)
public:
    static void __fastcall operator delete(void* object);   // ??3CDecal@NParticleEngine@@SIXPAX@Z
};

void __fastcall CDecal::operator delete(void* object)
{
    DecalHeap.Free(object);
}

} // namespace NParticleEngine