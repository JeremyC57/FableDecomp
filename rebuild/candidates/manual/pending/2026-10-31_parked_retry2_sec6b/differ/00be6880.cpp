// CLandscapeBackgroundPatch::BuildDefaultSection() -- PARTIAL (prefix only): set the default 5-vertex / 4-polygon section, allocate the patch vertex buffer (0x18-byte vertices) from the landscape memory manager with the resource manager's vertex-buffer usage and lock it; the vertex fill, edge strips and index buffer write after that are not reconstructed yet
// Grade: DIFFER(89v1776) 393 diff lines (parked stub 19v1776, 415 lines): PARTIAL -- the default section sizes, the vertex-buffer allocation (owner, type 0, VertexCount, stride 0x18, usage) and the full-size Lock are written on engine/CLandscapeBackgroundPatch.h; the /GS 0xf8 frame, the x87 vertex/normal fill with _ftol texture coordinates, the edge strips and the index-buffer write are not reconstructed
// Leaf: ?BuildDefaultSection@CLandscapeBackgroundPatch@@QAEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE6880, length 1776 bytes, 0xf8 frame with the /GS cookie (0x0139C8A8).
// Callees (prefix): CEngineResourceManager::GetVertexBufferUsage 0x00B3B030 (on 0x01436E98), CEngineLandscapeMemoryManager::
//   AllocateVertexBuffer(CMovableResource*, EVertexType, long, long, unsigned long) 0x00BDA3D0 ([[0x01436EA8]+0x6b0]),
//   CVertexBufferWin32::Lock(unsigned long, unsigned long, unsigned long) 0x00AC20A0 (three spellings in reloc_bindings).
// Layout: engine/CLandscapeBackgroundPatch.h (VertexBuffer +0xc0, VertexCount +0x104, PolyCountIncDegenerates +0x106).
#define CLandscapeBackgroundPatch CLandscapeBackgroundPatchLayout
#include "engine/CLandscapeBackgroundPatch.h"
#undef CLandscapeBackgroundPatch
class CMovableResource;
enum EVertexType { VertexTypeDefault = 0 };
class CVertexBufferWin32 { public:
    unsigned char _pad00[8];
    unsigned long Size;                                 // +0x08 (descriptive)
    void* Lock(unsigned long offset, unsigned long flags, unsigned long size);   // 0x00AC20A0
};
class CEngineResourceManager { public: unsigned long GetVertexBufferUsage(); };   // 0x00B3B030
extern CEngineResourceManager* g_pEngineResourceManager;                         // 0x01436E98
class CEngineLandscapeMemoryManager { public:
    CVertexBufferWin32* AllocateVertexBuffer(CMovableResource* owner, EVertexType type, long count, long stride, unsigned long usage);   // 0x00BDA3D0
};
struct SEngineLandscape { unsigned char _pad[0x6b0]; CEngineLandscapeMemoryManager* MemoryManager; };   // +0x6b0
extern SEngineLandscape* g_pEngineLandscape;                                     // 0x01436EA8
class CLandscapeBackgroundPatch : public CLandscapeBackgroundPatchLayout { public: void BuildDefaultSection(); };

void CLandscapeBackgroundPatch::BuildDefaultSection()
{
    PolyCountIncDegenerates = 4;
    VertexCount = 5;
    CEngineLandscapeMemoryManager* manager = g_pEngineLandscape->MemoryManager;
    VertexBuffer = manager->AllocateVertexBuffer((CMovableResource*)this, VertexTypeDefault, VertexCount, 0x18,
                                                 g_pEngineResourceManager->GetVertexBufferUsage());
    VertexBuffer->Lock(0, 0, VertexBuffer->Size);
}
