// CEngineSceneGrid::RenderCellClippedAndCheckVisibility
// Leaf: ?RenderCellClippedAndCheckVisibility@CEngineSceneGrid@@QAE_NKKW4EEngineRenderLayer@@KABVCEngineCamera@@_NAAVCEngineSceneRenderStats@@W4EEngineOcclusionLayer@@2AA_N@Z
// Flags: default
#define FABLE_RETAIL_SCENEGRID_VISIBILITY_PASS27
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_NEXT_PASS23
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_ISOCCLUDED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONGROUP
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_SET
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_OCCLUDED
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_GETPRIMITIVEMANAGER
#define FABLE_RETAIL_CENGINESCENEMAP_RESIZELEVELS
#define FABLE_RETAIL_CENGINESCENEGRIDCELL_CHECKVISIBILITYRESULTS
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_C3DVECTOR
#include "stlport/fable_map.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEngineSubPrimitiveChain.h"
#include "engine/CEngineSceneGridCell.h"
#include "engine/CEngineSceneMap.h"
#include "engine/CEngineSceneRenderStats.h"

namespace NEngineInternal {
    extern CEnginePrimitiveOcclusionManager* GPPrimitiveOcclusionManager;
    extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo;
}

FABLE_FORCEINLINE bool CEngineInternalPrimitiveOcclusionBase::IsOccluded(EEngineOcclusionLayer layer)
{
    if (!(EnginePrimitiveFlags & 0x4000)
        && (!HasOcclusionInfo(layer) || NEngineInternal::GPPrimitiveOcclusionManager->IsPrimitiveOccluded(*OcclusionInfo[layer])))
        return true;
    return false;
}

bool CEngineSceneGrid::RenderCellClippedAndCheckVisibility(unsigned long x, unsigned long y, EEngineRenderLayer layer,
    unsigned long occlusion_mask, const CEngineCamera& frustum, bool pre_clipped,
    CEngineSceneRenderStats& stats, EEngineOcclusionLayer occ_layer, bool occ_disable, bool& cell_visible)
{
    CEngineSceneGridCell* cell = GetCell(x, y);
    if (!cell)
        return false;
    if (cell->OccludedLayers & occlusion_mask)
        return false;

    bool has_previous_results = cell->HasOcclusionResults;
    cell->HasOcclusionResults = false;
    if (!(GetCellRenderMask(x, y) & layer))
        return false;

    C3DBoundingBox bbox;
    bool all_inside = pre_clipped;
    if (!pre_clipped) {
        float max_draw_dist;
        if (!GetCellBoundingBoxAndDrawDistance(bbox, max_draw_dist, x, y)
            || !frustum.ClipWorldSpaceBoundingBox(bbox, max_draw_dist, all_inside))
            return cell->HasOcclusionResults;
    }
    unsigned long render_result_flags = 0;
    CEngineInternalPrimitiveBase* current_prim = cell->FirstPrimitive;
    while (current_prim) {
        CEngineInternalPrimitiveBase& prim = *current_prim;
        CEngineInternalPrimitiveOcclusionBase* occlusion_prim = prim.GetAsOcclusionPrimitive();
        if (occlusion_prim && (prim.OcclusionLayerBitMask & occlusion_mask)) {
            if (occ_disable || !occlusion_prim->IsOccluded(occ_layer)) {
                cell_visible = true;
                if (prim.GetType() == ENGINE_INTERNAL_PRIMITIVE_OCCLUSION_GROUP) {
                    CEngineInternalPrimitiveOcclusionGroup& group = static_cast<CEngineInternalPrimitiveOcclusionGroup&>(prim);
                    CEngineSceneGridCell* group_cell = group.GetGridCell();
                    CEngineInternalPrimitiveBase* next_prim = current_prim->GetNextPrimitive();
                    current_prim->RemoveFromList();
                    group_cell->DeleteOcclusionGroup(occ_layer, group.IsRenderedMoreThanOnce());
                    current_prim = next_prim;
                    continue;
                }
            }
        }
        if (prim.GetRenderLayerMask() & layer) {
            if (prim.OcclusionLayerBitMask & occlusion_mask) {
                if (prim.IsInsideFrustum(frustum, all_inside)) {
                    CEnginePrimitiveManager* prim_manager = NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(prim.GetType());
                    CEngineSubPrimitiveChain subprim_chain;
                    ERendered result = prim_manager->RenderPrimitive(prim, layer, subprim_chain);
                    if (result == RENDERED_OBJECT)
                        prim.SetAsRendered();
                    render_result_flags |= result;
                }
            }
        }
        current_prim = current_prim->GetNextPrimitive();
    }
    if (render_result_flags)
        cell->HasOcclusionResults = true;

    if (LevelDown) {
        if (false) {} else for (unsigned long c_y = 0; c_y < 2; ++c_y) {
            if (false) {} else for (unsigned long c_x = 0; c_x < 2; ++c_x) {
                bool child_visible = false;
                bool result = LevelDown->RenderCellClippedAndCheckVisibility(c_x + x * 2, c_y + y * 2, layer,
                    occlusion_mask, frustum, all_inside, stats, occ_layer, occ_disable, child_visible);
                cell_visible |= child_visible;
                cell->HasOcclusionResults |= result;
            }
        }
    }
    if (has_previous_results && !cell_visible) {
        ++cell->OccludedFrames;
        if (cell->OccludedFrames >= OcclusionGroupDelay) {
            if (cell->Parent && GetCellTotalPrimitiveCount(x, y) >= OcclusionMinGroupSize)
                cell->CreateOcclusionGroup(occ_layer, this, x, y);
            cell->OccludedFrames = 0;
        }
    } else {
        cell->OccludedFrames = 0;
    }
    cell->WasRendered = true;
    return cell->HasOcclusionResults;
}