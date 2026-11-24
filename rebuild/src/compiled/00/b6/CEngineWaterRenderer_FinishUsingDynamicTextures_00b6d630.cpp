// Leaf: ?FinishUsingDynamicTextures@CEngineWaterRenderer@@AAEXXZ
// CEngineWaterRenderer::FinishUsingDynamicTextures() -- hand the four dynamic water textures back to the engine's texture pool and clear their slots
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6D630, length 103 bytes (+ int3 padding).
// Callees: the texture pool release @ 0x00F36F00 four times (on the global @ 0x01436E98 + 0x3190, kept in edi).
// Layout: four (texture, slot) word pairs at +0x1dc/+0x1e0, +0x1e4/+0x1e8, +0x1ec/+0x1f0, +0x1f4/+0x1f8. The PDB layout
// (engine/CEngineWaterRenderer.h) puts the shader objects at +0x1d4/+0x1dc and (handle, CTexture*) pairs from +0x1e4,
// which does not line up with these retail accesses (the retail block sits 8 bytes lower and is (texture, slot)
// ordered), so the block is modelled locally.
// Shape: the pool address is formed once into a local (edi); each release is followed by its slot clear, which
// VC7.1 schedules before the NEXT push (the zero lives in ebx); the order is +0x1dc, +0x1ec, +0x1e4, +0x1f4.
// NF44 (round 6): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) and its +0x3190 member the CGPUScratchBufferTextureManager
// (engine/CEngineResourceManager.h GetGPUScratchBufferTextureManager(), engine/CGPUScratchBufferTextureManager.h, Ego_r.pdb member
// spellings); were the address-named CEngineGlobal_01436e98 / g_pEngine_01436e98 and local scratch-manager stand-ins.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98


class CEngineWaterRenderer { public:
    unsigned char _pad[0x1dc];
    long           m_pReflectionTexture;            // +0x1dc (scratch texture handle)
    long          m_reflectionSlot;                // +0x1e0
    long           m_pRefractionTexture;            // +0x1e4 (scratch texture handle)
    long          m_refractionSlot;                // +0x1e8
    long           m_pSeaBumpTexture;               // +0x1ec (scratch texture handle)
    long          m_seaBumpSlot;                   // +0x1f0
    long           m_pWaterBumpTexture;             // +0x1f4 (scratch texture handle)
    long          m_waterBumpSlot;                 // +0x1f8
    private:
    void FinishUsingDynamicTextures();
    public:
};

void CEngineWaterRenderer::FinishUsingDynamicTextures()
{
    CGPUScratchBufferTextureManager* pool = &NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    pool->EndTextureUse(m_pReflectionTexture);
    m_reflectionSlot = 0;
    pool->EndTextureUse(m_pSeaBumpTexture);
    m_seaBumpSlot = 0;
    pool->EndTextureUse(m_pRefractionTexture);
    m_refractionSlot = 0;
    pool->EndTextureUse(m_pWaterBumpTexture);
    m_waterBumpSlot = 0;
}