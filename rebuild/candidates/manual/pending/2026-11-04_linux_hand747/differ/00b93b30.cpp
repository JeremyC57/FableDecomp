// CEngineSceneMap::AddPrimitive(CEngineInternalPrimitiveBase& primitive, const C2DVector& position, float radius) -- place a primitive in the scene map: its level from the radius, its cell from the position normalised over the map area (floored, clamped to the level's size), store the grid position, then add it to the cell of the deepest existing level (created on demand)
// Grade: DIFFER(377v395) -- try b dl=72: C2DVector rel = position - TL (shared C2DVector/C2DBoxF retail opt-ins) fixed the area normalisation; residue is frame-slot packing -- retail keeps pos in the dead radius slot, the first floor result in the position slot and builds the shifted cell pos as a 3-byte temporary at [ebp+0Dh] whose first word is copied back (best try b; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?AddPrimitive@CEngineSceneMap@@AAEXAAVCEngineInternalPrimitiveBase@@ABVC2DVector@@M@Z
// Flags: none
// Retail-asm: 0x00B93BB0 + 0x00B93BEA `fistp dword ptr` -- engine/GFFloatToLongFloor.h (GFFloatToLongFloor: 0.5f / 1.0f frame
//   constants, `fsub ; fistp ; fild ; fadd ; fstp ; cmp ; jne ; inc`, ebp frame). VC7.1 floor()/`(long)x` call the CRT and
//   __ftol2, `/QIfist` stores `fistp qword`, `_mm_cvtss_si32` emits cvtss2si (NOTES.md attempts table).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B93B30, length 400 bytes, `ret 0Ch` (ebp frame).
// Name: Ego_r.pdb ?AddPrimitive@CEngineSceneMap@@AAEXAAVCEngineInternalPrimitiveBase@@ABVC2DVector@@M@Z (private).
// Callees: CEngineSceneMap::CalculateLevel(float) const 0x00B91920 (Ego_r ABEKM), CEngineSceneGrid::GetCellCreate(ulong, ulong)
//   0x00B937A0, CEngineInternalPrimitiveBase::AddToList 0x00B8FDF0, CEngineSceneGridCell::InvalidateAll 0x00B91710; inlined
//   CEngineSceneGridCell::AddPrimitive (the landed 0x00B91870 body).
// Layout: Ego_r.pdb CEngineSceneMap (Area C2DBoxF +0x04, MaxLevels +0x14, GridLevels +0x18: CArray of 8-byte counted grid pointers).
#include "engine/GFFloatToLongFloor.h"
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#define FABLE_RETAIL_C2DBOXF
#include "engine/C2DBoxF.h"
class CEngineSceneGridCell;
class CEngineSceneGridPos { public:
    signed char   Level;
    unsigned char X;
    unsigned char Y;
};
class CEngineInternalPrimitiveBase { public:
    unsigned char         _pad00[0x20];
    CEngineSceneGridCell* GridCell;                 // +0x20
    CEngineSceneGridPos   GridPos;                  // +0x24
    void AddToList(CEngineInternalPrimitiveBase** list);   // 0x00B8FDF0
};
class CEngineSceneGridCell { public:
    CEngineSceneGridCell*         Parent;           // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;   // +0x04
    unsigned char                 _pad08[0x2c];
    long                          LocalCount;       // +0x34
    unsigned char                 _pad38[0xc];
    unsigned char                 Flags;            // +0x44
    void InvalidateAll();                           // 0x00B91710
    void AddPrimitive(CEngineInternalPrimitiveBase& primitive)
    {
        primitive.GridCell = this;
        primitive.AddToList(&FirstPrimitive);
        ++LocalCount;
        if (Flags & 7) {
            Flags &= ~7;
            if (Parent)
                Parent->InvalidateAll();
        }
    }
};
class CEngineSceneGrid { public:
    CEngineSceneGridCell* GetCellCreate(unsigned long x, unsigned long y);   // 0x00B937A0
};
template <class T> class CCountedPointer { public: T* Data; void* Info; T* operator->() const { return Data; } };
class CEngineScene;
class CEngineSceneMap { public:
    virtual ~CEngineSceneMap();
    C2DBoxF       Area;                             // +0x04
    unsigned long MaxLevels;                        // +0x14
    CCountedPointer<CEngineSceneGrid>* GridLevelsFirst;   // +0x18 CArray<CCountedPointer<CEngineSceneGrid> >
    CCountedPointer<CEngineSceneGrid>* GridLevelsLast;    // +0x1c
    CCountedPointer<CEngineSceneGrid>* GridLevelsEnd;     // +0x20
    CEngineScene* Scene;                            // +0x24
private:
    unsigned long CalculateLevel(float radius) const;     // 0x00B91920
    void AddPrimitive(CEngineInternalPrimitiveBase& primitive, const C2DVector& position, float radius);
};

void CEngineSceneMap::AddPrimitive(CEngineInternalPrimitiveBase& primitive, const C2DVector& position, float radius)
{
    C2DVector rel = position - C2DVector(Area.GetTLX(), Area.GetTLY());
    float u = rel.X / Area.GetWidth();
    float v = rel.Y / Area.GetHeight();
    CEngineSceneGridPos pos;
    pos.Level = (signed char)CalculateLevel(radius);
    unsigned long size = 1 << pos.Level;
    float fsize = (float)size;
    pos.X = (unsigned char)GFFloatToLongFloor(u * fsize);
    pos.Y = (unsigned char)GFFloatToLongFloor(fsize * v);
    if (pos.X > size - 1)
        pos.X = (unsigned char)(size - 1);
    if (pos.Y > size - 1)
        pos.Y = (unsigned char)(size - 1);
    primitive.GridPos = pos;
    unsigned long level = (unsigned long)pos.Level < (unsigned long)(GridLevelsLast - GridLevelsFirst) - 1
                              ? pos.Level : (GridLevelsLast - GridLevelsFirst) - 1;
    unsigned char shift = (unsigned char)(pos.Level - level);
    pos.Level = (signed char)level;
    pos.X >>= shift;
    pos.Y >>= shift;
    GridLevelsFirst[level]->GetCellCreate(pos.X, pos.Y)->AddPrimitive(primitive);
}
