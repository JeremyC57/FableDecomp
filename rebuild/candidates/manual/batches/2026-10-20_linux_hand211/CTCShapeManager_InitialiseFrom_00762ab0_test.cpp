// Behaviour test for CTCShapeManager::InitialiseFrom (0x00762AB0): one new CBasicShape per source shape, owned by this
// manager, holding its own copy of the source point list, handed to AddNewShape with a fresh count block (count 1).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#endif
struct C3DVector { float X, Y, Z; };
struct CPointVector {
    C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage;
    CPointVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CPointVector(const CPointVector& other)
    {
        size_t n = other._M_finish - other._M_start;
        _M_start = n ? (C3DVector*)malloc(n * sizeof(C3DVector)) : 0;
        if (n) memcpy(_M_start, other._M_start, n * sizeof(C3DVector));
        _M_finish = _M_end_of_storage = _M_start + n;
    }
    ~CPointVector() { if (_M_start) free(_M_start); }
};
struct CTCShapeManager;
struct CSubdivisionTriangle;
namespace NShape {
struct CShape {
    virtual void v00() { }
    CTCShapeManager* Owner;
    CPointVector Points;
    CPointVector Edges;
    CShape(CTCShapeManager* owner, const CPointVector& points) : Owner(owner), Points(points), Edges() { }
};
struct CSubdivisionPointer { CSubdivisionTriangle* Data; void* Info; };
struct CSubdivisionVector {
    CSubdivisionPointer* _M_start; CSubdivisionPointer* _M_finish; CSubdivisionPointer* _M_end_of_storage;
    CSubdivisionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CBasicShape : CShape {
    CSubdivisionVector Subdivisions;
    virtual void v00() { }
    CBasicShape(CTCShapeManager* owner, const CPointVector& points) : CShape(owner, points), Subdivisions() { }
};
}
static int g_destroyed = 0;
void __fastcall Destroy_00763a10(void* object) { ++g_destroyed; }
struct CCPPointerInfo {
    long References;
    void (__fastcall* PfnDestroy)(void*);
    void* Object;
    CCPPointerInfo(void* object) : References(1), PfnDestroy(Destroy_00763a10), Object(object) { }
};
struct CShapePointer {
    NShape::CShape* Data;
    CCPPointerInfo* Info;
    CShapePointer(NShape::CShape* p) : Data(p) { if (Data) Info = new CCPPointerInfo(Data); else Info = 0; }
    CShapePointer(const CShapePointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
    ~CShapePointer() { if (Info && --Info->References == 0) Info->PfnDestroy(Info->Object); }
};
struct CTCShapeManager {
    virtual void v00() { }
    void*          ParentThing;
    bool           ToBeRemoved;
    bool           IsInGlobalUpdateTCs;
    unsigned char  _pad0a[2];
    CShapePointer* PersistantShapes_first;
    CShapePointer* PersistantShapes_last;
    CShapePointer* PersistantShapes_end;
    CShapePointer* SelectedShape;
    NShape::CShape* Added[4]; long AddedCount[4]; CCPPointerInfo* AddedInfo[4]; int NumAdded;
    void AddNewShape(CShapePointer shape)
    {
        Added[NumAdded] = shape.Data; AddedInfo[NumAdded] = shape.Info;
        AddedCount[NumAdded] = shape.Info ? shape.Info->References : -1;
        if (shape.Info) ++shape.Info->References;          // keep it alive
        ++NumAdded;
    }
    void InitialiseFrom(const CTCShapeManager* other);
};

void CTCShapeManager::InitialiseFrom(const CTCShapeManager* other)
{
    for (CShapePointer* it = other->PersistantShapes_first; it != other->PersistantShapes_last; ++it) {
        CPointVector points(it->Data->Points);
        AddNewShape(CShapePointer(new NShape::CBasicShape(this, points)));
    }
}

int main()
{
    int bad = 0;
    C3DVector pa[2] = { { 1, 2, 3 }, { 4, 5, 6 } };
    C3DVector pb[1] = { { 7, 8, 9 } };
    CTCShapeManager src;
    NShape::CBasicShape* a = new NShape::CBasicShape(&src, CPointVector());
    NShape::CBasicShape* b = new NShape::CBasicShape(&src, CPointVector());
    a->Points._M_start = pa; a->Points._M_finish = pa + 2;
    b->Points._M_start = pb; b->Points._M_finish = pb + 1;
    CShapePointer list[2] = { CShapePointer(a), CShapePointer(b) };
    src.PersistantShapes_first = list; src.PersistantShapes_last = list + 2;
    CTCShapeManager dst; dst.NumAdded = 0;
    dst.InitialiseFrom(&src);
    if (dst.NumAdded != 2) bad |= 1;
    for (int i = 0; i < 2 && !bad; ++i) {
        NShape::CShape* s = dst.Added[i];
        C3DVector* want = i ? pb : pa; int n = i ? 1 : 2;
        if (!s || s == a || s == b || s->Owner != &dst) bad |= 2;
        if (s->Points._M_finish - s->Points._M_start != n || s->Points._M_start == want) bad |= 4;
        if (memcmp(s->Points._M_start, want, n * sizeof(C3DVector))) bad |= 8;
        if (s->Edges._M_start || ((NShape::CBasicShape*)s)->Subdivisions._M_start) bad |= 16;
        if (dst.AddedCount[i] != 1 || dst.AddedInfo[i]->Object != s || dst.AddedInfo[i]->PfnDestroy != Destroy_00763a10) bad |= 32;
    }
    if (g_destroyed != 0) bad |= 64;
    CTCShapeManager empty; empty.PersistantShapes_first = empty.PersistantShapes_last = 0; dst.NumAdded = 0;
    dst.InitialiseFrom(&empty);
    if (dst.NumAdded != 0) bad |= 128;
    if (bad) { std::printf("00762ab0_TEST FAIL\n"); return 1; }
    std::printf("00762ab0_TEST PASS\n");
    return 0;
}
