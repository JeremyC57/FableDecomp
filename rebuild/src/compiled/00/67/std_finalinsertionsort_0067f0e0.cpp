// std::__final_insertion_sort(CDegradableInfo* __first, CDegradableInfo* __last, CDegradableDef::CIsHealth1GTHealth2 __comp) -- STLport: insertion-sort the first 16 elements, then the unguarded pass over the rest (or a plain insertion sort of a short range)
// Leaf: ??$__final_insertion_sort@PAVCDegradableInfo@@VCIsHealth1GTHealth2@CDegradableDef@@@std@@YIXPAVCDegradableInfo@@0VCIsHealth1GTHealth2@CDegradableDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067F0E0, 86 bytes, `__fastcall` (first ecx, last edx, the empty comparator on the stack), `ret 4`.
// Identity (proven): its callees are the landed __insertion_sort 0x0067EE30 and __unguarded_insertion_sort_aux 0x0067E7F0 over CDegradableInfo with CDegradableDef::CIsHealth1GTHealth2 (descending HealthPercentage); 28-byte stride; its caller is that sort's introsort driver 0x0067F3D0.
// Callees: __insertion_sort(first, last, comp) 0x0067EE30, __unguarded_insertion_sort_aux(first, last, T*, comp) 0x0067E7F0.
// Shape (family M191, representative 0x0067F0E0): STLport 4.6 _algo.c `__final_insertion_sort` verbatim (the landed CBlockInfo
//   instance 0x00ADC080): `if (__last - __first > __stl_threshold) { __insertion_sort(__first, __first + 16, __comp);
//   __unguarded_insertion_sort(__first + 16, __last, __comp); } else __insertion_sort(__first, __last, __comp);` -- the
//   28-byte stride's `lea esi,[ecx+0x1C0]`. /Gr TU written as explicit __fastcall.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
class CEngineGraphic {                                      // Ego_r.pdb, 14 bytes (16 with padding)
public:
    unsigned long BankIndex;                              // +0x00
    float AnimStep;                                       // +0x04
    float RenderSizeX;                                    // +0x08
    bool AdditiveAlpha;                                   // +0x0C
    unsigned char Type;                                   // +0x0D
};
class CDegradableInfo {                                     // Ego_r.pdb, 28 bytes
public:
    float HealthPercentage;                               // +0x00
    CEngineGraphic EngineGraphic;                         // +0x04
    long SmashParticleEmitter;                            // +0x14
    bool BlocksNavigation;                                // +0x18
};
class CDegradableDef {
public:
    class CIsHealth1GTHealth2 {                             // stages sorted by descending health
    public:
        bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const { return a.HealthPercentage > b.HealthPercentage; }
    };
};
namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0067EE30
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                               _Compare __comp);   // 0x0067E7F0
template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (_Tp*)0, __comp);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __first, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template void __fastcall __final_insertion_sort<CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2 >(CDegradableInfo*, CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2);
}