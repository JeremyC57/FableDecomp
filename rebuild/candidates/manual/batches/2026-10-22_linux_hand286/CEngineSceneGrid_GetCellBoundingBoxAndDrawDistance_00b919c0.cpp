// CEngineSceneGrid::GetCellBoundingBoxAndDrawDistance(CAABox& box, float& drawDistance, long x, long y) -- return a cell's cached bounds and draw distance, building the cache on first use from the four child-level cells (recursively) and the cell's own primitives (union of boxes, maximum draw distance)
// Leaf: GetCellBoundingBoxAndDrawDistance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B919C0, length 736 bytes (body 0x2dc + int3 padding), `ret 0x10`, recursive.
// Callees: itself on the child grid, primitive virtuals GetBoundingBox (slot 0x44) and GetDrawRange (slot 0x34, by-value result).
// Layout: local model (grid: Child +0x0c, Side +0x10, cell table +0x14; cell: primitive list +0x04, Box +0x08, DrawDistance +0x20,
//   flags +0x44 bit 0 = cached).
struct C3DVector { float X, Y, Z; };
struct CAABox { C3DVector Min, Max; };
struct SDrawRange { float Near, Far; };
struct CEngineInternalPrimitiveBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30();
    virtual SDrawRange GetDrawRange() const;                  // +0x34
    virtual void v38(); virtual void v3c(); virtual void v40();
    virtual bool GetBoundingBox(CAABox* box) const;          // +0x44
    unsigned char                 _pad04[0x38 - 4];
    CEngineInternalPrimitiveBase* Next;                       // +0x38
};
struct CSceneCell {
    long                          _unk00;                     // +0x00
    CEngineInternalPrimitiveBase* Primitives;                 // +0x04
    CAABox                        Box;                        // +0x08
    float                         DrawDistance;               // +0x20
    unsigned char                 _pad24[0x44 - 0x24];
    unsigned char                 Flags;                      // +0x44
};
struct CEngineSceneGrid {
    void*             _vptr;                                  // +0x00
    long              Level;                                  // +0x04
    CEngineSceneGrid* Parent;                                 // +0x08
    CEngineSceneGrid* Child;                                  // +0x0c
    long              Side;                                   // +0x10
    CSceneCell**      Cells;                                  // +0x14
    bool GetCellBoundingBoxAndDrawDistance(CAABox& box, float& drawDistance, long x, long y);
};

bool CEngineSceneGrid::GetCellBoundingBoxAndDrawDistance(CAABox& box, float& drawDistance, long x, long y)
{
    CSceneCell* cell = Cells[x + Side * y];
    if (!cell)
        return false;
    if (!(cell->Flags & 1)) {
        bool found = false;
        if (Child) {
            for (unsigned long j = 0; j < 2; ++j) {
                for (unsigned long i = 0; i < 2; ++i) {
                    CAABox childBox;
                    float childDistance;
                    if (Child->GetCellBoundingBoxAndDrawDistance(childBox, childDistance, i + x * 2, j + y * 2)) {
                        if (found) {
                            if (cell->Box.Min.X > childBox.Min.X) cell->Box.Min.X = childBox.Min.X;
                            if (cell->Box.Min.Y > childBox.Min.Y) cell->Box.Min.Y = childBox.Min.Y;
                            if (cell->Box.Min.Z > childBox.Min.Z) cell->Box.Min.Z = childBox.Min.Z;
                            if (cell->Box.Max.X < childBox.Max.X) cell->Box.Max.X = childBox.Max.X;
                            if (cell->Box.Max.Y < childBox.Max.Y) cell->Box.Max.Y = childBox.Max.Y;
                            if (cell->Box.Max.Z < childBox.Max.Z) cell->Box.Max.Z = childBox.Max.Z;
                            if (cell->DrawDistance < childDistance) cell->DrawDistance = childDistance;
                        } else {
                            cell->Box = childBox;
                            cell->DrawDistance = childDistance;
                            found = true;
                        }
                    }
                }
            }
        }
        for (CEngineInternalPrimitiveBase* p = cell->Primitives; p; p = p->Next) {
            CAABox primitiveBox;
            if (p->GetBoundingBox(&primitiveBox)) {
                if (found) {
                    if (cell->Box.Min.X > primitiveBox.Min.X) cell->Box.Min.X = primitiveBox.Min.X;
                    if (cell->Box.Min.Y > primitiveBox.Min.Y) cell->Box.Min.Y = primitiveBox.Min.Y;
                    if (cell->Box.Min.Z > primitiveBox.Min.Z) cell->Box.Min.Z = primitiveBox.Min.Z;
                    if (cell->Box.Max.X < primitiveBox.Max.X) cell->Box.Max.X = primitiveBox.Max.X;
                    if (cell->Box.Max.Y < primitiveBox.Max.Y) cell->Box.Max.Y = primitiveBox.Max.Y;
                    if (cell->Box.Max.Z < primitiveBox.Max.Z) cell->Box.Max.Z = primitiveBox.Max.Z;
                    if (cell->DrawDistance < p->GetDrawRange().Far) cell->DrawDistance = p->GetDrawRange().Far;
                } else {
                    cell->Box = primitiveBox;
                    cell->DrawDistance = p->GetDrawRange().Far;
                    found = true;
                }
            }
        }
        if (found)
            cell->Flags |= 1;
        else
            return false;
    }
    box = cell->Box;
    drawDistance = cell->DrawDistance;
    return true;
}
