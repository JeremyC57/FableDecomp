// CEngineSceneGrid::InvalidateBoundingBoxAndDrawDistance(unsigned long x, unsigned long y, bool recurseDown) -- marks the cell's bounding box invalid and, when asked, recursively invalidates its four children in the next finer grid level
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B91CA0, length 120 bytes (+ int3 padding).
// Signature from Ego_r.pdb: ?InvalidateBoundingBoxAndDrawDistance@CEngineSceneGrid@@QAEXKK_N@Z.
// Callees: itself (0x00B91CA0) on LevelDown for each child cell.
// Layout: PDB CEngineSceneGrid with the retail 12-byte CArray: Level +4, LevelUp +8, LevelDown +0xc, Size +0x10,
// GridCells +0x14 (data pointer first). CEngineSceneGridCell (PDB): the bit-field byte at +0x44, BoundingBoxValid = bit 0.
// Shape: the cell is `GridCells[y * Size + x]`; the bit-field clear is `and byte ptr [eax+44h],0FEh`; the two nested
// unsigned loops (y offset outer, x offset inner) keep the inner counter in esi, the row `y*2 + j` in edi, and the
// outer counter in the dead `recurseDown` argument slot; the loop head is aligned (speed TU).

class CEngineSceneGridCell { public:
    unsigned char _pad00[0x44];
    unsigned char BoundingBoxValid : 1;                  // +0x44 bit 0
    unsigned char TotalCountValid : 1;
    unsigned char RenderMaskValid : 1;
    unsigned char WasRendered : 1;
    unsigned char HasOcclusionResults : 1;
    unsigned char OccludedFrames;                        // +0x45
    unsigned char PreloadFrames;                         // +0x46
};
struct CGridCellArray {                                  // retail CArray<CEngineSceneGridCell*> (12 bytes)
    CEngineSceneGridCell** m_pData;
    CEngineSceneGridCell** m_pLast;
    CEngineSceneGridCell** m_pEnd;
};

class CEngineSceneGrid {
public:
    void*             vptr;                              // +0x00
    unsigned long     Level;                             // +0x04
    CEngineSceneGrid* LevelUp;                           // +0x08
    CEngineSceneGrid* LevelDown;                         // +0x0c
    unsigned long     Size;                              // +0x10
    CGridCellArray    GridCells;                         // +0x14
    void InvalidateBoundingBoxAndDrawDistance(unsigned long x, unsigned long y, bool recurseDown);
};

void CEngineSceneGrid::InvalidateBoundingBoxAndDrawDistance(unsigned long x, unsigned long y, bool recurseDown)
{
    CEngineSceneGridCell* cell = GridCells.m_pData[y * Size + x];
    if (cell) {
        cell->BoundingBoxValid = false;
        if (recurseDown && LevelDown) {
            for (unsigned long j = 0; j < 2; ++j)
                for (unsigned long i = 0; i < 2; ++i)
                    LevelDown->InvalidateBoundingBoxAndDrawDistance(x * 2 + i, y * 2 + j, true);
        }
    }
}