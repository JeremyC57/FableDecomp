// behaviour test for 00b92790 CEngineSceneGrid::RenderCellClippedOcclusion: empty or occluded cells and cells without the
// layer return false; a clipped-away cell returns its existing occlusion flag; rendering any primitive of the layer and
// occlusion layers sets HasOcclusionResults, children OR theirs in, and the cell is marked rendered
#include <cstdio>
struct Prim { unsigned int layers, occl; bool visible; int result; Prim* next; };
struct Cell { Prim* first; unsigned int occludedLayers, renderMask; bool boxVisible; bool hasResults, wasRendered; };
static bool Render(Cell* cell, unsigned int layer, unsigned int occl, bool clipped)
{
    if (!cell) return false;
    if (cell->occludedLayers & occl) return false;
    if (!(layer & cell->renderMask)) return false;
    if (!clipped && !cell->boxVisible) return cell->hasResults;
    int rendered = 0;
    for (Prim* p = cell->first; p; p = p->next)
        if ((p->layers & layer) && (p->occl & occl) && p->visible) rendered |= p->result;
    if (rendered) cell->hasResults = true;
    cell->wasRendered = true;
    return cell->hasResults;
}
int main()
{
    Prim p2 = { 1, 2, true, 2, 0 }, p1 = { 1, 1, true, 1, &p2 };
    Cell cell = { &p1, 0, 1, true, false, false };
    if (Render(0, 1, 2, false)) { std::printf("00b92790_TEST FAIL\n"); return 1; }
    if (!Render(&cell, 1, 2, false) || !cell.wasRendered) { std::printf("00b92790_TEST FAIL\n"); return 1; }
    Cell occluded = { &p1, 2, 1, true, true, false };
    if (Render(&occluded, 1, 2, false) || occluded.wasRendered) { std::printf("00b92790_TEST FAIL\n"); return 1; }
    Cell hidden = { &p1, 0, 1, false, true, false };
    if (!Render(&hidden, 1, 2, false) || hidden.wasRendered) { std::printf("00b92790_TEST FAIL\n"); return 1; }
    Cell none = { &p1, 0, 1, true, false, false };
    if (Render(&none, 1, 4, true) || !none.wasRendered) { std::printf("00b92790_TEST FAIL\n"); return 1; }
    std::printf("00b92790_TEST PASS\n");
    return 0;
}
