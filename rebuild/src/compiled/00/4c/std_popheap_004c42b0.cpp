// std::pop_heap(pair<float, CSubtitledMessage::CCameraTag>* first, pair<...>* last, CKeyPairCompareLess<float, CCameraTag> comp) -- STLport pop_heap over a subtitled message's camera-tag vector map: forwards to __pop_heap_aux with the value-type tag
// Flags: /Gr
// Leaf: ??$pop_heap@PAU?$pair@MVCCameraTag@CSubtitledMessage@@@std@@V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@std@@YIXPAU?$pair@MVCCameraTag@CSubtitledMessage@@@0@0V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C42B0, 15 bytes, `ret 4` (/Gr: first in ecx, last in edx, the empty comparator
//   on the stack). An out-of-line COMDAT with no direct caller (the xref.py hit 0x0073A7BC decodes to no instruction).
// Identity: the callee 0x004C3BF0 is STLport __pop_heap_aux<pair<float,CCameraTag>*, pair<...>, CKeyPairCompareLess<...>>
//   (catalog `_Pop_heap_0<pair<float,CSubtitledMessage::CCameraTag>*,...>`): it copies *(last-1) (12-byte element:
//   float key + 8-byte CCameraTag), stores *first at last-1, and calls __adjust_heap 0x004C2C70 with hole 0 and
//   len (last-1-first)/12, `ret 8` (the null _Tp* tag and comp). This row is the pop_heap that pushes `comp` then the
//   null value-type tag (`push eax ; push 0`). The vector map is CSubtitledMessage::CameraTags
//   (CVectorMap<float, CCameraTag, CKeyPairCompareLess<float, CCameraTag>>, Ego_r.pdb); FableWin.pdb's
//   `_Pop_heap_0<pair<float,CCameraTag>*, ..., CKeyPairCompareLess<float,CCameraTag>>` is the Dinkumware spelling of
//   the same family. The element type is proven by the callee's stride and the PDB family; the STLport spelling is
//   pop_heap (as in the landed sort_heap 0x00AD4960).
class CSubtitledMessage {
public:
    class CCameraTag {                         // 8 bytes (the element is 12: float key + tag)
    public:
        unsigned long _body[2];
    };
};

template <class K, class V> class CKeyPairCompareLess {
public:
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x004C3BF0

template <class _RandomAccessIterator, class _Compare>
void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, (pair<float, CSubtitledMessage::CCameraTag>*)0, __comp);
}

template void pop_heap<pair<float, CSubtitledMessage::CCameraTag>*, CKeyPairCompareLess<float, CSubtitledMessage::CCameraTag> >(
    pair<float, CSubtitledMessage::CCameraTag>*, pair<float, CSubtitledMessage::CCameraTag>*,
    CKeyPairCompareLess<float, CSubtitledMessage::CCameraTag>);
}