// CTCShapeManager::IsPosInAnyShape(const C2DVector& pos) -- false outside the closed-shape bounds box; otherwise true when some persistent shape of type 0 contains the position
// Leaf: IsPosInAnyShape
// Retail: Fable.exe 0x00760870, 151 bytes (`ret 4`).
// Callees: NShape::CShape (vtable 0x0126A208) virtual slot 1 (+0x04, shape type as a long; 0 = closed area) and
// slot 4 (+0x10, bool contains(const C2DVector&)).
// Layout: retail CTCShapeManager (Ego_r.pdb less the 4 bytes of the 16-byte debug vector): PersistantShapes
// (vector<CCountedPointer<NShape::CShape>>, 8-byte elements) +0x0C, SelectedShape +0x18, Activated +0x1C,
// ClosedShapeBounds (C2DBoxF: TopLeft +0x20, BottomRight +0x28).
// Shape: the whole search sits inside `if (ClosedShapeBounds.Contains(pos)) { ... } return false;` so the miss paths
// share one epilogue (all four registers pushed up front); Contains takes the C2DVector BY VALUE with a user copy ctor
// (Y copied into the 8-byte frame, X forwarded on the FPU stack); the loop re-reads the vector end each pass.
struct C2DVector { float X, Y; C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { } };
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
    bool Contains(C2DVector p) const
    {
        return p.X >= TopLeft.X && p.X < BottomRight.X && p.Y >= TopLeft.Y && p.Y < BottomRight.Y;
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
    if (ClosedShapeBounds.Contains(pos)) {
        for (CShapePointer* it = PersistantShapes_first; it != PersistantShapes_last; ++it) {
            NShape::CShape* shape = it->Data;
            if (shape->GetShapeType() == 0 && shape->IsPosInShape(pos))
                return true;
        }
    }
    return false;
}