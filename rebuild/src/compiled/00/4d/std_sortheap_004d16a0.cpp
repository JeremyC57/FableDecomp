// std::sort_heap(pair<float,CSubtitledMessage::CCameraTag>* first, pair<float,CSubtitledMessage::CCameraTag>* last, CKeyPairCompareLess<float,CSubtitledMessage::CCameraTag> comp) -- STLport heap sort of a subtitled message's camera tags (keyed by time): pop the heap top to the back until one entry remains
// Instantiation: std::sort_heap<pair<float,CSubtitledMessage::CCameraTag>*,CKeyPairCompareLess<float,CSubtitledMessage::CCameraTag> >
// Leaf: ??$sort_heap@PAU?$pair@MVCCameraTag@CSubtitledMessage@@@std@@V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@std@@YIXPAU?$pair@MVCCameraTag@CSubtitledMessage@@@0@0V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D16A0, 254 bytes, __fastcall (first ecx, last edx, the empty comparator
//   slot on the stack), `ret 4`; speed TU (28-byte count by `imul 92492493h`).
// Identity (proven): (1) STLport 4.6 _heap.h sort_heap verbatim (`while (last - first > 1) pop_heap(first, last--, comp)`
//   with pop_heap -> __pop_heap_aux -> __pop_heap inline: the popped `_Tp(*(last - 1))` local, `*result = *first` as a
//   7-dword `rep movsd`, then __adjust_heap(first, 0, last - first, value, comp)); (2) the callee 0x004D0660 is bound as
//   ??$__adjust_heap@PAU?$pair@MVCCameraTag@CSubtitledMessage@@@std@@HU12@V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@std@@...
//   (28-byte pair<float, CSubtitledMessage::CCameraTag>, Ego_r CSubtitledMessage::CameraTags
//   CVectorMap<float, CCameraTag, CKeyPairCompareLess<float, CCameraTag> >); (3) the existing binding names this address
//   with exactly this sort_heap spelling (caller 0x004D1C80, __partial_sort of the same element).
// Label: the catalog's `_Sort_heap<CDebugCone*>` is propagated (Dinkumware spelling, wrong element).
// Shape: pop_heap / __pop_heap_aux / __pop_heap are __forceinline (VC7.1 keeps the plain-inline STLport helpers out of line).
// Callee: __adjust_heap 0x004D0660 (`__fastcall`: first ecx, hole edx; len, the 28-byte value and the comparator slot on the
//   stack, `ret 24h`).
enum EConversationProtagonist { };
enum EConversationCameraPosition { };
enum EConversationCameraFocus { };
enum EConversationCameraZoomType { };
enum EConversationCameraView { };

class CSubtitledMessage {
public:
    class CCameraTag {                                      // Ego_r.pdb, 24 bytes
    public:
        EConversationProtagonist    CameraPositionPerson;   // +0x00
        EConversationCameraPosition CameraPosition;         // +0x04
        EConversationProtagonist    CameraFocusPerson;      // +0x08
        EConversationCameraFocus    CameraFocusPosition;    // +0x0C
        EConversationCameraZoomType CameraZoomType;         // +0x10
        EConversationCameraView     CameraView;             // +0x14
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

typedef CSubtitledMessage::CCameraTag CCameraTag;
typedef std::pair<float, CCameraTag> CCameraTagEntry;
typedef CKeyPairCompareLess<float, CCameraTag> CCameraTagLess;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                    // 0x004D0660

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }       // _STLP_VALUE_TYPE for pointers
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }     // _STLP_DISTANCE_TYPE for pointers

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
__forceinline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, __distance_type(__first));
}

template <class _RandomAccessIterator, class _Compare>
__forceinline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}

template void __fastcall sort_heap(CCameraTagEntry*, CCameraTagEntry*, CCameraTagLess);
}