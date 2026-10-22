// Behaviour test for 00bde860: final.cpp copied verbatim (memcpy renamed to a wrapper under g++), every declared callee
// stubbed below so the calls, their arguments and the resulting collection list are observable.
// CLocalDetailCacheMap::CObjectCacheGroupCollection::LoadContents(CEngineStaticMapFileBlockManager*, const CObjectTypeCollectionPalette&)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
typedef std::size_t TNewSize;   // the class operator new size type (unsigned int under VC7.1, size_t on the 64-bit g++ host)
static void* CopyBytes(void* d, const void* s, unsigned int n) { return std::memcpy(d, s, n); }
#ifndef _MSC_VER
#include <stdint.h>
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __int64 int64_t
#define long int          // the model's longs are 32-bit words (LP64 g++ would read 8-byte longs from the 4-byte block)
extern "C" void* fable_memcpy(void* d, const void* s, unsigned int n) { return CopyBytes(d, s, n); }
#define memcpy fable_memcpy
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
struct CHandleBasedMemoryPool;
struct CScratchBuffer;
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
struct CMovableResource;
struct CMovableResourceMemoryPool {
    unsigned char           _pad00[0x8];
    CHandleBasedMemoryPool* HandlePool;                      // +0x08
    void* Allocate(unsigned int size, CMovableResource* owner);   // 0x00BD9AF0
};
struct CEngineGlobals {
    unsigned char               _pad000[0x6b0];
    CMovableResourceMemoryPool* LocalDetailPool;             // +0x6b0
};
extern CEngineGlobals* g_pEngineGlobals;                     // 0x01436EA8
struct CMovableResource {
    virtual void v00();
};
struct CEngineStaticMapFileBlockManager {
    bool IsBlockLoaded(long position) const;                 // 0x00BE0480
    const unsigned char* GetBlockData(long position) const;  // 0x00BE04C0
    void ReleaseRequest(long position);                      // 0x00BE0400
    void AddRequest(long position, long size, bool highPriority);   // 0x00BE06B0
};
struct CLocalDetailCacheMap {
    struct CObjectTypeCollectionPalette;
    struct CObjectTypeCollection : CMovableResource {        // vtable 0x012A7D9C
        void*                  Type;                         // +0x04
        void*                  Primitives;                   // +0x08
        CObjectTypeCollection* Next;                         // +0x0c
        CLocalDetailCacheMap*  Map;                          // +0x10
        bool                   RenderUpdateNeeded;           // +0x14
        CObjectTypeCollection(CLocalDetailCacheMap* map) : Type(0), Primitives(0), Next(0), Map(map), RenderUpdateNeeded(false) {}
        virtual void v00();
        static void* operator new(TNewSize size, CMovableResource* owner) { return g_pEngineGlobals->LocalDetailPool->Allocate(size, owner); }
        static void operator delete(void* p, CMovableResource* owner);
        bool Load(CDataInputStream& in, const CObjectTypeCollectionPalette& palette);   // 0x00BDE680
    };
    struct CObjectCacheGroupCollection : CMovableResource {
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
        CTemporaryBuffer buffer(uncompressedSize + 3, g_pEngineGlobals->LocalDetailPool->HandlePool, &g_ScratchBuffer);
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

// ---- stubs ----
struct CScratchBuffer { int dummy; };
struct CLocalDetailCacheMap::CObjectTypeCollectionPalette { int dummy; };
static CLocalDetailCacheMap::CObjectTypeCollectionPalette g_palette;
CScratchBuffer g_ScratchBuffer;
static CEngineGlobals g_globals; CEngineGlobals* g_pEngineGlobals = &g_globals;
static CMovableResourceMemoryPool g_pool; static CHandleBasedMemoryPool* g_handlePool = (CHandleBasedMemoryPool*)0x1234;
static int g_allocs, g_allocFailAt, g_lzss, g_loads, g_unloads, g_addRequests, g_releaseRequests, g_bufferCtor, g_bufferDtor, g_streamCtor, g_streamDtor;
static long g_addPos, g_addSize; static bool g_addPriority; static long g_releasePos; static unsigned long g_bufferSize; static CHandleBasedMemoryPool* g_bufferPool; static CScratchBuffer* g_bufferScratch;
static const unsigned char* g_lzssSrc; static unsigned int g_lzssSrcLen; static unsigned char* g_lzssDst; static unsigned int* g_lzssDstLen;
static long g_loadValues[8]; static long g_loadFailValue = 0x0BAD; static CMovableResource* g_allocOwner;
static bool g_blockLoaded = true; static unsigned char g_block[64];

extern "C" int __cdecl LZSS_Decompress(const unsigned char* src, unsigned int srcLen, unsigned char* dst, unsigned int* dstLen, void* work)
{
    ++g_lzss; g_lzssSrc = src; g_lzssSrcLen = srcLen; g_lzssDst = dst; g_lzssDstLen = dstLen; CHECK(work == 0);
    if (src) CopyBytes(dst, src, srcLen);
    *dstLen = srcLen;
    return 0;
}
void CDataInputStream::v00() { }
void CDataInputStream::v01() { }
unsigned long CDataInputStream::GetPosition() { return StreamPos; }
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    const unsigned char* src = ((CMemoryDataInputStream*)this)->StreamSource + StreamPos;
    CopyBytes(buffer, src, size); StreamPos += size; CurrentSourcePtr = src + size; SourceChunkBytesRemaining = StreamSize - (long)StreamPos;
}
void CDataInputStream::SetPositionAssumeWithinSourceChunk(unsigned long position)
{
    StreamPos = position; CurrentSourcePtr = ((CMemoryDataInputStream*)this)->StreamSource + position; SourceChunkBytesRemaining = StreamSize - (long)position;
}
CMemoryDataInputStream::CMemoryDataInputStream(const unsigned char* data, long size)
{
    ++g_streamCtor; StreamSource = data; CurrentSourcePtr = data; StreamPos = 0; StreamSize = size; SourceChunkStreamPos = 0; SourceChunkBytesRemaining = size;
}
CMemoryDataInputStream::~CMemoryDataInputStream() { ++g_streamDtor; StreamSource = 0; }
CTemporaryBuffer::CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* pool, CScratchBuffer* scratch)
{
    ++g_bufferCtor; g_bufferSize = size; g_bufferPool = pool; g_bufferScratch = scratch;
    Data = (unsigned char*)std::malloc(size); MemoryPool = pool; MemoryHandle = 0; ScratchBuffer = scratch; AllocateType = 0;
}
CTemporaryBuffer::~CTemporaryBuffer() { ++g_bufferDtor; std::free(Data); Data = 0; }
void CTemporaryBuffer::v00() { }
void* CMovableResourceMemoryPool::Allocate(unsigned int size, CMovableResource* owner)
{
    ++g_allocs; g_allocOwner = owner; CHECK(size == sizeof(CLocalDetailCacheMap::CObjectTypeCollection));   // 0x18 on the 32-bit retail layout
    if (g_allocs == g_allocFailAt) return 0;
    return std::malloc(size);
}
void CMovableResource::v00() { }
bool CEngineStaticMapFileBlockManager::IsBlockLoaded(long position) const { return g_blockLoaded && position == 0x40; }
const unsigned char* CEngineStaticMapFileBlockManager::GetBlockData(long position) const { CHECK(position == 0x40); return g_block; }
void CEngineStaticMapFileBlockManager::ReleaseRequest(long position) { ++g_releaseRequests; g_releasePos = position; }
void CEngineStaticMapFileBlockManager::AddRequest(long position, long size, bool highPriority) { ++g_addRequests; g_addPos = position; g_addSize = size; g_addPriority = highPriority; }
void CLocalDetailCacheMap::CObjectTypeCollection::v00() { }
void CLocalDetailCacheMap::CObjectTypeCollection::operator delete(void* p, CMovableResource*) { std::free(p); }
bool CLocalDetailCacheMap::CObjectTypeCollection::Load(CDataInputStream& in, const CObjectTypeCollectionPalette&)
{
    long value = in.ReadLong();
    if (g_loads < 8) g_loadValues[g_loads] = value;
    ++g_loads; Type = (void*)(long)value;
    return value != g_loadFailValue;
}
void CLocalDetailCacheMap::CObjectCacheGroupCollection::UnloadContents(CEngineStaticMapFileBlockManager*)
{
    ++g_unloads;
    while (ObjectTypeCollections) { CObjectTypeCollection* next = ObjectTypeCollections->Next; std::free(ObjectTypeCollections); ObjectTypeCollections = next; }
}
static void PutLong(unsigned char* p, long v) { CopyBytes(p, &v, 4); }
struct Fixture {
    CLocalDetailCacheMap map; CLocalDetailCacheMap::CObjectCacheGroupCollection group; CEngineStaticMapFileBlockManager blocks;
    Fixture(long typeCount, long first, long second)
    {
        std::memset(&group, 0, sizeof(group)); group.Map = &map; group.FileBlockPos = 0x40; group.FileBlockSize = 40; group.OffsetIntoFileBlock = 8;
        g_globals.LocalDetailPool = &g_pool; g_pool.HandlePool = g_handlePool;
        // block: 8 bytes of other data, then [uncompressedSize][compressedSize][payload = typeCount, first, second]
        std::memset(g_block, 0xEE, sizeof(g_block));
        long payload = 12; PutLong(g_block + 8, payload); PutLong(g_block + 12, payload);
        PutLong(g_block + 16, typeCount); PutLong(g_block + 20, first); PutLong(g_block + 24, second);
        g_allocs = g_allocFailAt = g_lzss = g_loads = g_unloads = g_addRequests = g_releaseRequests = g_bufferCtor = g_bufferDtor = g_streamCtor = g_streamDtor = 0;
        g_blockLoaded = true; g_addPos = g_addSize = g_releasePos = -1; g_bufferSize = 0;
    }
    bool Run() { return group.LoadContents(&blocks, g_palette); }
};
int main()
{
    {   // full load: request, decompress, two collections (list reversed), both loaded in list order, request released
        Fixture f(2, 111, 222);
        CHECK(f.Run() == true);
        CHECK(g_addRequests == 1 && g_addPos == 0x40 && g_addSize == 40 && !g_addPriority);
        CHECK(g_releaseRequests == 1 && g_releasePos == 0x40 && !f.group.Loading);
        CHECK(g_bufferCtor == 1 && g_bufferDtor == 1 && g_bufferSize == 12 + 3 && g_bufferPool == g_handlePool && g_bufferScratch == &g_ScratchBuffer);
        CHECK(g_lzss == 1 && g_lzssSrc == g_block + 16 && g_lzssSrcLen == 12 && g_lzssDstLen != 0 && *g_lzssDstLen == 12);
        CHECK(g_streamCtor == 2 && g_streamDtor == 2);
        CHECK(g_allocs == 2 && g_allocOwner == &f.group);
        CLocalDetailCacheMap::CObjectTypeCollection* c = f.group.ObjectTypeCollections;
        CHECK(c != 0 && c->Next != 0 && c->Next->Next == 0);
        CHECK(c->Map == &f.map && c->Primitives == 0 && !c->RenderUpdateNeeded && c->Next->Map == &f.map);
        CHECK(g_loads == 2 && g_loadValues[0] == 111 && g_loadValues[1] == 222);
        CHECK(c->Type == (void*)111 && c->Next->Type == (void*)222);
        g_unloads = 0; f.group.UnloadContents(&f.blocks);
    }
    {   // already loaded: false, nothing touched
        Fixture f(1, 5, 6); CLocalDetailCacheMap::CObjectTypeCollection existing(&f.map); f.group.ObjectTypeCollections = &existing;
        CHECK(f.Run() == false && g_addRequests == 0 && g_lzss == 0 && g_loads == 0);
        f.group.ObjectTypeCollections = 0;
    }
    {   // block not resident yet: request once, keep the loading bit, true; a second call does not re-request
        Fixture f(1, 5, 6); g_blockLoaded = false;
        CHECK(f.Run() == true && g_addRequests == 1 && f.group.Loading && g_lzss == 0 && g_releaseRequests == 0);
        CHECK(f.Run() == true && g_addRequests == 1 && f.group.Loading);
    }
    {   // a collection fails to load: contents unloaded, false, the request stays open
        Fixture f(2, 111, 0x0BAD);
        CHECK(f.Run() == false && g_unloads == 1 && g_loads == 2 && f.group.ObjectTypeCollections == 0 && g_releaseRequests == 0 && f.group.Loading);
        CHECK(g_bufferDtor == 1 && g_streamDtor == 2);
    }
#ifdef _MSC_VER
    {   // allocation fails on the second collection: unloaded, false (VC7.1 null-tests a custom operator new like retail; g++ does not)
        Fixture f(3, 1, 2); g_allocFailAt = 2;
        CHECK(f.Run() == false && g_allocs == 2 && g_unloads == 1 && g_loads == 0 && f.group.ObjectTypeCollections == 0);
    }
#endif
    if (fails) { printf("00bde860_TEST FAIL (%d)\n", fails); return 1; }
    printf("00bde860_TEST PASS\n");
    return 0;
}