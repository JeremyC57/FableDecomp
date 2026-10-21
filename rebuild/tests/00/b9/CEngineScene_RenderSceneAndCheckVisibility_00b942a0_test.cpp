#include <cstdio>
// behaviour test: CEngineScene::RenderSceneAndCheckVisibility sets the occlusion bit, runs every grid's visibility render, then both lists
struct CEngineCamera {}; struct CEngineSceneRenderStats {}; struct CEngineInternalPrimitiveBase {};
static int g_cells = 0, g_lists = 0; static unsigned long g_lastMask = 0; static bool g_visibleWasFalse = true;
struct CEngineSceneGrid {
    bool RenderCellClippedAndCheckVisibility(unsigned long, unsigned long, long, unsigned long mask, const CEngineCamera&, bool clipped,
                                             CEngineSceneRenderStats&, long, bool, bool& visible)
    { ++g_cells; g_lastMask = mask; if (visible || clipped) g_visibleWasFalse = false; visible = true; return true; }
    void RenderCellAndCheck(long layer, unsigned long mask, const CEngineCamera& camera, CEngineSceneRenderStats& stats, long occlusion, bool flag)
    { bool visible = false; RenderCellClippedAndCheckVisibility(0, 0, layer, mask, camera, false, stats, occlusion, flag, visible); }
};
struct CEngineScene {
    CEngineSceneGrid* Grids[2]; int GridCount; unsigned long GridLayerMask;
    CEngineInternalPrimitiveBase* PrimitiveListA; CEngineInternalPrimitiveBase* PrimitiveListB;
    CEngineSceneRenderStats Stats; unsigned long CurrentOcclusionMask;
    void RenderPrimitiveList(CEngineInternalPrimitiveBase*, long, const CEngineCamera&, CEngineSceneRenderStats&) { ++g_lists; }
    void RenderSceneAndCheckVisibility(long layer, const CEngineCamera& camera, long occlusion, bool flag);
};
void CEngineScene::RenderSceneAndCheckVisibility(long layer, const CEngineCamera& camera, long occlusion, bool flag)
{
    CEngineSceneRenderStats& stats = Stats;
    CurrentOcclusionMask = 1 << (occlusion + 1);
    if (GridLayerMask & layer)
        for (int i = 0; i < GridCount; ++i)
            Grids[i]->RenderCellAndCheck(layer, CurrentOcclusionMask, camera, stats, occlusion, flag);
    RenderPrimitiveList(PrimitiveListA, layer, camera, stats);
    RenderPrimitiveList(PrimitiveListB, layer, camera, stats);
}
int main()
{
    CEngineSceneGrid g; CEngineCamera cam;
    CEngineScene s; s.Grids[0] = &g; s.Grids[1] = &g; s.GridCount = 2; s.GridLayerMask = 2;
    s.RenderSceneAndCheckVisibility(2, cam, 1, true);
    bool ok = s.CurrentOcclusionMask == 4 && g_cells == 2 && g_lastMask == 4 && g_visibleWasFalse && g_lists == 2;
    s.RenderSceneAndCheckVisibility(1, cam, 0, false);
    ok = ok && s.CurrentOcclusionMask == 2 && g_cells == 2 && g_lists == 4;
    if (!ok) { std::printf("00b942a0_TEST FAIL\n"); return 1; }
    std::printf("00b942a0_TEST PASS\n");
    return 0;
}