// Behaviour test for 00f3f5a0: src/00f3f5a0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTriangleStripifier2::ReorderTriangleListInPlace(unsigned short* indices, long numTriangles)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTriangleStripifier2::ReorderTriangleListInPlace(unsigned short* indices, long numTriangles) -- copy the triangle list into an array, generate the cache-optimised strips from it and write the reordered list back over the input
// Leaf: ReorderTriangleListInPlace
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3F5A0, length 245 bytes, `ret 8`.
// Callees: the CArray<unsigned short> reallocating insert @ 0x005135D0 (catalog "Vector_InsertN_uint16"),
// CTriangleStripifier2::GenerateStrips @ 0x00F3EA50 (input list, reordered list, strip list), free @ 0x00BFEA14 (the three
// local arrays, destroyed in reverse order).
// Layout: CArray = the retail STLport vector {first, last, end_of_storage} (12 bytes).
// Shape: the input array is filled by the inlined push_back of each index by reference (the element itself is passed to
// the grow path; the empty tag is homed in the dead `numTriangles` slot; the growth template has a same-TU
// `auto_inline(off)` growth body: copy the old elements, append, free the old block); the output arrays are declared after the fill loop
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
void* GrowArrayStorage_005135d0(unsigned long bytes);          // allocation for the growth stand-in
#pragma auto_inline(off)
// Same-TU body of the CArray<unsigned short> growth template 0x005135D0 (retail's TU instantiated it): reads the value
// only through its const reference, never touches the empty tag, keeps the array to itself.
template<class T>
__declspec(noinline) void CArray<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    unsigned long oldSize = size();
    unsigned long newCapacity = oldSize + (oldSize > count ? oldSize : count);
    T* p = (T*)GrowArrayStorage_005135d0(newCapacity * sizeof(T));
    T* out = p;
    for (T* it = _M_start; it != where; ++it)
        *out++ = *it;
    for (unsigned long i = 0; i < count; ++i)
        *out++ = value;
    if (_M_start)
        free(_M_start);
    _M_start = p;
    _M_finish = out;
    _M_end_of_storage = p + newCapacity;
}
#pragma auto_inline(on)

class CTriangleStripifier2 {
public:
    void GenerateStrips(const CArray<unsigned short>& triangles, CArray<unsigned short>& reordered, CArray<long>& strips);   // 0x00F3EA50
    void ReorderTriangleListInPlace(unsigned short* indices, long numTriangles);
};

#pragma auto_inline(off)
// Same-TU stand-in for CTriangleStripifier2::GenerateStrips 0x00F3EA50: fills the two outputs through their own
// push_back (none of the three arrays' addresses escapes). With this body VC7.1 keeps the reordered list's first word
// in esi across the frees as retail; a stand-in that hands the member ADDRESSES to an outside helper makes it re-load.
__declspec(noinline) void CTriangleStripifier2::GenerateStrips(const CArray<unsigned short>& triangles, CArray<unsigned short>& reordered, CArray<long>& strips)
{
    for (unsigned long i = 0; i < triangles.size(); ++i)
        reordered.push_back(triangles[i]);
    strips.push_back((long)triangles.size());
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

// ---- stubs and driver ----

#include <stdlib.h>
static int g_allocs = 0; static unsigned long g_lastBytes = 0;
void* GrowArrayStorage_005135d0(unsigned long bytes) { ++g_allocs; g_lastBytes = bytes; return malloc(bytes); }
int main()
{
    CTriangleStripifier2 s;
    unsigned short idx[9] = { 5, 6, 7, 8, 9, 10, 11, 12, 13 };
    s.ReorderTriangleListInPlace(idx, 3);
    for (int i = 0; i < 9; ++i) CHECK(idx[i] == 5 + i);    // the stand-in strip generator keeps the order
    CHECK(g_allocs >= 3);                                    // the input list grew several times, plus the outputs
    unsigned short one[3] = { 1, 2, 3 };
    g_allocs = 0;
    s.ReorderTriangleListInPlace(one, 1);
    CHECK(one[0] == 1); CHECK(one[1] == 2); CHECK(one[2] == 3);
    CHECK(g_allocs >= 2);
    g_allocs = 0;
    s.ReorderTriangleListInPlace(one, 0);                     // nothing to do
    CHECK(one[0] == 1); CHECK(g_allocs == 1);                 // only the strip count is pushed
    if (fails == 0) printf("00f3f5a0_TEST PASS\n"); else printf("00f3f5a0_TEST FAIL\n");
    return fails;
}