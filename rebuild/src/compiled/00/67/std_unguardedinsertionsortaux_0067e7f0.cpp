// std::__unguarded_insertion_sort_aux(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo*, CIsHealth1GTHealth2 comp) -- STLport unguarded insertion pass over CDegradableDef's degradation stages (descending HealthPercentage), the linear insert inline
// Leaf: ??$__unguarded_insertion_sort_aux@PAVCDegradableInfo@@
//
// Retail 0x0067E7F0, 86 bytes, `ret 8` (/Gr: first in ecx, last in edx; the _Tp* tag and the
//   empty comparator on the stack); the 28-byte value copy lives in the `sub esp, 1Ch` frame.
// Callers: 0x0067EA27 / 0x0067F11A in the sort of vector<CDegradableInfo> (the __final_insertion_sort tail; the landed
//   out-of-line __unguarded_linear_insert 0x0067E550 names the element and CDegradableDef::CIsHealth1GTHealth2).
// Layout: Ego_r.pdb CDegradableInfo (28 bytes) = { float HealthPercentage; CEngineGraphic EngineGraphic (+4, 14 packed
//   bytes); long SmashParticleEmitter (+0x14); bool BlocksNavigation (+0x18); }; no engine header exists.
// Shape: STLport _algo.c `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with the
//   linear insert (`next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`)
//   inline (__forceinline); the greater-than comparator is `fld val ; fcomp [next] ; test ah,41h ; jne`, every element
//   copy a 7-dword `rep movsd`.  /Gr TU written as explicit __fastcall.
// Label: catalog "_Copy_impl<CActionInputControl*,CActionInputControl*>" is propagated.
#pragma pack(push, 1)
class CEngineGraphic {                                      // 14 bytes (Ego_r.pdb)
public:
    unsigned long BankIndex;                                // +0x00
    float         AnimStep;                                 // +0x04
    float         RenderSizeX;                              // +0x08
    bool          AdditiveAlpha;                            // +0x0c
    unsigned char Type;                                     // +0x0d
};
#pragma pack(pop)

class CDegradableInfo {                                     // 28 bytes
public:
    float          HealthPercentage;                        // +0x00
    CEngineGraphic EngineGraphic;                           // +0x04
    long           SmashParticleEmitter;                    // +0x14
    bool           BlocksNavigation;                        // +0x18
};

class CDegradableDef {
public:
    class CIsHealth1GTHealth2 {                             // stages sorted by descending health
    public:
        bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const
        {
            return a.HealthPercentage > b.HealthPercentage;
        }
    };
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

template void __fastcall std::__unguarded_insertion_sort_aux<CDegradableInfo*, CDegradableInfo, CDegradableDef::CIsHealth1GTHealth2>(
    CDegradableInfo*, CDegradableInfo*, CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2);