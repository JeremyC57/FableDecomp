// Grade: DIFFER(379v395) 64 diff lines (parked 282v395, 136 lines; retail_asm candidate): both cell coordinates go through the engine's inline-asm floor helper GFFloatToLongFloor (expanded in place, ebp frame via optimize("y", off)), and the area minimum is loaded as a C2DVector and the relative position kept as a frame C2DVector (retail loads Min.X/Min.Y onto the x87 stack first and pops them after the subtraction); left: dead-argument-slot assignment (retail homes the grid pos in radius' slot +0x10 and the first floor result in position's slot +0x0c, ours the reverse; second floor result at [ebp-4]), the level kept in al rather than bl, and a 4-byte smaller frame
// CEngineSceneMap::AddPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, float radius) -- place a primitive in the scene map's grid hierarchy: compute its level from the radius, its cell from the position normalised over the map area (clamped to the level's size), store the grid position, then add it to the cell of that level (created on demand)
// Leaf: AddPrimitive
// Retail-asm: 0x00B93BAA..0x00B93BC4 `fld [ebp-10h] ; fsub [ebp-0Ch] ; fistp dword ptr [ebp+0Ch] ; fild [ebp+0Ch] ; fadd [ebp-8] ; fstp [ebp-14h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-14h] ; jne ; inc dword ptr [ebp+0Ch]` -- the engine's inline-assembly floor helper GFFloatToLongFloor (standalone copy 0x0048D940) expanded twice; VC7.1 emits no bare dword fistp from C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B93B30, length 400 bytes, `ret 0Ch` (ebp frame).
// Callees: CEngineSceneMap::CalculateLevel(float) @ 0x00B91920, CEngineSceneGrid::GetCellCreate(ulong, ulong) @ 0x00B937A0,
//   CEngineInternalPrimitiveBase::AddToList @ 0x00B8FDF0, CEngineSceneGridCell::InvalidateAll @ 0x00B91710.
// Layout: Ego_r.pdb CEngineSceneMap (Area C2DBoxF +0x04, GridLevels +0x18: 8-byte counted grid pointers); primitive GridCell
//   +0x20, GridPos {Level, X, Y} +0x24; cell LocalCount +0x34, validity bits of +0x44.
#pragma optimize("y", off)
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; };
// the engine's inline-assembly FPU floor helper (standalone copy 0x0048D940, ?GFFloatToLongFloor@@YIJM@Z): expanded in place
__forceinline long GFFloatToLongFloor(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
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
    C2DVector minimum;
    minimum.X = MinX;
    minimum.Y = MinY;
    C2DVector rel;
    rel.X = position.X - minimum.X;
    rel.Y = position.Y - minimum.Y;
    rel.X /= MaxX - MinX;
    rel.Y /= MaxY - MinY;
    CEngineSceneGridPos pos;
    pos.Level = CalculateLevel(radius);
    unsigned long size = 1 << pos.Level;
    pos.X = (unsigned char)GFFloatToLongFloor(rel.X * (float)size);
    pos.Y = (unsigned char)GFFloatToLongFloor(rel.Y * (float)size);
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
