// _STL::__make_heap(pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* first, pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* last, CParticleUpdateCCompareParamDesc<unsigned long> comp, pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >*, int*) -- heapify [first, last) of particle-update descriptors, last parent first
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076ABE0, 97 bytes, `ret 0Ch`.
// Callers: 0x0076AFA9 (sort_heap/partial-sort path 0x0076AF10) and 0x0076BBD4 (__partial_sort 0x0076BBC0).
// Callee: the landed __adjust_heap 0x0076A4E0 (same instantiation; it destroys its by-value argument).
// Element: the landed siblings' 16-byte pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<ul,ul> > (0x00769EA0
//   __copy_backward, 0x00769E20 __push_heap, 0x0076A4E0 __adjust_heap, 0x0076BB70 __insertion_sort): the descriptor is
//   polymorphic (vtable 0x0125FEE4 at +4 of the pair), so the implicit operator= skips that word, the implicit copy
//   constructor stores the vtable immediate, and every by-value or local copy is destroyed through the
//   compiler-generated destructor, which only calls CBase::~CBase 0x0099A2E0. Key compares are UNSIGNED.
// Shape: STLport `__make_heap(first, last, comp, _Tp*, _Distance*)` -- the byte twin of the landed-shape 0x0076A480
//   except that `_Tp(first[parent])` stores the descriptor's vtable immediate instead of copying the word at +4.
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

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x0076A4E0

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}

template void __fastcall __make_heap<SKeyedDesc*, SKeyedDescLess, SKeyedDesc, int>(SKeyedDesc*, SKeyedDesc*, SKeyedDescLess, SKeyedDesc*, int*);

}