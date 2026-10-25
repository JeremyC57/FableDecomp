// std::__insertion_sort(SKeyedDesc* first, SKeyedDesc* last, CParticleUpdateCCompareParamDesc comp) -- STLport insertion sort over 16-byte pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<ulong,ulong> > records: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076BB70, 80 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: 0x0076B5D0 __linear_insert(first, last, value, comp) (manifest "Vector_Insert_ParticleUpdateParam_Backward";
// only this body calls it; it destroys its by-value parameter with CBase::~CBase @ 0x0099A2E0).
// Element: the vtable stored at +4 of the argument copy, 0x0125FEE4, is NParticleEngine::CParticleUpdateParamDesc's
// (rtti-vtables.tsv); Ego_r.pdb gives CParticleUpdateParamDesc<unsigned long,unsigned long> 0x0C bytes = vptr +0,
// Callback +4, Index +8 -- the same model as the landed sibling heap 0x0076A4E0.
// Label: catalogued as `_Insertion_sort1<pair<long,long>*, CKeyPairCompareLess<long,long> >` (propagated); the code is
// STLport's `__insertion_sort`.
// Shape: STLport's explicit pair copy constructor builds the by-value `*i` in the argument area (`sub esp,10h ; mov
// eax,esp`): the key, the descriptor's vtable immediate (its implicit copy constructor) and its two members, read
// through a second induction pointer (`lea edi,[esi+0Ch]`, `[edi-4]`, `[edi]`).
struct CBase {
    virtual void v00();
    ~CBase();                                         // 0x0099A2E0
};

template<class T1, class T2> struct CParticleUpdateParamCallback;

template<class T1, class T2> struct CParticleUpdateParamDesc : public CBase {   // vtable 0x0125FEE4
    CParticleUpdateParamCallback<T1, T2>* Callback;   // +0x04
    unsigned long Index;                              // +0x08
};

struct SKeyedDesc {                                   // std::pair<unsigned long, CParticleUpdateParamDesc<ulong,ulong> >
    unsigned long first;                              // +0x00 sort key
    CParticleUpdateParamDesc<unsigned long, unsigned long> second;   // +0x04
    SKeyedDesc(const SKeyedDesc& o) : first(o.first), second(o.second) { }
};

struct CParticleUpdateCCompareParamDesc {             // 4-byte comparator slot passed by value
    void* m_context;
};

void __fastcall __linear_insert(SKeyedDesc* first, SKeyedDesc* last, SKeyedDesc value, CParticleUpdateCCompareParamDesc comp);   // 0x0076B5D0

void __fastcall __insertion_sort(SKeyedDesc* first, SKeyedDesc* last, CParticleUpdateCCompareParamDesc comp)
{
    if (first == last)
        return;
    for (SKeyedDesc* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}