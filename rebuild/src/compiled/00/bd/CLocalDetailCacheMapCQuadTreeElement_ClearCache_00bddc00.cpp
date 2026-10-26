// CLocalDetailCacheMap::CQuadTreeElement::ClearCache(CEngineStaticMapFileBlockManager* manager) -- the fuller reset: cancel a pending load, delete the children and the collection chain, clear three state bits and forget the file-block position, size and offset
// Leaf: ClearCache
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDC00, length 127 bytes, `ret 4`.
// Callees: CEngineStaticMapFileBlockManager::ReleaseRequest(long) @ 0x00BE0400; the children's and collections' deleting
// destructors (virtual slot 0 with 1). Only caller 0x00BDDD5A (CLocalDetailCacheMap code just before Render); the
// catalog label ClearCache is shared with 0x00BDDB90, this body additionally clears RenderUpdateNeeded and the file-block
// words, so it is a separate member (named here as a ClearCache overload variant in its own TU).
// Layout: PDB CLocalDetailCacheMap::CQuadTreeElement (landed UnloadStaticContents 0x00BDE020 model).
// Shape: as 0x00BDDB90; the three bit-field clears (written after the zero stores) merge into one `and al,0F8h`
// that VC7.1 hoists above the three zero stores; written first, the byte store lands before them.
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
    FileBlockPos = 0;
    FileBlockSize = 0;
    OffsetIntoFileBlock = 0;
    HasDynamicAreasFlag = false;
    RenderUpdateNeeded = false;
    StaticContentsLoaded = false;
}