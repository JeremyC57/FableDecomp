// Behaviour test for 00762e10: src/00762e10.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCShapeManager::EditUpdateDisplacement(const C2DVector& delta)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCShapeManager::EditUpdateDisplacement(const C2DVector& delta) -- editor: move the selected shape's selected points by an XY delta (or, when none are selected, the whole shape) and rebuild the shape from the moved point list
// Leaf: EditUpdateDisplacement
// Retail: Fable.exe (TLC, VC7.1) @ 0x00762E10, 311 bytes, `ret 4`.
// Callees: NShape::CShape::UpdateDisplacePoints(const C3DVector&) @ 0x00761200 (bool), vector<C3DVector> copy ctor @ 0x006F0810,
//   vector<C3DVector>::operator= @ 0x0075B8D0 (manifest "Std_Vector_Assign"), free @ 0x00BFEA14, NShape::CShape virtual slot 1
//   (+0x04, shape type), vector<CCountedPointer<NShape::CShape>>::erase(iterator) @ 0x00764D10, CTCShapeManager::CreateShape
//   @ 0x007624E0, CTCShapeManager::AddNewShape @ 0x00762580 (as the landed sibling EditUpdateDelete 0x00762D80).
// Layout: retail CTCShapeManager PersistantShapes +0x0C (_M_finish +0x10), SelectedShape +0x18; NShape::CShape Points +0x08.
// Shape: the 3D delta (Z = 0) is a local built before the selection test; the point list is a default-constructed local
//   assigned from the shape's by-value point getter (copy, operator=, free) in both arms; with nothing selected every point
//   gets `*p += delta` (the constant Z = 0 folds, leaving Z copied through edx); then the shape is erased and rebuilt as in
//   EditUpdateDelete. Retail keeps the point list's _M_start in ebx across the calls and reuses delta's frame slot for the
//   first arm's temporary: VC7.1 escape analysis is flow-insensitive, so the vector copy ctor, operator=, CreateShape and
//   UpdateDisplacePoints (all instantiated or defined in retail's TU) are same-TU stand-ins taking the bounds / delta by value.
class C2DVector { public: float X, Y; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
struct CPointVector {                               // STLport vector<C3DVector>
    C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage;
    CPointVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CPointVector(const CPointVector& other);        // 0x006F0810
    ~CPointVector() { if (_M_start) free(_M_start); }
    CPointVector& operator=(const CPointVector& other);   // 0x0075B8D0
    C3DVector* begin() { return _M_start; }
    C3DVector* end() { return _M_finish; }
};
C3DVector* CopyPoints_006f0810(const C3DVector* first, const C3DVector* last);
void AssignPoints_0075b8d0(C3DVector** start, C3DVector** finish, C3DVector** endOfStorage, const C3DVector* first, const C3DVector* last);
// Stand-ins for the vector<C3DVector> copy ctor @ 0x006F0810 and operator= @ 0x0075B8D0 (instantiated in retail's TU):
// the bounds pass by value, so the local point list does not escape.
__declspec(noinline) CPointVector::CPointVector(const CPointVector& other)
{
    _M_start = CopyPoints_006f0810(other._M_start, other._M_finish);
    _M_finish = _M_start + (other._M_finish - other._M_start);
    _M_end_of_storage = _M_finish;
}
__declspec(noinline) CPointVector& CPointVector::operator=(const CPointVector& other)
{
    C3DVector* start = _M_start;
    C3DVector* finish = _M_finish;
    C3DVector* endOfStorage = _M_end_of_storage;
    AssignPoints_0075b8d0(&start, &finish, &endOfStorage, other._M_start, other._M_finish);
    _M_start = start;
    _M_finish = finish;
    _M_end_of_storage = endOfStorage;
    return *this;
}
namespace NShape {
class CShape { public:
    virtual void v00();
    virtual long GetShapeType() const;              // slot 1 (+0x04)
    void* Owner;                                    // +0x04
    CPointVector Points;                            // +0x08
    bool UpdateDisplacePoints(const C3DVector& delta);   // 0x00761200
    CPointVector GetPoints() const { return Points; }
};
}
class CCPPointerInfo;
struct CShapePointer {                              // CCountedPointer<NShape::CShape>
    NShape::CShape* Data;
    CCPPointerInfo* Info;
    CShapePointer(const CShapePointer& other);
    ~CShapePointer();
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
    void EditUpdateDisplacement(const C2DVector& delta);
};
bool UpdateDisplacePoints_00761200(NShape::CShape* shape, float x, float y, float z);
// Stand-in for NShape::CShape::UpdateDisplacePoints @ 0x00761200 (same TU): the delta passes by value.
__declspec(noinline) bool NShape::CShape::UpdateDisplacePoints(const C3DVector& delta)
{
    return UpdateDisplacePoints_00761200(this, delta.X, delta.Y, delta.Z);
}
CShapePointer CreateShape_007624e0(CTCShapeManager* manager, long type, const C3DVector* first, const C3DVector* last);
// Stand-in for CTCShapeManager::CreateShape @ 0x007624E0 (same TU): the point list's bounds pass by value.
__declspec(noinline) CShapePointer CTCShapeManager::CreateShape(long type, const CPointVector& points)
{
    return CreateShape_007624e0(this, type, points._M_start, points._M_finish);
}

void CTCShapeManager::EditUpdateDisplacement(const C2DVector& delta2D)
{
    C3DVector delta(delta2D.X, delta2D.Y, 0.0f);
    if (SelectedShape != PersistantShapes.end()) {
        NShape::CShape* shape = SelectedShape->Data;
        CPointVector points;
        if (shape->UpdateDisplacePoints(delta)) {
            points = shape->GetPoints();
        } else {
            points = shape->GetPoints();
            for (C3DVector* p = points.begin(); p != points.end(); ++p)
                *p += delta;
        }
        long type = shape->GetShapeType();
        SelectedShape = PersistantShapes.erase(SelectedShape);
        AddNewShape(CreateShape(type, points));
    }
}

// ---- stubs and driver ----
#include <cstdlib>
static int g_copies = 0, g_assigns = 0, g_updates = 0, g_creates = 0, g_adds = 0, g_erases = 0, g_ptrDtors = 0;
static bool g_updateResult = false; static float g_ux, g_uy, g_uz;
static C3DVector g_created[8]; static long g_createdCount = 0, g_createdType = 0;
static NShape::CShape* g_added = 0; static CShapePointer* g_erasedAt = 0;
static C3DVector* Dup(const C3DVector* first, const C3DVector* last)
{
    long n = (long)(last - first); if (n == 0) return 0;
    C3DVector* p = (C3DVector*)malloc(n * sizeof(C3DVector)); memcpy(p, first, n * sizeof(C3DVector)); return p;
}
C3DVector* CopyPoints_006f0810(const C3DVector* first, const C3DVector* last) { ++g_copies; return Dup(first, last); }
void AssignPoints_0075b8d0(C3DVector** start, C3DVector** finish, C3DVector** endOfStorage, const C3DVector* first, const C3DVector* last)
{
    ++g_assigns; if (*start) free(*start);
    *start = Dup(first, last); *finish = *start + (last - first); *endOfStorage = *finish;
}
bool UpdateDisplacePoints_00761200(NShape::CShape* shape, float x, float y, float z)
{
    ++g_updates; g_ux = x; g_uy = y; g_uz = z;
    if (g_updateResult) { shape->Points._M_start[0].X += x; shape->Points._M_start[0].Y += y; }   // "selected" point 0 moves
    return g_updateResult;
}
static NShape::CShape* g_newShape = 0; static unsigned char g_ptrBuf[sizeof(CShapePointer)];
CShapePointer::CShapePointer(const CShapePointer& other) : Data(other.Data), Info(other.Info) {}
CShapePointer::~CShapePointer() { ++g_ptrDtors; }
CShapePointer CreateShape_007624e0(CTCShapeManager* manager, long type, const C3DVector* first, const C3DVector* last)
{
    ++g_creates; g_createdType = type; g_createdCount = (long)(last - first);
    for (long i = 0; i < g_createdCount && i < 8; ++i) g_created[i] = first[i];
    CShapePointer* made = (CShapePointer*)g_ptrBuf; made->Data = g_newShape; made->Info = 0;
    return *made;
}
CShapePointer* CShapeVector::erase(CShapePointer* pos) { ++g_erases; g_erasedAt = pos; return pos; }
void CTCShapeManager::AddNewShape(CShapePointer shape) { ++g_adds; g_added = shape.Data; }
void CTCShapeManager::v00() {}
void NShape::CShape::v00() {}
long NShape::CShape::GetShapeType() const { return 7; }
static void SetPoints(NShape::CShape* s)
{
    C3DVector pts[3] = { C3DVector(1, 2, 3), C3DVector(4, 5, 6), C3DVector(7, 8, 9) };
    s->Points._M_start = Dup(pts, pts + 3); s->Points._M_finish = s->Points._M_start + 3; s->Points._M_end_of_storage = s->Points._M_finish;
}
int main()
{
    CTCShapeManager* m = new CTCShapeManager;
    unsigned char slots[2 * sizeof(CShapePointer)]; memset(slots, 0, sizeof(slots));
    CShapePointer* vec = (CShapePointer*)slots;
    NShape::CShape* shape = new NShape::CShape; SetPoints(shape);
    g_newShape = new NShape::CShape;
    vec[0].Data = shape; vec[0].Info = 0;
    m->PersistantShapes._M_start = vec; m->PersistantShapes._M_finish = vec + 1; m->PersistantShapes._M_end_of_storage = vec + 1;
    C2DVector d = { 10.0f, 20.0f };
    // nothing selected: no work
    m->SelectedShape = vec + 1;
    m->EditUpdateDisplacement(d);
    CHECK(g_updates == 0 && g_creates == 0 && g_erases == 0 && g_adds == 0);
    // selected points moved by the shape itself
    m->SelectedShape = vec; g_updateResult = true;
    m->EditUpdateDisplacement(d);
    CHECK(g_updates == 1 && g_ux == 10.0f && g_uy == 20.0f && g_uz == 0.0f);
    CHECK(g_copies == 1 && g_assigns == 1 && g_creates == 1 && g_createdType == 7 && g_createdCount == 3);
    CHECK(g_created[0].X == 11.0f && g_created[0].Y == 22.0f && g_created[0].Z == 3.0f && g_created[1].X == 4.0f && g_created[2].Z == 9.0f);
    CHECK(g_erases == 1 && g_erasedAt == vec && m->SelectedShape == vec && g_adds == 1 && g_added == g_newShape);
    // no point selected: every point moves in XY on the copy; the shape's own list is untouched
    SetPoints(shape); g_updateResult = false;
    m->EditUpdateDisplacement(d);
    CHECK(g_updates == 2 && g_copies == 2 && g_assigns == 2 && g_creates == 2 && g_createdCount == 3);
    CHECK(g_created[0].X == 11.0f && g_created[0].Y == 22.0f && g_created[0].Z == 3.0f);
    CHECK(g_created[1].X == 14.0f && g_created[1].Y == 25.0f && g_created[1].Z == 6.0f);
    CHECK(g_created[2].X == 17.0f && g_created[2].Y == 28.0f && g_created[2].Z == 9.0f);
    CHECK(shape->Points._M_start[1].X == 4.0f && shape->Points._M_start[1].Y == 5.0f);
    CHECK(g_erases == 2 && g_adds == 2);
    if (fails == 0) printf("00762e10_TEST PASS\n");
    return fails;
}
