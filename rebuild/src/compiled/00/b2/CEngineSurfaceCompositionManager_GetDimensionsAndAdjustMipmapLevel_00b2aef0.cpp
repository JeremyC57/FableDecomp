// CEngineSurfaceCompositionManager::GetDimensionsAndAdjustMipmapLevel(C2DExtentsI& dimensions, const CGraphicInfoFrame* frame,
// unsigned long& mipmap_level) -- the frame's width and height (the owner graphic's header FrameWidth +6 / FrameHeight +8) shifted down by the
// mip level; while either exceeds MaxCompositeTextureSize (+0x10C8), halve both and raise the level
// Leaf: ?GetDimensionsAndAdjustMipmapLevel@CEngineSurfaceCompositionManager@@AAEXAAVC2DExtentsI@@PBVCGraphicInfoFrame@@AAK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2AEF0, length 91 bytes, `ret 0Ch`; no callees.
// Shape: `movzx` loads shifted by `cl` from the level's low byte (re-read for the second), the loop re-reads the limit and both dimensions each
//   pass and halves with the signed `cdq ; sub ; sar 1`; the loop head is aligned with `lea esp,[esp]`.
// NF39 (2026-11-16): moved off its local manager / texture-entry models (pointer arguments) onto the shared engine/CEngineSurfaceCompositionManager.h
//   (MEMBERS opt-in, which names 0x00B2AEF0; MaxCompositeTextureSize +0x10C8), engine/CGraphicInfoFrame.h + engine/CGraphicInfo.h (the inline
//   FableWin.pdb GetWidth / GetHeight, whose header comment cites this function) and engine/C2DExtentsI.h. Evidence: Ego_r.pdb / FableWin.pdb
//   private member ?GetDimensionsAndAdjustMipmapLevel@CEngineSurfaceCompositionManager@@AAEXAAVC2DExtentsI@@PBVCGraphicInfoFrame@@AAK@Z;
//   UpdateDiskCacheTexture 0x00B2D51F passes &dimensions, the info frame and &mipmap_level (already bound).
// Defines: the landed 0x00B2D330 UpdateDiskCacheTexture preamble.
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_CGRAPHICBANKSTATEBLOCK
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGERCORE_WAITWHILEGPUISBUSY
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_AUGMENTATION_PARTICLES
#define FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CTEXTURE_GENERATEMIPMAPS
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYFROMSURFACE
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_DTOR
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
#include "engine/Direct3D9Types.h"
#include "engine/CEngineSurfaceCompositionManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicInfoFrame.h"
#include "engine/CCompositeSurfaceCache.h"
#include "engine/CGraphicBankStateBlock.h"
#include "engine/C2DExtentsI.h"
#include "engine/CArray.h"
#include "engine/CRenderManager.h"
#include "engine/GFGetNextPowerOfTwo.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/C3DVector.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
#include "engine/CGraphicFrame.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CSurfaceCompositor.h"

void CEngineSurfaceCompositionManager::GetDimensionsAndAdjustMipmapLevel(C2DExtentsI& dimensions, const CGraphicInfoFrame* frame, unsigned long& mipmap_level)
{
    dimensions.X = frame->GetWidth() >> mipmap_level;
    dimensions.Y = frame->GetHeight() >> mipmap_level;
    while (dimensions.X > MaxCompositeTextureSize || dimensions.Y > MaxCompositeTextureSize) {
        dimensions.X /= 2;
        dimensions.Y /= 2;
        ++mipmap_level;
    }
}
