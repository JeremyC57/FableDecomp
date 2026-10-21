// Grade: DIFFER(382v381) -- retail keeps Level+1 as a full-width inc and subtracts in cl (ours folds it to mov dl/dec), packs the two 3-byte GridPos copies at frame +2/+5 (ours +0/+4) and holds this in edi (ours ebx)
// CEngineSceneGrid::MovePrimitivesDown() -- walk every cell of this grid level and move each primitive whose grid position lies on a deeper level into the corresponding cell of the level below (creating that cell, parented to its cell on this level, when needed)
// Leaf: MovePrimitivesDown
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B93860, length 384 bytes (with int3 padding; body 0x17d), plain `ret`.
// PDB: ?MovePrimitivesDown@CEngineSceneGrid@@AAEXXZ.
// Callees: CEngineInternalPrimitiveBase::RemoveFromList() @ 0x00B8FE60, CEngineInternalPrimitiveBase::AddToList
//   (CEngineInternalPrimitiveBase**) @ 0x00B8FDF0, CEngineSceneGrid::GetCellCreate(ulong, ulong) @ 0x00B937A0, operator new
//   @ 0x00BFEA1A, CEngineSceneGridCell::CEngineSceneGridCell(CEngineSceneGridCell* parent) @ 0x00B935C0 (0x48 bytes),
//   CEngineSceneGridCell::InvalidateAll() @ 0x00B91710 (manifest names, PDB-confirmed).
// Layout: Ego_r.pdb CEngineSceneGrid (Level +0x04, LevelUp +0x08, LevelDown +0x0c, Size +0x10, GridCells +0x14),
//   CEngineSceneGridCell (Parent +0x00, FirstPrimitive +0x04, LocalCount +0x34, validity bits 0-2 of +0x44),
//   CEngineInternalPrimitiveBase (GridCell +0x20, GridPos {signed Level, X, Y} +0x24, NextPrimitive +0x38).
// Shape: the private inline CEngineSceneGrid::AddPrimitive(primitive) of the level below (GridPos copied again, coordinates
//   shifted by the level difference, inline GetCellCreate with the out-of-line parent lookup on LevelUp) and the inline
//   CEngineSceneGridCell::AddPrimitive (link, count, invalidate); the next pointer is read before the primitive moves.
class CEngineInternalPrimitiveBase;
struct CEngineSceneGridCell;

struct CEngineSceneGridPos {
    signed char   Level;
    unsigned char X;
    unsigned char Y;
};

class CEngineInternalPrimitiveBase {
public:
    unsigned char                 _pad00[0x20];
    CEngineSceneGridCell*         GridCell;         // +0x20
    CEngineSceneGridPos           GridPos;          // +0x24
    unsigned char                 _pad27[0x38 - 0x27];
    CEngineInternalPrimitiveBase* NextPrimitive;    // +0x38
    void AddToList(CEngineInternalPrimitiveBase** list);   // 0x00B8FDF0
    void RemoveFromList();                          // 0x00B8FE60
};

struct CEngineSceneGridCell {
    CEngineSceneGridCell*         Parent;           // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;   // +0x04
    unsigned char                 _pad08[0x2c];
    long                          LocalCount;       // +0x34
    unsigned char                 _pad38[0xc];
    unsigned char                 BoundingBoxValid : 1;   // +0x44
    unsigned char                 TotalCountValid : 1;
    unsigned char                 RenderMaskValid : 1;
    unsigned char                 WasRendered : 1;
    unsigned char                 HasOcclusionResults : 1;
    unsigned char                 _pad45[3];
    CEngineSceneGridCell(CEngineSceneGridCell* parent);   // 0x00B935C0
    void InvalidateAll();                           // 0x00B91710
    __forceinline void AddPrimitive(CEngineInternalPrimitiveBase& primitive)
    {
        primitive.AddToList(&FirstPrimitive);
        ++LocalCount;
        if (BoundingBoxValid || TotalCountValid || RenderMaskValid) {
            BoundingBoxValid = 0;
            TotalCountValid = 0;
            RenderMaskValid = 0;
            if (Parent)
                Parent->InvalidateAll();
        }
    }
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
    CEngineSceneGridCell* GetCellCreate(unsigned long x, unsigned long y);   // 0x00B937A0
    __forceinline void AddPrimitive(CEngineInternalPrimitiveBase& primitive, unsigned long level)
    {
        CEngineSceneGridPos pos = primitive.GridPos;
        unsigned char shift = pos.Level - level;
        unsigned char y = pos.Y >> shift;
        unsigned char x = pos.X >> shift;
        CEngineSceneGridCell* cell = GridCells[Size * y + x];
        if (!cell) {
            CEngineSceneGridCell* parent = 0;
            if (LevelUp)
                parent = LevelUp->GetCellCreate(x >> 1, y >> 1);
            cell = new CEngineSceneGridCell(parent);
            GridCells[Size * y + x] = cell;
        }
        primitive.GridCell = cell;
        cell->AddPrimitive(primitive);
    }
    void MovePrimitivesDown();
};

void CEngineSceneGrid::MovePrimitivesDown()
{
    for (unsigned long y = 0; y < Size; ++y) {
        for (unsigned long x = 0; x < Size; ++x) {
            CEngineSceneGridCell* cell = GridCells[Size * y + x];
            if (!cell)
                continue;
            CEngineInternalPrimitiveBase* primitive = cell->FirstPrimitive;
            while (primitive) {
                CEngineInternalPrimitiveBase* next = primitive->NextPrimitive;
                CEngineSceneGridPos pos = primitive->GridPos;
                if ((unsigned long)pos.Level > Level) {
                    primitive->RemoveFromList();
                    LevelDown->AddPrimitive(*primitive, Level + 1);
                }
                primitive = next;
            }
        }
    }
}
