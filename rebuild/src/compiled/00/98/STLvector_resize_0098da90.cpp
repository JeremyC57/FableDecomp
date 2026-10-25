// _STL::vector::resize(unsigned int new_size, const C4DVector& x) -- STLport vector<C4DVector>::resize(n, x): erase the surplus [begin + n, end) when shrinking, else fill-insert n - size() copies of x at the end
// Leaf: ?resize@?$vector@VC4DVector@@V?$allocator@VC4DVector@@@_STL@@@_STL@@QAEXIABVC4DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098DA90, 68 bytes, thiscall on the vector, `ret 8`.
// Identity: sole caller CShaderRenderManager::SetVertexShaderConstantLayout 0x0098B5E0: `LightGlobals.resize(Max(n,
//   size()), C4DVector(0, 0, 0, 1))` on the CArray<C4DVector> at retail +0x194 (the temporary's 1.0f at 0x0098B810);
//   FableWin.pdb has the Dinkumware ?resize@?$vector@VC4DVector@@...@@QAEXIVC4DVector@@@Z.
// Callees: vector<C4DVector>::erase(first, last) 0x0098CF70, vector<C4DVector>::_M_fill_insert(pos, n, const T&)
//   0x0098D360 (the inline insert's body).
// Layout: Ego_r.pdb C4DVector, 16 bytes (four floats).
// Shape: STLport 4.x `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n, end()); else insert(end(),
//   n - size(), x); }` out of line: the element count is `sar 4`, the erase address `shl eax,4 ; add eax,edx`, finish
//   is re-read for the second size() and x is forwarded from its argument slot.
// Label correction: catalogued as `vector<std::pair<CCharString...>::resize` (propagated); the element is C4DVector.
class C4DVector {                                                   // 16 bytes
public:
    float X, Y, Z, W;
};

namespace _STL {
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
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};
}

// 0x0098CF70 erase, 0x0098D360 _M_fill_insert (out of line in retail)
template void _STL::vector<C4DVector>::resize(unsigned int, const C4DVector&);