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
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR
#define protected public
#define private public
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
#undef private
#undef protected

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

#include <stdio.h>
#include <stdlib.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
void CIVCountedPointeeBase::Delete() const { delete this; }
CEngineComponent::CEngineComponent() : GlobalPointerAddress(0), RenderEnabled(true), KeepAlive(0) {}
CEngineComponent::~CEngineComponent() {}
CEngineCamera::CEngineCamera(bool) {}
unsigned long CEngineCamera::PeekSceneFilterFlags() const { return 0; }
CEnginePrimitiveTypeInfo::CEnginePrimitiveTypeInfo()
{
    for (unsigned i = 0; i < 47; ++i)
        PrimitiveManagers[i] = 0;
}
unsigned long CEnginePrimitiveTypeInfo::PeekSceneFilterFlags() const { return 0; }
CEnginePrimitiveOcclusionManager::CEnginePrimitiveOcclusionManager() : PrimitiveOccludeDelay(2) {}
unsigned long CEnginePrimitiveOcclusionManager::PeekSceneFilterFlags() const { return 0; }
CEnginePrimitiveRenderUtil::~CEnginePrimitiveRenderUtil() {}
CEnginePrimitiveManager::CEnginePrimitiveManager(CArray<CEngineScene*>& scenes) : SceneList(&scenes), Enable(true) {}
CEnginePrimitiveManager::~CEnginePrimitiveManager() {}
CEngineSceneGridPos::CEngineSceneGridPos() : Level(-1), X(0), Y(0) {}
CEngineSceneGridCell::~CEngineSceneGridCell() {}
CEnginePrimitiveOcclusionInfo::CEnginePrimitiveOcclusionInfo() : InvisibleFrames(0), Seen(false), TestFrame(0) {}
CEnginePrimitiveOcclusionInfo::~CEnginePrimitiveOcclusionInfo() {}
CEnginePersistentPrimitiveBase::CEnginePersistentPrimitiveBase(long type) : Type(type) {}
CEnginePersistentPrimitiveBase::~CEnginePersistentPrimitiveBase() {}
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&) {}
bool CEnginePersistentPrimitiveBase::WasRendered() const { return false; }
CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long mask)
    : CEnginePersistentPrimitiveBase(type), EnginePrimitiveFlags(0), OcclusionLayerBitMask(mask),
      GridCell(0), RenderLayerMask(RENDER_LAYER_MESHES_OCCLUSION), LastFrameRendered(0),
      NextPrimitive(0), RefPrimitive(0), NextLayerMask(0), RefLayerMask(0) {}
CEngineInternalPrimitiveBase::~CEngineInternalPrimitiveBase() {}
CEngineInternalPrimitiveOcclusionBase::~CEngineInternalPrimitiveOcclusionBase() {}
CEngineInternalPrimitiveOcclusionGroup::CEngineInternalPrimitiveOcclusionGroup(
    CEngineSceneGridCell* cell, CEngineSceneGrid* grid, unsigned long x, unsigned long y)
    : CEngineInternalPrimitiveOcclusionBase(ENGINE_INTERNAL_PRIMITIVE_OCCLUSION_GROUP, 2),
      GridCell(cell), Grid(grid), GridX(x), GridY(y), FramesRendered(0) {}
CEngineInternalPrimitiveOcclusionGroup::~CEngineInternalPrimitiveOcclusionGroup() {}

unsigned long CEngineSceneGrid::OcclusionGroupDelay = 2;
unsigned long CEngineSceneGrid::OcclusionMinGroupSize = 3;
unsigned short CEngineInternalPrimitiveBase::CurrentRenderFrame = 99;
CEnginePrimitiveOcclusionManager* NEngineInternal::GPPrimitiveOcclusionManager;
CEnginePrimitiveTypeInfo* NEngineInternal::GPPrimitiveTypeInfo;

static const EEngineRenderLayer layer = RENDER_LAYER_MESHES_OCCLUSION;
static const EEngineOcclusionLayer occ_layer = OCCLUSION_LAYER_PRIMARY_VIEW;
static const unsigned long occ_mask = 2;
static unsigned mask_calls, bounds_calls, clip_calls, primitive_frustum_calls, render_calls, occlusion_calls;
static unsigned remove_calls, delete_calls, create_calls, count_calls;
static unsigned long total_count, layer_mask = layer;
static long occluded_frames;
static bool bounds_visible = true, camera_visible = true, primitive_visible = true;
static bool seen_inside, group_reset;
static ERendered next_result = RENDERED_NOTHING;

unsigned long CEngineSceneGrid::GetCellRenderMask(unsigned long, unsigned long)
{
    ++mask_calls;
    return layer_mask;
}
bool CEngineSceneGrid::GetCellBoundingBoxAndDrawDistance(C3DBoundingBox&, float& dist, unsigned long, unsigned long)
{
    ++bounds_calls;
    dist = 5.0f;
    return bounds_visible;
}
bool CEngineCamera::ClipWorldSpaceBoundingBox(const C3DBoundingBox&, float dist, bool& inside) const
{
    ++clip_calls;
    inside = true;
    return camera_visible && dist == 5.0f;
}
unsigned long CEngineSceneGrid::GetCellTotalPrimitiveCount(unsigned long, unsigned long)
{
    ++count_calls;
    return total_count;
}
void CEngineSceneGridCell::CreateOcclusionGroup(EEngineOcclusionLayer check, CEngineSceneGrid*, unsigned long x, unsigned long y)
{
    if (check != occ_layer || x || y)
        abort();
    ++create_calls;
}
void CEngineSceneGridCell::DeleteOcclusionGroup(EEngineOcclusionLayer check, bool reset)
{
    if (check != occ_layer)
        abort();
    ++delete_calls;
    group_reset = reset;
}
long CEnginePrimitiveOcclusionManager::GetOccludedFrames(CEnginePrimitiveOcclusionInfo&)
{
    ++occlusion_calls;
    return occluded_frames;
}
void CEngineInternalPrimitiveBase::RemoveFromList()
{
    ++remove_calls;
    *RefPrimitive = NextPrimitive;
}
bool CEngineInternalPrimitiveBase::IsInsideFrustum(const CEngineCamera&, bool all_inside)
{
    ++primitive_frustum_calls;
    seen_inside = all_inside;
    return primitive_visible;
}
CEngineInternalPrimitiveOcclusionBase* CEngineInternalPrimitiveBase::GetAsOcclusionPrimitive()
{
    return 0;
}

class TestOcclusionGroup : public CEngineInternalPrimitiveOcclusionGroup {
public:
    TestOcclusionGroup(CEngineSceneGridCell* cell, CEngineSceneGrid* grid)
        : CEngineInternalPrimitiveOcclusionGroup(cell, grid, 0, 0)
    {
        RenderLayerMask = 0;
    }
    CEngineInternalPrimitiveOcclusionBase* GetAsOcclusionPrimitive() { return this; }
};

class TestOccludingPrimitive : public CEngineInternalPrimitiveOcclusionBase {
public:
    TestOccludingPrimitive() : CEngineInternalPrimitiveOcclusionBase(ENGINE_PRIMITIVE_LINE, occ_mask) {}
    CEngineInternalPrimitiveOcclusionBase* GetAsOcclusionPrimitive() { return this; }
};

class TestPrimitiveManager : public CEnginePrimitiveManager {
public:
    TestPrimitiveManager(CArray<CEngineScene*>& scenes) : CEnginePrimitiveManager(scenes) {}
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType) { abort(); }
    void GetSupportedTypes(CEnginePrimitiveTypeList&) const { abort(); }
    ERendered RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer check, CEngineSubPrimitiveChain& chain)
    {
        if (check != layer || chain.LastOpaque || chain.LastAlpha)
            abort();
        ++render_calls;
        return next_result;
    }
};

static void SetupCell(CEngineSceneGridCell& cell)
{
    cell.Parent = 0;
    cell.FirstPrimitive = 0;
    cell.WasRendered = false;
    cell.HasOcclusionResults = false;
    cell.OccludedLayers = 0;
    cell.OccludedFrames = 0;
}

int main()
{
    CEngineSceneGrid grid(0), children(1);
    CEngineSceneGridCell cell, child;
    SetupCell(cell);
    SetupCell(child);
    const CEngineCamera camera(false);
    CEngineSceneRenderStats stats = {};
    CEnginePrimitiveTypeInfo type_info;
    CEnginePrimitiveOcclusionManager occlusion_manager;
    NEngineInternal::GPPrimitiveTypeInfo = &type_info;
    NEngineInternal::GPPrimitiveOcclusionManager = &occlusion_manager;
    CArray<CEngineScene*> scenes;
    TestPrimitiveManager manager(scenes);
    type_info.PrimitiveManagers[ENGINE_PRIMITIVE_LINE] = &manager;
    bool cell_visible = false;

    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible) || mask_calls)
        return 1;
    grid.GridCells[0] = &cell;
    cell.OccludedLayers = occ_mask;
    cell.HasOcclusionResults = true;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || mask_calls || !cell.HasOcclusionResults)
        return 2;
    cell.OccludedLayers = 0;
    layer_mask = 0;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || mask_calls != 1 || cell.HasOcclusionResults || cell.WasRendered)
        return 3;

    layer_mask = layer;
    bounds_visible = false;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, false, stats, occ_layer, false, cell_visible)
        || bounds_calls != 1 || clip_calls || cell.WasRendered)
        return 4;
    bounds_visible = true;
    camera_visible = false;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, false, stats, occ_layer, false, cell_visible)
        || bounds_calls != 2 || clip_calls != 1 || cell.WasRendered)
        return 5;

    camera_visible = true;
    CEngineInternalPrimitiveBase plain(ENGINE_PRIMITIVE_LINE, occ_mask);
    cell.FirstPrimitive = &plain;
    plain.RenderLayerMask = 0;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || primitive_frustum_calls || render_calls)
        return 6;
    plain.RenderLayerMask = layer;
    plain.OcclusionLayerBitMask = 0;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || primitive_frustum_calls || render_calls)
        return 7;
    plain.OcclusionLayerBitMask = occ_mask;
    primitive_visible = false;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || primitive_frustum_calls != 1 || render_calls)
        return 8;
    primitive_visible = true;
    next_result = RENDERED_BOUNDING_VOLUME;
    if (!grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, false, stats, occ_layer, false, cell_visible)
        || render_calls != 1 || plain.LastFrameRendered || !seen_inside || !cell.WasRendered)
        return 9;
    next_result = RENDERED_OBJECT;
    if (!grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || render_calls != 2 || plain.LastFrameRendered != 99)
        return 10;

    TestOcclusionGroup group(&cell, &grid);
    group.NextPrimitive = &plain;
    group.RefPrimitive = &cell.FirstPrimitive;
    group.FramesRendered = 2;
    cell.FirstPrimitive = &group;
    plain.RenderLayerMask = 0;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || cell_visible || occlusion_calls || remove_calls)
        return 17;
    group.OcclusionInfo[occ_layer].Reset(new CEnginePrimitiveOcclusionInfo);
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, OCCLUSION_LAYER_NONE, false, cell_visible)
        || cell_visible || occlusion_calls || remove_calls)
        return 18;
    occluded_frames = 3;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || cell_visible || occlusion_calls != 1 || remove_calls)
        return 11;
    occluded_frames = 1;
    plain.RenderLayerMask = layer;
    if (!grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || !cell_visible || occlusion_calls != 2 || remove_calls != 1 || delete_calls != 1 || !group_reset
        || cell.FirstPrimitive != &plain)
        return 12;

    cell_visible = false;
    TestOccludingPrimitive always_visible;
    always_visible.EnginePrimitiveFlags = 0x4000;
    cell.FirstPrimitive = &always_visible;
    if (!grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || !cell_visible || occlusion_calls != 2 || remove_calls != 1)
        return 19;

    cell_visible = false;
    cell.FirstPrimitive = 0;
    grid.LevelDown = &children;
    children.GridCells[3] = &child;
    TestOcclusionGroup child_group(&child, &children);
    child_group.NextPrimitive = &plain;
    child_group.RefPrimitive = &child.FirstPrimitive;
    child.FirstPrimitive = &child_group;
    if (!grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, true, cell_visible)
        || !cell_visible || !child.WasRendered || !cell.HasOcclusionResults || delete_calls != 2 || group_reset)
        return 13;

    grid.LevelDown = 0;
    cell_visible = false;
    cell.Parent = &child;
    cell.OccludedFrames = 0;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || cell.OccludedFrames != 1 || create_calls)
        return 14;
    cell.HasOcclusionResults = true;
    total_count = 2;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || cell.OccludedFrames || create_calls || count_calls != 1)
        return 15;
    cell.HasOcclusionResults = true;
    cell.OccludedFrames = 1;
    total_count = 3;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || cell.OccludedFrames || create_calls != 1 || count_calls != 2)
        return 16;

    cell.Parent = 0;
    cell.HasOcclusionResults = true;
    cell.OccludedFrames = 1;
    if (grid.RenderCellClippedAndCheckVisibility(0, 0, layer, occ_mask, camera, true, stats, occ_layer, false, cell_visible)
        || cell.OccludedFrames || create_calls != 1 || count_calls != 2)
        return 20;

    puts("00b92950_TEST PASS");
    return 0;
}

// An unexpected call to another engine vtable entry fails the fixture.
void CEngineComponent::FrameStart(const CEngineComponentParams&) { abort(); }
void CEngineComponent::FrameEnd(const CEngineComponentParams&) { abort(); }
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&) { abort(); }
void CEngineComponent::Render(const CEngineComponentRenderParams&) { abort(); }
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&) { abort(); }
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&) { abort(); }
void CEngineComponent::WorldFrameUpdate(bool) { abort(); }
void CEngineComponent::PausedWorldFrameUpdate() { abort(); }
CCharString CEngineComponent::GetConsoleEnableFunctionName() { abort(); }
void CEngineComponent::OnReleaseDefaultPoolResources() { abort(); }
bool CEngineComponent::OnRestoreDefaultPoolResources() { abort(); }
void CEnginePrimitiveManager::FrameStart() { abort(); }
void CEnginePrimitiveManager::FrameEnd() { abort(); }
void CEnginePrimitiveManager::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) { abort(); }
void CEnginePrimitiveManager::RenderBoundingVolume(CEngineSubPrimitiveBase& sub) { abort(); }
ERendered CEnginePrimitiveManager::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& prim) { abort(); }
bool CEnginePrimitiveManager::PreloadPrimitive(CEngineInternalPrimitiveBase& prim, const C3DVector& pos, unsigned long flags) { abort(); }
void CEnginePrimitiveManager::Render2DPrimitive(CEngineInternalPrimitiveBase& prim, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type) { abort(); }
void CEnginePrimitiveManager::RenderNonsortedPrimitives() { abort(); }
void CEnginePrimitiveManager::AddStaticShadowData(CEngineInternalPrimitiveBase& prim, NEngineShadowRenderer::CShadowSceneContext& context) { abort(); }
void CEngineInternalPrimitiveBase::Update(const CEnginePrimitive&,unsigned long) { abort(); }
unsigned long CEngineInternalPrimitiveBase::AddChildPrimitive(const CEnginePrimitive&,unsigned long) { abort(); }
void CEngineInternalPrimitiveBase::SetChildPrimitive(unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long) { abort(); }
void CEngineInternalPrimitiveBase::SetChildPrimitive(unsigned long,const CEnginePrimitive&,unsigned long) { abort(); }
void CEngineInternalPrimitiveBase::GetChildPrimitiveHandle(unsigned long,CEnginePrimitiveHandle&) { abort(); }
void CEngineInternalPrimitiveBase::ClearChildPrimitive(unsigned long) { abort(); }
void CEngineInternalPrimitiveBase::ClearAllChildPrimitives() { abort(); }
CEnginePersistentPrimitiveBase* CEngineInternalPrimitiveBase::GetChildPrimitive(unsigned long) { abort(); }
CEngineFadeDistance CEngineInternalPrimitiveBase::GetFadeDistance() const { abort(); }
void CEngineInternalPrimitiveBase::PostRender() { abort(); }
bool CEngineInternalPrimitiveBase::GetWorldPosition(C3DVector&) { abort(); }
bool CEngineInternalPrimitiveBase::GetBoundingBoxTransform(CMatrix3x4&) { abort(); }
bool CEngineInternalPrimitiveBase::GetBoundingBoxWorldSpace(C3DBoundingBox&) { abort(); }
bool CEngineInternalPrimitiveBase::GetBoundingSphereWorldSpace(C3DBoundingSphere&) { abort(); }
bool CEngineInternalPrimitiveBase::GetBoundingSphereRadiusWorldSpace(float&) { abort(); }
bool CEngineInternalPrimitiveBase::GetObjectTransform(CMatrix3x4&) const { abort(); }
bool CEngineInternalPrimitiveBase::SetObjectTransform(const CMatrix3x4&) { abort(); }
void CEngineInternalPrimitiveBase::SetAnimation(const CEnginePrimitiveAnimation&) { abort(); }
void CEngineInternalPrimitiveBase::SetPosition(const CEnginePrimitivePosition&) { abort(); }
void CEngineInternalPrimitiveBase::SetMeshEffect(const CIVCountedPointer<CEngineMeshEffect>&) { abort(); }
bool CEngineInternalPrimitiveBase::GetMeshEffect(CIVCountedPointer<CEngineMeshEffect>&) const { abort(); }
void CEngineInternalPrimitiveBase::CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long) const { abort(); }
void CEngineInternalPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&) {}
bool CEngineInternalPrimitiveBase::WasRendered() const { return false; }