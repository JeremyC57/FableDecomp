// CEngineLandscapePatch::Clear() -- releases every layer mesh's foreground vertex buffer, deletes its child object, frees its vertex block through the landscape memory manager, releases its counted resource and deletes its second owned object
// Leaf: ?Clear@CEngineLandscapePatch@@QAEXXZ
// Retry 10b: the +0x14 object is deleted through a by-reference helper (`SafeDeleteRef(m_pResource)` = `delete p; p = 0;` on a T*&) inside the `if`: the reference keeps delete's own null test (`mov ecx,eax ; test ecx,ecx`) that a direct `delete m_pResource` folds; a reference local does the same.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF38F0, length 132 bytes (144 with padding).
// Callees: CLandscapeLayerMesh::DeleteForegroundVertexBuffer @ 0x00BFDD40 (landed; manifest "CLandscapeBackgroundPatch::ClearTextures"),
// the owned objects' virtual scalar deleting destructors (vtable slot 0, `push 1`),
// CEngineLandscapeMemoryManager::FreeVirtual @ 0x00BD9B90 (the manager at engine 0x01436EA8 -> +0x6B0),
// the counted-resource member's DiscardAsyncData-shaped release @ 0x009D68A0 on +0x40 (`lea ecx,[esi+40h]`).
// Shape: a 3-bit state field in the byte at +0x3C (bit 1 = "textures attached"): when set, the chain of
// layer meshes hung at +8 (LayerMeshList, linked through Next +0x3C) is cleared and the state reset; the state is
// reset again after the first mesh at +8 is deleted. `delete` on a known non-null member drops VC's own null
// test; the second owned pointer (+0x14) is deleted through the reference helper, which keeps it (`mov ecx,eax ;
// test ecx,ecx`). The patch's only header is quarantined (engine/_quarantine/); it is modelled locally (the landed destructor @ 0x00BF3A40
// shows the CMovableResource vptr at +0 and the counted pointer at +0x40).
class CEngineLandscapeMemoryManager { public:
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
// Name-fix pass (2026-11-12, NF29): the engine global 0x01436EA8 is g_pLandscapeRenderer (descriptive, CEngineLandscapeRenderer*, MemoryManager
//   +0x6B0: engine/CEngineLandscapeRenderer.h), the spelling of the landed landscape rows; was SEngine_01436ea8 / g_pEngine_01436ea8.
// NF272 (2026-11-22): the chain element is CLandscapeLayerMesh (was the stand-in `struct CTextureOwner` binding 0x00BFDD40 as
//   ?ClearBlock@CTextureOwner@@QAEXXZ): the patch's +0x08 is CEngineLandscapePatch::LayerMeshList and the chain link +0x3C is
//   CLandscapeLayerMesh::Next (Ego_r layouts, engine/_quarantine/CEngineLandscapePatch.h / CLandscapeLayerMesh.h); 0x00BFDD40 is
//   ?DeleteForegroundVertexBuffer@CLandscapeLayerMesh@@QAEXXZ (Ego_r.pdb 00a49360 + FableWin.pdb 02ddde80; frees +0x38 ForegroundVertexBuffer).
//   Quarantined class (header-less for the local-model rule): modelled in the landed siblings' shape (0x00BFDE20 / 0x00BFDD70) on the shared
//   retail CMovableResource base; deleted through its virtual destructor (slot 0, Ego_r / FableWin ??1CLandscapeLayerMesh@@UAE@XZ).
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#include "engine/CMovableResource.h"
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)

class CLandscapeLayerMesh : public CMovableResource { public:    // vtable 0x012A835C (the chain element, 0x54 bytes)
    virtual ~CLandscapeLayerMesh();                // vtable slot 0
    unsigned char        _pad04[0x38];
    CLandscapeLayerMesh* Next;                     // +0x3C
    void DeleteForegroundVertexBuffer();           // 0x00BFDD40
};

struct CCountedResourcePointer {
    void* m_pObject;                               // +0x00
    void* m_pBlock;                                // +0x04
    void Discard();                                // 0x009D68A0
};

struct COwnedResource {
    virtual ~COwnedResource();                     // vtable slot 0
};

template <class T> inline void SafeDeleteRef(T*& p) { delete p; p = 0; }
class CEngineLandscapePatch { public:
    virtual ~CEngineLandscapePatch();              // +0x00 vptr (CMovableResource)
    unsigned char           _pad04[0x4];
    CLandscapeLayerMesh*    LayerMeshList;         // +0x08
    void*                   m_pVertexBlock;        // +0x0C
    unsigned char           _pad10[0x4];
    COwnedResource*         m_pResource;           // +0x14
    unsigned char           _pad18[0x24];
    unsigned char           m_state : 3;           // +0x3C
    CCountedResourcePointer m_counted;             // +0x40
    void Clear();
};

void CEngineLandscapePatch::Clear()
{
    if (m_state & 2) {
        for (CLandscapeLayerMesh* mesh = LayerMeshList; mesh; mesh = mesh->Next)
            mesh->DeleteForegroundVertexBuffer();
        m_state = 0;
    }
    if (LayerMeshList) {
        delete LayerMeshList;
        LayerMeshList = 0;
        m_state = 0;
    }
    if (m_pVertexBlock) {
        NEngineInternal::GPLandscapeRenderer->MemoryManager->FreeVirtual(m_pVertexBlock);
        m_pVertexBlock = 0;
    }
    m_counted.Discard();
    if (m_pResource)
        SafeDeleteRef(m_pResource);
}