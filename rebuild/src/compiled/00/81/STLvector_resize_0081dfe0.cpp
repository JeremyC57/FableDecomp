// _STL::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the particle morph entries
// Leaf: ?resize@?$vector@VCEntry@CParticleMorphs@@V?$allocator@VCEntry@CParticleMorphs@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081DFE0, 68 bytes, thiscall, `ret 4`.
// Identity: vector<CParticleMorphs::CEntry> (36 bytes, Ego_r.pdb members; see 0x0081DED0 for the pair-map relative).
//   The callee 0x0081DB00 is the two-argument resize. No direct reference (xref 0). Catalog
//   "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x0081DB00.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`. Nine dword zero stores then `mov [esp+14h],-1` for
//   LocationName (+0xC) after the pushes: the constructor clears the record with an intrinsic memset and re-assigns
//   `LocationName = CDefString()`. Without a user constructor VC7.1 does not zero a non-POD value-initialisation
//   (first grade: 31v68, only the -1 store).
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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x0081DB00)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CParticleMorphs::CEntry>::resize(size_type);
}