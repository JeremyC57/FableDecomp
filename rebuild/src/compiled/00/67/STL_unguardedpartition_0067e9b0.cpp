// _STL::__unguarded_partition(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo pivot, CDegradableDef::CIsHealth1GTHealth2 comp) -- STLport quicksort partition of CDegradableDef's degradation stages around a pivot (descending HealthPercentage), swapping out-of-place pairs
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067E9B0, length 99 bytes, `ret 20h` (the 28-byte pivot + the comparator).
// Callers: __introsort_loop 0x0067F340 of CDegradableDef's vector<CDegradableInfo> (the sort beside the landed
//   __partial_sort 0x0067F150, __unguarded_insertion_sort_aux 0x0067E7F0, __unguarded_linear_insert 0x0067E550).
// Callees: none (comparator, iter_swap and the element copies inline).
// Label: catalog "std::partition_pivot" is descriptive; STLport spelling __unguarded_partition.
// Layout: Ego_r.pdb CDegradableInfo (28 bytes) = { float HealthPercentage; CEngineGraphic EngineGraphic (+4, 14 packed
//   bytes); long SmashParticleEmitter (+0x14); bool BlocksNavigation (+0x18); } -- the landed siblings' model.
// Shape: STLport `_algo.c` verbatim (`for (;;) { while (__comp(*__first, __pivot)) ++__first; --__last; while
//   (__comp(__pivot, *__last)) --__last; if (!(__first < __last)) return __first; iter_swap(__first, __last);
//   ++__first; }`): the greater-than comparator is `fld a ; fcomp b ; test ah,41h`, iter_swap's `_Tp __tmp = *__a;
//   *__a = *__b; *__b = __tmp;` three 7-dword `rep movsd` through the 28-byte frame temp. /Gr algorithm TU
//   (`__fastcall` template: first in ecx, last in edx, the pivot by value and the empty comparator on the stack).
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
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
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
}

template CDegradableInfo* __fastcall _STL::__unguarded_partition<CDegradableInfo*, CDegradableInfo, CDegradableDef::CIsHealth1GTHealth2>(CDegradableInfo*, CDegradableInfo*, CDegradableInfo, CDegradableDef::CIsHealth1GTHealth2);