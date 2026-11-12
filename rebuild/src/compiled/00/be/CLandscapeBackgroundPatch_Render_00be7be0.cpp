// CLandscapeBackgroundPatch::Render(bool withOcclusion, long& queryHandle, bool& occluded) -- draw the background landscape patch (through its occlusion query, or directly) and queue its water sub-patch on the water renderer's list for its water type; a water-only patch draws nothing and queues its sub-patch as midground sea
// Leaf: ?Render@CLandscapeBackgroundPatch@@QAEX_NAAJAA_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE7BE0, 391 bytes, `__fastcall`, `ret 0Ch`. Name: FableWin.pdb and Ego_r.pdb
//   ?Render@CLandscapeBackgroundPatch@@QAEX_NAAJAA_N@Z (the manifest label `_global::Render` has the wrong module). Callers:
//   CLandscapeBackgroundTreeNode::Render 0x00BF71D0 and CEngineLandscapeRenderer::RenderSortedPatches 0x00B68DA0.
// Callees: CLandscapeBackgroundPatch::RenderWithOcclusion(long&, bool&) 0x00BE79D0, CLandscapeBackgroundPatch::Render(bool)
//   0x00BE6F70 (true = the patch was drawn alpha-blended), CArray<CEngineWaterBackgroundSubPatch*>::push_back 0x00BE8FF0 (the four
//   out-of-line calls) and std::vector<CEngineWaterBackgroundSubPatch*>::_M_insert_overflow 0x00BE8F10 (the two expanded
//   push_backs); the water renderer global 0x01436E54.
// Layout: engine/CLandscapeBackgroundPatch.h (WaterSubPatch +0xF4, WaterOnlyPatch +0x11A), engine/CEngineWaterBackgroundSubPatch.h
//   (PolyCount +0x30, WaterType +0x40), engine/CEngineWaterRenderer.h (the six CArray render lists +0x208 .. +0x268).
// Shape: everything after the two draw calls is inline code from the headers, named in FableWin.pdb: HasWater() (the null test),
//   CEngineWaterRenderer::AddBackgroundWaterPatchToRenderList(patch, alphaed) and, nested in it for sea water and called directly
//   for a water-only patch, AddMidgroundSeaPatchToRenderList(patch, alphaed). Both helpers take the pointer by value and hand its
//   address to push_back(const T&), which is why retail stores the pointer to the dead first-argument slot once per expansion
//   (0x00BE7C2D, 0x00BE7C55; 0x00BE7D1F), tests PolyCount once (the nested helper's own IsEmpty() test folds away) and cross-jumps
//   the sea helper's two push_back expansions (0x00BE7C95), keeping the element size 4 in esi. push_back is expanded in the sea
//   helper and called out of line in the background helper's four statements. The STLport overflow body is visible (not
//   declaration-only): with it the compiler puts both pointer copies in the one argument slot.
// Evidence for the helper bodies: retail keeps an unreferenced out-of-line copy of each next to this function's COMDATs --
//   0x00BE9030 (sea helper) and 0x00BE9080 (background helper, sea helper expanded in it), both `ret 8` -- with the same tests in
//   the same order. The helpers are FABLE_FORCEINLINE in the header opt-in (inline budget: plain `inline` leaves them as calls
//   here, 180 v 391 bytes).
// Flags: none
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH_RENDER
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_ADDTORENDERLIST
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "engine/CEngineWaterRenderer.h"
#include "engine/CLandscapeBackgroundPatch.h"

extern CEngineWaterRenderer* g_pEngineWaterRenderer;   // 0x01436E54

void CLandscapeBackgroundPatch::Render(bool withOcclusion, long& queryHandle, bool& occluded)
{
    if (!WaterOnlyPatch) {
        bool alphaed = false;
        if (withOcclusion)
            RenderWithOcclusion(queryHandle, occluded);
        else
            alphaed = Render(false);
        if (HasWater())
            g_pEngineWaterRenderer->AddBackgroundWaterPatchToRenderList(WaterSubPatch, alphaed);
    } else {
        if (HasWater())
            g_pEngineWaterRenderer->AddMidgroundSeaPatchToRenderList(WaterSubPatch, false);
    }
}