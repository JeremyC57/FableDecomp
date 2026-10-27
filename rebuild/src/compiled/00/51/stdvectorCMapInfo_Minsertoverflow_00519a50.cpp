// std::vector<CMapInfo>::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- STLport growth insert: allocate size + max(size, fill_len) elements, copy the head, x (once, or fill_len times) and the tail unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCMapInfo@@V?$allocator@VCMapInfo@@@std@@@std@@IAEXPAVCMapInfo@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00519a50, 286 bytes, thiscall, `ret 14h` (five arguments).
// Identity (proven): the element copy 0x005110B0 is bound ??0CMapInfo@@QAE@ABV0@@Z and the stride is 72 (0x38E38E39 sar 4 = Ego_r.pdb sizeof(CMapInfo)); its tail copy / destroy callees are bound to the CMapInfo __uninitialized_copy / __destroy_aux; only caller 0x0051A6D0, the vector<CMapInfo> fill insert (L004).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14; the element copy CMapInfo::CMapInfo(const CMapInfo&) 0x005110B0 (bound ??0CMapInfo@@QAE@ABV0@@Z) (head, x and fill copies);
//   __uninitialized_copy<T*, T*>(first, last, result, const __false_type&) 0x00513830 (bound ??$__uninitialized_copy@PAVCMapInfo@@PAV1@@std@@...) (the tail, __fastcall);
//   __destroy_aux<T*>(first, last, const __false_type&) 0x00515250 (bound ??$__destroy_aux@PAVCMapInfo@@@std@@...) (_M_clear's _Destroy, __fastcall).
// Shape (family L003, landed twin 0x0051C470): STLport 4.6 vector.c verbatim (no EH): the head copy and the fill are inline
//   loops around the out-of-line element copy (placement new's null test), the tail copy stays out of line, and every
//   __false_type tag shares the live atend slot (an empty class with a user constructor); stride 72.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    vector(const vector<_Tp, _Alloc>& __x);               // out of line
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
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

class CMapInfo {                                           // Ego_r.pdb, 72 bytes (members not touched here)
public:
    unsigned char Data[0x48];                             // Area, PMap, LevelName, ... IsSea
    CMapInfo(const CMapInfo& other);                      // 0x005110B0
};
template void std::vector<CMapInfo >::_M_insert_overflow(CMapInfo*, const CMapInfo&, const std::__false_type&, unsigned int, bool);