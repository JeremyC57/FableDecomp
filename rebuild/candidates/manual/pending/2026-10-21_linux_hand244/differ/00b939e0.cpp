// Grade: DIFFER(316v336) -- register allocation: retail pins the layer in ebx for the whole function, homes the result bool in the dead layer slot and spills this/cell to the frame (edi = child row, ebp = x*2); ours keeps this in edi, the result in a frame byte and reloads the layer
// CEngineSceneGrid::CheckVisibilityResults(EEngineOcclusionLayer layer, ulong x, ulong y, ulong maxOccludedFrames, ulong minGroupSize, bool flag) -- collect the occlusion-query results of one grid cell: recurse into the four child cells of the level below that carry the layer, then ask the cell itself; when nothing below was visible and the cell stayed occluded too long, rebuild its occlusion group
// Leaf: CheckVisibilityResults@CEngineSceneGrid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B939E0, length 336 bytes, `ret 18h`.
// Callees: CEngineSceneGridCell::CheckVisibilityResults(EEngineOcclusionLayer, ulong, ulong, bool) @ 0x00B936E0,
//   CEngineSceneGridCell::CreateOcclusionGroup(EEngineOcclusionLayer, CEngineSceneGrid*, ulong, ulong) @ 0x00B92300,
//   itself (recursion on LevelDown). Signatures from the manifest (PDB-derived parameter lists).
// Layout: Ego_r.pdb CEngineSceneGrid (LevelDown +0x0c, Size +0x10, GridCells +0x14 begin) and CEngineSceneGridCell (Parent
//   +0x00, LocalCount +0x34, OccludedLayers +0x40, flag bits at +0x44 with HasOcclusionResults = bit 4, OccludedFrames
//   +0x45); no generated engine headers exist for these classes, so they are modelled locally.
// Shape: the result bool lives in the dead `layer` argument slot once the layer is in ebx; the early `return true` after
//   the cell's own check goes through that slot (`result = true; return result;`); the 2x2 child walk keeps x*2 in ebp.
enum EEngineOcclusionLayer { OCCLUSION_LAYER_NONE = 0 };

struct CEngineSceneGrid;
struct CEngineSceneGridCell {
    CEngineSceneGridCell* Parent;                // +0x00
    unsigned char         _pad04[0x30];          // FirstPrimitive, BoundingBox, DrawDistance, GroupPrimitiveHandle
    long                  LocalCount;            // +0x34
    unsigned long         TotalCount;            // +0x38
    unsigned long         RenderMask;            // +0x3c
    unsigned long         OccludedLayers;        // +0x40
    unsigned char         BoundingBoxValid : 1;  // +0x44
    unsigned char         TotalCountValid : 1;
    unsigned char         RenderMaskValid : 1;
    unsigned char         WasRendered : 1;
    unsigned char         HasOcclusionResults : 1;
    unsigned char         OccludedFrames;        // +0x45
    unsigned char         PreloadFrames;         // +0x46
    bool CheckVisibilityResults(EEngineOcclusionLayer layer, unsigned long maxOccludedFrames, unsigned long minGroupSize, bool flag);   // 0x00B936E0
    void CreateOcclusionGroup(EEngineOcclusionLayer layer, CEngineSceneGrid* grid, unsigned long x, unsigned long y);   // 0x00B92300
};

template <class T> struct CArray {
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};

struct CEngineSceneGrid {
    void*                         _vptr;         // +0x00
    unsigned long                 Level;         // +0x04
    CEngineSceneGrid*             LevelUp;       // +0x08
    CEngineSceneGrid*             LevelDown;     // +0x0c
    unsigned long                 Size;          // +0x10
    CArray<CEngineSceneGridCell*> GridCells;     // +0x14
    bool CheckVisibilityResults(EEngineOcclusionLayer layer, unsigned long x, unsigned long y, unsigned long maxOccludedFrames, unsigned long minGroupSize, bool flag);
};

bool CEngineSceneGrid::CheckVisibilityResults(EEngineOcclusionLayer layer, unsigned long x, unsigned long y, unsigned long maxOccludedFrames, unsigned long minGroupSize, bool flag)
{
    CEngineSceneGridCell* cell = GridCells[Size * y + x];
    if (!cell)
        return false;
    if (layer & cell->OccludedLayers)
        return false;
    if (!cell->HasOcclusionResults)
        return false;
    cell->HasOcclusionResults = 0;
    bool result = false;
    if (LevelDown) {
        for (unsigned long j = 0; j < 2; ++j) {
            for (unsigned long i = 0; i < 2; ++i) {
                CEngineSceneGridCell* child = LevelDown->GridCells[LevelDown->Size * (y * 2 + j) + x * 2 + i];
                if (child && !(child->OccludedLayers & layer)) {
                    if (LevelDown->CheckVisibilityResults(layer, x * 2 + i, y * 2 + j, maxOccludedFrames, minGroupSize, flag))
                        result = true;
                }
            }
        }
    }
    if (cell->CheckVisibilityResults(layer, maxOccludedFrames, minGroupSize, flag)) {
        result = true;
        return result;
    }
    if (!result && cell->OccludedFrames > maxOccludedFrames) {
        if (cell->Parent && (unsigned long)cell->LocalCount >= minGroupSize)
            cell->CreateOcclusionGroup(layer, this, x, y);
        cell->OccludedFrames = 0;
    }
    return result;
}
