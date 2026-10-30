// std::vector<CTVertexWaterBackground>::_M_insert_overflow(CTVertexWaterBackground* __position, const CTVertexWaterBackground& __x, const __false_type&, unsigned int __fill_len, bool __atend) -- STLport growth insert for 56-byte water background vertices: allocate size + max(size, fill_len) elements, copy the head, x (once, or fill_len times), then the tail unless inserting at the end, free the old block through _M_clear and adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCTVertexWaterBackground@@V?$allocator@VCTVertexWaterBackground@@@std@@@std@@IAEXPAVCTVertexWaterBackground@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B7FFC0, 302 bytes, thiscall, `ret 14h`.
// Identity (proven, two sources): (1) its callers are the landed CArray<CTVertexWaterBackground>::push_back 0x00BE4F90
//   (Ego_r.pdb ?push_back@?$CArray@VCTVertexWaterBackground@@@@: `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)`)
//   and the vector's _M_fill_insert at 0x00B7FE..(the `push 0 ; push n ; lea tag ; push x ; push pos` call at 0x00B7FFA7);
//   (2) the 56-byte stride (`imul 92492493h` = /56, `imul eax,eax,38h`, 14-dword `rep movsd`) is Ego_r.pdb
//   CTVertexWaterBackground (56 bytes); (3) the body is STLport 4.6 _vector.c `_M_insert_overflow(..., const __false_type&,
//   ...)` verbatim. CArray<T> derives from std::vector<T> (Ego_r), so the overflow member is the vector's (protected, IAE);
//   the landed binding `?_M_insert_overflow@?$CArray@VCTVertexWaterBackground@@@@QAEX...` is a stand-in spelling
//   (NAME_FIXES.tsv).
// Label: the manifest's "Vector_PushBack_CActiveFile_14Elements" is descriptive.
// Callees: malloc 0x00BFEA0E (STLport allocator::allocate through __malloc_alloc), vector::_M_clear 0x00B800F0 (out of
//   line: `if (_M_start) free(_M_start)`, landed as the placeholder CFirstMemberOwner_00b800f0::ReleaseFirstMember_00b800f0;
//   NAME_FIXES.tsv).
// Shape: everything but _M_clear is inline (__uninitialized_copy / __uninitialized_fill_n / _Construct each a
//   placement-new loop with the null test, max() through references); _M_clear stays an out-of-line protected member.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CTVertexWaterBackground {                    // Ego_r.pdb, 56 bytes (CTVertexWaterBackgroundBase layout)
public:
    short X;                                       // +0x00
    short Y;                                       // +0x02
    float Z;                                       // +0x04
    float ShoreLookUpArray[12];                    // +0x08
};

namespace std {

struct __false_type { };

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

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
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    __forceinline size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                                                   // 0x00B800F0
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

template void vector<CTVertexWaterBackground>::_M_insert_overflow(CTVertexWaterBackground*, const CTVertexWaterBackground&, const __false_type&, unsigned int, bool);

} // namespace std