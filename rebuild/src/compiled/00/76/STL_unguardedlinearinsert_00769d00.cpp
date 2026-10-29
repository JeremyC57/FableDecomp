// std::__unguarded_linear_insert(pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* last, pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > val, CParticleUpdateCCompareParamDesc<unsigned long> comp) -- shift larger-keyed descriptors up one slot until val's place is found, store val, destroy the by-value val
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769D00, 77 bytes (+int3 pad to 80), `ret 14h`.
// Callers: 0x0076A3DF (__unguarded_insertion_sort_aux 0x0076A3B0) and 0x0076B631 (__linear_insert 0x0076B5D0).
// Callee: CBaseClass::~CBaseClass 0x0099A2E0 (the by-value val is destroyed by the callee, `lea ecx,[esp+0Ch]`).
// Element: the landed siblings' 16-byte pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<ul,ul> > (0x00769EA0
//   __copy_backward, 0x00769E20 __push_heap, 0x0076A4E0 __adjust_heap, 0x0076BB70 __insertion_sort): the descriptor is
//   polymorphic (vtable 0x0125FEE4 at +4 of the pair), so the implicit operator= skips that word, the implicit copy
//   constructor stores the vtable immediate, and every by-value or local copy is destroyed through the
//   compiler-generated destructor, which only calls CBaseClass::~CBaseClass 0x0099A2E0. Key compares are UNSIGNED.
// Shape: STLport `__unguarded_linear_insert(last, val, comp)`, /Gr (__fastcall: last in ecx; val and the comparator
//   on the stack). The copy loop strength-reduces a second pointer at next + 0xC for the two payload words.
class CBaseClass {
public:
    virtual ~CBaseClass();                                               // 0x0099A2E0 (restores the CBase vtable)
};

namespace NParticleEngine {

template <class T1, class T2> class CParticleUpdateParamCallback;

template <class T1, class T2> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4, 12 bytes (Ego_r.pdb)
public:
    CParticleUpdateParamCallback<T1, T2>* Callback;         // +0x04
    unsigned long                         Index;            // +0x08
};

}

namespace std {

template <class _T1, class _T2>
class pair {
public:
    _T1 first;                                              // +0x00 sort key
    _T2 second;                                             // +0x04 (its vptr is never assigned by operator=)
    pair(const pair& o) : first(o.first), second(o.second) { }
};

}

typedef std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > SKeyedDesc;   // 16 bytes

namespace NParticleEngine {

template <class K> struct CParticleUpdateCCompareParamDesc {
    bool operator()(const SKeyedDesc& a, const SKeyedDesc& b) const { return a.first < b.first; }
};

}

typedef NParticleEngine::CParticleUpdateCCompareParamDesc<unsigned long> SKeyedDescLess;

namespace std {

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template void __fastcall __unguarded_linear_insert<SKeyedDesc*, SKeyedDesc, SKeyedDescLess>(SKeyedDesc*, SKeyedDesc, SKeyedDescLess);

}