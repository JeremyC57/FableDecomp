// std::vector<CMatrix3x4>::_M_insert_overflow(CMatrix3x4* __position, const CMatrix3x4& __x, const __false_type&, unsigned int __fill_len, bool __atend) -- STLport growth insert for 48-byte matrices: allocate size + max(size, fill_len) elements, copy the head, x (once, or fill_len times), then the tail unless inserting at the end, free the old block and adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCMatrix3x4@@V?$allocator@VCMatrix3x4@@@std@@@std@@IAEXPAVCMatrix3x4@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAFE50, 305 bytes, thiscall, `ret 14h`.
// Identity (two sources): (1) its callers are the push_back of C3DAnimFileSequenceChunk::Matrices (+0x40) inlined in the
//   landed C3DAnimFileSequenceChunk::ReadFromFile 0x00AAF850 and in 0x00AAF800 (Ego_r.pdb AddMatrix(const CMatrix3x4&),
//   donor-ordered right before ReadFromFile); Ego_r.pdb C3DAnimFileSequenceChunk::Matrices is CArray<CMatrix3x4>, which
//   derives from std::vector<CMatrix3x4> (the PDB name set holds vector<CMatrix3x4> members), and the callers push
//   `push 1 ; push 1 ; lea tag ; push x ; push finish`; (2) the 48-byte stride (`imul 2AAAAAABh ; sar edx,3` = /48,
//   `*3 <<4`, 12-dword `rep movsd`) is CMatrix3x4 (float[3][4], V-keyed); (3) the body is STLport 4.6 _vector.c
//   `_M_insert_overflow(..., const __false_type&, ...)` verbatim. Byte-identical to the
//   vector<CPreTransposedBoneMatrix> overflow 0x00B7C870 (two instantiations, no ICF in retail).
// Label: the manifest's "Vector_InsertAt" is descriptive; the landed bindings `CMatrixArray::InsertGrow` /
//   `CRetailVector_00aaf800::InsertGrow` are stand-in spellings (NAME_FIXES.tsv).
// Callees: malloc 0x00BFEA0E (allocator::allocate through __malloc_alloc), free 0x00BFEA14 (the inline _M_clear's
//   deallocate).
// Shape: as the landed CTVertexWaterBackground overflow 0x00B7FFC0, except that this TU inlines _M_clear (null test +
//   free); the element's trivial destructor makes the _Destroy loop vanish.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CMatrix3x4 {                                 // Ego_r.pdb, 48 bytes (V-keyed); copied memberwise (rep movsd)
public:
    float m_row[3][4];
};

namespace std {

struct __false_type { };

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> __forceinline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
__forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp> class allocator {
public:
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    __forceinline void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    __forceinline size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    __forceinline void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CMatrix3x4>::_M_insert_overflow(CMatrix3x4*, const CMatrix3x4&, const __false_type&, unsigned int, bool);

} // namespace std