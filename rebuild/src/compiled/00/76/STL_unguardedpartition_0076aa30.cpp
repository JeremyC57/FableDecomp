// _STL::__unguarded_partition(pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* first, pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* last, pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > pivot, CParticleUpdateCCompareParamDesc<unsigned long> comp) -- Hoare partition of particle-update descriptors around pivot's key, return the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076AA30, 148 bytes, `ret 14h`.
// Caller: 0x0076C10B (the introsort loop 0x0076C080 of the same instantiation).
// Callee: CBase::~CBase 0x0099A2E0 (the swap temporary after every swap, then the by-value pivot on return).
// Element: the landed siblings' 16-byte pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<ul,ul> > (0x00769EA0
//   __copy_backward, 0x00769E20 __push_heap, 0x0076A4E0 __adjust_heap, 0x0076BB70 __insertion_sort): the descriptor is
//   polymorphic (vtable 0x0125FEE4 at +4 of the pair), so the implicit operator= skips that word, the implicit copy
//   constructor stores the vtable immediate, and every by-value or local copy is destroyed through the
//   compiler-generated destructor, which only calls CBase::~CBase 0x0099A2E0. Key compares are UNSIGNED.
// Shape: STLport `__unguarded_partition(first, last, pivot, comp)`, /Gr (__fastcall: first in ecx, last in edx, pivot
//   and comparator on the stack), both scans rotated; `iter_swap(first, last)` is inlined with a real frame
//   temporary (`_Tp tmp = *a; *a = *b; *b = tmp;`, destroyed at the end of each swap).
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

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template SKeyedDesc* __fastcall __unguarded_partition<SKeyedDesc*, SKeyedDesc, SKeyedDescLess>(SKeyedDesc*, SKeyedDesc*, SKeyedDesc, SKeyedDescLess);

}