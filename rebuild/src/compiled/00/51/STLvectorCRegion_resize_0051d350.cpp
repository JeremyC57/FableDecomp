// _STL::vector<CRegion>::resize(size_type new_size, const CRegion& x) -- shrink with erase or grow with a fill insert (88-byte world-map regions)
// Leaf: ?resize@?$vector@VCRegion@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051D350, 99 bytes, thiscall, `ret 8`.
// Identity: CWorldMap::DeleteAllRegions (0x00507610: `Regions.erase(begin(), end()); Regions.resize(1, CRegion());`
//   on the vector at +0x2C, Ego_r.pdb CWorldMap::Regions is vector<CRegion>), LoadGameStateInternal_ParseRegions
//   (0x00507A32) and the one-argument resize 0x0051D5C0. The divide is by 88 (0x2E8BA2E9, `sar edx,4`): Ego_r
//   CRegion is 100 bytes with two vectors (16 -> 12) and one map (16 -> 12) that are shorter in retail STLport.
//   Catalog "vector<CVectorMap<unsigned long,...>>::resize" is propagated.
// Callees: vector<CRegion>::erase(iterator, iterator) @ 0x0051CC80 (the inlined __copy + ~CRegion loop, stride 0x58),
//   vector<CRegion>::_M_fill_insert @ 0x0051D0A0.
// Shape: STLport 4.6 `resize(n, x)` (see 0x0052BEA0): first/finish loaded once for the compare, the else arm
//   re-reads _M_finish.
class CCharString {
public:
    void* PStringData;
};

class C2DVector {
public:
    float X, Y;
};

class CRegion {                               // retail 88 bytes
public:
    long* Maps[3];                            // +0x00  STLport vector<long>
    long* VisMaps[3];                         // +0x0C  STLport vector<long>
    CCharString Name;                         // +0x18
    CCharString DisplayName;                  // +0x1C
    CCharString RegionDefName;                // +0x20
    void* PDef;                               // +0x24  CDefPointer<const CRegionDef>
    CCharString MiniMapGraphic;               // +0x28
    float MiniMapScale;                       // +0x2C
    C2DVector MiniMapOffset;                  // +0x30
    void* MiniMapRegionExitTextOffsets[3];    // +0x38  STLport map<CCharString, C2DVector> (12 bytes)
    C2DVector WorldMapOffset;                 // +0x44
    C2DVector NameGraphicOffset;              // +0x4C
    bool AppearOnWorldMap;                    // +0x54
    bool CreatureGenerationEnabled;           // +0x55
    bool SoundThemesEnabled;                  // +0x56
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

template void vector<CRegion>::resize(size_type, const CRegion&);
}