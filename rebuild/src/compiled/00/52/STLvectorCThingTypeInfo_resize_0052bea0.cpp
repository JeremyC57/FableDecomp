// std::vector<CThingTypeInfo>::resize(size_type new_size, const CThingTypeInfo& x) -- shrink with erase or grow with a fill insert
// Leaf: ?resize@?$vector@VCThingTypeInfo@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052BEA0, 98 bytes, thiscall, `ret 8`.
// Identity: CThingManager::InitThingTypes (0x00522A3D: `resize(16, CThingTypeInfo())` on the vector at +0x3C, Ego_r.pdb
//   CThingManager::ThingTypeInfo) and the one-argument resize 0x0052BF50 call it; the divide is by 12
//   (0x2AAAAAAB, `sar edx,1`) = Ego_r sizeof(CThingTypeInfo) (DefType, ThingTypeFlags, CCharString DefClassType).
//   Catalog "vector<CVectorMap<unsigned long,CRandomAppearanceMorph::CTextureMorph,...>>::resize" is propagated.
// Callees: vector<CThingTypeInfo>::erase(iterator, iterator) @ 0x0052B100, vector<CThingTypeInfo>::_M_fill_insert
//   @ 0x0052B9D0 (both thiscall, out of line in this TU).
// Shape: STLport 4.6 `resize(n, x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }`
//   with size()/begin()/end()/insert() inlined: finish in ebx and start in edi for the first size(); the else arm
//   re-reads _M_finish for its own end() and size() and forwards the reference argument to _M_fill_insert.
class CCharString {
public:
    void* PStringData;
};

class CThingTypeInfo {                        // 12 bytes
public:
    long          DefType;                    // +0x00
    unsigned long ThingTypeFlags;             // +0x04
    CCharString   DefClassType;               // +0x08
};

namespace std {
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

template void vector<CThingTypeInfo>::resize(size_type, const CThingTypeInfo&);
}