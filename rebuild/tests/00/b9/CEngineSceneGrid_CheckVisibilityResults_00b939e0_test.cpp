// Behaviour test for CEngineSceneGrid::CheckVisibilityResults (0x00B939E0): child recursion over the 2x2 block of the level below,
// the cell's own check, and the occlusion-group creation for a cell that stayed occluded too long.
#include <cstdio>
#include <vector>
class CEngineSceneGrid;
class CEngineSceneGridCell { public:
    CEngineSceneGridCell* Parent; long LocalCount; unsigned long OccludedLayers; bool HasOcclusionResults; unsigned char OccludedFrames;
    bool Answer; int Checks; int Groups; unsigned long GroupX, GroupY;
    bool CheckVisibilityResults(long, unsigned long, unsigned long, bool) { ++Checks; return Answer; }
    void CreateOcclusionGroup(long, CEngineSceneGrid*, unsigned long x, unsigned long y) { ++Groups; GroupX = x; GroupY = y; }
};
class CEngineSceneGrid { public:
    CEngineSceneGrid* LevelDown; unsigned long Size; std::vector<CEngineSceneGridCell*> GridCells;
    CEngineSceneGridCell* GetCell(unsigned long x, unsigned long y) { return GridCells[Size * y + x]; }
    bool CheckVisibilityResults(long layer, unsigned long x, unsigned long y, unsigned long max_occluded_frames, unsigned long min_group_size, bool force)
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
};
static CEngineSceneGridCell MakeCell(CEngineSceneGridCell* parent, bool answer)
{
    CEngineSceneGridCell c; c.Parent = parent; c.LocalCount = 5; c.OccludedLayers = 0; c.HasOcclusionResults = true; c.OccludedFrames = 0;
    c.Answer = answer; c.Checks = 0; c.Groups = 0; c.GroupX = c.GroupY = 99;
    return c;
}
int main()
{
    CEngineSceneGridCell top = MakeCell(0, false);
    CEngineSceneGridCell kids[4];
    for (int i = 0; i < 4; ++i) kids[i] = MakeCell(&top, false);
    CEngineSceneGrid low; low.LevelDown = 0; low.Size = 4; low.GridCells.assign(16, (CEngineSceneGridCell*)0);
    low.GridCells[4 * 2 + 2] = &kids[0]; low.GridCells[4 * 2 + 3] = &kids[1]; low.GridCells[4 * 3 + 2] = &kids[2];   // children of (1,1); (3,3) empty
    CEngineSceneGrid high; high.LevelDown = &low; high.Size = 2; high.GridCells.assign(4, (CEngineSceneGridCell*)0);
    high.GridCells[2 * 1 + 1] = &top;
    bool ok = true;
    ok = ok && !high.CheckVisibilityResults(1, 0, 0, 3, 2, false);                       // empty cell
    kids[1].Answer = true; kids[2].OccludedLayers = 1;                                    // one visible child, one occluded on the layer
    ok = ok && high.CheckVisibilityResults(1, 1, 1, 3, 2, false);                         // a child was visible
    ok = ok && kids[0].Checks == 1 && kids[1].Checks == 1 && kids[2].Checks == 0 && top.Checks == 1 && !top.HasOcclusionResults;
    ok = ok && !high.CheckVisibilityResults(1, 1, 1, 3, 2, false) && top.Checks == 1;     // results already consumed
    top.HasOcclusionResults = true;                                                       // children have no results now
    top.OccludedFrames = 4;
    ok = ok && !high.CheckVisibilityResults(1, 1, 1, 3, 2, false) && top.Checks == 2;     // nothing visible, occluded too long
    ok = ok && top.Groups == 0 && top.OccludedFrames == 0;                                // no parent: no group, counter reset
    kids[0].HasOcclusionResults = true; kids[0].OccludedFrames = 9;
    ok = ok && !low.CheckVisibilityResults(1, 2, 2, 3, 6, false) && kids[0].Groups == 0 && kids[0].OccludedFrames == 0;   // too few primitives
    kids[0].HasOcclusionResults = true; kids[0].OccludedFrames = 9;
    ok = ok && !low.CheckVisibilityResults(1, 2, 2, 3, 5, false) && kids[0].Groups == 1 && kids[0].GroupX == 2 && kids[0].GroupY == 2;
    kids[0].HasOcclusionResults = true; kids[0].OccludedFrames = 9; kids[0].Answer = true;
    ok = ok && low.CheckVisibilityResults(1, 2, 2, 3, 5, false) && kids[0].Groups == 1 && kids[0].OccludedFrames == 9;    // visible itself
    top.HasOcclusionResults = true; top.OccludedLayers = 1;
    ok = ok && !high.CheckVisibilityResults(1, 1, 1, 3, 2, false) && top.HasOcclusionResults;                              // occluded on the layer
    if (!ok) { std::printf("00b939e0_TEST FAIL\n"); return 1; }
    std::printf("00b939e0_TEST PASS\n");
    return 0;
}