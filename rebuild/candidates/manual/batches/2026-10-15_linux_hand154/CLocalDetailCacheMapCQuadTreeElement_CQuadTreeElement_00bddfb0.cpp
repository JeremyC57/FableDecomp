// CLocalDetailCacheMap::CQuadTreeElement::~CQuadTreeElement() -- cancel a pending file-block load through the owner map's block manager, free the object-collection chain and delete the four children, then fall into the CMovableResource base
// Leaf: ??1CQuadTreeElement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDFB0, length 101 bytes (padded to 112).
// Callees: CEngineStaticMapFileBlockManager::ReleaseRequest(long) @ 0x00BE0400 on Map->+4, RemoveObjectCollections
// @ 0x00BDDA50 (landed, called out of line here), the children's deleting destructors (slot 0 with 1).
// Vtables: 0x012A7DF0 (CQuadTreeElement) on entry, 0x0129B860 (CMovableResource, RTTI) restored by the inline empty base dtor.
// Callers: 0x00BDED87 (`jmp`, the deleting-dtor tail) and 0x00BDFA13.
// Layout: PDB CLocalDetailCacheMap::CQuadTreeElement (76 bytes); the map's file-block manager pointer at +4.
// Shape: the Loading test is scheduled before the vtable store; the four child deletes are written out (no nulling, no
// loop counter: retail has four separate null-tested deletes).
struct CLocalDetailCacheMap;
struct CObjectCacheGroupCollection {               // polymorphic; the chain is singly linked through +4
    virtual ~CObjectCacheGroupCollection();        // slot 0: the deleting destructor
    CObjectCacheGroupCollection* Next;             // +0x04
};
struct CEngineStaticMapFileBlockManager {
    void ReleaseRequest(long fileBlockPos);        // 0x00BE0400
};
struct C3DBoundingSphere {                         // centre + radius (16 bytes)
    float X, Y, Z, Radius;
    C3DBoundingSphere() : X(0.0f), Y(0.0f), Z(0.0f), Radius(0.0f) { }
    void Reset() { X = 0.0f; Y = 0.0f; Z = 0.0f; Radius = 0.0f; }
};
struct CMovableResource {                          // vtable 0x0129B860
    virtual ~CMovableResource() { }
};
struct CLocalDetailCacheMap {
    unsigned char                     _pad00[4];
    CEngineStaticMapFileBlockManager* FileBlockManager;   // +0x04
};
struct CQuadTreeElement : CMovableResource {       // CLocalDetailCacheMap::CQuadTreeElement, PDB layout (76 bytes), vtable 0x012A7DF0
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
    void RemoveObjectCollections();                // 0x00BDDA50 (landed)
};
CQuadTreeElement::~CQuadTreeElement()
{
    if (Loading) {
        Map->FileBlockManager->ReleaseRequest(FileBlockPos);
        Loading = false;
    }
    RemoveObjectCollections();
    delete Children[0];
    delete Children[1];
    delete Children[2];
    delete Children[3];
}
