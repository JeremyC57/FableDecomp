// _STL::vector<CPreTransposedBoneMatrix>::resize(size_type new_size, const CPreTransposedBoneMatrix& x) -- shrink with erase or grow with a fill insert (48-byte bone matrices)
// Leaf: ?resize@?$vector@UCPreTransposedBoneMatrix@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B7EDF0, 100 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity: the 14 callers are the CEngineWaterRenderer render passes (e.g. 0x00B75824: `resize(4, m)` on
//   g_pShaderRenderManager (0x01436E14) + 0x3DC just before CShaderRenderManager::SetBoneTransform) and the
//   one-argument resize 0x00B7EE80 / CShaderRenderManager::SetBoneMatrixCount 0x00B7EEA0 (`BoneMatrices.resize(n)` at
//   +0x3DC): Ego_r.pdb CShaderRenderManager::BoneMatrices is CArray<CPreTransposedBoneMatrix>, 48 bytes (float[3][4]).
//   Divide by 48 (0x2AAAAAAB, `sar edx,3`). Catalog "vector<CVectorMap<unsigned long,...>>::resize" is propagated.
// Callees: erase(iterator, iterator) @ 0x00B7C120, _M_fill_insert @ 0x00B7CAB0.
// Shape: STLport 4.6 `resize(n, x)` (see 0x0052BEA0).
struct CPreTransposedBoneMatrix {             // 48 bytes (a struct in FableWin.pdb: U-mangled)
    float Data[3][4];
};

namespace _STL {
template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                     // out of line in retail
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);    // out of line in retail
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x) {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<CPreTransposedBoneMatrix>::resize(size_type, const CPreTransposedBoneMatrix&);
}