// CTCShapeManager::InitialiseFrom(const CTCShapeManager* other) -- rebuild this manager's persistent shapes as basic shapes over copies of the other manager's point lists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00762AB0, 230 bytes (`ret 4`).
// Callees: vector<C3DVector> copy constructor @ 0x006F0810 (twice: the frame copy, then the shape's own list), operator new
// @ 0x00BFEA1A (the 0x2c-byte shape, then the 12-byte count block), free @ 0x00BFEA14 (the frame copy's storage),
// CTCShapeManager::AddNewShape(CCountedPointer<NShape::CShape>) @ 0x00762580 (by value); the count block holds the
// per-type deleter 0x00763A10 (as the landed CCountedPointer Reset 0x007642C0).
// Layout (retail 12-byte vectors): CTCShapeManager PersistantShapes +0x0c; NShape::CShape (vtable 0x0126A208) Owner +0x04,
// Points +0x08, a second point vector +0x14; NShape::CBasicShape (vtable 0x0126A31C, 0x2c bytes) Subdivisions +0x20
// (the landed CBasicShape ctor 0x00762440 is the out-of-line copy of the constructor inlined here).
// Shape: the point list is copied into a frame vector, `new CBasicShape(this, points)` inlines both constructors (vtable,
// owner, copied list, zeroed members), the counted pointer is built straight in AddNewShape's argument area through the
// raw-pointer constructor (`if (Data) Info = new block; else Info = 0;`, three exits), and the frame list is freed.
#include <stddef.h>
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
void* operator new(size_t size);                    // 0x00BFEA1A
struct C3DVector { float X, Y, Z; };
struct CPointVector {                               // STLport vector<C3DVector>
    C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage;
    CPointVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CPointVector(const CPointVector& other);        // 0x006F0810
    ~CPointVector() { if (_M_start) free(_M_start); }
};
struct CTCShapeManager;
struct CSubdivisionTriangle;
namespace NShape {
struct CShape {                                     // vtable 0x0126A208
    virtual void v00();
    CTCShapeManager* Owner;                         // +0x04
    CPointVector Points;                            // +0x08
    CPointVector Edges;                             // +0x14
    CShape(CTCShapeManager* owner, const CPointVector& points) : Owner(owner), Points(points), Edges() { }
};
struct CSubdivisionPointer { CSubdivisionTriangle* Data; void* Info; };
struct CSubdivisionVector {                         // STLport vector<CCountedPointer<CSubdivisionTriangle>>
    CSubdivisionPointer* _M_start; CSubdivisionPointer* _M_finish; CSubdivisionPointer* _M_end_of_storage;
    CSubdivisionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CBasicShape : CShape {                       // vtable 0x0126A31C
    CSubdivisionVector Subdivisions;                // +0x20
    virtual void v00();
    CBasicShape(CTCShapeManager* owner, const CPointVector& points) : CShape(owner, points), Subdivisions() { }
};
}
void __fastcall Destroy_00763a10(void* object);     // 0x00763A10: the per-type deleter stored in the count block
struct CCPPointerInfo {
    long References;                                // +0
    void (__fastcall* PfnDestroy)(void*);           // +4
    void* Object;                                   // +8
    CCPPointerInfo(void* object) : References(1), PfnDestroy(Destroy_00763a10), Object(object) { }
};
struct CShapePointer {                              // CCountedPointer<NShape::CShape>
    NShape::CShape* Data;
    CCPPointerInfo* Info;
    CShapePointer(NShape::CShape* p) : Data(p) { if (Data) Info = new CCPPointerInfo(Data); else Info = 0; }
    CShapePointer(const CShapePointer& other);
    ~CShapePointer();
};
struct CTCShapeManager {
    virtual void v00();
    void*          ParentThing;                     // +0x04 (CTCBase)
    bool           ToBeRemoved;                     // +0x08
    bool           IsInGlobalUpdateTCs;             // +0x09
    unsigned char  _pad0a[2];
    CShapePointer* PersistantShapes_first;          // +0x0c
    CShapePointer* PersistantShapes_last;           // +0x10
    CShapePointer* PersistantShapes_end;            // +0x14
    CShapePointer* SelectedShape;                   // +0x18
    void AddNewShape(CShapePointer shape);          // 0x00762580
    void InitialiseFrom(const CTCShapeManager* other);
};

void CTCShapeManager::InitialiseFrom(const CTCShapeManager* other)
{
    for (CShapePointer* it = other->PersistantShapes_first; it != other->PersistantShapes_last; ++it) {
        CPointVector points(it->Data->Points);
        AddNewShape(CShapePointer(new NShape::CBasicShape(this, points)));
    }
}
