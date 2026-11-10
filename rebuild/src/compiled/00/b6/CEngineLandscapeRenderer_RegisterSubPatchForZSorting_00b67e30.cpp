// CEngineLandscapeRenderer::RegisterSubPatchForZSorting(long& index, CLandscapeBackgroundPatch* patch) -- give a background sub-patch a slot in this frame's Z-sort list the first time (index = the slot; entry added this frame, not an occlusion box, no occlusion id), else mark its existing slot as added this frame
// Leaf: ?RegisterSubPatchForZSorting@CEngineLandscapeRenderer@@QAEXAAJPAVCLandscapeBackgroundPatch@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B67E30, 129 bytes, thiscall `ret 8`.
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_ZSORT
#include "engine/CEngineLandscapeRenderer.h"

void CEngineLandscapeRenderer::RegisterSubPatchForZSorting(long& index, CLandscapeBackgroundPatch* patch)
{
    if (index < 0) {
        index = SortedSubPatchList.size();
        CSubPatchSorterEntry entry;
        entry.Patch = patch;
        entry.AddedThisFrame = 1;
        entry.IsOcclusionBox = 0;
        entry.OcclusionID = -1;
        SortedSubPatchList.push_back(entry);
    } else {
        SortedSubPatchList[index].AddedThisFrame = 1;
    }
}