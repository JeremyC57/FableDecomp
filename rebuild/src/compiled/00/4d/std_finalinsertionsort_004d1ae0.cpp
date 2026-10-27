// std::__final_insertion_sort(std::pair<float, CSubtitledMessage::CCameraTag>* first, std::pair<float, CSubtitledMessage::CCameraTag>* last, CKeyPairCompareLess<float, CSubtitledMessage::CCameraTag> comp) -- finish an introsort: insertion-sort the first 16 elements, then the unguarded tail
// Leaf: ??$__final_insertion_sort@PAU?$pair@MVCCameraTag@CSubtitledMessage@@@std@@V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@std@@YIXPAU?$pair@MVCCameraTag@CSubtitledMessage@@@0@0V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D1AE0, 86 bytes, `__fastcall` (first ecx, last edx, the comparator's slot on the stack), `ret 4`.
// Identity (proven): its callees are the landed __insertion_sort 0x004D1530 and __unguarded_insertion_sort_aux 0x004D0460 of the 28-byte pair<float, CSubtitledMessage::CCameraTag> (the camera-tag vector map of the CSubtitledMessage TU, ascending float key: CKeyPairCompareLess); its caller is that map's std::sort 0x004D2570.
// Callees: std::__insertion_sort 0x004D1530, std::__unguarded_insertion_sort_aux 0x004D0460 (both __fastcall).
// Shape (family M093, representative 0x004D1AE0): STLport 4.6 _algo.c `if (__last - __first > __stl_threshold) {
//   __insertion_sort(__first, __first + __stl_threshold, __comp); __unguarded_insertion_sort(__first + __stl_threshold, __last,
//   __comp); } else __insertion_sort(__first, __last, __comp);` with __unguarded_insertion_sort inline (its null _Tp* tag).
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
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x004D1530
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x004D0460
template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (std::pair<float, CSubtitledMessage::CCameraTag>*)0, __comp);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template void __fastcall __final_insertion_sort<std::pair<float, CSubtitledMessage::CCameraTag>*, CKeyPairCompareLess<float, CSubtitledMessage::CCameraTag> >(std::pair<float, CSubtitledMessage::CCameraTag>*, std::pair<float, CSubtitledMessage::CCameraTag>*, CKeyPairCompareLess<float, CSubtitledMessage::CCameraTag>);
}