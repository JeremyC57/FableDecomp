// std::vector::resize(unsigned int new_size) -- resize the C3DQuad2 vector: erase the surplus tail, or append default-constructed elements up to the new size
// Leaf: ?resize@?$vector@VC3DQuad2@@V?$allocator@VC3DQuad2@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE7AA0, 75 bytes, thiscall `ret 4`.
// Callers: one, 0x00AE3E5E in CParticleProgram::ReadFromFile 0x00AE3C30 on the program's RenderQuads array (retail +0xA4:
//   the PDB +0xAC less the 4 bytes each of the two 16-byte Ego_r CArrays before it).
// Callees: vector::erase(first, last) 0x00AE6430 (thiscall, returns the iterator, dropped), vector::_M_fill_insert(pos, n, x)
//   0x00AE6E60.
// Element: Ego_r.pdb C3DQuad2 (8 bytes, unsigned short VertexIndex[4]) with its empty default constructor ??0C3DQuad2@@QAE@XZ,
//   so the default value is an uninitialised 8-byte temporary (no stores).
// Label: catalog "vector<std::pair<CCharString ...>::resize" is propagated across the byte-identical-shaped resize bodies.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` with `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }` and `insert(pos, n, x)` forwarding to the out-of-line _M_fill_insert: the
//   default temporary lives in the `sub esp,8` frame, size() is recomputed after the erase/insert test.
class C3DQuad2 {                                // Ego_r.pdb, 8 bytes
public:
    unsigned short VertexIndex[4];              // +0x0
    C3DQuad2() { }
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
    iterator erase(iterator __first, iterator __last);                          // 0x00AE6430
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x00AE6E60
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

template void vector<C3DQuad2 >::resize(unsigned int);
}