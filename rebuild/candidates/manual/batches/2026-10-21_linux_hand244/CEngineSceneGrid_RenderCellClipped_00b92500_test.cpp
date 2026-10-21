#include <cstdio>
// behaviour test: CEngineSceneGrid::RenderCellClipped skips empty/unmasked/clipped cells, renders visible primitives, recurses, marks the cell
struct CEngineSceneRenderStats {};
struct C3DBoundingBox { float v[6]; };
static int g_rendered = 0, g_recursions = 0; static bool g_cameraVisible = true;
struct CEngineCamera {
    bool ClipWorldSpaceBoundingBox(const C3DBoundingBox&, float d, bool& clipped) const { clipped = true; return g_cameraVisible && d == 50.0f; }
};
struct CEngineInternalPrimitiveBase {
    unsigned long RenderLayerMask; bool visible; unsigned short LastFrameRendered;
    CEngineInternalPrimitiveBase* NextPrimitive; CEngineInternalPrimitiveBase* NextLayerMask;
    bool IsVisibleToCamera(const CEngineCamera&, bool clipped) { return visible && clipped; }
};
struct CEngineSceneGridCell { CEngineInternalPrimitiveBase* FirstPrimitive; bool WasRendered; };
struct CEngineSceneGrid {
    CEngineSceneGrid* LevelDown; unsigned long Size; CEngineSceneGridCell** GridCells; unsigned long Mask;
    unsigned long GetCellRenderMask(unsigned long, unsigned long) { return Mask; }
    bool GetCellBoundingBoxAndDrawDistance(C3DBoundingBox&, float& d, unsigned long, unsigned long) { d = 50.0f; return true; }
    void RenderCellClipped(unsigned long x, unsigned long y, long layer, const CEngineCamera& camera, bool clipped, CEngineSceneRenderStats& stats);
};
void CEngineSceneGrid::RenderCellClipped(unsigned long x, unsigned long y, long layer, const CEngineCamera& camera, bool clipped, CEngineSceneRenderStats& stats)
{
    ++g_recursions;
    CEngineSceneGridCell* cell = GridCells[Size * y + x];
    if (!cell) return;
    if (!(layer & GetCellRenderMask(x, y))) return;
    bool inside = clipped;
    if (!inside) {
        C3DBoundingBox box; float drawDistance;
        if (!GetCellBoundingBoxAndDrawDistance(box, drawDistance, x, y)) return;
        if (!camera.ClipWorldSpaceBoundingBox(box, drawDistance, inside)) return;
    }
    CEngineInternalPrimitiveBase* primitive = cell->FirstPrimitive;
    while (primitive) {
        if (primitive->RenderLayerMask & layer) {
            if (primitive->IsVisibleToCamera(camera, inside)) { ++g_rendered; primitive->LastFrameRendered = 9; }
            primitive = primitive->NextPrimitive;
        }
        else primitive = primitive->NextLayerMask;
    }
    if (LevelDown)
        for (unsigned long j = 0; j < 2; ++j)
            for (unsigned long i = 0; i < 2; ++i)
                LevelDown->RenderCellClipped(x * 2 + i, y * 2 + j, layer, camera, inside, stats);
    cell->WasRendered = true;
}
int main()
{
    CEngineSceneRenderStats stats; CEngineCamera cam;
    CEngineInternalPrimitiveBase p2 = { 1, true, 0, 0, 0 };
    CEngineInternalPrimitiveBase p1 = { 1, false, 0, &p2, 0 };
    CEngineSceneGridCell top = { &p1, false };
    CEngineSceneGridCell child = { 0, false };
    CEngineSceneGridCell* lowCells[4] = { &child, 0, 0, 0 };
    CEngineSceneGridCell* topCells[1] = { &top };
    CEngineSceneGrid low = { 0, 2, lowCells, 1 };
    CEngineSceneGrid grid = { &low, 1, topCells, 1 };
    grid.RenderCellClipped(0, 0, 1, cam, false, stats);
    bool ok = g_rendered == 1 && p2.LastFrameRendered == 9 && p1.LastFrameRendered == 0 && top.WasRendered && child.WasRendered && g_recursions == 5;
    top.WasRendered = false; g_cameraVisible = false; g_rendered = 0;
    grid.RenderCellClipped(0, 0, 1, cam, false, stats);
    ok = ok && !top.WasRendered && g_rendered == 0;
    grid.Mask = 2;
    grid.RenderCellClipped(0, 0, 1, cam, true, stats);
    ok = ok && !top.WasRendered;
    if (!ok) { std::printf("00b92500_TEST FAIL\n"); return 1; }
    std::printf("00b92500_TEST PASS\n");
    return 0;
}
