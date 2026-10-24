// _STL::__uninitialized_fill_n(pair<long, CRandomAppearanceMorph::CSkeletalMorph>* first, unsigned int n, const pair<long, CRandomAppearanceMorph::CSkeletalMorph>& x, const __false_type&) -- STLport placement-copy of one skeletal-morph map entry into n raw slots; returns the end of the run
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@JVCSkeletalMorph@CRandomAppearanceMorph@@
// Retail 0x0086AC40, 0x3A bytes, `ret 8` (/Gr: first in ecx -> eax as `cur`, n in edx counted down; &x and the empty
// __false_type tag on the stack).
// Callers: 0x0086CCDE (vector<pair<long, CSkeletalMorph> >::_M_insert_overflow 0x0086CC50, the fill_len != 1 arm),
// 0x0086AD6A, 0x0086CE2D -- all in the CRandomAppearanceMorph container COMDAT run (0x0086A9D0..0x0086E8xx).
// Element: pair<long, CRandomAppearanceMorph::CSkeletalMorph> (0x10 bytes). The catalog label "fill<C3DClothQuad*,
// C3DClothQuad>" names a byte-identical instantiation that /OPT:ICF can fold here (C3DClothQuad is also 0x10); the pair
// is the type this code was emitted for: its callers are the skeletal-morph map's growth path (0x0086CC50 is reached from
// CVectorMap<long, CSkeletalMorph>::Insert 0x0086D740 and AddSkeletalMorph 0x0086BB60), and the copy is STLport pair's
// user copy constructor (key dword, then the 12-byte POD second struct-copied through `lea esi,[ecx+4] ; lea edi,[eax+4]`).
// Shape: STLport _uninitialized.h -- `cur = first; for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;`
// (unsigned size_t: `test edx,edx ; jbe`), the placement-new null test and the copy inline.
// Layout (Ego_r): CSkeletalMorph {long GroupID +0, CDefString Name +4, bool AllowVariation +8}.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CDefString
{
public:
    long TablePos;                                   // +0x00
};

class CRandomAppearanceMorph
{
public:
    class CSkeletalMorph                             // 12 bytes of POD
    {
    public:
        long       GroupID;                          // +0x00
        CDefString Name;                             // +0x04
        bool       AllowVariation;                   // +0x08
    };
};

namespace _STL {
struct __false_type { };

template <class _T1, class _T2> struct pair
{
    _T1 first;                                       // +0x00
    _T2 second;                                      // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

typedef _STL::pair<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphPair;

template TSkeletalMorphPair* __fastcall _STL::__uninitialized_fill_n<TSkeletalMorphPair*, unsigned int, TSkeletalMorphPair>(
    TSkeletalMorphPair*, unsigned int, const TSkeletalMorphPair&, const _STL::__false_type&);