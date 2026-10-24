// CEngineSceneGrid::MovePrimitivesUp() -- before a grid level is dropped, move the primitives of every one of its cells into the cell's parent on the coarser level
// Leaf: MovePrimitivesUp
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B91F00, 67 bytes, thiscall, `ret`.
// Identity: Ego_r.pdb ?MovePrimitivesUp@CEngineSceneGrid@@AAEXXZ (the private grid pair MovePrimitivesUp/Down; this one
//   hands each cell to its Parent). One caller 0x00B94785 in CEngineSceneMap::ResizeLevels, on the last grid level just
//   before the level above clears its LevelDown and the level is popped. Catalog
//   "CEngineScene_IterateGrid_MovePrimitives" is descriptive.
// Callees: CEngineSceneGridCell::MovePrimitivesTo(CEngineSceneGridCell&) 0x00B918B0 (landed).
// Layout: the PDB CEngineSceneGrid / CEngineSceneGridCell as in the landed GetCellTotalPrimitiveCount 0x00B91D20 (Size
//   +0x10, GridCells CArray data +0x14; the cell's Parent at +0x00). No engine header exists for these classes.
// Shape: two unsigned loops over Size x Size (Size re-read at every test), cell = GridCells[y * Size + x], null-tested.
struct CEngineInternalPrimitiveBase;

struct CEngineSceneGridCell {
    CEngineSceneGridCell*         Parent;          // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;  // +0x04
    void MovePrimitivesTo(CEngineSceneGridCell& target);   // 0x00B918B0
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
    void MovePrimitivesUp();
};

void CEngineSceneGrid::MovePrimitivesUp()
{
    for (unsigned long y = 0; y < Size; ++y)
        for (unsigned long x = 0; x < Size; ++x) {
            CEngineSceneGridCell* cell = GridCells.m_pData[y * Size + x];
            if (cell)
                cell->MovePrimitivesTo(*cell->Parent);
        }
}