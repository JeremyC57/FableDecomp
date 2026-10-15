// CLocalDetailCacheMap::CQuadTreeElement::CQuadTreeElement(CLocalDetailCacheMap* map, unsigned short x, unsigned short y, unsigned short width, unsigned short height) -- an empty quad-tree cell: owner map and map rectangle stored, children, collections, bounding sphere, flags and file-block words zeroed
// Grade: DIFFER(119v119) -- scheduling only: retail loads the flag byte (mov dl,[eax+48h]) before the four child zero stores and stores MaxFadeDistance after the flag byte; ours loads it after +0x04 and stores +0x2c before the and/store. Tried flags after/before the children, an inline ClearStateFlags() before/after the collection reset
// Leaf: ??0CQuadTreeElement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDF30, length 119 bytes (padded to 128), `ret 14h`. No callees.
// Vtable 0x012A7DF0 (the one the landed destructor family restores). Callers 0x00BDE599 and 0x00BDEFE0 (the landed
// CLocalDetailCacheMap ctor), both after a `new`.
// Layout: PDB CLocalDetailCacheMap::CQuadTreeElement (76 bytes).
// Shape: `this` is moved to eax and one zero register (ecx) serves every store; the bounding sphere's inline default ctor
// zeroes +0x18..+0x24 in the initialiser list (before Map and the four word arguments), and the body then clears the
// children, the collection chain, the four state bits (`and dl,0F0h`), the fade distance, the sphere again (an inline
// Reset), the file-block words and the primitive mask in that order.
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
    void ClearStateFlags() { HasDynamicAreasFlag = false; RenderUpdateNeeded = false; StaticContentsLoaded = false; Loading = false; }
    CQuadTreeElement(CLocalDetailCacheMap* map, unsigned short x, unsigned short y, unsigned short width, unsigned short height);
};
CQuadTreeElement::CQuadTreeElement(CLocalDetailCacheMap* map, unsigned short x, unsigned short y, unsigned short width, unsigned short height)
    : Map(map), MapX(x), MapY(y), Width(width), Height(height)
{
    for (long i = 0; i < 4; ++i)
        Children[i] = 0;
    ClearStateFlags();
    ObjectCacheGroupCollections = 0;
    MaxFadeDistance = 0.0f;
    BoundingSphere.Reset();
    FileBlockPos = 0;
    FileBlockSize = 0;
    OffsetIntoFileBlock = 0;
    PrimitiveTypeMask = 0;
}
