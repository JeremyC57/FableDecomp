// CLandscapeLayerMesh::~CLandscapeLayerMesh() -- this class's vtable (0x012A835C), the index buffer at +0x34 queued for disposal (DeleteIndexBuffer 0x00BDA090) unless it is shared (+0x48), the foreground vertex buffer at +0x38 queued (DeleteVertexBuffer 0x00BDA0D0) and cleared, the next mesh of the chain at +0x3c deleted through its virtual destructor, then the CMovableResource base's inline destructor stores its vtable (0x0129B860)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDE20, length 96 bytes.
// Leaf: ??1CLandscapeLayerMesh@@UAE@XZ
// Callee names (NF269, 2026-11-22; were the stand-ins AppendFreeEdgeNode(void*) / FreeBlock(void*)): Ego_r.pdb 009c25e0 / 009c2640 and
//   FableWin.pdb 02cb7350 / 02cb7450 ?DeleteIndexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCIndexBuffer@@@Z /
//   ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z, the spellings the landed self rows define.
// Identity (NF276, 2026-11-22): the virtual destructor ??1CLandscapeLayerMesh@@UAE@XZ (Ego_r.pdb 00a494c0 + FableWin.pdb 02ddd720); its only
//   caller, ??_GCLandscapeLayerMesh 0x00BFE900 (vtable 0x012A835C slot 0), already binds that spelling. Was ??1CLandscapeLayerMesh@@QAE@XZ on a
//   row-local CMovableResource with a non-virtual destructor. Now on the shared retail engine/CMovableResource.h (virtual destructor) and the
//   retail branch of engine/CEngineLandscapeMemoryManager.h (the landed Delete<X> opt-ins), as 0x00BFDD40 is.
// Layout: CLandscapeLayerMesh's only header is quarantined (engine/_quarantine/, header-less for the local-model rule, EXTRA_RULES 2026-11-20):
//   modelled locally on the shared CMovableResource with the Ego_r.pdb members (0x54 bytes) and virtuals of the landed sibling 0x00BFDE80
//   (RTTI .?AVCLandscapeLayerMesh@@ : CMovableResource; slot 1 ?RelocateData@CLandscapeLayerMesh@@EAEXJPAX0J@Z 0x00BFDE80, slot 2
//   ?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z 0x00BFDDB0, both PDBs, private). +0x3C is Next (Ego_r; was the stand-in m_pOwned).
// TU: the destructor is virtual, so this TU also emits ??_7CLandscapeLayerMesh@@6B@ and ??_GCLandscapeLayerMesh@@UAEPAXI@Z (pick-any; retail
//   0x00BFE900: ??1 called, then the class delete) and the CMovableResource vtable / ??_G. The class delete is FableWin.pdb
//   ??3CLandscapeLayerMesh@@SIXPAX@Z (02ddd700; static __fastcall; no out-of-line Ego_r copy, so inline in retail): FreeVirtual 0x00BD9B90.
// Shape: `this` in esi, `ret`.
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEINDEXBUFFER    // DeleteIndexBuffer 0x00BDA090 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER   // DeleteVertexBuffer 0x00BDA0D0 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#include "engine/CMovableResource.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)

class CIndexBuffer;
class CVertexBufferWin32;
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
    static void FABLE_FASTCALL operator delete(void* block) { NEngineInternal::GPLandscapeRenderer->MemoryManager->FreeVirtual(block); }
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

CLandscapeLayerMesh::~CLandscapeLayerMesh()
{
    if (IndexBuffer && !UsesSharedIndexBuffer)
        NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteIndexBuffer(IndexBuffer);
    if (ForegroundVertexBuffer) {
        NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(ForegroundVertexBuffer);
        ForegroundVertexBuffer = 0;
    }
    if (Next)
        delete Next;
}
