// CLocalDetailCacheMap::CObjectCacheGroupCollection::~CObjectCacheGroupCollection() -- cancel a pending load request through the owning map's file-block manager, delete every object-type collection, then fall into the CMovableResource base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDE80, length 82 bytes. Callees:
// CEngineStaticMapFileBlockManager::ReleaseRequest @ 0x00BE0400, each collection's virtual deleting destructor (slot 0).
// Layout from the PDB (offsets agree with retail here: no containers precede them): CObjectCacheGroupCollection --
// vptr, Next +0x04, BoundingSphere +0x08, MaxFadeDistance +0x18, PrimitiveTypeMask +0x1C, ObjectTypeCollections +0x20,
// Map +0x24, FileBlockPos +0x28, FileBlockSize +0x2C, OffsetIntoFileBlock +0x30, CacheGroupIndex +0x34, bool:1
// RenderUpdateNeeded / Loading in the byte at +0x36, SourceObjectCount +0x38; CObjectTypeCollection -- vptr, Type +0x04,
// Primitives +0x08, Next +0x0C, Map +0x10, SourceObjects +0x14; CLocalDetailPrimitiveBase -- vptr, Next +0x04;
// CLocalDetailCacheMap -- FileBlockManager +0x04. Vtables (rtti-vtables.tsv): 0x012A7DDC CObjectCacheGroupCollection,
// 0x012A7D9C CObjectTypeCollection, 0x0129B860 CMovableResource (the base whose inline empty destructor stores it).
// Shape: the body of UnloadContents (0x00BDD9A0) written out with `Map->FileBlockManager` as the manager of the inline
// ClearLoadRequest (FileBlockPos is loaded before
// the manager chain), the derived vtable stored after the bit-field test, the base vtable at the end.
// Leaf: ??1CObjectCacheGroupCollection
class CEngineStaticMapFileBlockManager {
public:
    void ReleaseRequest(long fileBlockPos);        // 0x00BE0400
};

class CEngineMap;
class CLocalDetailObjectCollectionType;
class CSourceObject;

class CMovableResource {                           // vtable 0x0129B860
public:
    virtual ~CMovableResource() {}
    virtual void v001();
    virtual void v002();
    virtual void v003();
};

class CLocalDetailPrimitiveBase {
public:
    virtual ~CLocalDetailPrimitiveBase();
    CLocalDetailPrimitiveBase* Next;               // +0x04
};

class CLocalDetailCacheMap {
public:
    bool                              LockedFromMemoryRelocation;   // +0x00
    CEngineStaticMapFileBlockManager* FileBlockManager;             // +0x04
    CEngineMap*                       Map;                          // +0x08

    class CObjectTypeCollection : public CMovableResource {         // vtable 0x012A7D9C
    public:
        CLocalDetailObjectCollectionType* Type;                     // +0x04
        CLocalDetailPrimitiveBase*        Primitives;               // +0x08
        CObjectTypeCollection*            Next;                     // +0x0C
        CLocalDetailCacheMap*             Map;                      // +0x10
        CSourceObject*                    SourceObjects;            // +0x14
        bool                              RenderUpdateNeeded;       // +0x18
        virtual ~CObjectTypeCollection();
    };

    class CObjectCacheGroupCollection : public CMovableResource {   // vtable 0x012A7DDC
    public:
        CObjectCacheGroupCollection* Next;                          // +0x04
        float                        BoundingSphere[4];             // +0x08 C3DBoundingSphere
        float                        MaxFadeDistance;               // +0x18
        unsigned long                PrimitiveTypeMask;             // +0x1C
        CObjectTypeCollection*       ObjectTypeCollections;         // +0x20
        CLocalDetailCacheMap*        Map;                           // +0x24
        long                         FileBlockPos;                  // +0x28
        long                         FileBlockSize;                 // +0x2C
        long                         OffsetIntoFileBlock;           // +0x30
        unsigned short               CacheGroupIndex;               // +0x34
        bool                         RenderUpdateNeeded : 1;        // +0x36 bit 0
        bool                         Loading : 1;                   // +0x36 bit 1
        long                         SourceObjectCount;             // +0x38

        void ClearLoadRequest(CEngineStaticMapFileBlockManager* manager)
        {
            if (Loading) {
                manager->ReleaseRequest(FileBlockPos);
                Loading = false;
            }
        }
        void UnloadContents(CEngineStaticMapFileBlockManager* manager);
        virtual ~CObjectCacheGroupCollection();
    };
};
CLocalDetailCacheMap::CObjectCacheGroupCollection::~CObjectCacheGroupCollection()
{
    ClearLoadRequest(Map->FileBlockManager);
    while (ObjectTypeCollections) {
        CObjectTypeCollection* next = ObjectTypeCollections->Next;
        delete ObjectTypeCollections;
        ObjectTypeCollections = next;
    }
}