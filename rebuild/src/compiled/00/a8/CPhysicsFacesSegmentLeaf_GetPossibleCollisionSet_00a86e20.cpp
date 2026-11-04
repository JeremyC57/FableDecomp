// CPhysicsFacesSegmentLeaf::GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, std::vector<long>& faces) -- add each of the leaf's triangle indices not yet stamped in this query to the result, stamping it so shared triangles are reported once
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A86E20, length 115 bytes (+ int3 padding).
// Leaf: ?GetPossibleCollisionSet@CPhysicsFacesSegmentLeaf@@MBEXMABVC3DVector@@0AAV?$vector@JV?$allocator@J@std@@@std@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Naming wave 27 (family virt, DECISIONS_W26 D5): GetPossibleCollisionSet is a protected virtual const in both PDBs (MBE; slot 4 of
//   vtable 0x0129DEA4). Was a public non-virtual (QAE).
// Naming wave 26 (family vlong): the vector is STLport 4.6 std::vector<long, allocator<long> >; its grow-insert 0x00440990 is the
//   protected ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z (size_type unsigned int; the
//   own row's spelling, c689 queue). Was CLongVector::InsertGrow(..., const SScalarTag&, unsigned long, bool).
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
#include "engine/C3DVector.h"

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { // STLport vector<long>
public:
    _Tp* first;
    _Tp* last;
    _Tp* end;
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool noTail);   // 0x00440990
public:
    void push_back(const _Tp& value)
    {
        if (last != end) {
            new (last) _Tp(value);
            ++last;
        }
        else
            _M_insert_overflow(last, value, __true_type(), 1, true);
    }
};
}


// Same-TU stand-in for the vector<long> insert 0x00440990 (not the retail body).
void GrowInsert_00440990(std::vector<long>* v, long* where, long value, unsigned long count);
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* where, const _Tp& value, const std::__true_type& tag, unsigned int count, bool noTail)
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
protected:
    virtual void GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, std::vector<long>& faces) const;   // slot 4
};

void CPhysicsFacesSegmentLeaf::GetPossibleCollisionSet(float radius, const C3DVector& from, const C3DVector& to, std::vector<long>& faces) const
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