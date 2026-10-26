// std::vector::resize(size_type new_size, const value_type& x) -- (vector<CFireHeartPatternDef >) STLport two-argument resize: erase the surplus tail through the out-of-line range erase, or insert new_size - size() copies of x at the end through the out-of-line _M_fill_insert
// Leaf: ?resize@?$vector@VCFireHeartPatternDef@@V?$allocator@VCFireHeartPatternDef@@@std@@@std@@QAEXIABVCFireHeartPatternDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F33DE0, 99 bytes (+ int3 padding to 112), thiscall, `ret 8`.
// Identity (proven): vector<CFireHeartPatternDef >::resize(n, x); evidence: its callers are the landed CPersistContext_TransferableVectorOfSubComponents<CFireHeartPatternDef>::Resize 0x00F342B0 (which builds a default CFireHeartPatternDef for it) and the landed one-argument resize 0x00F33D90; 52-byte stride; erase 0x00F33E50 (vector<CFireHeartPatternDef>::erase per the landed 0x00F342B0); the PDB name sets hold ?resize@?$vector@VCFireHeartPatternDef@@....
// Label: the manifest's "resize" is propagated across byte-identical bodies (family H022: seven rows that differ only in
//   the element size (here 52 bytes: the reciprocal division of size() and `begin() + n`) and the call displacements;
//   retail has no identical-code folding, so each address is one instantiation).
// Callees: vector::erase(iterator first, iterator last) @ 0x00F33E50 (result dropped), vector::_M_fill_insert(iterator pos,
//   size_type n, const value_type& x) @ 0x00F33EF0 (both thiscall, out of line).
// Shape: STLport 4.6 `if (__new_size < size()) erase(begin() + __new_size, end()); else insert(end(), __new_size - size(),
//   __x);` with `insert(pos, n, x)` forwarding to _M_fill_insert (the landed resize 0x00BBDD40 model); size() is recomputed
//   on the insert branch.
class CFireHeartPatternDef {                    // 52 bytes in retail
public:
    unsigned char Data[52];
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
    iterator erase(iterator __first, iterator __last);                          // 0x00F33E50
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x00F33EF0
    void insert(iterator __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};
}
typedef CFireHeartPatternDef TElem;
template void std::vector<TElem>::resize(unsigned int, const TElem&);