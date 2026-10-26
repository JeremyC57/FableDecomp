// CTCShapeManager::EditUpdateSplit() -- editor: split the selected shape at its selected point; on success rebuild it from a copy of its points (same type) and re-add it in place of the old entry
// Leaf: EditUpdateSplit
// Retail: Fable.exe 0x00762F50, 108 bytes (plain `ret`); the twin of EditUpdateDelete 0x00762D80 without the else arm.
// Callees: NShape::CShape::UpdateSplit() @ 0x007622C0 (bool), vector<C3DVector> copy ctor @ 0x006F0810 (manifest
// "Vector_CopyFrom"), NShape::CShape virtual slot 1 (+0x04, shape type), vector<CCountedPointer<NShape::CShape>>::erase
// (iterator) @ 0x00764D10 (returns the iterator in eax; manifest "CScriptBase::RemovePredicate" is propagated),
// CTCShapeManager::CreateShape(long type, const vector<C3DVector>& points) @ 0x007624E0 (returns CCountedPointer by
// value; manifest "CEngineLightingManager::CreateInternalPrimitive" is propagated), CTCShapeManager::AddNewShape
// @ 0x00762580, free @ 0x00BFEA14 (the copied vector's storage).
// Layout: retail CTCShapeManager: PersistantShapes +0x0C (_M_finish +0x10), SelectedShape +0x18; NShape::CShape:
// Points (vector<C3DVector>) +0x08.
// Shape: CreateShape's by-value result is built straight in AddNewShape's argument area (`sub esp,8 ; mov ecx,esp`);
// the erase result's store into SelectedShape is scheduled after the pushes.
class C3DVector { public: float X, Y, Z; };
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
struct CPointVector {                               // STLport vector<C3DVector>
    C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage;
    CPointVector(const CPointVector& other);        // 0x006F0810
    ~CPointVector() { if (_M_start) free(_M_start); }
};
namespace NShape {
class CShape { public:
    virtual void v00();
    virtual long GetShapeType() const;              // slot 1 (+0x04)
    void* Owner;                                    // +0x04
    CPointVector Points;                            // +0x08
    bool UpdateSplit();                        // 0x007622C0
};
}
class CCPPointerInfo;
struct CShapePointer {                              // CCountedPointer<NShape::CShape>
    NShape::CShape* Data;
    CCPPointerInfo* Info;
    CShapePointer(const CShapePointer& other);
    ~CShapePointer();
    NShape::CShape* operator->() const { return Data; }
};
struct CShapeVector {                               // STLport vector<CCountedPointer<NShape::CShape>>
    CShapePointer* _M_start;
    CShapePointer* _M_finish;
    CShapePointer* _M_end_of_storage;
    CShapePointer* end() const { return _M_finish; }
    CShapePointer* erase(CShapePointer* pos);       // 0x00764D10
};
class CTCShapeManager { public:
    virtual void v00();
    void*          ParentThing;                     // +0x04 (CTCBase)
    bool           ToBeRemoved;                     // +0x08
    bool           IsInGlobalUpdateTCs;             // +0x09
    CShapeVector   PersistantShapes;                // +0x0c
    CShapePointer* SelectedShape;                   // +0x18
    CShapePointer CreateShape(long type, const CPointVector& points);   // 0x007624E0
    void AddNewShape(CShapePointer shape);          // 0x00762580
    void EditUpdateSplit();
};

void CTCShapeManager::EditUpdateSplit()
{
    if (SelectedShape != PersistantShapes.end()) {
        NShape::CShape* shape = SelectedShape->Data;
        if (shape->UpdateSplit()) {
            CPointVector points(shape->Points);
            long type = shape->GetShapeType();
            SelectedShape = PersistantShapes.erase(SelectedShape);
            AddNewShape(CreateShape(type, points));
        }
    }
}