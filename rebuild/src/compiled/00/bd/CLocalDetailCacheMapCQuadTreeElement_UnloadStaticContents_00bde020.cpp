// CLocalDetailCacheMap::CQuadTreeElement::UnloadStaticContents(CEngineStaticMapFileBlockManager*) -- cancel a pending file-block load (ReleaseRequest on FileBlockPos, clear Loading); if the static contents are loaded, delete the object collections and the four children and clear StaticContentsLoaded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDE020, length 87 bytes.
// Callees: CEngineStaticMapFileBlockManager::ReleaseRequest(long) @ 0x00BE0400, RemoveObjectCollections @ 0x00BDDA50,
// CQuadTreeElement's deleting destructor (virtual slot 0, with 1) for each child.
// Identity: FableWin.pdb names ?UnloadStaticContents@CQuadTreeElement@CLocalDetailCacheMap@@QAEXPAVCEngineStaticMapFileBlockManager@@@Z;
// the landed CLocalDetailCacheMap::OnMapVisibilityChanged (0x00BDE160) calls it with the map's file-block manager.
// Layout: PDB bool bitfields at +0x48 (HasDynamicAreasFlag bit 0, RenderUpdateNeeded 1, StaticContentsLoaded 2,
// Loading 3), FileBlockPos +0x34, Children[4] +0x08.
// Shape: `test byte [+48h],8` / `and byte [+48h],0F7h` for Loading; the children are a counted loop (ebx = 4) that
// deletes through the vtable (null-tested) and zeroes each slot; StaticContentsLoaded is cleared through al.
class CLocalDetailCacheMap;
struct CQuadTreeElement;

struct CObjectCacheGroupCollection {               // polymorphic; the chain is singly linked through +4
    virtual ~CObjectCacheGroupCollection();        // slot 0: the deleting destructor
    CObjectCacheGroupCollection* Next;             // +0x04
};

class CEngineStaticMapFileBlockManager { public:
    void ReleaseRequest(long fileBlockPos);        // 0x00BE0400
};

class CEngineLandscapeMemoryManager { public:
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
    void RemoveObjectCollections();                // 0x00BDDA50
    void UnloadStaticContents(CEngineStaticMapFileBlockManager* manager);
};

void CQuadTreeElement::UnloadStaticContents(CEngineStaticMapFileBlockManager* manager)
{
    if (Loading) {
        manager->ReleaseRequest(FileBlockPos);
        Loading = false;
    }
    if (StaticContentsLoaded) {
        RemoveObjectCollections();
        for (long i = 0; i < 4; ++i) {
            delete Children[i];
            Children[i] = 0;
        }
        StaticContentsLoaded = false;
    }
}