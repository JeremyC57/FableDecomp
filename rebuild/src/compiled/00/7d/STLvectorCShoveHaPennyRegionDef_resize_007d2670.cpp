// _STL::vector<CShoveHaPennyRegionDef>::resize(size_type new_size, const CShoveHaPennyRegionDef& x) -- shrink with erase or grow with a fill insert (48-byte defs)
// Leaf: ?resize@?$vector@VCShoveHaPennyRegionDef@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D2670, 102 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity: the only caller is the one-argument resize 0x007D26E0 (a 48-byte def temporary: CResource-style base
//   constructor 0x009FBEC0, vtable 0x0124383C -- the CShoveHaPennyRegionDef vtable in rtti-vtables.tsv), called from
//   TransferVectorOfSubComponents 0x007D2740, which CTavernGameShoveHaPennyDef::Transfer (0x007D0A58) calls. The
//   divide is by 48 (0x2AAAAAAB, `sar edx,3`) = Ego_r.pdb sizeof(CShoveHaPennyRegionDef) (40-byte def base, Start,
//   Score). Catalog "vector<CVectorMap<unsigned long,...>>::resize" is propagated.
// Callees: vector<CShoveHaPennyRegionDef>::erase(iterator, iterator) @ 0x007D2260,
//   vector<CShoveHaPennyRegionDef>::_M_fill_insert @ 0x007D24F0.
// Shape: STLport 4.6 `resize(n, x)` (see 0x0052BEA0); 48 = 3*16 gives `lea edx,[esi+esi*2] ; shl edx,4`.
class CGameDefinitionBase {                   // 40-byte def base (vtable + def bookkeeping)
public:
    virtual ~CGameDefinitionBase();
    unsigned char _defData[0x24];
};

class CShoveHaPennyRegionDef : public CGameDefinitionBase {   // 48 bytes
public:
    float Start;                              // +0x28
    float Score;                              // +0x2C
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

template void vector<CShoveHaPennyRegionDef>::resize(size_type, const CShoveHaPennyRegionDef&);
}