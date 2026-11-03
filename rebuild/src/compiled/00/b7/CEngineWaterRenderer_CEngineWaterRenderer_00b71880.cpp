// CEngineWaterRenderer::~CEngineWaterRenderer() -- release the constant buffers and the dynamic texture page, then destroy every member in reverse order and the CEngineComponent base
// Leaf: ??1CEngineWaterRenderer@@UAE@XZ
// Flags: none
// Grade: RELOCATION_MATCH (q.py with FABLE_HDR_OVERLAY=hdr/, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B71880, length 1829 bytes (+ int3 padding), __fastcall, tail jump to
//   ~CEngineComponent 0x00B59730.
// Identity: Ego_r.pdb / FableWin.pdb ??1CEngineWaterRenderer@@UAE@XZ; it stores the CEngineWaterRenderer vtable 0x012A3364 first and
//   its only caller is the class's ??_G 0x00B7EE60 (vtable slot 0). The catalog label "~CEnginePrimitiveRendererSharedData" is wrong
//   (that destructor is 0x00B36EC0).
// Body: DeleteConstantBuffers() (Ego_r / FableWin ?DeleteConstantBuffers@CEngineWaterRenderer@@AAEXXZ; retail keeps an out-of-line
//   copy at 0x00B6D4B0 and inlines it here: CIndexBuffer::Uninitialise 0x00AC1EE0 on +0x0C / +0x20, CVertexBufferWin32::Uninitialise
//   0x00A63200 on +0x34) and UninitialiseDynamicTextures() (?UninitialiseDynamicTextures@CEngineWaterRenderer@@AAEXXZ, inline:
//   CGPUScratchBufferTextureManager::FreeMemoryPage 0x00F37030 on the resource manager's scratch manager, then the handle cleared).
//   The rest is the implicit member destruction: ~CCountedPointer x2, ~CCharString 0x0099EAE0 x3, the tesselators' CArrays, Settings
//   (~CBaseClassNonCopyable 0x0099A300), the render lists, the shaders (CPixelShader / CVertexShader releases, the CArray<CVertexShader>
//   elements through their virtual destructor with flag 0), the vertex buffers (Uninitialise + ~CBaseClassNonCopyable) and index
//   buffers (Uninitialise + the CMovableResource vtable store).
// Header: FABLE_RETAIL_CENGINEWATERRENDERER / FABLE_RETAIL_CENGINERESOURCEMANAGER (hand-back hdr/engine/CEngineWaterRenderer.h, typed members) and the new CPixelShader /
//   CShaderBase / CVertexBufferBase / CLandscapeTesselatorBase / CWaterPatchTesselatorBase / CEngineWaterSettings headers plus the
//   FABLE_RETAIL_CVERTEXSHADER / CVERTEXBUFFERWIN32 / CENGINECOMPONENT branches.
// Resource manager: FABLE_RETAIL_CENGINERESOURCEMANAGER (hand-back hdr/engine/CEngineResourceManager.h): the flattened Ego_r header
//   places GPUScratchBufferTextureManager at +0x33C0; retail reads it at +0x3190 (here and in the landed 0x00B23F60 / 0x00B6D630),
//   because retail CShaderDataBank is 0x14 bytes shorter (28 banks before it).
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineResourceManager.h"

class CGPUScratchBufferTextureManager { public:
    void FreeMemoryPage(long page);                    // 0x00F37030 ?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z
};
extern CEngineResourceManager* g_pResourceManager;    // 0x01436E98 (descriptive; no PDB name for this global)

void CEngineWaterRenderer::DeleteConstantBuffers()
{
    ForegroundIndexBuffers.Uninitialise();
    ForegroundTextureIndexBuffers.Uninitialise();
    ForegroundTextureVertexBuffers.Uninitialise();
}

void CEngineWaterRenderer::UninitialiseDynamicTextures()
{
    g_pResourceManager->GetGPUScratchBufferTextureManager().FreeMemoryPage(DynamicTexturePageHandle);
    DynamicTexturePageHandle = 0;
}

CEngineWaterRenderer::~CEngineWaterRenderer()
{
    DeleteConstantBuffers();
    UninitialiseDynamicTextures();
}