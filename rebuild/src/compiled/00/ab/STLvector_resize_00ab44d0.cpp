// std::vector::resize(unsigned int new_size) -- vector<_D3DVERTEXELEMENT9>::resize(n): erase the surplus tail, or append value-initialised (zeroed) vertex elements up to the new size
// Leaf: ?resize@?$vector@U_D3DVERTEXELEMENT9@@V?$allocator@U_D3DVERTEXELEMENT9@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB44D0, 85 bytes, thiscall `ret 4`.
// Callers: none found (no direct call, jump or address reference; an out-of-line COMDAT the linker kept).
// Callees: vector::erase(first, last) 0x00AB3FB0 (thiscall, returns the iterator, dropped), vector::_M_fill_insert(pos, n, x)
//   0x00AB4260.
// Element: the callees fix it -- the same _M_fill_insert 0x00AB4260 is called at 0x00AB3927 inside
//   CVertexShaderInput::GenerateD3DDeclaration 0x00AB3870, which sums (DataElements.size() + 1) over the 20-byte
//   DataStreams and resizes the vector at +0x10 (Ego_r.pdb CVertexShaderInput::D3DDeclaration, CArray<_D3DVERTEXELEMENT9>);
//   the landed CRetailVector_pod8::erase 0x00AB3FB0 is its erase. _D3DVERTEXELEMENT9 (d3d9types.h, 8 bytes) is a POD with
//   no constructor: `_Tp()` value-initialises it with one `xor eax,eax`, the first zero store before the size subtraction
//   and the erase start formed in edx (the 0x0066E6D0 shape); a user zeroing constructor (STLport pair) puts both stores
//   after the subtraction and the erase start in eax.
// Label correction: catalog "vector<std::pair<CCharString ...>::resize" is propagated across the byte-identical-shaped resize
//   bodies; this one is vector<_D3DVERTEXELEMENT9>::resize(n).
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` with `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }` and `insert(pos, n, x)` forwarding to the out-of-line _M_fill_insert: the
//   default temporary lives in the `sub esp,8` frame, size() is recomputed after the erase/insert test.
typedef struct _D3DVERTEXELEMENT9 {                        // d3d9types.h / Ego_r.pdb, 8 bytes, POD
    unsigned short Stream;                                  // +0x00
    unsigned short Offset;                                  // +0x02
    unsigned char  Type;                                    // +0x04
    unsigned char  Method;                                  // +0x05
    unsigned char  Usage;                                   // +0x06
    unsigned char  UsageIndex;                              // +0x07
} D3DVERTEXELEMENT9;

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
    iterator erase(iterator __first, iterator __last);                          // 0x00AB3FB0
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x00AB4260
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

template void vector<_D3DVERTEXELEMENT9>::resize(unsigned int);
}