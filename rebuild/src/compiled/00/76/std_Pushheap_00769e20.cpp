// std::_Push_heap(std::pair<unsigned long,NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long> >* first, int hole, int top, std::pair<...> val, CParticleUpdateCCompareParamDesc<unsigned long> pred) -- percolate a keyed particle update-param descriptor up a max-heap
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00769E20, 128 bytes (/O2). Callee: 0x0099A2E0 (the CBase-style base destructor
// that only restores the base vtable) on the by-value argument's descriptor.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 16-byte element with an
// UNSIGNED key compare (`cmp [edi],ebp ; jae`). The element copy skips the word at +4: the pair's
// second member is polymorphic (Ego_r.pdb CParticleUpdateParamDesc<ul,ul>, 12 bytes: vptr +0,
// Callback +4, Index +8) and the implicit operator= never assigns a vptr. The callee destroys its
// by-value argument at the end: the descriptor declares no destructor, so its compiler-generated one
// drops the vtable reset and just calls the base destructor (0x0099A2E0) on `val.second` (`lea
// ecx,[esp+18h]`). `ret 18h` = top + 16-byte pair + 4-byte empty comparator. The propagated label
// pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> is wrong: this body sits beside
// 0x00769EA0 (the landed __copy_backward over the same 16-byte particle-desc pairs).
namespace N00769e20 {
struct CBase {
    virtual ~CBase();                                     // 0x0099A2E0 (restores the base vtable)
};
namespace NParticleEngine {
template<class A, class B> struct CParticleUpdateParamCallback;
template<class A, class B> struct CParticleUpdateParamDesc : CBase {
    CParticleUpdateParamCallback<A, B>* Callback;          // +0x04
    unsigned long Index;                                  // +0x08
};
template<class A> struct CParticleUpdateCCompareParamDesc {
    template<class P> bool operator()(const P& a, const P& b) const { return a.first < b.first; }
};
}
template<class K, class V> struct pair { K first; V second; };
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > Elem;
typedef NParticleEngine::CParticleUpdateCCompareParamDesc<unsigned long> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}