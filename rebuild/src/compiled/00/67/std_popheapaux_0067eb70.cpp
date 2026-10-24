// std::__pop_heap_aux(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo*, CIsHealth1GTHealth2 comp) -- STLport pop_heap core over CDegradableDef's degradation stages: move the top to the back and sift the old back element down from the top
// Leaf: ??$__pop_heap_aux@PAVCDegradableInfo@@
//
// Retail 0x0067EB70, 101 bytes (+ int3 padding to 112), `ret 8` (/Gr: first in ecx, last in edx; the _Tp* tag and the
//   empty comparator on the stack); the 28-byte value copy lives in the `sub esp, 1Ch` frame.
// Callee: std::__adjust_heap<CDegradableInfo*, int, CDegradableInfo, CIsHealth1GTHealth2> @ 0x0067E8F0 (`ret 24h`).
// Caller: 0x0067EDD7 in the pop_heap(first, last, comp) front end 0x0067EDD0 (CDegradableDef's vector<CDegradableInfo>).
// Layout: Ego_r.pdb CDegradableInfo (28 bytes) = { float HealthPercentage; CEngineGraphic EngineGraphic (+4, 14 packed
//   bytes); long SmashParticleEmitter (+0x14); bool BlocksNavigation (+0x18); }; no engine header exists.
// Shape: STLport _heap.h `__pop_heap(first, last - 1, last - 1, _Tp(*(last - 1)), comp, _Distance*)` with __pop_heap
//   (`*result = *first; __adjust_heap(first, _Distance(0), _Distance(last - first), value, comp);`) inline
//   (__forceinline; the stand-alone `ret 28h` __pop_heap at 0x0067EA60 is unreferenced): value copied first, then *first -> *result, then the
//   by-value re-push and the /28 length.  /Gr TU written as explicit __fastcall.
// Label: catalog "_Pop_heap_0<CDebugSphere*,CDebugSphere>" is a propagated Dinkumware label.
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                 // 0x0067E8F0

template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                              _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, __distance_type(__first));
}
}

template void __fastcall std::__pop_heap_aux<CDegradableInfo*, CDegradableInfo, CDegradableDef::CIsHealth1GTHealth2>(
    CDegradableInfo*, CDegradableInfo*, CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2);