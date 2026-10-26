// NParticleEngine::CRandomAcceleration::operator delete(void* object) -- the class's static pool free: the block goes back on the RandomAcceleration heap's free list (CDynamicObjectHeap::Free inline)
// Leaf: ??3CRandomAcceleration@NParticleEngine@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8620, 41 bytes (+ int3 padding to 48), ecx = the block, plain `ret` or a
//   tail jump to CDynamicObjectHeapBase::RefCountedRelease.
// Identity (proven): NParticleEngine::CRandomAcceleration::operator delete(void*); evidence: FableWin.pdb names ??3CRandomAcceleration@NParticleEngine@@SIXPAX@Z (static __fastcall operator delete) and its sibling ??2 operator new; its callers (0x00B0F3A0) include 0x00B0F3A0, slot 0 of vtable 0x0129FC08 (RTTI .?AVCRandomAcceleration@NParticleEngine@@, bases CBaseClassNonCopyable <- CBaseClass), which frees `this` through this address; the heap is CDynamicObjectHeap<24, 853> (RTTI of the stored vtable .?AV?$CDynamicObjectHeap@$0BI@$0DFF@@@), whose object size 24 is the class's retail size (Ego_r.pdb sizeof CRandomAcceleration = 24); the inline operator new on the same heap is 0x00AF8680
// Label: the manifest's "operator_delete" is propagated (family J025: six pool frees of one shape, each on its own static heap;
//   retail has no identical-code folding, so each address is one class's function).
// Callees: CDynamicObjectHeapBase::RefCountedRelease() 0x00F36CA0 (?RefCountedRelease@CDynamicObjectHeapBase@@QAEXXZ, FableWin.pdb + Ego_r.pdb; landed).
// Layout: CDynamicObjectHeapBase (Ego_r.pdb, 0x14): vptr, AllocCount +4, TotalAllocatorBlocks +8, RefCountReleaseEnabled +0xC,
//   HeapNext +0x10; CDynamicObjectHeap<N,M>::FreeList +0x14 (the landed heap constructors 0x00AF8700 / 0x00AF8B40). The heap is
//   a static object at 0x013D2E38 (CDynamicObjectHeap<24, 853>, built by the static initialiser 0x012244F0 that stores vtable 0x0129F638); no PDB
//   data symbol names it, so its identifier `RandomAccelerationHeap` is descriptive.
// Shape: CDynamicObjectHeap::Free (FableWin.pdb ?Free@?$CDynamicObjectHeap@...@@QAEXPAX@Z) inline: AllocCount decremented, the
//   block pushed on the free list, RefCountedRelease tail-called when the count reaches 0 and release is enabled; the out-of-line static `__fastcall` operator delete takes the block in ecx (the deleting destructor 0x00B0F3A0 calls it with `mov ecx, esi`).
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

extern CDynamicObjectHeap<24, 853> RandomAccelerationHeap;                     // 0x013D2E38

class CBaseClass {
public:
    CBaseClass();                                               // 0x0099A2D0
    virtual ~CBaseClass();                                      // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                    // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                           // 0x0099A300
};
namespace NParticleEngine {

class CRandomAcceleration : public CBaseClassNonCopyable {                     // RTTI .?AVCRandomAcceleration@NParticleEngine@@ (vtable 0x0129FC08)
public:
    static void __fastcall operator delete(void* object);   // ??3CRandomAcceleration@NParticleEngine@@SIXPAX@Z
};

void __fastcall CRandomAcceleration::operator delete(void* object)
{
    RandomAccelerationHeap.Free(object);
}

} // namespace NParticleEngine