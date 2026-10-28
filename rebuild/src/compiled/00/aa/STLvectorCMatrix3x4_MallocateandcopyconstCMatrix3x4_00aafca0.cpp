// std::vector<CMatrix3x4>::_M_allocate_and_copy<const CMatrix3x4*>(unsigned int n, const CMatrix3x4* first, const CMatrix3x4* last) -- STLport growth helper: allocate n 48-byte matrices and copy-construct [first, last) into the block, return it
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAFCA0, length 78 bytes, thiscall `ret 0Ch` (`this` is never read: the
//   allocator is empty).
// Callees: malloc @ 0x00BFEA0E (allocator::allocate).
// Identity: the only caller (0x00AAFDD7) is vector<CMatrix3x4>::reserve 0x00AAFD90, which the landed
//   C3DAnimFileSequenceChunk::ReadFromFile 0x00AAF850 calls as its Matrices array's Reserve (48-byte CMatrix3x4
//   elements, `lea eax,[eax+eax*2] ; shl eax,4`). The manifest's "Array_Allocate_0x30Bytes" is descriptive.
// Shape: the landed STLport `_M_allocate_and_copy` (0x0075B800): `result = allocate(n)` then the inlined
//   `__uninitialized_copy(first, last, result, __false_type())` -- placement-new null test per element, the POD matrix
//   copied as a 12-dword `rep movsd` block, the destination formed as `first + (result - first)`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CMatrix3x4 {                                          // 48 bytes: three rows of four floats
public:
    float m_row[3][4];
};

namespace std {

struct __false_type { __false_type() { } };

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
};

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
inline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    protected:
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    public:
};

}   // namespace std

template CMatrix3x4* std::vector<CMatrix3x4>::_M_allocate_and_copy<const CMatrix3x4*>(
    unsigned int n, const CMatrix3x4* first, const CMatrix3x4* last);