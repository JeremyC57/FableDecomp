// std::vector::_M_insert_overflow(CTVertexSeaBackground* position, const CTVertexSeaBackground& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport 4.6 vector<CTVertexSeaBackground> growth: allocate old+max(old,fill_len) 12-byte vertices with malloc, copy the prefix, construct one or fill_len copies of x, copy the suffix unless appending, free the old block (_M_clear) and adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCTVertexSeaBackground@@V?$allocator@VCTVertexSeaBackground@@@std@@@std@@IAEXPAVCTVertexSeaBackground@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B80400, 333 bytes, __fastcall, `ret 14h` (five arguments).
// Identity: STLport 4.6 _vector.c `vector<_Tp,_Alloc>::_M_insert_overflow(..., const __false_type&, ...)` verbatim (old size by
//   the /12 multiply, `(max)` through a reference, malloc-backed allocate, placement-new copy loops with the null test, the
//   fill_len == 1 construct, `if (!__atend)` suffix copy, out-of-line _M_clear 0x00B80550 (free 0x00BFEA14 of _M_start),
//   _M_set). T = CTVertexSeaBackground (12 bytes, Ego_r.pdb class X, Y, Z floats): the landed caller 0x00BE5950 is
//   CArray<CTVertexSeaBackground>::push_back (CWaterPatchTesselatorBase<CTVertexSeaBackground>::BridgeVertexBuffer), and
//   Ego_r.pdb lays CArray<T> out over its vector<T> base, so the growth member is the base's std::vector one (protected in
//   STLport 4.6's _vector.h: IAE). The binding `?_M_insert_overflow@?$CArray@VCTVertexSeaBackground@@...` is a wrong
//   spelling (NAME_FIXES). Manifest label "Vector_PushBack_CActiveFile_3Floats" is wrong.
// Callees: malloc 0x00BFEA0E (MSVCR71 import thunk), std::vector<CTVertexSeaBackground>::_M_clear 0x00B80550 (out of line).
#include <stdlib.h>

class CTVertexSeaBackground {                      // 12 bytes (Ego_r.pdb: float X, Y, Z), implicit copy
public:
    float X, Y, Z;                                 // +0 / +4 / +8
};

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

namespace std {
struct __false_type { };
template <class T> inline const T& max(const T& a, const T& b) { return a < b ? b : a; }
template <class T> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int count) { return count ? (T*)malloc(count * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};
template <class P, class T, class A> class _STLP_alloc_proxy : public A {
public:
    P _M_data;
    _STLP_alloc_proxy(const A& a, P p) : A(a), _M_data(p) { }
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class Input, class Forward>
__forceinline Forward __uninitialized_copy(Input first, Input last, Forward result, const __false_type&)
{
    Forward current = result;
    for (; first != last; ++first, ++current)
        _Construct(&*current, *first);
    return current;
}
template <class Forward, class Size, class T>
__forceinline Forward __uninitialized_fill_n(Forward first, Size count, const T& value, const __false_type&)
{
    Forward current = first;
    for (; count > 0; --count, ++current)
        _Construct(&*current, value);
    return current;
}
template <class T, class A> class _Vector_base {
protected:
    T* _M_start;                                   // +0
    T* _M_finish;                                  // +4
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage; // +8
public:
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef unsigned int size_type;
    typedef T* pointer;
    explicit vector(const A& a = A()) : _Vector_base<T, A>(a) { }
    size_type size() const { return this->_M_finish - this->_M_start; }
protected:
    void _M_clear();                               // 0x00B80550 (out of line in this TU's caller)
    void _M_set(pointer start, pointer finish, pointer end)
    {
        this->_M_start = start;
        this->_M_finish = finish;
        this->_M_end_of_storage._M_data = end;
    }
    void _M_insert_overflow(pointer position, const T& x, const __false_type&, size_type fill_len, bool atend = false);
};
template <class T, class A>
void vector<T, A>::_M_insert_overflow(pointer position, const T& x, const __false_type&, size_type fill_len, bool atend)
{
    const size_type old_size = size();
    const size_type len = old_size + (max)(old_size, fill_len);

    pointer new_start = this->_M_end_of_storage.allocate(len);
    pointer new_finish = new_start;
    new_finish = __uninitialized_copy(this->_M_start, position, new_start, __false_type());
    // handle insertion
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    } else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        // copy remainder
        new_finish = __uninitialized_copy(position, this->_M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}
template void vector<CTVertexSeaBackground>::_M_insert_overflow(CTVertexSeaBackground*, const CTVertexSeaBackground&,
                                                                 const __false_type&, unsigned int, bool);
}