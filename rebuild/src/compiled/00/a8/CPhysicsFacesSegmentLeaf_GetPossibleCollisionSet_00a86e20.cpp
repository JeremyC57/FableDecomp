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
struct C3DVector { float x, y, z; };

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