// std::vector::resize(unsigned int new_size) -- resize the C2DVector vector: erase the surplus tail, or append default-constructed elements up to the new size
// Leaf: ?resize@?$vector@VC2DVector@@V?$allocator@VC2DVector@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE7B10, 75 bytes, thiscall `ret 4`.
// Callers: one, 0x00AE40A7 in CParticleProgram::ReadFromFile 0x00AE3C30 on the program's IndexedTextureCoords array (retail
//   +0xC4, the PDB +0xD4 less 0x10); its erase / fill-insert are COMDAT-folded with other 8-byte vectors' bodies.
// Callees: vector::erase(first, last) 0x00440E00 (thiscall, returns the iterator, dropped), vector::_M_fill_insert(pos, n, x)
//   0x00475ED0.
// Element: C2DVector (8 bytes, float X, Y) with its empty default constructor ??0C2DVector@@QAE@XZ, so the default value is
//   an uninitialised 8-byte temporary (no stores).
// Label: catalog "vector<std::pair<CCharString ...>::resize" is propagated across the byte-identical-shaped resize bodies.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` with `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }` and `insert(pos, n, x)` forwarding to the out-of-line _M_fill_insert: the
//   default temporary lives in the `sub esp,8` frame, size() is recomputed after the erase/insert test.
class C2DVector {                               // 8 bytes
public:
    float X;                                    // +0x0
    float Y;                                    // +0x4
    C2DVector() { }
};

namespace std {
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
    iterator erase(iterator __first, iterator __last);                          // 0x00440E00
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x00475ED0
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

template void vector<C2DVector >::resize(unsigned int);
}