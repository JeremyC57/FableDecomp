// CTCShapeManager::IsPosInAnyShape(const C2DVector& pos) -- false outside the closed-shape bounds box; otherwise true when some persistent shape of type 0 contains the position
// Leaf: IsPosInAnyShape
// Grade: DIFFER(142v153) -- compares now match; retail has an 8-byte frame (Y copy at [esp+14h], i.e. a by-value C2DVector whose X copy is forwarded), pushes esi/edi before the compares, re-reads the vector end in the loop test (`cmp edi,[ebx+10h]`) and keeps fstp st(0) on a shared early exit; next: Contains(C2DVector p) by value with this && / `<` form
// Retail: Fable.exe 0x00760870, 151 bytes (`ret 4`).
// Callees: NShape::CShape (vtable 0x0126A208) virtual slot 1 (+0x04, shape type as a long; 0 = closed area) and
// slot 4 (+0x10, bool contains(const C2DVector&)).
// Layout: retail CTCShapeManager (Ego_r.pdb less the 4 bytes of the 16-byte debug vector): PersistantShapes
// (vector<CCountedPointer<NShape::CShape>>, 8-byte elements) +0x0C, SelectedShape +0x18, Activated +0x1C,
// ClosedShapeBounds (C2DBoxF: TopLeft +0x20, BottomRight +0x28).
// Shape: the bounds test is four early-out float compares (X kept on the FPU stack across the first two, Y
// copied into a frame word by an inline by-value test); the loop re-reads the vector end each pass.
struct C2DVector { float X, Y; };
namespace NShape {
struct CShape {
    virtual void v00();
    virtual long GetShapeType() const;                  // slot 1 (+0x04)
    virtual void v02(); virtual void v03();
    virtual bool IsPosInShape(const C2DVector& pos) const;   // slot 4 (+0x10)
};
}
struct CCPPointerInfo;
struct CShapePointer {                                  // CCountedPointer<NShape::CShape>
    NShape::CShape* Data;
    CCPPointerInfo* Info;
};
struct C2DBoxF {
    C2DVector TopLeft;
    C2DVector BottomRight;
    bool Contains(float x, float y) const
    {
        return x >= TopLeft.X && x < BottomRight.X && y >= TopLeft.Y && y < BottomRight.Y;
    }
};
struct CTCShapeManager {
    virtual void v00();
    void*          ParentThing;                         // +0x04 (CTCBase)
    bool           ToBeRemoved;                         // +0x08
    bool           IsInGlobalUpdateTCs;                 // +0x09
    CShapePointer* PersistantShapes_first;              // +0x0c
    CShapePointer* PersistantShapes_last;               // +0x10
    CShapePointer* PersistantShapes_end;                // +0x14
    CShapePointer* SelectedShape;                       // +0x18
    bool           Activated;                           // +0x1c
    bool           NewShapeActivated;                   // +0x1d
    bool           LeavingShape;                        // +0x1e
    C2DBoxF        ClosedShapeBounds;                   // +0x20
    bool IsPosInAnyShape(const C2DVector& pos) const;
};

bool CTCShapeManager::IsPosInAnyShape(const C2DVector& pos) const
{
    if (!ClosedShapeBounds.Contains(pos.X, pos.Y))
        return false;
    for (CShapePointer* it = PersistantShapes_first; it != PersistantShapes_last; ++it) {
        NShape::CShape* shape = it->Data;
        if (shape->GetShapeType() == 0 && shape->IsPosInShape(pos))
            return true;
    }
    return false;
}
