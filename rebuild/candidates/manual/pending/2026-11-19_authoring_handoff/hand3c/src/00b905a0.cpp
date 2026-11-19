// CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest(CEngineInternalPrimitiveOcclusionBase& primitive, ERenderPassType render_pass, unsigned long& out_occlusion_handle)
// Leaf: ?PrepareOcclusionTest@CEnginePrimitiveManagerOcclusionBase@@IAEXAAVCEngineInternalPrimitiveOcclusionBase@@W4ERenderPassType@@AAK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B905A0, 278 bytes, `ret 0Ch`. Callees: memmove 0x00BFEAE6 and vector<unsigned long>::
//   _M_insert_overflow 0x00464ACE (both from the inline STLport clear / push_back). Globals: NEngineInternal::GPPrimitiveRendererSharedData
//   0x01436E78 (CurrentOcclusionLayer +0x78, OcclusionEnable +0xBD) and NEngineInternal::GPOcclusionManager 0x01436E5C (CurrentFrame
//   +0x4010, Entries +0x4014, EntriesSize +0x401C).
// Shape: FableWin.pdb 0x02ED79B0 statement for statement: GetOcclusionEnable() && render_pass == 2 && primitive.HasOcclusionInfo(layer);
//   GetOcclusionInfo(layer); out_occlusion_handle = AllocateOccluderHandle(); != -1 -> AddOcclusionTest; else out_occlusion_handle = -1.
//   Ego_d.pdb names: primitive, render_pass, out_occlusion_handle, occlusion_info. Every helper is a FableWin inline that retail
//   expands. The static getters and AddOcclusionTest are defined in the row, as the landed 0x00BB3E80 does (its header comments say
//   the including row defines them). The rest comes from the shared headers. The opt-in set is 0x00BB3E80's, plus
//   ..._PREPAREOCCLUSIONTEST and ..._ISOCCLUDED (HasOcclusionInfo).
// The earlier park (DIFFER 272v280, "shrink-wrapped prologue lane gap") used a local model. On the shared headers VC7.1 pushes
//   ebx/esi/edi after the early-outs exactly as retail.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPAREOCCLUSIONTEST
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_ISOCCLUDED
#include "fable_compiler.h"
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CENGINECLOTHRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINECLOTHRENDERER_RENDER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHSHADERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_ACCESSORS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD_DIRECT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ALLOCATE
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONINFO_ADDOCCLUSIONTEST
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC
#define FABLE_RETAIL_C3DMESHMATERIAL_FLAGS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER
#define FABLE_RETAIL_C3DCLOTHPRIMITIVE_GETPARTICLEPROGRAM_GET
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETRGBCOLOUR_NAMED_RESULT
#include "engine/CMatrix3x4.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEngineSubPrimitiveRepeatedStaticMesh.h"
#include "engine/CEngineInternalPrimitiveMeshBase.h"
#include "engine/CEngineInternalPrimitiveMorphedStaticMesh.h"
#include "engine/CEnginePrimitiveOcclusionInfo.h"
#include "engine/CEngineRenderInfoMeshBase.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CGraphicInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/C3DPrimitive2.h"
#include "engine/CParticleSimulation.h"
#include "engine/CParticleProgram.h"
#include "engine/CCountedPointer.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C4DVector.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CArray.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFLimit.h"
#include "fable_assert.h"
#include "engine/C3DClothPrimitive.h"
#include "engine/CEngineClothRenderer.h"
#include "engine/CEnginePrimitiveManagerOcclusionBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"

namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }   // 0x01436E78
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }                        // 0x01436E5C

inline EEngineOcclusionLayer __fastcall CEnginePrimitiveRenderUtil::GetCurrentOcclusionLayer()
{
    return NEngineInternal::GPPrimitiveRendererSharedData->GetCurrentOcclusionLayer();
}
inline bool __fastcall CEnginePrimitiveRenderUtil::GetOcclusionEnable()
{
    return NEngineInternal::GPPrimitiveRendererSharedData->GetOcclusionEnable();
}
FABLE_FORCEINLINE void CEnginePrimitiveOcclusionInfo::AddOcclusionTest(unsigned long handle)
{
    if (NEngineInternal::GPOcclusionManager->CurrentFrame != TestFrame)
        ClearHandles();
    OcclusionTestHandles.push_back(handle);
    TestFrame = NEngineInternal::GPOcclusionManager->CurrentFrame;
}

void CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest(CEngineInternalPrimitiveOcclusionBase& primitive, ERenderPassType render_pass,
    unsigned long& out_occlusion_handle)
{
    if (CEnginePrimitiveRenderUtil::GetOcclusionEnable() && render_pass == RENDER_PASS_COMBINED &&
        primitive.HasOcclusionInfo(CEnginePrimitiveRenderUtil::GetCurrentOcclusionLayer()))
    {
        CEnginePrimitiveOcclusionInfo& occlusion_info = primitive.GetOcclusionInfo(CEnginePrimitiveRenderUtil::GetCurrentOcclusionLayer());
        out_occlusion_handle = NEngineInternal::GPOcclusionManager->AllocateOccluderHandle();
        if (out_occlusion_handle != 0xffffffff)
            occlusion_info.AddOcclusionTest(out_occlusion_handle);
    }
    else
        out_occlusion_handle = 0xffffffff;
}
