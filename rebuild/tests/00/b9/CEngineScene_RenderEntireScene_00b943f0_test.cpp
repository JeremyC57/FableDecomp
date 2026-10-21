#include <cstdio>
// behaviour test: CEngineScene::RenderEntireScene renders every map's top grid and the layer's moving primitives
struct CEngineSceneRenderStats { int dummy; };
static int g_cells = 0, g_rendered = 0; static unsigned short g_frame = 77;
struct CEngineSubPrimitiveChain { void* a; void* b; CEngineSubPrimitiveChain() : a(0), b(0) {} };
struct CEngineInternalPrimitiveBase {
    long Type; unsigned long RenderLayerMask; unsigned short LastFrameRendered;
    CEngineInternalPrimitiveBase* NextPrimitive; CEngineInternalPrimitiveBase* NextLayerMask;
};
struct CEnginePrimitiveManager {
    long result;
    long RenderPrimitive(CEngineInternalPrimitiveBase&, long, CEngineSubPrimitiveChain& c) { ++g_rendered; return (c.a == 0 && c.b == 0) ? result : -1; }
};
static CEnginePrimitiveManager g_managers[2];
struct CEngineSceneGrid { void RenderCellAll(unsigned long x, unsigned long y, long, CEngineSceneRenderStats&) { if (x == 0 && y == 0) ++g_cells; } };
struct CEngineScene {
    CEngineSceneGrid* Grids[3]; int GridCount;
    unsigned long GridLayerMask; CEngineInternalPrimitiveBase* PrimitiveListA; long CurrentCell;
    CEngineSceneRenderStats Stats;
    void RenderEntireScene(long layer);
};
void CEngineScene::RenderEntireScene(long layer)
{
    CEngineSceneRenderStats& stats = Stats;
    CurrentCell = -1;
    if (GridLayerMask & layer)
        for (int i = 0; i < GridCount; ++i) Grids[i]->RenderCellAll(0, 0, layer, stats);
    CEngineInternalPrimitiveBase* primitive = PrimitiveListA;
    while (primitive) {
        if (primitive->RenderLayerMask & layer) {
            CEnginePrimitiveManager* manager = &g_managers[primitive->Type];
            CEngineSubPrimitiveChain chain;
            if (manager->RenderPrimitive(*primitive, layer, chain) == 2) primitive->LastFrameRendered = g_frame;
            primitive = primitive->NextPrimitive;
        }
        else primitive = primitive->NextLayerMask;
    }
}
int main()
{
    g_managers[0].result = 2; g_managers[1].result = 0;
    CEngineSceneGrid g;
    CEngineInternalPrimitiveBase c = { 0, 4, 0, 0, 0 };
    CEngineInternalPrimitiveBase b = { 1, 4, 0, &c, 0 };
    CEngineInternalPrimitiveBase skip = { 0, 8, 0, 0, &b };
    CEngineScene s; s.Grids[0] = &g; s.Grids[1] = &g; s.GridCount = 2; s.GridLayerMask = 4; s.PrimitiveListA = &skip; s.CurrentCell = 5;
    s.RenderEntireScene(4);
    bool ok = s.CurrentCell == -1 && g_cells == 2 && g_rendered == 2 && c.LastFrameRendered == 77 && b.LastFrameRendered == 0 && skip.LastFrameRendered == 0;
    s.GridLayerMask = 8; g_cells = 0;
    s.RenderEntireScene(4);
    ok = ok && g_cells == 0;
    if (!ok) { std::printf("00b943f0_TEST FAIL\n"); return 1; }
    std::printf("00b943f0_TEST PASS\n");
    return 0;
}