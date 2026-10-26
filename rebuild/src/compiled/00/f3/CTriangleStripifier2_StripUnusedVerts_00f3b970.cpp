// CTriangleStripifier2::StripUnusedVerts(CArray<unsigned short>& verts, const CArray<CArray<long> >& vertTriangles, const CArray<bool>& triangleUsed) -- keep only the vertices that belong to at least one used triangle
// Leaf: ?StripUnusedVerts@CTriangleStripifier2@@AAEXAAV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@_N@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3B970, length 244 bytes (+ int3 padding), `ret 0Ch` (`this` unused).
// Callees: the CArray<unsigned short> reallocating insert @ 0x005135D0 (catalog "Vector_InsertN_uint16"), CArray<unsigned
// short>::operator= @ 0x00F3B640 (catalog "std_vector_assign_with_realloc"), free @ 0x00BFEA14 (the kept list's storage).
// Layout: CArray = the retail STLport vector {first, last, end_of_storage} (12 bytes); CArray<bool> is a byte array.
// Shape: the kept list is a local vector (three zero registers); the vertex is an `unsigned long` local (movzx at the
// load), converted to the push_back's const unsigned short& temporary homed in the frame (the growth template gets a
// same-TU `auto_inline(off)` body that only reads its const reference, the landed push_back idiom); the inner loop
// re-indexes vertTriangles[vert] rather than naming it; every size() is recomputed per pass; `break` after the push.
#include <new>
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };
template<class T> class CArray { public:                              // retail STLport vector: first, last, end_of_storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CArray() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    CArray& operator=(const CArray& other);                    // 0x00F3B640 (unsigned short instantiation)
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
void AssignArrayRange_00f3b640(void* array, const void* first, const void* last);
// Same-TU body of CArray<unsigned short>::operator= 0x00F3B640: reads the source only through its range (the kept list's
// address does not escape, so its `last` can stay in ebx through the loop, as retail's LTCG saw).
template<class T>
__declspec(noinline) CArray<T>& CArray<T>::operator=(const CArray<T>& other)
{
    if (&other != this)
        AssignArrayRange_00f3b640(this, other._M_start, other._M_finish);
    return *this;
}
#pragma auto_inline(on)

class CTriangleStripifier2 {
public:
    void StripUnusedVerts(CArray<unsigned short>& verts, const CArray<CArray<long> >& vertTriangles, const CArray<bool>& triangleUsed);
};

void CTriangleStripifier2::StripUnusedVerts(CArray<unsigned short>& verts, const CArray<CArray<long> >& vertTriangles, const CArray<bool>& triangleUsed)
{
    CArray<unsigned short> kept;
    for (unsigned long i = 0; i < verts.size(); ++i) {
        unsigned long vert = verts[i];
        for (unsigned long j = 0; j < vertTriangles[vert].size(); ++j) {
            if (triangleUsed[vertTriangles[vert][j]]) {
                kept.push_back(vert);
                break;
            }
        }
    }
    verts = kept;
}