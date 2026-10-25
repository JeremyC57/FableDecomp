// _STL::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the (EHeroMorphType, CParticleMorphs::CEntry) pair vector
// Leaf: ?resize@?$vector@U?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@_STL@@V?$allocator@U?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@_STL@@@2@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081DED0, 77 bytes, thiscall, `ret 4`.
// Identity: the storage vector of CVectorMap<EHeroMorphType, CParticleMorphs::CEntry> (40-byte pair: the enum + the
//   36-byte CEntry of MorphType, Blending, Location, CDefString LocationName +0xC, LocationIndex, ParticleIndex,
//   SecondParticleIndex, StartAtMorphStrength, EndAtMorphStrength -- Ego_r.pdb). The callee 0x0081D980 is the
//   two-argument resize. No direct reference (xref 0). FableWin.pdb has ??0CEntry@CParticleMorphs@@QAE@XZ.
//   Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x0081D980.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over `pair() : first(_T1()), second(_T2())`. The entry is
//   dword-zero-filled and THEN its CDefString gets -1 (`mov [esp+18h],-1` after the pushes): a constructor that clears
//   the record with an intrinsic memset and re-assigns `LocationName = CDefString()`; first's zero is the late immediate.
extern "C" void* __cdecl memset(void* dst, int value, unsigned int count);
#pragma intrinsic(memset)
enum EHeroMorphType { MORPH_TYPE_NONE };
enum EMeshLocationPoint { MESH_LOCATION_NONE };
class CDefString {
public:
    long Index;
    CDefString() : Index(-1) { }
};
class CParticleMorphs {
public:
    class CEntry {
    public:
        EHeroMorphType MorphType;
        bool Blending;
        EMeshLocationPoint Location;
        CDefString LocationName;
        long LocationIndex;
        long ParticleIndex;
        long SecondParticleIndex;
        float StartAtMorphStrength;
        float EndAtMorphStrength;
        CEntry() { memset(this, 0, sizeof(*this)); LocationName = CDefString(); }
    };
};
namespace _STL {
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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x0081D980)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<_STL::pair<EHeroMorphType, CParticleMorphs::CEntry> >::resize(size_type);
}