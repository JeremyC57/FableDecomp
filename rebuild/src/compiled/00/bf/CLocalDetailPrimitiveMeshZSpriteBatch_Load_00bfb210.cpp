// CLocalDetailPrimitiveMeshZSpriteBatch::Load(CDataInputStream* stream) -- read the bounding box and sphere, the object count, allocate the 0x4c-byte objects from the landscape memory manager and read each one's matrix, scale and 16-byte block, then allocate and bulk-read the 16-byte z-sprite entries; false when either allocation fails
// Leaf: ?Load@CLocalDetailPrimitiveMeshZSpriteBatch
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFB210, length 896 bytes (+ int3 padding), `ret 4` (primitive vtable slot 8,
//   the catalog's "vfunc_8"; CObjectTypeCollection::Load 0x00BDE680 calls slot 8 as Load(stream)).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CEngineLandscapeMemoryManager::
//   AllocArrayOfObjects<CObject>(count, owner) 0x00BFBC80 (out of line), CEngineLandscapeMemoryManager::AllocVirtual
//   0x00BD9AF0 (inside the inlined AllocArray<CObjectEntry>), __allmul 0x00BFEB50 (the 64-bit size check).
//   The manager is [[0x01436EA8]+0x6b0].
// Layout (PDB): BoundingBox +0x08 (0x18), BoundingSphere +0x20 (0x10), Objects +0x38, ZSpriteEntries +0x3c, ObjectCount +0x40;
//   CObject is 0x4c bytes (CMatrix3x4 +0, a float +0x30, a 16-byte block +0x34).
// Shape: the inline Read of the landed CDataInputStream::ReadCompressed 0x00995FB0 (size <= 0 guard, `size <= remaining`);
//   the count comes from an inline ReadLong() (its fast path keeps the value in eax), the inlined AllocArray tests the
//   64-bit size as `if (valid) return Alloc(...); return 0;` (valid path falls through); the manager is read once into a
//   named local used by both allocations; the scale goes through a local homed in the dead
//   stream-argument slot.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { public:
    void*                __vftable;
    unsigned long        StreamPos;                          // +0x04
    long                 StreamSize;                         // +0x08
    const unsigned char* CurrentSourcePtr;                   // +0x0c
    unsigned long        SourceChunkStreamPos;               // +0x10
    long                 SourceChunkBytesRemaining;          // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    template <class T> __forceinline void Read(T& value) { Read(&value, sizeof(T)); }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
};

class C3DVector { public: float X, Y, Z; };
class C3DBoundingBox { public: C3DVector MinCoord, MaxCoord; };             // 0x18
class C3DBoundingSphere { public: C3DVector Centre; float Radius; };        // 0x10
class CMatrix3x4 { public: float E[12]; };                                  // 0x30
struct SObjectBlock { long m_words[4]; };                            // 0x10

struct CObject {                                                     // 0x4c
    CMatrix3x4   Matrix;                                             // +0x00
    float        Scale;                                              // +0x30
    SObjectBlock Block;                                              // +0x34
    unsigned char _pad44[8];
};
struct CObjectEntry { long m_words[4]; };                            // 0x10

class CEngineLandscapeMemoryManager { public:
    void* AllocVirtual(long bytes, void* owner);                     // 0x00BD9AF0
    template <class T> T* AllocArrayOfObjects(long count, void* owner);   // 0x00BFBC80 (T = CObject)
    template <class T> __forceinline T* AllocArray(long count, void* owner)
    {
        __int64 bytes = (__int64)count * sizeof(T);
        if (CDataInputStream::IsValidPosition(bytes))
            return (T*)AllocVirtual(count * sizeof(T), owner);
        return 0;
    }
};
struct SEngineLandscape {                                            // reached through the pointer at 0x01436EA8
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* MemoryManager;                    // +0x6b0
};
extern SEngineLandscape* g_pEngineLandscape;                         // 0x01436EA8

struct CLocalDetailPrimitiveMeshZSpriteBatch {
    virtual void v00();
    void*             Next;                                          // +0x04
    C3DBoundingBox    BoundingBox;                                   // +0x08
    C3DBoundingSphere BoundingSphere;                                // +0x20
    void*             Type;                                          // +0x30
    void*             Map;                                           // +0x34
    CObject*          Objects;                                       // +0x38
    CObjectEntry*     ZSpriteEntries;                                // +0x3c
    long              ObjectCount;                                   // +0x40
    bool Load(CDataInputStream* stream);
};

bool CLocalDetailPrimitiveMeshZSpriteBatch::Load(CDataInputStream* stream)
{
    stream->Read(BoundingBox);
    stream->Read(BoundingSphere);
    ObjectCount = stream->ReadLong();
    CEngineLandscapeMemoryManager* manager = g_pEngineLandscape->MemoryManager;
    Objects = manager->AllocArrayOfObjects<CObject>(ObjectCount, this);
    if (Objects == 0)
        return false;
    for (long i = 0; i < ObjectCount; ++i) {
        stream->Read(Objects[i].Matrix);
        float scale;
        stream->Read(scale);
        Objects[i].Scale = scale;
        stream->Read(Objects[i].Block);
    }
    ZSpriteEntries = manager->AllocArray<CObjectEntry>(ObjectCount, this);
    if (ZSpriteEntries == 0)
        return false;
    stream->Read(ZSpriteEntries, ObjectCount * sizeof(CObjectEntry));
    return true;
}