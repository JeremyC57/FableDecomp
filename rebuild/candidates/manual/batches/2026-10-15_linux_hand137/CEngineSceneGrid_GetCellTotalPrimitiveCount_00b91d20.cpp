// CEngineSceneGrid::GetCellTotalPrimitiveCount(unsigned long, unsigned long) -- number of primitives in a grid cell including every finer-level cell below it (the four children at (2x+j, 2y+i) on LevelDown, recursively), cached in the cell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B91D20, length 149 bytes (+ int3 padding).
// Callees: itself @ 0x00B91D20 (on LevelDown, re-read for every call).
// Layout: the PDB CEngineSceneGrid (Size +0x10, LevelDown +0x0c, GridCells CArray<CEngineSceneGridCell*> +0x14) and
// CEngineSceneGridCell (LocalCount +0x34, TotalCount +0x38, bit-field byte +0x44 with TotalCountValid = bit 1); no
// engine header exists for either class.
// Shape: a missing cell returns 0 straight from the lookup; a valid cache returns TotalCount through the shared tail; the
// cell and the outer index live in frame words (`sub esp,8`), the inner index in esi, the running total in ebx; the
// child row `y*2 + i` is formed once per outer iteration (edi), the column `x*2 + j` per call; the cache update stores
// TotalCount between the load and the store of the bit-field byte, and the result is re-read from the cell.
struct CEngineInternalPrimitiveBase;

struct CEngineSceneGridCell {                      // 0x48 bytes (PDB layout, matches retail)
    CEngineSceneGridCell*         Parent;          // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;  // +0x04
    float                         BoundingBox[6];  // +0x08 C3DBoundingBox
    float                         DrawDistance;    // +0x20
    unsigned char                 GroupPrimitiveHandle[0x10];   // +0x24 CEnginePrimitiveHandle[2]
    long                          LocalCount;      // +0x34
    unsigned long                 TotalCount;      // +0x38
    unsigned long                 RenderMask;      // +0x3c
    unsigned long                 OccludedLayers;  // +0x40
    unsigned char                 BoundingBoxValid : 1;     // +0x44 bit 0
    unsigned char                 TotalCountValid : 1;      //        bit 1
    unsigned char                 RenderMaskValid : 1;      //        bit 2
    unsigned char                 WasRendered : 1;          //        bit 3
    unsigned char                 HasOcclusionResults : 1;  //        bit 4
    unsigned char                 OccludedFrames;  // +0x45
    unsigned char                 PreloadFrames;   // +0x46
};

template <class T> struct CArray {                 // retail CArray: data pointer first
    T*   m_pData;
    long m_size;
    long m_capacity;
};

struct CEngineSceneGrid {                          // 0x24 bytes (PDB layout, matches retail)
    unsigned long                 _vfptr_or_pad;   // +0x00
    unsigned long                 Level;           // +0x04
    CEngineSceneGrid*             LevelUp;         // +0x08
    CEngineSceneGrid*             LevelDown;       // +0x0c
    unsigned long                 Size;            // +0x10
    CArray<CEngineSceneGridCell*> GridCells;       // +0x14
    unsigned long GetCellTotalPrimitiveCount(unsigned long x, unsigned long y);
};

unsigned long CEngineSceneGrid::GetCellTotalPrimitiveCount(unsigned long x, unsigned long y)
{
    CEngineSceneGridCell* cell = GridCells.m_pData[y * Size + x];
    if (!cell)
        return 0;
    if (!cell->TotalCountValid) {
        unsigned long total = cell->LocalCount;
        if (LevelDown) {
            for (unsigned long i = 0; i < 2; ++i)
                for (unsigned long j = 0; j < 2; ++j)
                    total += LevelDown->GetCellTotalPrimitiveCount(x * 2 + j, y * 2 + i);
        }
        cell->TotalCount = total;
        cell->TotalCountValid = 1;
    }
    return cell->TotalCount;
}
