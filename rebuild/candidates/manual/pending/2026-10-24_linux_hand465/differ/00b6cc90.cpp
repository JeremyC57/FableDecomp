// _STL::vector::resize(unsigned int new_size) -- resize the pair<long, long> vector: erase the surplus tail, or append default-constructed elements up to the new size
// Grade: DIFFER(85v85) -- scheduling only: retail stores the first zero dword of the default element right after `xor eax,eax` (before `mov edi,esi ; sub edi,edx`) and forms the erase start as `lea edx,[edx+eax*8] ; push edx`; ours stores both zero dwords after the subtract and uses `lea eax ; push eax`.
// Leaf: ?resize@?$vector@U?$pair@JJ@_STL@@V?$allocator@U?$pair@JJ@_STL@@@2@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6CC90, 85 bytes, thiscall `ret 4`.
// Callers: none found (no direct call, jump or address reference; an out-of-line COMDAT the linker kept).
// Callees: vector::erase(first, last) 0x00B6C0D0 (thiscall, returns the iterator, dropped), vector::_M_fill_insert(pos, n, x)
//   0x00B6C840.
// Element: an 8-byte element whose default value is two zero dwords (`xor eax,eax` stored twice): STLport pair<long, long> with
//   its `pair() : first(_T1()), second(_T2())` constructor -- the element type is unconfirmed (no callers; descriptive).
// Label: catalog "vector<std::pair<CCharString ...>::resize" is propagated across the byte-identical-shaped resize bodies.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` with `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }` and `insert(pos, n, x)` forwarding to the out-of-line _M_fill_insert: the
//   default temporary lives in the `sub esp,8` frame, size() is recomputed after the erase/insert test.
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                          // 0x00B6C0D0
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x00B6C840
    void insert(iterator __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

template void vector<pair<long, long> >::resize(unsigned int);
}
