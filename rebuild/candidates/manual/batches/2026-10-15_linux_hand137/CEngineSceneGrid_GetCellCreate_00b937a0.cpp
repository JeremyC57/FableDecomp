// CEngineSceneGrid::GetCellCreate(unsigned long, unsigned long) -- return the grid cell at (x, y), creating it on demand: a new cell is parented to the (recursively created) cell (x/2, y/2) of the coarser LevelUp grid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B937A0, length 105 bytes.
// Callees: itself @ 0x00B937A0 (on LevelUp), operator new @ 0x00BFEA1A (0x48 bytes), CEngineSceneGridCell::CEngineSceneGridCell(CEngineSceneGridCell*)
// @ 0x00B935C0.
// Layout: the PDB CEngineSceneGrid (LevelUp +0x08, Size +0x10, GridCells +0x14) and the 0x48-byte CEngineSceneGridCell; no engine
// header exists for these classes.
// Shape: x and y stay in ebp/ebx; the parent local is zeroed (edi) before the LevelUp test; `new` keeps its null test
// (`xor eax,eax` arm); the slot address is recomputed from Size and GridCells for the store.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
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
    CEngineSceneGridCell(CEngineSceneGridCell* parent);   // 0x00B935C0
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
    CEngineSceneGridCell* GetCellCreate(unsigned long x, unsigned long y);
};

CEngineSceneGridCell* CEngineSceneGrid::GetCellCreate(unsigned long x, unsigned long y)
{
    CEngineSceneGridCell* cell = GridCells.m_pData[y * Size + x];
    if (!cell) {
        CEngineSceneGridCell* parent = 0;
        if (LevelUp)
            parent = LevelUp->GetCellCreate(x >> 1, y >> 1);
        cell = new CEngineSceneGridCell(parent);
        GridCells.m_pData[y * Size + x] = cell;
    }
    return cell;
}
