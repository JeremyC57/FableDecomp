// std::vector<CHeroMarriageInfo>::_M_insert_overflow(pointer pos, const CHeroMarriageInfo& x, const __false_type&, size_type fill_len, bool atend) -- STLport grow-and-insert for 24-byte marriage records
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058044A, 182 bytes, thiscall `ret 14h`, ebp frame (size TU). Called by the
//   vector's push_back (0x00580500) and range insert (0x00580928) when the capacity is short; retail keeps the
//   hero's marriages in a vector<CHeroMarriageInfo> (GFSerialiseVectorBinaryOut<CHeroMarriageInfo> 0x0057F109).
//   The catalog "Vector_InsertRangeInit" is descriptive.
// Callees: _STLP_alloc_proxy::allocate(n, 0) @ 0x0057EFF0 (out of line), __uninitialized_copy(first, last, result,
//   const __false_type&) @ 0x0057F24E, CHeroMarriageInfo::CHeroMarriageInfo(const CHeroMarriageInfo&) @ 0x0057A3A2
//   (user copy ctor, out of line: the single-element placement arm), __uninitialized_fill_n @ 0x0057F2D1,
//   free @ 0x00BFEA14.
// Shape: STLport 4.x verbatim, the same source as the 12-byte twin 0x00579A4B (this chunk): `(max)` by reference,
//   the tag temporaries in the dead `__atend` slot, inline `_M_clear` / `_M_set`; with the out-of-line copy ctor
//   VC7.1 keeps `__new_finish` in the frame ([ebp-4], the old size's word) and `__len` in edi.
// Layout: CHeroMarriageInfo is defined here (not engine/CHeroMarriageInfo.h) because the row needs its user copy
//   constructor; members and offsets as the header (WifeUID +0, MaritalHomeUID +8, State +0x10, GayMarriage +0x14).
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

enum EMarriageState { EMarriageState_Force32 = 0x7fffffff };

class CHeroMarriageInfo {
public:
    unsigned __int64 WifeUID;                       // +0x00
    unsigned __int64 MaritalHomeUID;                // +0x08
    EMarriageState State;                           // +0x10
    bool GayMarriage;                               // +0x14
    CHeroMarriageInfo(const CHeroMarriageInfo& other);                          // 0x0057A3A2
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(unsigned int __n, const void* __hint = 0);                    // 0x0057EFF0
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// Same-TU out-of-line bodies (as retail's COMDATs beside it): VC7.1 then homes the tag temporaries in the dead
// `__atend` argument slot instead of a frame byte.
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                               // 0x0057F24E
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                               // 0x0057F2D1
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
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
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CHeroMarriageInfo>::_M_insert_overflow(CHeroMarriageInfo*, const CHeroMarriageInfo&,
                                                            const __false_type&, unsigned int, bool);

}