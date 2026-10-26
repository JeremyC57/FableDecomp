// std::vector::resize(size_type new_size, const value_type& x) -- (vector<CArray<C3DVector> >) STLport two-argument resize: erase the surplus tail through the out-of-line range erase, or insert new_size - size() copies of x at the end through the out-of-line _M_fill_insert
// Leaf: ?resize@?$vector@V?$CArray@VC3DVector@@@@V?$allocator@V?$CArray@VC3DVector@@@@@std@@@std@@QAEXIABV?$CArray@VC3DVector@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCDE70, 98 bytes (+ int3 padding to 112), thiscall, `ret 8`.
// Identity (proven): vector<CArray<C3DVector> >::resize(n, x); evidence: its erase callee 0x00BCDB10 is the landed vector<CArray<C3DVector> >::erase(first, last); its callers are CEngineInternalPrimitiveWeaponTrail::Update 0x00BCD660 and the one-argument resize 0x00BCDEE0; the PDB name sets hold ?resize@?$vector@V?$CArray@VC3DVector@@@@....
// Label: the manifest's "resize" is propagated across byte-identical bodies (family H027: seven rows that differ only in
//   the element size (here 12 bytes: the reciprocal division of size() and `begin() + n`) and the call displacements;
//   retail has no identical-code folding, so each address is one instantiation).
// Callees: vector::erase(iterator first, iterator last) @ 0x00BCDB10 (result dropped), vector::_M_fill_insert(iterator pos,
//   size_type n, const value_type& x) @ 0x00BCDCA0 (both thiscall, out of line).
// Shape: STLport 4.6 `if (__new_size < size()) erase(begin() + __new_size, end()); else insert(end(), __new_size - size(),
//   __x);` with `insert(pos, n, x)` forwarding to _M_fill_insert (the landed resize 0x00BBDD40 model); size() is recomputed
//   on the insert branch.
class C3DVector;
template <class T> class CArray {                      // Fable's CArray<T>: an STLport vector<T>, 12 bytes in retail
public:
    T* Data[3];
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
    iterator erase(iterator __first, iterator __last);                          // 0x00BCDB10
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x00BCDCA0
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
typedef CArray<C3DVector> TElem;
template void std::vector<TElem>::resize(unsigned int, const TElem&);