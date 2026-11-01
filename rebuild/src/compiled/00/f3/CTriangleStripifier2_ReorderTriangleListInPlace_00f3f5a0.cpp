// CTriangleStripifier2::ReorderTriangleListInPlace(unsigned short* indices, long numTriangles) -- copy the triangle list into an array, generate the cache-optimised strips from it and write the reordered list back over the input
// Leaf: ReorderTriangleListInPlace
// Naming wave 27 (family vl2, DECISIONS_W26 D6): CArray<T> derives from STLport std::vector<T, allocator<T> >, whose protected
//   _M_insert_overflow(.., const __true_type&, size_type unsigned int, bool) names the vector<unsigned short> 0x005135D0 instantiation as
//   its own row defines it. Was CArray<T>::InsertGrow(.., const SScalarTag&, unsigned long, bool).
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

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { // STLport 4.6 vector: first, last, end_of_storage
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    _Tp* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    _Tp& operator[](unsigned long n) const { return *(begin() + n); }
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool atEnd);   // 0x005135D0
public:
    void push_back(const _Tp& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(value);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, value, __true_type(), 1, true);
    }
};
}
template<class T> class CArray : public std::vector<T, std::allocator<T> > { public: };   // Fable's CArray over the STLport vector
void* GrowArrayStorage_005135d0(unsigned long bytes);          // allocation for the growth stand-in
#pragma auto_inline(off)
// Same-TU body of the CArray<unsigned short> growth template 0x005135D0 (retail's TU instantiated it): reads the value
// only through its const reference, never touches the empty tag, keeps the array to itself.
template <class T, class _Alloc>
__declspec(noinline) void std::vector<T, _Alloc>::_M_insert_overflow(T* where, const T& value, const __true_type& tag, unsigned int count, bool atEnd)
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