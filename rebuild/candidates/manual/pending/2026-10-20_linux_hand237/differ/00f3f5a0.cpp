// Grade: DIFFER(249v245) -- register choice after GenerateStrips: retail keeps the reordered list's first in esi across the frees (value copy through dx, pops at the end); ours holds it in edx, copies through si and reloads after the first free
// CTriangleStripifier2::ReorderTriangleListInPlace(unsigned short* indices, long numTriangles) -- copy the triangle list into an array, generate the cache-optimised strips from it and write the reordered list back over the input
// Leaf: ReorderTriangleListInPlace
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3F5A0, length 245 bytes, `ret 8`.
// Callees: the CArray<unsigned short> reallocating insert @ 0x005135D0 (catalog "Vector_InsertN_uint16"),
// CTriangleStripifier2::GenerateStrips @ 0x00F3EA50 (input list, reordered list, strip list), free @ 0x00BFEA14 (the three
// local arrays, destroyed in reverse order).
// Layout: CArray = the retail STLport vector {first, last, end_of_storage} (12 bytes).
// Shape: the input array is filled by the inlined push_back of each index by reference (the element itself is passed to
// the grow path; the empty tag is homed in the dead `numTriangles` slot; the growth template has a same-TU
// `auto_inline(off)` body that only reads its const reference); the output arrays are declared after the fill loop
// (their zero stores follow it); both loops count down `numTriangles * 3`.
#include <new>
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };
template<class T> struct CArray {                              // retail STLport vector: first, last, end_of_storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CArray() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x005135D0
    void push_back(const T& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(value);
            ++_M_finish;
        }
        else
            InsertGrow(_M_finish, value, SScalarTag(), 1, true);
    }
};
void* GrowArrayStorage_005135d0(unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the CArray<unsigned short> growth template 0x005135D0 (retail's TU instantiated it): reads the value
// only through its const reference, never touches the empty tag, keeps the array to itself.
template<class T>
__declspec(noinline) void CArray<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    T* p = (T*)GrowArrayStorage_005135d0(count);
    if (p) {
        *p = value;
        _M_start = p;
        _M_finish = p + 1;
        _M_end_of_storage = p + count;
    }
}
#pragma auto_inline(on)

class CTriangleStripifier2 {
public:
    void GenerateStrips(const CArray<unsigned short>& triangles, CArray<unsigned short>& reordered, CArray<long>& strips);   // 0x00F3EA50
    void ReorderTriangleListInPlace(unsigned short* indices, long numTriangles);
};

void GenerateStripsFrom_00f3ea50(CTriangleStripifier2* self, const unsigned short* first, const unsigned short* last,
                                 unsigned short** outFirst, unsigned short** outLast, long** stripFirst, long** stripLast);
#pragma auto_inline(off)
// Same-TU stand-in for CTriangleStripifier2::GenerateStrips 0x00F3EA50: reads the input only through its range and fills
// the two outputs through their members (none of the three arrays' addresses escapes, as retail's LTCG saw).
__declspec(noinline) void CTriangleStripifier2::GenerateStrips(const CArray<unsigned short>& triangles, CArray<unsigned short>& reordered, CArray<long>& strips)
{
    GenerateStripsFrom_00f3ea50(this, triangles._M_start, triangles._M_finish, &reordered._M_start, &reordered._M_finish,
                                &strips._M_start, &strips._M_finish);
}
#pragma auto_inline(on)

void CTriangleStripifier2::ReorderTriangleListInPlace(unsigned short* indices, long numTriangles)
{
    CArray<unsigned short> triangles;
    long numIndices = numTriangles * 3;
    for (long i = 0; i < numIndices; ++i)
        triangles.push_back(indices[i]);
    CArray<unsigned short> reordered;
    CArray<long> strips;
    GenerateStrips(triangles, reordered, strips);
    for (long i = 0; i < numIndices; ++i)
        indices[i] = reordered[i];
}
