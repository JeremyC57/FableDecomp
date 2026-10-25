// _STL::vector::resize(unsigned int new_size) -- STLport vector<CRegionMinimap::RegionMinimapCellData>::resize(n): erase the surplus [begin + n, end) when shrinking, else fill-insert n - size() zeroed (theme, light) cells at the end
// Leaf: ?resize@?$vector@URegionMinimapCellData@CRegionMinimap@@V?$allocator@URegionMinimapCellData@CRegionMinimap@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066E6D0, 85 bytes, thiscall on the vector, `ret 4`.
// Identity: FableWin.pdb ?resize@?$vector@URegionMinimapCellData@CRegionMinimap@@...@@QAEXI@Z; sole caller
//   CRegionMinimap::CreateRegionMinimap 0x0066DC50 (landed; `RegionMinimapCellList.resize(RegionMinimapSize *
//   RegionMinimapSize)`; the catalog's "CParticleSimulation::Clear" for that caller is propagated).
// Callees: vector<RegionMinimapCellData>::erase(first, last) 0x0066E210 (landed as CRetailVector_pod8::erase),
//   vector<RegionMinimapCellData>::_M_fill_insert(pos, n, const T&) 0x0066E480 (the inline insert's body).
// Layout: Ego_r.pdb struct CRegionMinimap::RegionMinimapCellData (8 bytes): eTheme RegionMinimapTheme +0, light float +4.
//   The leaf names the exact one-argument symbol (the inline two-argument resize is also emitted as a COMDAT).
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument
//   `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }` inline:
//   the cell has no constructor, so `_Tp()` value-initialises the POD (one `xor eax,eax`, the two zero stores
//   interleaved with the size subtraction, the erase address formed in edx); a user zeroing constructor (0x00639810)
//   puts both stores after the subtraction.
// Label correction: catalogued as `vector<std::pair<CCharString...>::resize` (propagated); the element is
//   CRegionMinimap::RegionMinimapCellData.
enum RegionMinimapTheme { REGION_MINIMAP_THEME_NONE = 0 };     // enumerators not needed here

class CRegionMinimap {
public:
    struct RegionMinimapCellData {                                  // 8 bytes, POD (no constructor)
        RegionMinimapTheme eTheme;                                  // +0x00
        float              light;                                   // +0x04
    };
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
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

// 0x0066E210 erase, 0x0066E480 _M_fill_insert (out of line in retail)
template void _STL::vector<CRegionMinimap::RegionMinimapCellData>::resize(unsigned int);