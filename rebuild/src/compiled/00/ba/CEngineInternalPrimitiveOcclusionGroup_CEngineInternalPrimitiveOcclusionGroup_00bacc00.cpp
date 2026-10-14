// CEngineInternalPrimitiveOcclusionGroup::CEngineInternalPrimitiveOcclusionGroup(CEngineSceneGridCell* cell, CEngineSceneGrid* grid, unsigned long x, unsigned long y) -- an occlusion group primitive (base type 0x29, flags 8) over its grid cell, grid and cell coordinates, with no frames rendered and both occlusion infos empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACC00, length 63 bytes.
// Callees: CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long flags) @ 0x00B8FF00 (0x29, 8). Vtable 0x012A5364 =
// CEngineInternalPrimitiveOcclusionGroup (rtti-vtables.tsv). Shape: the inlined `__declspec(novtable)` CEngineInternalPrimitiveOcclusionBase constructor
// zeroes its two counted pointers (+0x48, +0x4c) from the zero register; the group's init list stores the cell, the grid, the zero frame count and the
// coordinates with the vtable store scheduled between them; `mov eax,esi` returns `this`. Members from engine/CEngineInternalPrimitiveOcclusionGroup.h (PDB):
// GridCell +0x50, Grid +0x54, GridX +0x58, GridY +0x5c, FramesRendered +0x60.
struct CEngineSceneGridCell;
struct CEngineSceneGrid;

struct CIVCountedPointeeBase {
    virtual void v00();
    unsigned long IVRefCount;
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
};

struct CEngineInternalPrimitiveBase {               // engine/CEngineInternalPrimitiveBase.h: 0x48 bytes
    virtual void v00();
    unsigned char _pad04[0x44];
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

struct __declspec(novtable) CEngineInternalPrimitiveOcclusionBase : CEngineInternalPrimitiveBase {
    CIVCountedPointer OcclusionInfo[2];            // +0x48
    CEngineInternalPrimitiveOcclusionBase(long type, unsigned long flags) : CEngineInternalPrimitiveBase(type, flags) { }
};

struct CEngineInternalPrimitiveOcclusionGroup : CEngineInternalPrimitiveOcclusionBase {   // vtable 0x012A5364
    CEngineSceneGridCell* GridCell;                // +0x50
    CEngineSceneGrid*     Grid;                    // +0x54
    unsigned long         GridX;                   // +0x58
    unsigned long         GridY;                   // +0x5c
    unsigned long         FramesRendered;          // +0x60
    virtual void v00();
    CEngineInternalPrimitiveOcclusionGroup(CEngineSceneGridCell* cell, CEngineSceneGrid* grid, unsigned long x, unsigned long y);
};

CEngineInternalPrimitiveOcclusionGroup::CEngineInternalPrimitiveOcclusionGroup(CEngineSceneGridCell* cell, CEngineSceneGrid* grid, unsigned long x, unsigned long y)
    : CEngineInternalPrimitiveOcclusionBase(0x29, 8), GridCell(cell), Grid(grid), GridX(x), GridY(y), FramesRendered(0)
{
}