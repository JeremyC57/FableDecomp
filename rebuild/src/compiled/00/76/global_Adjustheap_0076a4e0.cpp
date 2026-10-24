// _global::_Adjust_heap(std::pair<unsigned long,NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long> >*, long, long, pair, CParticleUpdateCCompareParamDesc<unsigned long>) -- Dinkumware heap sift-down for 16-byte (unsigned long key, particle update parameter descriptor) pairs, then _Push_heap @ 0x00769E20; destroys the by-value pair
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076a4e0, 169 bytes (+ int3 padding).
// Callees: 0x00769E20 _Push_heap (same instantiation), 0x0099A2E0 CBase::~CBase (stores the CBase
// vtable 0x01231710).  first in ecx, hole in edx (link-time __fastcall); bottom, the 16-byte pair
// by value and the empty comparator follow (`ret 18h`); VC destroys the by-value pair in the callee
// (`lea ecx,[esp+1Ch] ; call ~CBase` on value.second after the push call).
// Element: the vtable stored at +4 of the argument copy, 0x0125FEE4, is
// NParticleEngine::CParticleUpdateParamDesc's (rtti-vtables.tsv).  Ego_r.pdb gives
// CParticleUpdateParamDesc<unsigned long,unsigned long> 0x0C bytes = vptr +0, Callback +4, Index +8,
// so the element is pair<unsigned long, CParticleUpdateParamDesc<..> > (16 bytes, `shl esi,4`).
// Shape: the element assignment is the IMPLICIT operator= (key, Callback, Index: three dword moves,
// the vptr at +4 skipped); the argument copy is the implicit copy constructor (key, vtable
// immediate, two members); the compiler-generated destructor of the descriptor stores no vtable and
// just calls the out-of-line CBase destructor.  The key test is an UNSIGNED `jae`.
// Label correction: the propagated pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> (12
// bytes, no vtable) is wrong; this is the CParticleUpdateParameters heap.
namespace FableHeap_0076a4e0 {

struct CBase {
    virtual void v00();
    ~CBase();                                  // 0x0099A2E0
};

template<class T1, class T2> struct CParticleUpdateParamCallback;

template<class T1, class T2> struct CParticleUpdateParamDesc : public CBase {   // vtable 0x0125FEE4
    CParticleUpdateParamCallback<T1, T2>* Callback;   // +0x04
    unsigned long Index;                              // +0x08
};

template<class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > SKeyedDesc;

template<class K> struct CParticleUpdateCCompareParamDesc {
    bool operator()(const SKeyedDesc& a, const SKeyedDesc& b) const { return a.first < b.first; }
};

typedef CParticleUpdateCCompareParamDesc<unsigned long> SCompare;

void __fastcall _Push_heap(SKeyedDesc* first, long hole, long top, SKeyedDesc value, SCompare pred);   // 0x00769E20

void __fastcall _Adjust_heap(SKeyedDesc* first, long hole, long bottom, SKeyedDesc value, SCompare pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

}