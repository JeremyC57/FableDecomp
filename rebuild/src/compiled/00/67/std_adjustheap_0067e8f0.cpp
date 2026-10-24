// std::__adjust_heap(CDegradableInfo* first, int holeIndex, int len, CDegradableInfo value, CIsHealth1GTHealth2 comp) -- STLport heap sift-down over CDegradableDef's degradation stages (the heap orders by descending-health comparator), then the out-of-line __push_heap of value
// Leaf: ??$__adjust_heap@PAVCDegradableInfo@@
//
// Retail 0x0067E8F0, 151 bytes (+ int3 padding to 160), `ret 24h` (/Gr: first in ecx, holeIndex in edx; len, the 28-byte
//   value and the empty comparator on the stack: 4 + 0x1C + 4).
// Callee: std::__push_heap<CDegradableInfo*, int, CDegradableInfo, CIsHealth1GTHealth2> @ 0x0067E650 (landed as
//   _Push_heap; `ret 24h`).
// Callers: 0x0067EAA3 (__pop_heap 0x0067EA60), 0x0067EB25 / 0x0067EB58 (__make_heap 0x0067EAD0), 0x0067EBC7
//   (__pop_heap_aux 0x0067EB70), 0x0067F0B1 (sort_heap 0x0067F040), 0x0067F1CC (__partial_sort 0x0067F150) --
//   the heap half of the sort of CDegradableDef's vector<CDegradableInfo> (CDegradableDef::Transfer 0x0067E290).
// Layout: Ego_r.pdb CDegradableInfo (28 bytes) = { float HealthPercentage; CEngineGraphic EngineGraphic (+4, 14 packed
//   bytes); long SmashParticleEmitter (+0x14); bool BlocksNavigation (+0x18); }; no engine header exists.
// Shape: STLport _heap.c `topIndex = holeIndex; secondChild = 2 * holeIndex + 2; while (secondChild < len) {
//   if (comp(first[secondChild], first[secondChild - 1])) secondChild--; first[holeIndex] = first[secondChild];
//   holeIndex = secondChild; secondChild = 2 * (secondChild + 1); } if (secondChild == len) { first[holeIndex] =
//   first[secondChild - 1]; holeIndex = secondChild - 1; } __push_heap(first, holeIndex, topIndex, value, comp);` --
//   the element moves are 7-dword `rep movsd`, the value re-pushed by copy; topIndex lives in the `push ecx` word.
// Label: catalog "_Adjust_heap<CDebugBox*,int,CDebugBox>" is a propagated Dinkumware label.
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
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);                                   // 0x0067E650

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
}

template void __fastcall std::__adjust_heap<CDegradableInfo*, int, CDegradableInfo, CDegradableDef::CIsHealth1GTHealth2>(
    CDegradableInfo*, int, int, CDegradableInfo, CDegradableDef::CIsHealth1GTHealth2);