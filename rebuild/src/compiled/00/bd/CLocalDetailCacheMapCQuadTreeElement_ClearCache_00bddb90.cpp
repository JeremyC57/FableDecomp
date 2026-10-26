// CLocalDetailCacheMap::CQuadTreeElement::ClearCache(CEngineStaticMapFileBlockManager* manager) -- cancel a pending file-block load, delete the four children and the object-collection chain, then clear HasDynamicAreasFlag and StaticContentsLoaded
// Leaf: ClearCache
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDB90, length 102 bytes (padded to 112), `ret 4`.
// Callees: CEngineStaticMapFileBlockManager::ReleaseRequest(long) @ 0x00BE0400; the children's and collections' deleting
// destructors (virtual slot 0 with 1). Called by the landed CBlockCacheOwner_00bdf0d0::Reset (0x00BDF0D0), which names it.
// Layout: PDB CLocalDetailCacheMap::CQuadTreeElement (landed UnloadStaticContents 0x00BDE020 model).
// Shape: Loading is `test byte [+48h],8` / `and byte [+48h],0F7h`; the children are the counted delete-and-null loop
// (ebx = 4); the collection chain is the inline RemoveObjectCollections loop; the two trailing bit-field clears merge
// into one `and byte [+48h],0FAh`.
class CLocalDetailCacheMap;
struct CObjectCacheGroupCollection {               // polymorphic; the chain is singly linked through +4
    virtual ~CObjectCacheGroupCollection();        // slot 0: the deleting destructor
    CObjectCacheGroupCollection* Next;             // +0x04
};
class CEngineStaticMapFileBlockManager { public:
    void ReleaseRequest(long fileBlockPos);        // 0x00BE0400
};
class C3DBoundingSphere { public:                         // centre + radius (16 bytes)
    float X, Y, Z, Radius;
    C3DBoundingSphere() : X(0.0f), Y(0.0f), Z(0.0f), Radius(0.0f) { }
    void Reset() { X = 0.0f; Y = 0.0f; Z = 0.0f; Radius = 0.0f; }
};
struct CQuadTreeElement {                          // CLocalDetailCacheMap::CQuadTreeElement, PDB layout (76 bytes), vtable 0x012A7DF0
    virtual ~CQuadTreeElement();                   // slot 0: the deleting destructor
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;   // +0x04
    CQuadTreeElement*  Children[4];                // +0x08
    C3DBoundingSphere  BoundingSphere;             // +0x18
    CLocalDetailCacheMap* Map;                     // +0x28
    float              MaxFadeDistance;            // +0x2c
    unsigned long      PrimitiveTypeMask;          // +0x30
    long               FileBlockPos;               // +0x34
    long               FileBlockSize;              // +0x38
    long               OffsetIntoFileBlock;        // +0x3c
    unsigned short     MapX;                       // +0x40
    unsigned short     MapY;                       // +0x42
    unsigned short     Width;                      // +0x44
    unsigned short     Height;                     // +0x46
    bool               HasDynamicAreasFlag : 1;    // +0x48 bit 0
    bool               RenderUpdateNeeded : 1;     // +0x48 bit 1
    bool               StaticContentsLoaded : 1;   // +0x48 bit 2
    bool               Loading : 1;                // +0x48 bit 3
    void RemoveObjectCollections()                 // out of line at 0x00BDDA50 (landed); inlined here
    {
        while (ObjectCacheGroupCollections) {
            CObjectCacheGroupCollection* next = ObjectCacheGroupCollections->Next;
            delete ObjectCacheGroupCollections;
            ObjectCacheGroupCollections = next;
        }
    }
    void ClearCache(CEngineStaticMapFileBlockManager* manager);
};
void CQuadTreeElement::ClearCache(CEngineStaticMapFileBlockManager* manager)
{
    if (Loading) {
        manager->ReleaseRequest(FileBlockPos);
        Loading = false;
    }
    for (long i = 0; i < 4; ++i) {
        delete Children[i];
        Children[i] = 0;
    }
    RemoveObjectCollections();
    HasDynamicAreasFlag = false;
    StaticContentsLoaded = false;
}