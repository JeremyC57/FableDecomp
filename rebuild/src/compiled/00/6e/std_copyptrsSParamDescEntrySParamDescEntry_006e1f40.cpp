// std::__copy_ptrs<SParamDescEntry*,SParamDescEntry*>(const SParamDescEntry* first, const SParamDescEntry* last, SParamDescEntry* result, const __false_type&) -- STLport assignment copy of particle update-parameter entries (pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >): assign [first, last) onto result in order, return the advanced result
// Leaf: ??$__copy_ptrs@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E1F40, 54 bytes (+ int3 padding), `ret 8` (/Gr `__fastcall`: first in ecx, last in
//   edx, result and the empty tag reference on the stack).
// Callers: vector<pair<unsigned long, CParticleUpdateParamDesc> >::operator= @ 0x006E22F0 (call sites 0x006E2388 and
//   0x006E23B7: the shrink-or-equal and the grow-within-capacity copies), the UpdateParams vector of the landed
//   CParticleUpdateParameters::operator= 0x006E2570. The catalog's `_Move<C3DAnimationStateAlpha*,...>` is a propagated
//   Dinkumware name (the PDB C3DAnimationStateAlpha is 20 bytes; this element is 16).
// Element: 16 bytes = pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > (as the
//   landed __uninitialized_copy 0x006E1F80 of the same vector): the key at +0, the desc's vptr at +4 and its two words at
//   +8/+0xC. The implicit pair/desc assignment copies the key and the two words and never the vptr, hence the skipped +4.
// Shape: STLport `__copy_ptrs(first, last, result, __false_type) { return __copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0); }` with the counted `__copy` inlined (16-byte stride: `sar 4`); VC7.1
//   strength-reduces the source walk to `lea edx,[ecx+0Ch]` and the third store to `[esi+edx]` (esi = result - first).
namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class OutputIter, class Distance>
__forceinline OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                                const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class InputIter, class OutputIter>
OutputIter __fastcall __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

template <class T1, class T2>
struct pair {                                           // STLport pair
    T1 first;
    T2 second;
};

}

class CBase {                                           // the polymorphic root (a vptr only)
public:
    virtual void v00();
};

namespace NParticleEngine {
template <class T1, class T2>
class CParticleUpdateParamDesc : public CBase {         // 12 bytes, vtable 0x0125FEE4
public:
    virtual void v00();
    void* m_pName;                                      // +0x04
    void* m_pParam;                                     // +0x08
};
}

typedef std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > SParamDescEntry;

template SParamDescEntry* __fastcall std::__copy_ptrs<const SParamDescEntry*, SParamDescEntry*>(const SParamDescEntry* first,
    const SParamDescEntry* last, SParamDescEntry* result, const std::__false_type&);