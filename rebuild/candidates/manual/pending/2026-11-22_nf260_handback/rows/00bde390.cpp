// CLocalDetailCacheMap::CQuadTreeElement::LoadStaticContents(CEngineStaticMapFileBlockManager* manager, const CObjectTypeCollectionPalette& palette) -- once the element's file block is in memory, read its object-collection headers and the four optional children from it (requesting the block on first call); false when already loaded or on an allocation failure (which unloads again)
// Leaf: LoadStaticContents
// NF131 (2026-11-20, q6182): the row-local CMemoryDataInputStream ctor declaration respelled in place to the PDB (const void*, unsigned long):
//   ??0CMemoryDataInputStream@@QAE@PBXK@Z (Ego_r.pdb 0x00AFCA20 + FableWin.pdb 0x030849C0), the spelling the other landed callers of 0x00994930 bind.
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
// NF188 (2026-11-20, q4251): the row-local CDataInputStream / CMemoryDataInputStream models (a user-declared destructor that
//   bound a CMemoryDataInputStream dtor spelling at 0x00993B10) are replaced by the shared engine/CDataInputStream.h /
//   engine/CMemoryDataInputStream.h classes, whose compiler-generated destructor calls ~CDataStream 0x00993B10 directly
//   (??1CDataStream@@UAE@XZ, Ego_r.pdb 0x00AFBBB0 + FableWin.pdb 0x03080440; engine/CDataInputStream.h retry11a note).
#include "fable_compiler.h"
#include <string.h>
#pragma intrinsic(memcpy)

#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include "engine/CMemoryDataInputStream.h"
class CEngineStaticMapFileBlockManager { public:
    void RequestFileBlock(long pos, long size, long priority);   // 0x00BE06B0
    void ReleaseRequest(long pos);                      // 0x00BE0400
public:
    const unsigned char* GetBlockData(long) const;   // 0x00BE04C0
    bool IsBlockLoaded(long) const;   // 0x00BE0480
};
class CMovableResource { public: virtual ~CMovableResource() {} };
class CEngineLandscapeMemoryManager { public:
public:
    void* AllocVirtual(long, CMovableResource*);   // 0x00BD9AF0
};
class CEngineLandscapeRenderer { public:
    unsigned char _pad[0x6b0];
    CEngineLandscapeMemoryManager* MemoryManager;       // +0x6b0
};
namespace NEngineInternal { extern CEngineLandscapeRenderer* GPLandscapeRenderer; }   // 0x01436EA8 (Ego_r / FableWin / Ego_d PDB spelling)
class C3DBoundingSphere { public:
    float X, Y, Z, Radius;
    C3DBoundingSphere() : X(0), Y(0), Z(0), Radius(0) {}
};
class CLocalDetailCacheMap;
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
        return NEngineInternal::GPLandscapeRenderer->MemoryManager->AllocVirtual(size, owner);
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
        return NEngineInternal::GPLandscapeRenderer->MemoryManager->AllocVirtual(size, owner);
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
    if (manager->IsBlockLoaded(FileBlockPos)) {
        CMemoryDataInputStream stream(manager->GetBlockData(FileBlockPos) + OffsetIntoFileBlock,
                                      FileBlockSize - OffsetIntoFileBlock);
        long count = stream.ReadSLONG();
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
            if (stream.ReadSLONG()) {
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