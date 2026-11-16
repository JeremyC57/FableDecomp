// CEngineSurfaceCompositionManager::IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance) -- the same
// number of blends, and per blend the same texture index and diffuse / specular colours within the tolerance (CalcColourDifference 0x00B2AF50)
// Leaf: ?IsNearlyIdentical@CEngineSurfaceCompositionManager@@AAE_NABVCEngineTextureMorph@@0M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2B050.
// NF39 (2026-11-16): moved off its `CEngineSurfaceCompositionManager_Methods` stand-in (const, with local CRGBColour / CCompositeBlendDesc /
//   vector / CEngineTextureMorph models) onto the shared engine/CEngineSurfaceCompositionManager.h (MEMBERS opt-in, which names 0x00B2B050; new
//   nested CALCCOLOURDIFFERENCE opt-in for the private callee), engine/CEngineTextureMorph.h, engine/CCompositeBlendDesc.h and engine/CRGBColour.h.
//   Evidence: Ego_r.pdb / FableWin.pdb private non-const ?IsNearlyIdentical@CEngineSurfaceCompositionManager@@AAE_NABVCEngineTextureMorph@@0M@Z;
//   UpdateDiskCacheTexture 0x00B2D430 calls it with DiskCacheTolerance (already bound).
// Defines: the landed 0x00B2D330 UpdateDiskCacheTexture preamble + CALCCOLOURDIFFERENCE.
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_CALCCOLOURDIFFERENCE
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
#include "engine/CRGBColour.h"
#include "engine/CEngineTextureMorph.h"

extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

// The private callee's body is defined in this TU (as in the landed model this row replaces) so VC7.1 sees that it writes no memory and
//   keeps the two blend-array starts in registers across the calls; retail calls it out of line. Its
//   own landed row is 0x00B2AF50 (not part of the graded body here). Written `inline` (a COMDAT, so it never clashes with that row's
//   definition at link time) and kept out of line with __declspec(noinline).
__declspec(noinline) inline float CEngineSurfaceCompositionManager::CalcColourDifference(const CRGBColour& a, const CRGBColour& b)
{
    float dr = (float)((long)a.R - (long)b.R);
    float dg = (float)((long)a.G - (long)b.G);
    float db = (float)((long)a.B - (long)b.B);
    float da = (float)((long)a.A - (long)b.A);
    return (float)sqrt(da * da + db * db + dg * dg + dr * dr);
}

bool CEngineSurfaceCompositionManager::IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance)
{
    if (a.Blends.size() != b.Blends.size())
        return false;
    unsigned int count = a.Blends.size();
    for (unsigned int i = 0; i < count; ++i) {
        const CCompositeBlendDesc& blendA = a.Blends[i];
        const CCompositeBlendDesc& blendB = b.Blends[i];
        if (blendA.TextureIndex != blendB.TextureIndex)
            return false;
        if (CalcColourDifference(blendA.DiffuseColour, blendB.DiffuseColour) > tolerance)
            return false;
        if (CalcColourDifference(blendA.SpecularColour, blendB.SpecularColour) > tolerance)
            return false;
    }
    return true;
}
