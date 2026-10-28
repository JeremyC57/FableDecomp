// CTCShapeManager::GetPrincipalVector(C3DVector& out) const -- when a shape is selected and has points, out = the thing's position minus the shape's first point; returns whether it was set
// Leaf: GetPrincipalVector
// Retail: Fable.exe 0x00760910, 121 bytes (`ret 4`).
// Callees: CThing::GetPos() const @ 0x004C73D0 (returns const C3DVector&; manifest "CVertexBufferWin32::DoSizeof"
// is propagated).
// Layout: retail CTCShapeManager: PersistantShapes (vector<CCountedPointer<NShape::CShape>>) +0x0C (_M_finish +0x10),
// SelectedShape (vector iterator) +0x18; NShape::CShape (0x28): Owner +0x04, Points (vector<C3DVector>) +0x08.
// Shape: the selected shape is read twice through the CCountedPointer operator-> (empty test, front), which gives
// retail's `mov eax,[eax] ; mov edx,[eax+8]` register use; operator- takes the first point BY VALUE (copy before the call); the difference goes through
// float temporaries and is copied into `out` as dwords.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(C3DVector o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
class CThing { public:
    const C3DVector& GetPos() const;               // 0x004C73D0
};
class CTCShapeManager;
namespace NShape {
class CShape { public:
    virtual void v00();
    CTCShapeManager* Owner;                             // +0x04
    struct CPointVector {                               // STLport vector<C3DVector>
        C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage;
        bool empty() const { return _M_start == _M_finish; }
        const C3DVector& front() const { return *_M_start; }
    } Points;                                           // +0x08
};
}
class CCPPointerInfo;
struct CShapePointer {                                  // CCountedPointer<NShape::CShape>
    NShape::CShape* Data;
    CCPPointerInfo* Info;
    NShape::CShape* operator->() const { return Data; }
};
class CTCShapeManager { public:
    virtual void v00();
    CThing*        ParentThing;                         // +0x04 (CTCBase)
    bool           ToBeRemoved;                         // +0x08
    bool           IsInGlobalUpdateTCs;                 // +0x09
    CShapePointer* PersistantShapes_first;              // +0x0c
    CShapePointer* PersistantShapes_last;               // +0x10
    CShapePointer* PersistantShapes_end;                // +0x14
    CShapePointer* SelectedShape;                       // +0x18
    bool GetPrincipalVector(C3DVector& out) const;
};

bool CTCShapeManager::GetPrincipalVector(C3DVector& out) const
{
    if (SelectedShape != PersistantShapes_last) {
        if (!(*SelectedShape)->Points.empty()) {
            out = ParentThing->GetPos() - (*SelectedShape)->Points.front();
            return true;
        }
    }
    return false;
}