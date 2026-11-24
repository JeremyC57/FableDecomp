// CLandscapeLayerMesh::OnPostRelocation() -- after this mesh moved, re-register it as the owner of its two landscape-memory blocks: the index buffer at +0x34 (only when it is not shared, byte +0x48) and the foreground vertex buffer at +0x38, each through the engine's landscape memory manager (0x01436EA8 -> +0x6b0, 0x00BD9AB0 with (block, this))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDD70, length 64 bytes.
// Callees: 0x00BD9AB0 (manager member, `ret 8`). Shape: `this` in esi, the manager re-read from the engine global for each call, `ret`.
// Leaf: ?OnPostRelocation@CLandscapeLayerMesh@@QAEXXZ
// NF276 (2026-11-22): on the shared retail engine/CMovableResource.h (was a row-local CMovableResource with a placeholder virtual), the local
//   CLandscapeLayerMesh in the landed sibling 0x00BFDE80's Ego_r.pdb layout (quarantined header, header-less for the local-model rule): +0x34 is
//   IndexBuffer and +0x38 ForegroundVertexBuffer (the HEAD names had them the wrong way round, as NF269 found in 0x00BFDE20), +0x48
//   UsesSharedIndexBuffer. The two unused manager declarations respelled in place to the PDB names of 0x00BDA090 / 0x00BDA0D0 (NF269).
#define FABLE_RETAIL_CMOVABLERESOURCE
#include "engine/CMovableResource.h"
class CIndexBuffer;
class CVertexBufferWin32;
class CEngineLandscapeMemoryManager { public:
    void RebindBlockOwner(void* block, CMovableResource* owner);   // 0x00BD9AB0 (an empty `ret 8` in retail; the landed 0x00BDD8E0 model calls it too)
    void DeleteIndexBuffer(CIndexBuffer* buffer);          // 0x00BDA090
    void DeleteVertexBuffer(CVertexBufferWin32* buffer);   // 0x00BDA0D0
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
// Name-fix pass (2026-11-12, NF29): the engine global 0x01436EA8 is g_pLandscapeRenderer (descriptive, CEngineLandscapeRenderer*, MemoryManager
//   +0x6B0: engine/CEngineLandscapeRenderer.h), the spelling of the landed landscape rows; was SEngine_01436ea8 / g_pEngine_01436ea8.
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)
class CGraphicInfoFrame;
class CEngineLandscapeMap;
class CVertex;
#define FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGBLEND   // the shared global enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION (NF276: was a local
#include "engine/CEngineLandscapeMeshBuilder.h"            //   copy with a made-up enumerator)

class CLandscapeLayerMesh : public CMovableResource {    // vtable 0x012A835C, 0x54 bytes
public:
    virtual ~CLandscapeLayerMesh();
private:
    virtual void RelocateData(long handle, void* oldData, void* newData, long size);
    virtual bool CanRelocateData(long handle) const;
public:
    void OnPostRelocation();
    long ForegroundTextureIndex;                         // +0x04
    long BackgroundTextureIndex;                         // +0x08
    long BumpMapTextureIndex;                            // +0x0C
    float SelfIllumination;                              // +0x10
    CGraphicInfoFrame* ForegroundTexture;                // +0x14
    CGraphicInfoFrame* BackgroundTexture;                // +0x18
    CGraphicInfoFrame* BumpMap;                          // +0x1C
    long MinTextureMipMap;                               // +0x20
    long MinBumpMapMipMap;                               // +0x24
    LANDSCAPE_TEXTURE_MAPPING_DIRECTION MappingDirection;// +0x28
    long ForegroundTextureMipmapLevel;                   // +0x2C
    long ForegroundBumpmapTextureMipmapLevel;            // +0x30
    CIndexBuffer* IndexBuffer;                           // +0x34
    CVertexBufferWin32* ForegroundVertexBuffer;          // +0x38
    CLandscapeLayerMesh* Next;                           // +0x3C
    CEngineLandscapeMap* Map;                            // +0x40
    unsigned short VertexCount;                          // +0x44
    unsigned short PolyCountIncDegenerates;              // +0x46
    bool UsesSharedIndexBuffer;                          // +0x48
    CVertexBufferWin32* ProceduralTextureVertexBuffer;   // +0x4C
    CVertex* Vertices;                                   // +0x50
};

void CLandscapeLayerMesh::OnPostRelocation()
{
    if (IndexBuffer && !UsesSharedIndexBuffer)
        NEngineInternal::GPLandscapeRenderer->MemoryManager->RebindBlockOwner(IndexBuffer, this);
    if (ForegroundVertexBuffer)
        NEngineInternal::GPLandscapeRenderer->MemoryManager->RebindBlockOwner(ForegroundVertexBuffer, this);
}