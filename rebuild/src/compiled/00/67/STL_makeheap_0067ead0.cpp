// std::__make_heap(CDegradableInfo* first, CDegradableInfo* last, CDegradableDef::CIsHealth1GTHealth2 comp, CDegradableInfo*, int*) -- STLport make_heap core over CDegradableDef's degradation stages: sift every parent down from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067EAD0, length 155 bytes (+ int3 padding), `ret 0Ch`.
// Callers: the make_heap forwarders of partial_sort 0x0067EDB9 and __partial_sort 0x0067F168 (landed 0x0067F150, which
//   declares this callee with the same signature).
// Callees: __adjust_heap(first, holeIndex, len, T value, comp) @ 0x0067E8F0 (landed).
// Label: catalog "std::make_heap<CDebugBox>" is propagated; the element is CDegradableInfo (28 bytes, the 0x92492493
//   divide by 28) and the comparator the landed siblings' CIsHealth1GTHealth2.
// Layout: Ego_r.pdb CDegradableInfo (28 bytes), the landed siblings' model.
// Shape: STLport 4.x `_heap.c` verbatim (`if (__last - __first < 2) return; _Distance __len = __last - __first;
//   _Distance __parent = (__len - 2)/2; for (;;) { __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)),
//   __comp); if (__parent == 0) return; __parent--; }`): the value copy is a 7-dword `rep movsd` into the argument
//   area, the source pointer strength-reduced (`sub esi,1Ch`) and spilled to the `sub esp,8` frame with first.
//   /Gr algorithm TU (`__fastcall` template: first in ecx, last in edx, the comparator and the two null tags on the stack).
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
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x0067E8F0

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}
}

template void __fastcall std::__make_heap<CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2, CDegradableInfo, int>(CDegradableInfo*, CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2, CDegradableInfo*, int*);