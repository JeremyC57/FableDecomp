// C3DMeshStats::AllocateBuffers(unsigned long, unsigned long, unsigned long) -- one movable-pool block for the helper points, dummy objects and mesh-volume bytes; false on overflow or allocation failure
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E760, length 192 bytes (`ret 0Ch`).
// MESH LAYER: Ego_r.pdb ?AllocateBuffers@C3DMeshStats@@AAE_NKKK@Z.
// Callees: CMovableResourceMemoryPool::AllocateResource(long*, CMovableResource*, unsigned long, unsigned long) @ 0x00A62E40,
// CHandleBasedMemoryPool::GetPointerFromHandle(long) @ 0x00A026B0.
// Layout (retail; PDB vector maps differ): MemoryPool +4, StatsMemoryHandle +8, HelperPoints data/count +0x3c/+0x40
// (C3DHelperPoint 0x14 bytes), DummyObjects data/count +0x4c/+0x50 (C3DDummyObject 0x38 bytes), the mesh-volume
// buffer +0x74 and its byte size (unsigned short) +0x78.
// Shape: the 64-bit size check (`total > 0xFFFFFFFF`), then the 32-bit byte counts recomputed; the buffers carved
// out of the block in helper, dummy, volume order; the volume pointer is the end of the dummy-object array, typed
// pointer arithmetic `(C3DDummyObject*)base + dummyObjects` (`add ebp,eax`: the byte offset as destination; a byte-offset
// add commutes to `add eax,ebp`).
// Leaf: AllocateBuffers@C3DMeshStats
struct CMovableResource { virtual void RelocateData(); };
struct CMovableResourceMemoryPool {
    bool AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long flags);   // 0x00A62E40
    void* GetPointerFromHandle(long handle);        // 0x00A026B0 (CHandleBasedMemoryPool)
};
struct C3DHelperPoint { unsigned char _body[0x14]; };
struct C3DDummyObject { unsigned char _body[0x38]; };

template<class T> struct CPreAllocBuffer {        // the pre-allocated vector map's storage words
    T* Data;
    unsigned long Count;
    void SetBuffer(void* data, unsigned long count) { Data = (T*)data; Count = count; }
};

struct C3DMeshStats : CMovableResource {
    CMovableResourceMemoryPool* MemoryPool;         // +0x04
    long StatsMemoryHandle;                         // +0x08
    unsigned char _0c[0x30];
    CPreAllocBuffer<C3DHelperPoint> HelperPoints;   // +0x3c data, +0x40 count
    unsigned char _44[0x08];
    CPreAllocBuffer<C3DDummyObject> DummyObjects;   // +0x4c data, +0x50 count
    unsigned char _54[0x20];
    unsigned char* MeshVolumeData;                  // +0x74
    unsigned short MeshVolumeDataSize;              // +0x78
    bool AllocateBuffers(unsigned long helperPoints, unsigned long dummyObjects, unsigned long meshVolumeBytes);
};

bool C3DMeshStats::AllocateBuffers(unsigned long helperPoints, unsigned long dummyObjects, unsigned long meshVolumeBytes)
{
    unsigned __int64 total = (unsigned __int64)dummyObjects * sizeof(C3DDummyObject)
                           + (unsigned __int64)helperPoints * sizeof(C3DHelperPoint) + meshVolumeBytes;
    MeshVolumeData = 0;
    MeshVolumeDataSize = (unsigned short)meshVolumeBytes;
    if (total > 0xffffffff)
        return false;
    unsigned long dummyBytes = dummyObjects * sizeof(C3DDummyObject);
    unsigned long helperBytes = helperPoints * sizeof(C3DHelperPoint);
    unsigned long size = helperBytes + dummyBytes + meshVolumeBytes;
    if (size) {
        if (!MemoryPool->AllocateResource(&StatsMemoryHandle, this, size, 0))
            return false;
        unsigned char* base = (unsigned char*)MemoryPool->GetPointerFromHandle(StatsMemoryHandle);
        HelperPoints.SetBuffer(base, helperPoints);
        base += helperBytes;
        DummyObjects.SetBuffer(base, dummyObjects);
        if (MeshVolumeDataSize)
            MeshVolumeData = (unsigned char*)((C3DDummyObject*)base + dummyObjects);
    }
    return true;
}