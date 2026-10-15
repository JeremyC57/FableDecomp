// CLocalDetailCacheMap::CObjectTypeCollection::~CObjectTypeCollection() -- delete every local-detail primitive of the collection's singly linked list, then fall into the CMovableResource base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDDA0, length 51 bytes. Callees: each primitive's virtual deleting
// destructor (slot 0, `push 1 ; call [eax]`).
// Layout from the PDB (offsets agree with retail here: no containers precede them): CObjectCacheGroupCollection --
// vptr, Next +0x04, BoundingSphere +0x08, MaxFadeDistance +0x18, PrimitiveTypeMask +0x1C, ObjectTypeCollections +0x20,
// Map +0x24, FileBlockPos +0x28, FileBlockSize +0x2C, OffsetIntoFileBlock +0x30, CacheGroupIndex +0x34, bool:1
// RenderUpdateNeeded / Loading in the byte at +0x36, SourceObjectCount +0x38; CObjectTypeCollection -- vptr, Type +0x04,
// Primitives +0x08, Next +0x0C, Map +0x10, SourceObjects +0x14; CLocalDetailPrimitiveBase -- vptr, Next +0x04;
// CLocalDetailCacheMap -- FileBlockManager +0x04. Vtables (rtti-vtables.tsv): 0x012A7DDC CObjectCacheGroupCollection,
// 0x012A7D9C CObjectTypeCollection, 0x0129B860 CMovableResource (the base whose inline empty destructor stores it).
// Shape: `while (Primitives) { next = Primitives->Next; delete Primitives; Primitives = next; }` -- the entry test reads
// the member once, the body re-reads it for the `delete` null test with the successor loaded in between, the loop test
// runs on the stored successor; the base destructor is inline (its vtable store closes the body).
// Leaf: ??1CObjectTypeCollection
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
CLocalDetailCacheMap::CObjectTypeCollection::~CObjectTypeCollection()
{
    while (Primitives) {
        CLocalDetailPrimitiveBase* next = Primitives->Next;
        delete Primitives;
        Primitives = next;
    }
}
