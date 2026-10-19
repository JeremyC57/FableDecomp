// CContainmentVolume::~CContainmentVolume() -- release the plane array: through the movable-resource
// pool when one owns it (handle looked up from the block, then freed), else `delete[]`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A752F0, length 77 bytes (+ int3 padding).
// Callees: CMovableResourceMemoryPool::GetHandle(void*) @ 0x00A01D40 (reads the handle stored
// 0x1c bytes before the block; the manifest's "CWorld::DrawGetEnvironment" label is propagated),
// CHandleBasedMemoryPool::Free(long) @ 0x00A62ED0 (the pool's base method, as in the landed
// CBankFileAsyncData::FreeBuffer), operator delete[] @ 0x00BFEB1C.
// Vtables: 0x0129DC94 = CContainmentVolume (stored first), 0x0129B860 = CMovableResource (the
// inlined empty base destructor's store, last). engine/CContainmentVolume.h (PDB) gives the
// layout used here -- CPlane* ContainmentPlanes +0x04, unsigned long Size +0x08,
// CMovableResourceMemoryPool* MemoryPool +0x0c -- but it is a flat POD view, so the polymorphic
// base/derived pair is modelled locally with the same member names.
// Shape: the plane pointer is pushed once before the pool test (shared argument of both calls),
// the pool pointer is re-read for the Free call, then the pointer and the size are cleared.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
struct CPlane {                                // C3DVector Normal + float Distance
    float Normal_X;
    float Normal_Y;
    float Normal_Z;
    float Distance;
};

struct CMovableResourceMemoryPool_Calls {
    long GetHandle(void* block);               // 0x00A01D40
    void Free(long handle);                    // 0x00A62ED0 (CHandleBasedMemoryPool::Free)
};

struct CMovableResource {                      // vtable 0x0129B860; engine/CMovableResource.h: vptr only
    virtual void v00();
    ~CMovableResource() { }
};

struct CContainmentVolume : CMovableResource { // vtable 0x0129DC94
    CPlane*                           ContainmentPlanes;   // +0x04
    unsigned long                     Size;               // +0x08
    CMovableResourceMemoryPool_Calls* MemoryPool;         // +0x0c
    virtual void v00();
    ~CContainmentVolume();
};

CContainmentVolume::~CContainmentVolume()
{
    if (ContainmentPlanes) {
        if (MemoryPool) {
            MemoryPool->Free(MemoryPool->GetHandle(ContainmentPlanes));
            MemoryPool = 0;
        } else {
            delete[] ContainmentPlanes;
        }
        ContainmentPlanes = 0;
        Size = 0;
    }
}