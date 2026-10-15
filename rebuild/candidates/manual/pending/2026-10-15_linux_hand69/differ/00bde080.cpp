// CLocalDetailCacheMap::CQuadTreeElement::ValidationTest() -- the release build's consistency walk: step through the object-collection chain (the per-node checks are compiled out), then validate the first two children recursively
// Grade: DIFFER(ours v retail) -- 44v49: the lane compiler turns the tail call on Children[1] into a loop back to +3 (tail-recursion elimination: `mov esi,[esi+0Ch] ; test esi,esi ; jne`), retail keeps `mov ecx,[esi+0Ch] ; test ; pop esi ; je ; jmp ValidationTest`; tried out-of-line, in-class inline + forcing caller, optimize("g",off)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDE080, length 49 bytes.
// Callees: itself (Children[0] by call, Children[1] by tail jump).
// Identity: FableWin.pdb names ?ValidationTest@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ; the body sits between
// UnloadStaticContents (0x00BDE020) and CLocalDetailCacheMap::OnMapVisibilityChanged (0x00BDE160).
// Shape: the empty chain walk survives as a bare `mov eax,[eax+4] ; test ; jne` loop; only Children[0] (+0x08) and
// Children[1] (+0x0c) are visited, each null-tested, the second as a sibling-call `jmp` after `pop esi`.
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
    void ValidationTest();
};

void CQuadTreeElement::ValidationTest()
{
    for (CObjectCacheGroupCollection* collection = ObjectCacheGroupCollections; collection; collection = collection->Next) {
    }
    if (Children[0])
        Children[0]->ValidationTest();
    if (Children[1])
        Children[1]->ValidationTest();
}
