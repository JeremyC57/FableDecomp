// std::sort(std::pair<float, CSubtitledMessage::CCameraTag>* first, std::pair<float, CSubtitledMessage::CCameraTag>* last, CKeyPairCompareLess<float, CSubtitledMessage::CCameraTag> comp) -- introsort the range, then finish with the final insertion sort
// Leaf: ??$sort@PAU?$pair@MVCCameraTag@CSubtitledMessage@@@std@@V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@std@@YIXPAU?$pair@MVCCameraTag@CSubtitledMessage@@@0@0V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2570, 85 bytes, `__fastcall` (first ecx, last edx, the comparator's slot on the stack), `ret 4`.
// Identity (proven): its __final_insertion_sort is 0x004D1AE0 (family M093: pair<float, CSubtitledMessage::CCameraTag>, CKeyPairCompareLess); its callers are the CSubtitledMessage constructor 0x004CE550 and the camera-tag vector map's Sort 0x004D2640.
// Callees: std::__introsort_loop 0x004D2330, std::__final_insertion_sort 0x004D1AE0 (both __fastcall).
// Shape (family M094, representative 0x004D2570): STLport 4.6 _algo.c `if (__first != __last) { __introsort_loop(__first, __last,
//   _STLP_VALUE_TYPE(__first), __lg(__last - __first) * 2, __comp); __final_insertion_sort(__first, __last, __comp); }` -- __lg
//   inline (`sar 1 ; inc` until 1).
class CSubtitledMessage {
public:
    class CCameraTag {                                    // Ego_r.pdb, 24 bytes
    public:
        unsigned long Members[6];
    };
};
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
template <class Key, class Value> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};
namespace std {
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x004D2330
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x004D1AE0
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (std::pair<float, CSubtitledMessage::CCameraTag>*)0, __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<std::pair<float, CSubtitledMessage::CCameraTag>*, CKeyPairCompareLess<float, CSubtitledMessage::CCameraTag> >(std::pair<float, CSubtitledMessage::CCameraTag>*, std::pair<float, CSubtitledMessage::CCameraTag>*, CKeyPairCompareLess<float, CSubtitledMessage::CCameraTag>);
}