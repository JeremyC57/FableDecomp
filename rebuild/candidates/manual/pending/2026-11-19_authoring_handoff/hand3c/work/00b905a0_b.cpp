// CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest(CEngineInternalPrimitiveOcclusionBase& primitive, ERenderPassType render_pass, unsigned long& out_occlusion_handle)
// Leaf: ?PrepareOcclusionTest@CEnginePrimitiveManagerOcclusionBase@@IAEXAAVCEngineInternalPrimitiveOcclusionBase@@W4ERenderPassType@@AAK@Z
// Flags: none
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPAREOCCLUSIONTEST
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_ISOCCLUDED
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ALLOCATE
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONINFO_ADDOCCLUSIONTEST
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEnginePrimitiveOcclusionInfo.h"
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
