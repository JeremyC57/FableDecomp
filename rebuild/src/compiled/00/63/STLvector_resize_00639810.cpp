// std::vector::resize(unsigned int new_size) -- STLport vector<NThingMap::CThingMapManager::CThingMapInfo>::resize(n): erase the surplus [begin + n, end) when shrinking, else fill-insert n - size() zeroed (map, count) records at the end
// Leaf: ?resize@?$vector@VCThingMapInfo@CThingMapManager@NThingMap@@V?$allocator@VCThingMapInfo@CThingMapManager@NThingMap@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00639810, 85 bytes, thiscall on the vector, `ret 4`.
// Identity: FableWin.pdb ?resize@?$vector@VCThingMapInfo@CThingMapManager@NThingMap@@...@@QAEXI@Z; sole caller
//   NThingMap::CThingMapManager::Init 0x00638540 (landed; `ThingMaps.resize(Width * Height)` on the CArray<CThingMapInfo>
//   at +8).
// Callees: vector<CThingMapInfo>::erase(first, last) 0x00639050 (landed as CRetailVector_pod8::erase),
//   vector<CThingMapInfo>::_M_fill_insert(pos, n, const T&) 0x00639600 (the inline insert's body).
// Layout: Ego_r.pdb NThingMap::CThingMapManager::CThingMapInfo (8 bytes): PThingMap +0, RefCount +4.
//   The leaf names the exact one-argument symbol (the inline two-argument resize is also emitted as a COMDAT).
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument
//   `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }` inline:
//   the temporary comes from CThingMapInfo's USER default constructor (FableWin.pdb ??0CThingMapInfo@...@QAE@XZ) zeroing
//   both words, so both zero stores follow the size subtraction (a value-initialised POD, as 0x0066E6D0, interleaves
//   them and forms the erase address in edx); finish is re-read for the second size().
// Label correction: catalogued as `vector<std::pair<CCharString...>::resize` (propagated); the element is CThingMapInfo.
namespace NThingMap {
class CThingMap;
class CThingMapManager {
public:
    class CThingMapInfo {                                           // 8 bytes
    public:
        CThingMap* PThingMap;                                       // +0x00
        long       RefCount;                                        // +0x04
        CThingMapInfo() : PThingMap(0), RefCount(0) { }
    };
};
}

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

// 0x00639050 erase, 0x00639600 _M_fill_insert (out of line in retail)
template void std::vector<NThingMap::CThingMapManager::CThingMapInfo>::resize(unsigned int);