// std::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the (EBodyLocation, CWoundMorphs::CEntry) pair vector
// Leaf: ?resize@?$vector@U?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@V?$allocator@U?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@@2@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759770, 68 bytes, thiscall, `ret 4`.
// Identity: the storage vector of CWoundMorphs::Morphs, a CVectorMap<EBodyLocation, CWoundMorphs::CEntry> (Ego_r.pdb
//   _Pair_base<EBodyLocation, CWoundMorphs::CEntry> is 44 bytes: the enum + a 40-byte entry of Location, six texture /
//   bump-map indices, FadeToScarDuration, TotalDuration, PermanentScarStrength). The callee 0x007595F0 is the
//   two-argument resize. No direct reference (xref 0). FableWin.pdb has ??0CEntry@CWoundMorphs@@QAE@XZ; the pair
//   mangles `U?$pair@`. Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x007595F0.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over `pair() : first(_T1()), second(_T2())`; the CEntry
//   constructor zeroes all ten members (the float as an integer zero), so all eleven dwords are eax stores.
enum EBodyLocation { BODY_LOCATION_NONE };
class CWoundMorphs {
public:
    class CEntry {
    public:
        EBodyLocation Location;
        long ReplacingTextureIndex;
        long ReplacingBumpMapIndex;
        long WoundTextureIndex;
        long WoundBumpMapIndex;
        long ScarTextureIndex;
        long ScarBumpMapIndex;
        long FadeToScarDuration;
        long TotalDuration;
        float PermanentScarStrength;
        CEntry() : Location(BODY_LOCATION_NONE), ReplacingTextureIndex(0), ReplacingBumpMapIndex(0), WoundTextureIndex(0),
                   WoundBumpMapIndex(0), ScarTextureIndex(0), ScarBumpMapIndex(0), FadeToScarDuration(0), TotalDuration(0),
                   PermanentScarStrength(0.0f) { }
    };
};
namespace std {
template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

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
    typedef unsigned int size_type;
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x007595F0)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<std::pair<EBodyLocation, CWoundMorphs::CEntry> >::resize(size_type);
}