// Grade: DIFFER(282v395) -- lane gap: asm_bake family: retail floors both cell coordinates with the inline-asm fistp/fild helper (ebp frame, 0.5/1.0 frame constants); compiled code cannot produce it
// CEngineSceneMap::AddPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, float radius) -- place a primitive in the scene map's grid hierarchy: compute its level from the radius, its cell from the position normalised over the map area (clamped to the level's size), store the grid position, then add it to the cell of that level (created on demand)
// Leaf: AddPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B93B30, length 400 bytes, `ret 0Ch` (ebp frame).
// Callees: CEngineSceneMap::CalculateLevel(float) @ 0x00B91920, CEngineSceneGrid::GetCellCreate(ulong, ulong) @ 0x00B937A0,
//   CEngineInternalPrimitiveBase::AddToList @ 0x00B8FDF0, CEngineSceneGridCell::InvalidateAll @ 0x00B91710.
// Layout: Ego_r.pdb CEngineSceneMap (Area C2DBoxF +0x04, GridLevels +0x18: 8-byte counted grid pointers); primitive GridCell
//   +0x20, GridPos {Level, X, Y} +0x24; cell LocalCount +0x34, validity bits of +0x44.
// Note: retail truncates the two cell coordinates with the engine's inline-asm floor helper (fistp / fild / compare with
//   frame constants 0.5 and 1.0 in an ebp frame: asm_bake family); the compiled version uses a cast.
struct C3DVector { float X, Y, Z; };
class CEngineInternalPrimitiveBase;
struct CEngineSceneGridCell {
    CEngineSceneGridCell*         Parent;
    CEngineInternalPrimitiveBase* FirstPrimitive;
    unsigned char                 _pad08[0x2c];
    long                          LocalCount;       // +0x34
    unsigned char                 _pad38[0xc];
    unsigned char                 Flags;            // +0x44
    void InvalidateAll();                           // 0x00B91710
};
struct CEngineSceneGridPos { signed char Level; unsigned char X; unsigned char Y; };
class CEngineInternalPrimitiveBase {
public:
    unsigned char         _pad00[0x20];
    CEngineSceneGridCell* GridCell;                 // +0x20
    CEngineSceneGridPos   GridPos;                  // +0x24
    void AddToList(CEngineInternalPrimitiveBase** list);   // 0x00B8FDF0
};
struct CEngineSceneGrid {
    CEngineSceneGridCell* GetCellCreate(unsigned long x, unsigned long y);   // 0x00B937A0
};
struct CGridPointer { CEngineSceneGrid* Object; long* Count; };
class CEngineSceneMap {
public:
    void*         _vptr;
    float         MinX, MinY, MaxX, MaxY;           // +0x04 Area
    unsigned long MaxLevels;                        // +0x14
    CGridPointer* GridLevelsFirst;                  // +0x18
    CGridPointer* GridLevelsLast;                   // +0x1c
    signed char CalculateLevel(float radius);       // 0x00B91920
    void AddPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, float radius);
};
void CEngineSceneMap::AddPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, float radius)
{
    float u = (position.X - MinX) / (MaxX - MinX);
    float v = (position.Y - MinY) / (MaxY - MinY);
    CEngineSceneGridPos pos;
    pos.Level = CalculateLevel(radius);
    unsigned long size = 1 << pos.Level;
    pos.X = (unsigned char)(long)(u * (float)size);
    pos.Y = (unsigned char)(long)(v * (float)size);
    if (pos.X > size - 1)
        pos.X = (unsigned char)(size - 1);
    if (pos.Y > size - 1)
        pos.Y = (unsigned char)(size - 1);
    primitive.GridPos = pos;
    unsigned long levels = GridLevelsLast - GridLevelsFirst - 1;
    unsigned long level = (unsigned long)pos.Level < levels ? pos.Level : levels;
    unsigned char shift = pos.Level - (unsigned char)level;
    CEngineSceneGridCell* cell = GridLevelsFirst[level].Object->GetCellCreate(pos.X >> shift, pos.Y >> shift);
    primitive.GridCell = cell;
    primitive.AddToList(&cell->FirstPrimitive);
    ++cell->LocalCount;
    if (cell->Flags & 7) {
        cell->Flags &= ~7;
        if (cell->Parent)
            cell->Parent->InvalidateAll();
    }
}
