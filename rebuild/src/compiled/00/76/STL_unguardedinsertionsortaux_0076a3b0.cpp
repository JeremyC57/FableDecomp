// _STL::__unguarded_insertion_sort_aux(pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* first, pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* last, pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >*, CParticleUpdateCCompareParamDesc<unsigned long> comp) -- unguarded linear insertion of every particle-update descriptor of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076A3B0, 65 bytes, `ret 8`.
// Callers: 0x0076AAD7, 0x0076BF3C, 0x0076C1F9 (the sort 0x0076C1A0's inlined __final_insertion_sort).
// Callee: __unguarded_linear_insert 0x00769D00 (same instantiation, destroys its by-value argument itself).
// Element: the landed siblings' 16-byte pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<ul,ul> > (0x00769EA0
//   __copy_backward, 0x00769E20 __push_heap, 0x0076A4E0 __adjust_heap, 0x0076BB70 __insertion_sort): the descriptor is
//   polymorphic (vtable 0x0125FEE4 at +4 of the pair), so the implicit operator= skips that word, the implicit copy
//   constructor stores the vtable immediate, and every by-value or local copy is destroyed through the
//   compiler-generated destructor, which only calls CBase::~CBase 0x0099A2E0. Key compares are UNSIGNED.
// Shape: STLport `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)`, /Gr: the comparator is loaded once into
//   ebx; `_Tp(*i)` is built in the argument area (key, vtable immediate, Callback, Index).
struct CBase {
    virtual void v00();
    ~CBase();                                               // 0x0099A2E0 (restores the CBase vtable)
};

namespace NParticleEngine {

template <class T1, class T2> class CParticleUpdateParamCallback;

template <class T1, class T2> class CParticleUpdateParamDesc : public CBase {   // vtable 0x0125FEE4, 12 bytes (Ego_r.pdb)
public:
    CParticleUpdateParamCallback<T1, T2>* Callback;         // +0x04
    unsigned long                         Index;            // +0x08
};

}

namespace _STL {

template <class _T1, class _T2>
class pair {
public:
    _T1 first;                                              // +0x00 sort key
    _T2 second;                                             // +0x04 (its vptr is never assigned by operator=)
    pair(const pair& o) : first(o.first), second(o.second) { }
};

}

typedef _STL::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > SKeyedDesc;   // 16 bytes

namespace NParticleEngine {

template <class K> struct CParticleUpdateCCompareParamDesc {
    bool operator()(const SKeyedDesc& a, const SKeyedDesc& b) const { return a.first < b.first; }
};

}

typedef NParticleEngine::CParticleUpdateCCompareParamDesc<unsigned long> SKeyedDescLess;

namespace _STL {

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00769D00

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<SKeyedDesc*, SKeyedDesc, SKeyedDescLess>(SKeyedDesc*, SKeyedDesc*, SKeyedDesc*, SKeyedDescLess);

}