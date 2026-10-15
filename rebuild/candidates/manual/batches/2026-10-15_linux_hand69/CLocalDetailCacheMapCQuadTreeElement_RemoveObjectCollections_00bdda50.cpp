// CLocalDetailCacheMap::CQuadTreeElement::RemoveObjectCollections() -- delete every object-cache group collection chained from ObjectCacheGroupCollections, leaving the chain empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDA50, length 44 bytes.
// Callees: CObjectCacheGroupCollection's deleting destructor (virtual slot 0, with 1).
// Identity: FableWin.pdb names ?RemoveObjectCollections@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ; the body sits in
// the CLocalDetailCacheMap translation unit (0x00BDDxxx-0x00BDE1xx) and is called by UnloadStaticContents (0x00BDE020).
// Layout: PDB CQuadTreeElement (76 bytes, retail-identical: UnloadStaticContents reads FileBlockPos +0x34 and the
// flag byte +0x48): ObjectCacheGroupCollections +0x04, the collection's Next at +0x04.
// Shape: the head is tested once on entry, the rotated loop re-reads it, loads Next BEFORE delete's own null test,
// deletes through the vtable and stores Next back (tested through eax).
struct CLocalDetailCacheMap;
struct CQuadTreeElement;

struct CObjectCacheGroupCollection {               // polymorphic; the chain is singly linked through +4
    virtual ~CObjectCacheGroupCollection();        // slot 0: the deleting destructor
    CObjectCacheGroupCollection* Next;             // +0x04
};

struct CEngineStaticMapFileBlockManager {
    void ReleaseRequest(long fileBlockPos);        // 0x00BE0400
};

struct CEngineLandscapeMemoryManager {
    void RebindBlockOwner(void* block, CQuadTreeElement* owner);   // 0x00BD9AB0 (an empty `ret 8` in retail)
};

struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8

struct CQuadTreeElement {                          // CLocalDetailCacheMap::CQuadTreeElement, PDB layout (76 bytes)
    virtual ~CQuadTreeElement();                   // slot 0: the deleting destructor
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;   // +0x04
    CQuadTreeElement*  Children[4];                // +0x08
    unsigned char      BoundingSphere[0x10];       // +0x18 C3DBoundingSphere
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
    void RemoveObjectCollections();
};

void CQuadTreeElement::RemoveObjectCollections()
{
    while (ObjectCacheGroupCollections) {
        CObjectCacheGroupCollection* next = ObjectCacheGroupCollections->Next;
        delete ObjectCacheGroupCollections;
        ObjectCacheGroupCollections = next;
    }
}
