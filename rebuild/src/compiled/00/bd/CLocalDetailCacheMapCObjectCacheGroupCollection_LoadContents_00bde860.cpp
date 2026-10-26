// CLocalDetailCacheMap::CObjectCacheGroupCollection::LoadContents(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette) -- once this group's file block is in memory, decompress its contents and load one object-type collection per stored type; false when already loaded or a collection fails (the contents are unloaded again)
// Leaf: ?LoadContents@CObjectCacheGroupCollection@CLocalDetailCacheMap@@
// Shape (what keeps retail's register plan: ebx = 4 for the two ReadLongs, then the buffer data across the
//   GetDataAndSkip calls, and the zero register created only before the type-collection loop; ebp = the written size
//   across the CTemporaryBuffer constructor):
//   * the buffer is constructed from `uncompressedSize + 3` and `written` is assigned the same expression AFTER the
//     constructor, so the CSE'd sum lives in ebp across the call and is stored to its slot afterwards
//     (`mov ebp,[esp+10h] ; add ebp,3 ; push ebp ; call ; mov [esp+18h],ebp`); a `written` computed before the
//     constructor is stored before the call and frees a callee-saved register for an entry-hoisted zero;
//   * the decompression target and the contents stream source are `buffer.GetData()` (an inline accessor): VC7.1
//     evaluates call-shaped arguments first, so the data pointer is loaded into ebx BEFORE the inlined GetDataAndSkip
//     (`mov ebx,[esp+20h]` right after the constructor) and re-read for the contents stream; a plain `buffer.Data`
//     argument is loaded at its push, after the calls. With ebx/ebp busy that way, the zero is materialised late.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDE860, 750 bytes (`ret 8`).
// Callees: CEngineStaticMapFileBlockManager AddRequest 0x00BE06B0, IsBlockLoaded 0x00BE0480, GetBlockData 0x00BE04C0,
//   ReleaseRequest 0x00BE0400; CMemoryDataInputStream(data, size) 0x00994930 / dtor 0x00993B10, CDataInputStream
//   ReadWithSrcChunkOverflow 0x00993CA0 and SetPositionAssumeWithinSourceChunk 0x00993C10, virtual GetPosition (slot 2);
//   CTemporaryBuffer(size, pool, scratch) 0x00A98860 / dtor 0x00A98910, LZSS_Decompress 0x00C069D0 (cdecl); the movable
//   resource pool's Allocate 0x00BD9AF0 (CObjectTypeCollection's class operator new), CObjectTypeCollection::Load
//   0x00BDE680, UnloadContents 0x00BDD9A0; the engine global 0x01436EA8 (+0x6B0 the local-detail pool), the shared
//   scratch buffer 0x013CB498, CObjectTypeCollection vtable 0x012A7D9C.
// Layout (Ego_r.pdb, retail offsets confirmed by the body): CObjectCacheGroupCollection ObjectTypeCollections +0x20,
//   Map +0x24, FileBlockPos +0x28, FileBlockSize +0x2c, OffsetIntoFileBlock +0x30, Loading bit 1 of +0x36;
//   CObjectTypeCollection (retail 0x18): Type +4, Primitives +8, Next +0xc, Map +0x10, RenderUpdateNeeded +0x14.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern "C" int __cdecl LZSS_Decompress(const unsigned char* src, unsigned int srcLen, unsigned char* dst,
                                        unsigned int* dstLen, void* work);   // 0x00C069D0

class CDataInputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual unsigned long GetPosition();                     // slot 2 (+8)
    unsigned long        StreamPos;                          // +0x04
    long                 StreamSize;                         // +0x08
    const unsigned char* CurrentSourcePtr;                   // +0x0c
    unsigned long        SourceChunkStreamPos;               // +0x10
    long                 SourceChunkBytesRemaining;          // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    void SetPositionAssumeWithinSourceChunk(unsigned long position);    // 0x00993C10
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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
};
class CMemoryDataInputStream : public CDataInputStream {
public:
    const unsigned char* StreamSource;                       // +0x18
    CMemoryDataInputStream(const unsigned char* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();                               // 0x00993B10
    __forceinline const unsigned char* GetDataAndSkip(long bytes)
    {
        if (bytes <= 0)
            return 0;
        unsigned long position = GetPosition();
        __int64 newPosition = (__int64)(position + bytes);
        if (IsValidPosition(newPosition)) {
            const unsigned char* data = StreamSource + position;
            SetPositionAssumeWithinSourceChunk((unsigned long)newPosition);
            return data;
        }
        return 0;
    }
};
class CHandleBasedMemoryPool;
class CScratchBuffer;
extern CScratchBuffer g_ScratchBuffer;                       // 0x013CB498
struct CTemporaryBuffer {                                    // 0x18 bytes
    CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* pool, CScratchBuffer* scratch);   // 0x00A98860
    ~CTemporaryBuffer();                                     // 0x00A98910
    virtual void v00();
    unsigned char*          Data;                            // +0x04
    CHandleBasedMemoryPool* MemoryPool;                      // +0x08
    long                    MemoryHandle;                    // +0x0c
    CScratchBuffer*         ScratchBuffer;                   // +0x10
    long                    AllocateType;                    // +0x14
    __forceinline unsigned char* GetData() const { return Data; }
};
class CMovableResource;
class CMovableResourceMemoryPool { public:
    unsigned char           _pad00[0x8];
    CHandleBasedMemoryPool* HandlePool;                      // +0x08
    void* Allocate(unsigned int size, CMovableResource* owner);   // 0x00BD9AF0
};
struct CEngineGlobals {
    unsigned char               _pad000[0x6b0];
    CMovableResourceMemoryPool* LocalDetailPool;             // +0x6b0
};
extern CEngineGlobals* g_pEngineGlobals_01436ea8;                     // 0x01436EA8
class CMovableResource { public:
    virtual void v00();
};
class CEngineStaticMapFileBlockManager { public:
    bool IsBlockLoaded(long position) const;                 // 0x00BE0480
    const unsigned char* GetBlockData(long position) const;  // 0x00BE04C0
    void ReleaseRequest(long position);                      // 0x00BE0400
    void AddRequest(long position, long size, bool highPriority);   // 0x00BE06B0
};
class CLocalDetailCacheMap { public:
    class CObjectTypeCollectionPalette;
    class CObjectTypeCollection : public CMovableResource { public:        // vtable 0x012A7D9C
        void*                  Type;                         // +0x04
        void*                  Primitives;                   // +0x08
        CObjectTypeCollection* Next;                         // +0x0c
        CLocalDetailCacheMap*  Map;                          // +0x10
        bool                   RenderUpdateNeeded;           // +0x14
        CObjectTypeCollection(CLocalDetailCacheMap* map) : Type(0), Primitives(0), Next(0), Map(map), RenderUpdateNeeded(false) {}
        virtual void v00();
        static void* operator new(unsigned int size, CMovableResource* owner) { return g_pEngineGlobals_01436ea8->LocalDetailPool->Allocate(size, owner); }
        static void operator delete(void* p, CMovableResource* owner);
        bool Load(CDataInputStream& in, const CObjectTypeCollectionPalette& palette);   // 0x00BDE680
    };
    class CObjectCacheGroupCollection : public CMovableResource { public:
        CObjectCacheGroupCollection* Next;                   // +0x04
        float                  BoundingSphere[4];            // +0x08
        float                  MaxFadeDistance;              // +0x18
        unsigned long          PrimitiveTypeMask;            // +0x1c
        CObjectTypeCollection* ObjectTypeCollections;        // +0x20
        CLocalDetailCacheMap*  Map;                          // +0x24
        long                   FileBlockPos;                 // +0x28
        long                   FileBlockSize;                // +0x2c
        long                   OffsetIntoFileBlock;          // +0x30
        unsigned short         CacheGroupIndex;              // +0x34
        bool                   RenderUpdateNeeded : 1;       // +0x36 bit 0
        bool                   Loading : 1;                  // +0x36 bit 1
        long                   SourceObjectCount;            // +0x38
        void UnloadContents(CEngineStaticMapFileBlockManager* blocks);   // 0x00BDD9A0
        bool LoadContents(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette);
    };
};

bool CLocalDetailCacheMap::CObjectCacheGroupCollection::LoadContents(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette)
{
    if (ObjectTypeCollections)
        return false;
    if (!Loading) {
        blocks->AddRequest(FileBlockPos, FileBlockSize, false);
        Loading = true;
    }
    if (blocks->IsBlockLoaded(FileBlockPos)) {
        CMemoryDataInputStream stream(blocks->GetBlockData(FileBlockPos) + OffsetIntoFileBlock, FileBlockSize - OffsetIntoFileBlock);
        long uncompressedSize = stream.ReadLong();
        long compressedSize = stream.ReadLong();
        CTemporaryBuffer buffer(uncompressedSize + 3, g_pEngineGlobals_01436ea8->LocalDetailPool->HandlePool, &g_ScratchBuffer);
        unsigned int written = uncompressedSize + 3;
        LZSS_Decompress(stream.GetDataAndSkip(compressedSize), compressedSize, buffer.GetData(), &written, 0);
        CMemoryDataInputStream contents(buffer.GetData(), uncompressedSize);
        long typeCount = contents.ReadLong();
        for (long i = 0; i < typeCount; ++i) {
            CObjectTypeCollection* collection = new (this) CObjectTypeCollection(Map);
            if (collection == 0) {
                UnloadContents(blocks);
                return false;
            }
            collection->Next = ObjectTypeCollections;
            ObjectTypeCollections = collection;
        }
        for (CObjectTypeCollection* collection = ObjectTypeCollections; collection != 0; collection = collection->Next) {
            if (!collection->Load(contents, palette)) {
                UnloadContents(blocks);
                return false;
            }
        }
        if (Loading) {
            blocks->ReleaseRequest(FileBlockPos);
            Loading = false;
        }
    }
    return true;
}