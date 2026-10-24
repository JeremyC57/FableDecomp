// std::vector<ExperienceSpendInfo>::_M_insert_overflow(pointer pos, const T& x, const __false_type&, size_type fill_len, bool atend) -- STLport grow-and-insert for 12-byte spend records
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00579A4B, 191 bytes, thiscall `ret 14h`, ebp frame (size TU). Called by the
//   vector's _M_fill_insert (0x00579B3C, this chunk) when the capacity is short. The element is
//   CTCInventoryExperience::ExperienceSpendInfo (the ExperienceSpendValues* vectors; landed 0x005724CD). The
//   catalog "Vector_InsertAndRealloc" is descriptive.
// Callees: _STLP_alloc_proxy::allocate(n, 0) @ 0x00578BDE (out of line), __uninitialized_copy(first, last, result,
//   const __false_type&) @ 0x005790C3, __uninitialized_fill_n @ 0x00578C8D (this chunk), free @ 0x00BFEA14.
// Shape: STLport 4.x verbatim (as the landed basic_string twin 0x00569E30): `__len = __old_size + (max)(__old_size,
//   __fill_len)` (max by reference: the old size spilled to [ebp-4]), the single-element arm an inline placement
//   copy, the tag temporaries homed in the dead `__atend` slot ([ebp+1Bh]); `_M_clear()` inline (the element's
//   trivial destroy loop folds away, leaving `if (_M_start) free(_M_start)`), then `_M_set`. The two placement
//   loops need same-TU noinline bodies (declared only, the tags land in a frame byte [ebp-1] and the frame grows).
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CTCInventoryExperience {
public:
    class ExperienceSpendInfo {
    public:
        long currentSpend;                          // +0x0
        long initialSpend;                          // +0x4
        long levelUpSpend;                          // +0x8
    };
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
    _Tp* allocate(unsigned int __n, const void* __hint = 0);                    // 0x00578BDE
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
{                                                                               // 0x005790C3
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                               // 0x00578C8D
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

template void vector<CTCInventoryExperience::ExperienceSpendInfo>::_M_insert_overflow(
    CTCInventoryExperience::ExperienceSpendInfo*, const CTCInventoryExperience::ExperienceSpendInfo&,
    const __false_type&, unsigned int, bool);

}