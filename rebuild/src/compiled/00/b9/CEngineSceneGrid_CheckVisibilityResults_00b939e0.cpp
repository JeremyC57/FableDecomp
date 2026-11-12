// CEngineSceneGrid::CheckVisibilityResults(layer, x, y, max_occluded_frames, min_group_size, force) -- collect the occlusion
// results of one grid cell: recurse into the four child cells of the level below that are not occluded on the layer, then ask
// the cell itself; when neither a child nor the cell was visible and the cell has been occluded for more than
// max_occluded_frames, it gets an occlusion group (if it has a parent and enough primitives) and its counter is reset.
// Leaf: ?CheckVisibilityResults@CEngineSceneGrid@@QAE_NW4EEngineOcclusionLayer@@KKKK_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B939E0, length 336 bytes, `ret 18h`.
// Callees: CEngineSceneGridCell::CheckVisibilityResults 0x00B936E0, CEngineSceneGridCell::CreateOcclusionGroup 0x00B92300, itself.
// Layout: engine/CEngineSceneGridCell.h (Parent +0x00, LocalCount +0x34, OccludedLayers +0x40, HasOcclusionResults bit 4 of
// +0x44, OccludedFrames +0x45); Ego_r.pdb CEngineSceneGrid (LevelDown +0x0C, Size +0x10, GridCells +0x14), no shared header.
// Shape: both cell lookups go through the inline GetCell(x, y) (FableWin.pdb ?GetCell@CEngineSceneGrid@@QAEPAVCEngineSceneGridCell@@KK@Z):
// with it LevelDown is loaded once per child (kept in ecx for the recursive call), the layer takes ebx and `this` and the cell
// live in the frame during the 2x2 walk; indexing GridCells in place keeps `this` in edi and reloads the layer (316v336).
// There is one `return visible`: the cell's own positive answer sets the flag in the arm (`mov byte [slot],1 ; mov al,[slot]`,
// the return sequence duplicated), the flag itself is homed in the dead `layer` argument word. The layer is tested against
// OccludedLayers as the raw enumerator, as retail does (`test ebx,[cell+40h]`).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DATA
#define FABLE_RETAIL_CENGINESCENEGRIDCELL_CHECKVISIBILITYRESULTS
#include "engine/CEngineSceneGridCell.h"
#include "engine/CArray.h"
#include "engine/CBaseClassNonCopyable.h"
class CEngineSceneGrid : public CBaseClassNonCopyable {    // no shared header: Ego_r.pdb members (retail 0x20 bytes)
public:
    bool CheckVisibilityResults(EEngineOcclusionLayer layer, unsigned long x, unsigned long y, unsigned long max_occluded_frames, unsigned long min_group_size, bool force);   // 0x00B939E0
    CEngineSceneGridCell* GetCell(unsigned long x, unsigned long y) { return GridCells[Size * y + x]; }   // FableWin.pdb ?GetCell@CEngineSceneGrid@@QAEPAVCEngineSceneGridCell@@KK@Z (inline)
private:
    unsigned long Level;                                   // +0x04
    CEngineSceneGrid* LevelUp;                             // +0x08
    CEngineSceneGrid* LevelDown;                           // +0x0C
    unsigned long Size;                                    // +0x10
    CArray<CEngineSceneGridCell*> GridCells;               // +0x14
};
bool CEngineSceneGrid::CheckVisibilityResults(EEngineOcclusionLayer layer, unsigned long x, unsigned long y, unsigned long max_occluded_frames, unsigned long min_group_size, bool force)
{
    CEngineSceneGridCell* cell = GetCell(x, y);
    if (!cell)
        return false;
    if (layer & cell->OccludedLayers)
        return false;
    if (!cell->HasOcclusionResults)
        return false;
    cell->HasOcclusionResults = false;
    bool visible = false;
    if (LevelDown) {
        for (unsigned long j = 0; j < 2; ++j) {
            for (unsigned long i = 0; i < 2; ++i) {
                CEngineSceneGridCell* child = LevelDown->GetCell(x * 2 + i, y * 2 + j);
                if (child && !(child->OccludedLayers & layer)) {
                    if (LevelDown->CheckVisibilityResults(layer, x * 2 + i, y * 2 + j, max_occluded_frames, min_group_size, force))
                        visible = true;
                }
            }
        }
    }
    if (cell->CheckVisibilityResults(layer, max_occluded_frames, min_group_size, force)) {
        visible = true;
    } else if (!visible && cell->OccludedFrames > max_occluded_frames) {
        if (cell->Parent && (unsigned long)cell->LocalCount >= min_group_size)
            cell->CreateOcclusionGroup(layer, this, x, y);
        cell->OccludedFrames = 0;
    }
    return visible;
}