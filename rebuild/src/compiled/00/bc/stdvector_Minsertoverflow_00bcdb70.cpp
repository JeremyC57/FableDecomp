// std::vector::_M_insert_overflow(CArray<C3DVector>* __position, const CArray<C3DVector>& __x, const __false_type&, unsigned int __fill_len, bool __atend) -- (vector<CArray<C3DVector> >) STLport 4.6 growth insert: allocate old + max(old, n) elements, copy-construct the prefix, one or n copies of x and (unless at the end) the suffix, then free the old block and adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@V?$CArray@VC3DVector@@@@V?$allocator@V?$CArray@VC3DVector@@@@@std@@@std@@IAEXPAV?$CArray@VC3DVector@@@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCDB70, length 263 bytes, thiscall, `ret 14h`.
// Identity: its only caller (0x00BCDDC7) is 0x00BCDCA0, vector<CArray<C3DVector> >::_M_fill_insert (builds x_copy with the
//   CArray<C3DVector> copy, calls __uninitialized_copy 0x00BCD8A0 / __copy_backward 0x00BCD850 / __uninitialized_fill_n
//   0x00BCD920), reached from the resize 0x00BCDE70 that runs on Ego_r.pdb CEngineInternalPrimitiveWeaponTrail::Trail
//   (CArray<CArray<C3DVector> >), the same chain the landed erase 0x00BCDB10 proves; the 12-byte element's copy is the
//   vector<C3DVector> copy constructor 0x006F0810 (landed). The catalog label "std_vector_realloc_with_fill_12byte" is
//   descriptive. Body: STLport 4.6 vector<_Tp>::_M_insert_overflow(__false_type) verbatim (no _STLP_TRY: retail has no EH here).
// Callees: malloc 0x00BFEA0E, vector<C3DVector>::vector(const vector&) 0x006F0810 (the implicit CArray copy inlines to it),
//   __uninitialized_copy<CArray<C3DVector>*, CArray<C3DVector>*> 0x00BCD8A0 (`__fastcall`, the suffix copy),
//   vector<CArray<C3DVector> >::_M_clear 0x00BCDA90 (landed under the invented CRetailVector_SBufferEntry12::~ name; NAME_FIXES).
// Shape: the landed STLport model of 0x00BBD7F0 with plain `inline` __uninitialized_copy / __uninitialized_fill_n /
//   _Construct: VC7.1 then inlines the prefix copy and the fill loop and calls the suffix copy out of line, exactly as
//   retail does (chunk 575 saw the same split); `_M_clear` is a declared out-of-line member; max() through references
//   (`lea ecx,[esp+24h] ; jb ; lea ecx,[esp+0Ch]`); the __false_type temporary for the suffix copy is homed in the dead
//   __atend slot. CArray<T> is the retail engine wrapper: an STLport vector<T> (12 bytes), as in the landed erase 0x00BCDB10.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector { public: float X, Y, Z; };

namespace std {

struct __true_type { };
struct __false_type { __false_type() { } };

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
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
    vector(const vector<_Tp, _Alloc>& __x);                 // vector<C3DVector>: 0x006F0810
    __forceinline size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                        // vector<CArray<C3DVector> >: 0x00BCDA90
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

} // namespace std

template <class T> class CArray : public std::vector<T> { };    // retail CArray: an STLport vector (12 bytes)

template void std::vector<CArray<C3DVector> >::_M_insert_overflow(CArray<C3DVector>*, const CArray<C3DVector>&, const std::__false_type&, unsigned int, bool);