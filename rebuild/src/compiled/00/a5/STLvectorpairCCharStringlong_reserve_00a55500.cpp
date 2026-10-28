// std::vector<pair<CCharString, long> >::reserve(unsigned int n) -- grow the capacity to n: allocate, copy the elements across and release the old block (or just allocate when empty), keeping the size
// Leaf: ?reserve@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A55500, length 151 bytes (+9 int3 pad), `ret 4`.
// Caller: 0x00A50BFE (0x00A50BF0, which reserves the output vector to the size of the CCharString-keyed map at +0x20 /
//   count +0x24 and then copies the map's entries into it). Callees: malloc 0x00BFEA0E, 0x006FB1C0
//   __uninitialized_copy over 8-byte {CCharString, word} elements (an ICF-shared COMDAT, labelled for CFeatDef), 0x006FBE60
//   the vector's out-of-line `_M_clear()` (destroys each string, frees the block).
// Identity: catalogued as `CDefendingCombatantInfo_CCombatWheel_Reallocate` (propagated; CCombatRing is 32 bytes). The
//   element is an 8-byte pair<CCharString, 4-byte value> (the map's entries; the value type is not recoverable).
// Shape: STLport 4.x `reserve` verbatim: `if (capacity() < n) { old_size = size(); if (_M_start) { tmp =
//   _M_allocate_and_copy(n, _M_start, _M_finish); _M_clear(); } else tmp = allocate(n); _M_set(tmp, tmp + old_size,
//   tmp + n); }` with `_M_allocate_and_copy` inlined; its `__false_type()` tag temporary sits on the still-live `n` slot
//   (n is re-read after the calls), which VC7.1 does when the tag-taking callee is a same-TU body that never touches it.
extern "C" void* __cdecl malloc(unsigned int);       // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

namespace std {
struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

#pragma auto_inline(off)
// Same-TU stand-in for the STLport COMDAT 0x006FB1C0 (it never touches the tag).
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        _Construct(&*__result, *__first);
    return __result;
}
#pragma auto_inline(on)

template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return unsigned int(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    void _M_clear();                                 // 0x006FBE60
    public:
    protected:
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    _Tp* _M_allocate_and_copy(unsigned int __n, const _Tp* __first, const _Tp* __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    public:
    void reserve(unsigned int __n);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(unsigned int __n)
{
    if (capacity() < __n) {
        const unsigned int __old_size = size();
        _Tp* __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        }
        else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

template void vector<pair<CCharString, long> >::reserve(unsigned int);
}