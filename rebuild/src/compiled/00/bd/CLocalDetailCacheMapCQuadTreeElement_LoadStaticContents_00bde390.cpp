// CLocalDetailCacheMap::CQuadTreeElement::LoadStaticContents(CEngineStaticMapFileBlockManager* manager, const CObjectTypeCollectionPalette& palette) -- once the element's file block is in memory, read its object-collection headers and the four optional children from it (requesting the block on first call); false when already loaded or on an allocation failure (which unloads again)
// Leaf: LoadStaticContents
// Retail 0x00BDE390, 624 bytes, `ret 8` (Ego_r.pdb ?LoadStaticContents@CQuadTreeElement@CLocalDetailCacheMap@@QAE_NPAVCEngineStaticMapFileBlockManager@@ABVCObjectTypeCollectionPalette@2@@Z).
// Callees: CEngineStaticMapFileBlockManager RequestFileBlock 0x00BE06B0 / IsFileBlockLoaded 0x00BE0480 / GetFileBlockData
// 0x00BE04C0 / ReleaseRequest 0x00BE0400 (names but the last unconfirmed), CMemoryDataInputStream(const void*, long)
// 0x00994930 / dtor 0x00993B10, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined Read of the landed
// LoadHeader 0x00BDE290), the landscape memory manager's Allocate(size, owner) 0x00BD9AF0 (g_pLandscapeRenderer
// 0x01436EA8 +0x6B0; the inline class operator new), CObjectCacheGroupCollection::LoadHeader 0x00BDE190 (vtable
// 0x012A7DDC), CQuadTreeElement(CLocalDetailCacheMap*, long, long, long, long) 0x00BDDF30, CQuadTreeElement::LoadHeader
// 0x00BDE290, UnloadStaticContents 0x00BDE020.
// Layout: PDB CQuadTreeElement (landed dtor 0x00BDDFB0); retail CObjectCacheGroupCollection is the PDB layout less the
// trailing SourceObjectCount (0x38 bytes).
#include <string.h>
#pragma intrinsic(memcpy)

struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    __forceinline void Read(void* destination, long bytes)
    {
        __int64 next = (unsigned long)(StreamPos + bytes);
        if (next >= 0 && next <= 0x7fffffff) {
            if (SourceChunkBytesRemaining >= bytes) {
                memcpy(destination, CurrentSourcePtr, bytes);
                CurrentSourcePtr += bytes;
                SourceChunkBytesRemaining -= bytes;
                StreamPos += bytes;
            } else
                ReadWithSrcChunkOverflow((unsigned char*)destination, bytes);
        }
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, 4);
        return value;
    }
};
struct CMemoryDataInputStream : CDataInputStream {
    unsigned char* StreamSource;                        // +0x18
    CMemoryDataInputStream(const void* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();                          // 0x00993B10
};
struct CEngineStaticMapFileBlockManager {
    void RequestFileBlock(long pos, long size, long priority);   // 0x00BE06B0
    bool IsFileBlockLoaded(long pos);                   // 0x00BE0480
    unsigned char* GetFileBlockData(long pos);          // 0x00BE04C0
    void ReleaseRequest(long pos);                      // 0x00BE0400
};
struct CMovableResource { virtual ~CMovableResource() {} };
struct CEngineLandscapeMemoryManager {
    void* Allocate(unsigned long size, CMovableResource* owner);   // 0x00BD9AF0
};
struct CEngineLandscapeRenderer {
    unsigned char _pad[0x6b0];
    CEngineLandscapeMemoryManager* MemoryManager;       // +0x6b0
};
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;  // 0x01436EA8
struct C3DBoundingSphere {
    float X, Y, Z, Radius;
    C3DBoundingSphere() : X(0), Y(0), Z(0), Radius(0) {}
};
struct CLocalDetailCacheMap;
struct CObjectTypeCollection;
struct CObjectTypeCollectionPalette;
struct CObjectCacheGroupCollection : CMovableResource {     // retail 0x38 bytes, vtable 0x012A7DDC
    CObjectCacheGroupCollection* Next;                  // +0x04
    C3DBoundingSphere BoundingSphere;                   // +0x08
    float MaxFadeDistance;                              // +0x18
    unsigned long PrimitiveTypeMask;                    // +0x1c
    CObjectTypeCollection* ObjectTypeCollections;       // +0x20
    CLocalDetailCacheMap* Map;                          // +0x24
    long FileBlockPos;                                  // +0x28
    long FileBlockSize;                                 // +0x2c
    long OffsetIntoFileBlock;                           // +0x30
    unsigned short CacheGroupIndex;                     // +0x34
    bool RenderUpdateNeeded : 1;                        // +0x36 bit 0
    bool Loading : 1;                                   // +0x36 bit 1
    static void* operator new(unsigned int size, CMovableResource* owner)
    {
        return g_pLandscapeRenderer->MemoryManager->Allocate(size, owner);
    }
    static void operator delete(void*, CMovableResource*) {}
    CObjectCacheGroupCollection(CLocalDetailCacheMap* map)
        : Next(0), PrimitiveTypeMask(0), ObjectTypeCollections(0), Map(map), FileBlockPos(0), FileBlockSize(0),
          OffsetIntoFileBlock(0), CacheGroupIndex(0xffff), RenderUpdateNeeded(false), Loading(false)
    {
    }
    virtual ~CObjectCacheGroupCollection();
    void LoadHeader(CMemoryDataInputStream& stream);    // 0x00BDE190
};
struct CQuadTreeElement : CMovableResource {            // CLocalDetailCacheMap::CQuadTreeElement (PDB, 76 bytes)
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;   // +0x04
    CQuadTreeElement*  Children[4];                     // +0x08
    C3DBoundingSphere  BoundingSphere;                  // +0x18
    CLocalDetailCacheMap* Map;                          // +0x28
    float              MaxFadeDistance;                 // +0x2c
    unsigned long      PrimitiveTypeMask;               // +0x30
    long               FileBlockPos;                    // +0x34
    long               FileBlockSize;                   // +0x38
    long               OffsetIntoFileBlock;             // +0x3c
    unsigned short     MapX, MapY, Width, Height;       // +0x40
    bool               HasDynamicAreasFlag : 1;         // +0x48 bit 0
    bool               RenderUpdateNeeded : 1;          // +0x48 bit 1
    bool               StaticContentsLoaded : 1;        // +0x48 bit 2
    bool               Loading : 1;                     // +0x48 bit 3
    static void* operator new(unsigned int size, CMovableResource* owner)
    {
        return g_pLandscapeRenderer->MemoryManager->Allocate(size, owner);
    }
    static void operator delete(void*, CMovableResource*) {}
    CQuadTreeElement(CLocalDetailCacheMap* map, long x, long y, long width, long height);   // 0x00BDDF30
    virtual ~CQuadTreeElement();
    void LoadHeader(CMemoryDataInputStream& stream);    // 0x00BDE290
    void UnloadStaticContents(CEngineStaticMapFileBlockManager* manager);   // 0x00BDE020
    bool LoadStaticContents(CEngineStaticMapFileBlockManager* manager, const CObjectTypeCollectionPalette& palette);
};

bool CQuadTreeElement::LoadStaticContents(CEngineStaticMapFileBlockManager* manager,
                                          const CObjectTypeCollectionPalette& palette)
{
    if (StaticContentsLoaded)
        return false;
    if (!Loading) {
        manager->RequestFileBlock(FileBlockPos, FileBlockSize, 0);
        Loading = true;
    }
    if (manager->IsFileBlockLoaded(FileBlockPos)) {
        CMemoryDataInputStream stream(manager->GetFileBlockData(FileBlockPos) + OffsetIntoFileBlock,
                                      FileBlockSize - OffsetIntoFileBlock);
        long count = stream.ReadLong();
        for (long i = 0; i < count; ++i) {
            CObjectCacheGroupCollection* collection = new (this) CObjectCacheGroupCollection(Map);
            if (!collection) {
                StaticContentsLoaded = true;
                UnloadStaticContents(manager);
                return false;
            }
            collection->Next = ObjectCacheGroupCollections;
            ObjectCacheGroupCollections = collection;
        }
        for (CObjectCacheGroupCollection* c = ObjectCacheGroupCollections; c; c = c->Next)
            c->LoadHeader(stream);
        for (int child = 0; child < 4; ++child) {
            if (stream.ReadLong()) {
                Children[child] = new (this) CQuadTreeElement(Map, 0, 0, 0, 0);
                if (!Children[child]) {
                    StaticContentsLoaded = true;
                    UnloadStaticContents(manager);
                    return false;
                }
                Children[child]->LoadHeader(stream);
            } else
                Children[child] = 0;
        }
        StaticContentsLoaded = true;
        if (Loading) {
            manager->ReleaseRequest(FileBlockPos);
            Loading = false;
        }
    }
    return true;
}