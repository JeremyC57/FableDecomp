// std::__introsort_loop(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo*, int depth_limit, CDegradableDef::CIsHealth1GTHealth2 comp) -- STLport introsort core over CDegradableDef's degradation stages (descending HealthPercentage): while more than 16 stages remain, heapsort when the depth budget is spent, else partition around the median-of-three and recurse on the right part
// Leaf: ??$__introsort_loop@PAVCDegradableInfo@@V1@HVCIsHealth1GTHealth2@CDegradableDef@@@std@@YIXPAVCDegradableInfo@@00HVCIsHealth1GTHealth2@CDegradableDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067F280, 283 bytes, /Gr `__fastcall` (first ecx, last edx; the _Tp* tag,
//   depth_limit and the empty comparator on the stack), `ret 0Ch`.
// Identity (proven): called by the landed std::sort<CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2> 0x0067F3D0
//   (call 0x0067F410, whose notes name this address) and by itself (0x0067F357); bound under this spelling. Ego_r.pdb sorts
//   CDegradableDef's vector<CDegradableInfo> with CIsHealth1GTHealth2 (??RCIsHealth1GTHealth2@CDegradableDef@@QBE_N...);
//   the 28-byte stride (`imul 92492493h ; sar 4`) = CDegradableInfo. The catalog's "std::IntroSort" is descriptive.
// Callees: std::__unguarded_partition<CDegradableInfo*, CDegradableInfo, CIsHealth1GTHealth2> 0x0067E9B0 (the pivot by
//   value: `rep movsd` of 7 dwords), std::__partial_sort<CDegradableInfo*, CDegradableInfo, CIsHealth1GTHealth2> 0x0067F150
//   (behind the inline partial_sort), itself.
// Shape: STLport 4.6 _algo.c verbatim; __median and the comparator (`fld a ; fcomp b ; test ah,41h` = a > b) inline.
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

class CDegradableInfo {                                     // Ego_r.pdb, 28 bytes
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

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);                           // 0x0067E9B0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);                                               // 0x0067F150

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

#define __stl_threshold 16

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop<CDegradableInfo*, CDegradableInfo, int, CDegradableDef::CIsHealth1GTHealth2>(
    CDegradableInfo*, CDegradableInfo*, CDegradableInfo*, int, CDegradableDef::CIsHealth1GTHealth2);

}