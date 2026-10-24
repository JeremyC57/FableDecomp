// _global::CopyBackward_PairULongUpdateParamDesc_00769ea0(pair<unsigned long,NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long> >*, pair<..>*, pair<..>*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over 16-byte (key, update-param descriptor) pairs
// Leaf: CopyBackward_PairULongUpdateParamDesc_00769ea0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769EA0, length 68 bytes.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); count = (last - first)
// >> 4, both pointers step back one element, the moved-back result is returned.
// Element: std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > (the
// manifest label; 16 bytes). Ego_r.pdb CParticleUpdateParamDesc<ul,ul> (12 bytes) is polymorphic: vtable +0,
// CParticleUpdateParamCallback<ul,ul>* Callback +4, unsigned long Index +8 -- so the implicit pair operator= copies the
// key (+0), skips the descriptor's vtable (+4) and copies Callback (+8) and Index (+0xC), each through its own
// induction pointer (`lea esi,[eax+0Ch]`, `lea ecx,[edx+0Ch]`).
namespace NParticleEngine {
template <class A, class B> class CParticleUpdateParamCallback;

template <class A, class B> class CParticleUpdateParamDesc {      // Ego_r.pdb, 12 bytes
public:
    virtual void v00();                                         // +0x00 vtable (not copied by the implicit operator=)
    CParticleUpdateParamCallback<A, B>* Callback;               // +0x04
    unsigned long                       Index;                  // +0x08
};
}

struct PairULongUpdateParamDesc {
    unsigned long first;                                                            // +0x00
    NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> second; // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairULongUpdateParamDesc* __fastcall CopyBackward_PairULongUpdateParamDesc_00769ea0(PairULongUpdateParamDesc* first, PairULongUpdateParamDesc* last, PairULongUpdateParamDesc* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}