// std::uninitialized_fill_n(pair<EHeroMorphType, CParticleMorphs::CEntry>* first, unsigned int n, const pair<EHeroMorphType, CParticleMorphs::CEntry>& x) -- STLport placement fill: copy-construct x into n raw slots, return the end
// Leaf: ??$uninitialized_fill_n@PAU?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@std@@IU12@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081A530, 49 bytes, /Gr `__fastcall` (first in ecx, n in edx; &x on the stack),
//   `ret 4`.
// Identity (proven): only caller the landed _M_fill_insert 0x0081CD00, which names its uninitialized_fill_n 0x0081A530 (__uninitialized_fill_n inlined into it); the landed hero-morph map rows (_M_insert_overflow 0x0081C990, _M_fill_insert 0x0081CD00) are vector<pair<EHeroMorphType, CParticleMorphs::CEntry> > (Ego_r.pdb CParticleMorphs::Morphs) and name this callee; 40-byte stride, the 36-byte entry as 9 dwords.
// Shape (family K022, representative 0x00758720): STLport 4.6 `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);
//   return __cur;` (unsigned n: `test edx,edx ; jbe`), the placement new's null test, STLport's memberwise pair copy
//   (the key word, then the trivially copyable record as one `rep movsd`); this is the tag dispatcher `return __uninitialized_fill_n(__first, __n, __x, __false_type());` with the worker inlined.
inline void* operator new(unsigned int, void* where) { return where; }
enum EHeroMorphType { };
enum EMeshLocationPoint { };
class CDefString {                                       // 4 bytes, an index into the def string table
public:
    long TablePos;
};
class CParticleMorphs {
public:
    class CEntry {                                       // Ego_r.pdb, 36 bytes, trivially copyable
    public:
        EHeroMorphType MorphType;                        // +0x00
        bool Blending;                                   // +0x04
        EMeshLocationPoint Location;                     // +0x08
        CDefString LocationName;                         // +0x0C
        long LocationIndex;                              // +0x10
        long ParticleIndex;                              // +0x14
        long SecondParticleIndex;                        // +0x18
        float StartAtMorphStrength;                      // +0x1C
        float EndAtMorphStrength;                        // +0x20
    };
};

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x00
    _T2 second;                                          // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
typedef pair<EHeroMorphType, CParticleMorphs::CEntry> _Elem;
template _Elem* __fastcall uninitialized_fill_n<_Elem*, unsigned int, _Elem>(_Elem*, unsigned int, const _Elem&);
} // namespace std