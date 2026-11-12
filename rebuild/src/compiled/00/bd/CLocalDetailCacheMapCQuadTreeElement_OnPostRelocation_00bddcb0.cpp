// CLocalDetailCacheMap::CQuadTreeElement::OnPostRelocation() -- after this element moved, re-register it as the owner of its four child elements and of every object-cache group collection in its chain with the engine's landscape memory manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDCB0, length 85 bytes.
// Callees: CEngineLandscapeMemoryManager member @ 0x00BD9AB0 ((block, owner), an empty `ret 8` in retail) through
// the engine global @ 0x01436EA8 -> +0x6b0 (as in the landed CLocalDetailPrimitiveMeshZSpriteBatch::OnPostRelocation).
// Identity: FableWin.pdb names ?OnPostRelocation@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ.
// Shape: the manager is read once into ebx (a named local); the four children are a counted loop (ebp = 4, esi
// walks &Children[i]) with a null test; the collection chain is a for loop over Next (aligned loop heads).
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

// Name-fix pass (2026-11-12, NF29): the engine global 0x01436EA8 is g_pLandscapeRenderer (descriptive, CEngineLandscapeRenderer*, MemoryManager
//   +0x6B0: engine/CEngineLandscapeRenderer.h), the spelling of the landed landscape rows; was SEngine_01436ea8 / g_pEngine_01436ea8.
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;   // 0x01436EA8 (descriptive)

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
    void OnPostRelocation();
};

void CQuadTreeElement::OnPostRelocation()
{
    CEngineLandscapeMemoryManager* manager = g_pLandscapeRenderer->MemoryManager;
    for (long i = 0; i < 4; ++i) {
        if (Children[i])
            manager->RebindBlockOwner(Children[i], this);
    }
    for (CObjectCacheGroupCollection* collection = ObjectCacheGroupCollections; collection; collection = collection->Next)
        manager->RebindBlockOwner(collection, this);
}