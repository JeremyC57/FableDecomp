// Behaviour test for 00a86e20: src/00a86e20.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CPhysicsFacesSegmentLeaf::GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, std::vector<long>& faces)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPhysicsFacesSegmentLeaf::GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, std::vector<long>& faces) -- add each of the leaf's triangle indices not yet stamped in this query to the result, stamping it so shared triangles are reported once
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A86E20, length 115 bytes (+ int3 padding).
// Leaf: GetPossibleCollisionSet
// Slot 4 of CPhysicsFacesSegmentLeaf's vtable 0x0129DEA4 (a leaf ignores the query volume: radius/from/to are unused).
// Callees: the vector<long> reallocating insert @ 0x00440990 (`this` = the vector; where, value&, tag&, count 1, true).
// Globals: the current query stamp @ 0x013D2D44 and the per-triangle stamp array @ 0x013D2D48 (both re-read per index).
// Layout: engine/CPhysicsFacesSegmentLeaf.h: TriangleListIndex is a CArray<unsigned short> at +4 (retail first/last/end);
// the header models it as opaque bytes, so the array is spelled out here with the header's offset.
// Shape: the count `(last - first) >> 1` kept in ebp, the index in edi, the result vector in esi; the loop body reloads
// first each pass; the index is an unsigned short local whose long conversion temporary (made for push_back's
// const long&) is homed in the dead `faces` argument slot, the push_back inlined as
// the landed 0x00BBAC20 (in-place store when there is room, else the insert with an empty scalar tag); `ret 10h`.
// No frame word only with a same-TU noinline stand-in for the insert 0x00440990 (the const-ref taker of the pushed
// value); the unsigned short index then gives retail's register roles and the late store of the temporary.
#include <new>
class C3DVector { public: float x, y, z; };

struct SScalarTag { SScalarTag() { } };
struct CLongVector {                               // STLport vector<long>
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};

// Same-TU stand-in for the vector<long> insert 0x00440990 (not the retail body).
void GrowInsert_00440990(CLongVector* v, long* where, long value, unsigned long count);
#pragma auto_inline(off)
__declspec(noinline) void CLongVector::InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    GrowInsert_00440990(this, where, value, count);
}
#pragma auto_inline(on)
extern unsigned long  g_PhysicsFacesQueryStamp;    // 0x013D2D44
extern unsigned long* g_PhysicsFacesTriangleStamps;   // 0x013D2D48

struct CPhysicsFacesSegmentLeaf {                  // engine/CPhysicsFacesSegmentLeaf.h (0x14 bytes)
    virtual void v00();
    unsigned short* TriangleListIndexFirst;        // +0x04 CArray<unsigned short>
    unsigned short* TriangleListIndexLast;         // +0x08
    unsigned short* TriangleListIndexEnd;          // +0x0c
    unsigned long   TriangleListIndexPad;          // +0x10
    long GetNoFaces() const { return TriangleListIndexLast - TriangleListIndexFirst; }
    void GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, CLongVector& faces);
};

void CPhysicsFacesSegmentLeaf::GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, CLongVector& faces)
{
    long count = GetNoFaces();
    for (long i = 0; i < count; ++i) {
        unsigned short index = TriangleListIndexFirst[i];
        if (g_PhysicsFacesTriangleStamps[index] != g_PhysicsFacesQueryStamp) {
            g_PhysicsFacesTriangleStamps[index] = g_PhysicsFacesQueryStamp;
            faces.push_back(index);
        }
    }
}

// ---- stubs and driver ----
unsigned long  g_PhysicsFacesQueryStamp = 0;
unsigned long* g_PhysicsFacesTriangleStamps = 0;
void CPhysicsFacesSegmentLeaf::v00() { }
static int g_grows = 0;
static long g_storage[64];
void GrowInsert_00440990(CLongVector* v, long* where, long value, unsigned long count)
{
    ++g_grows; CHECK(count == 1 && where == v->last);
    // move to the big buffer with room
    long n = v->last - v->first;
    for (long i = 0; i < n; ++i) g_storage[i] = v->first[i];
    g_storage[n] = value;
    v->first = g_storage; v->last = g_storage + n + 1; v->end = g_storage + 64;
}
int main()
{
    static unsigned long stamps[16];
    static unsigned short tris[6] = { 3, 5, 3, 9, 12, 5 };
    g_PhysicsFacesTriangleStamps = stamps; g_PhysicsFacesQueryStamp = 7;
    stamps[9] = 7;                                 // already stamped in this query
    CPhysicsFacesSegmentLeaf leaf;
    leaf.TriangleListIndexFirst = tris; leaf.TriangleListIndexLast = tris + 6; leaf.TriangleListIndexEnd = tris + 6;
    static long small[2];
    CLongVector faces; faces.first = small; faces.last = small; faces.end = small + 2;
    C3DVector a = { 0, 0, 0 };
    leaf.GetPossibleCollisionSet(1.0f, a, a, faces);
    CHECK(faces.last - faces.first == 3);
    CHECK(faces.first[0] == 3 && faces.first[1] == 5 && faces.first[2] == 12);
    CHECK(g_grows == 1 && faces.first == g_storage);
    CHECK(stamps[3] == 7 && stamps[5] == 7 && stamps[12] == 7 && stamps[9] == 7 && stamps[0] == 0);
    // same query again: everything already stamped, nothing added
    leaf.GetPossibleCollisionSet(1.0f, a, a, faces);
    CHECK(faces.last - faces.first == 3);
    // a new query stamp: all four distinct triangles (in order of first appearance) are added again
    g_PhysicsFacesQueryStamp = 8;
    leaf.GetPossibleCollisionSet(1.0f, a, a, faces);
    CHECK(faces.last - faces.first == 7 && faces.first[3] == 3 && faces.first[4] == 5 && faces.first[5] == 9 && faces.first[6] == 12);
    // empty leaf: nothing
    leaf.TriangleListIndexLast = tris;
    g_PhysicsFacesQueryStamp = 9; leaf.GetPossibleCollisionSet(1.0f, a, a, faces);
    CHECK(faces.last - faces.first == 7);
    if (fails == 0) printf("00a86e20_TEST PASS\n");
    return fails;
}
