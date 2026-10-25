// std::__partial_sort(pair<float, CSubtitledMessage::CCameraTag>* first, pair<float, CSubtitledMessage::CCameraTag>* middle, pair<float, CSubtitledMessage::CCameraTag>* last, pair<float, CSubtitledMessage::CCameraTag>*, CKeyPairCompareLess comp) -- STLport partial sort of a subtitled message's camera tags by time: heapify the prefix, pop every earlier tail tag into it, then heap-sort the prefix
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D1C80, 133 bytes, `ret 0Ch` (/Gr algorithm TU, explicit __fastcall here: first in
//   ecx, middle in edx, then last, the value-type tag pointer and the comparator slot).
// Callers: 0x004D1FAC (partial_sort front end) and the depth-limit fallback of __introsort_loop (call at 0x004D2454).
// Callees: __make_heap 0x004D0C90 (this chunk), __pop_heap 0x004D0B50 (landed, out of line for the 28-byte element),
//   sort_heap 0x004D16A0 (landed).
// Identity: CSubtitledMessage::CameraTags, CVectorMap<float, CSubtitledMessage::CCameraTag> entries; the catalog's
//   "std::introsort_Heapsort_Top_CDebugCone" is a mis-propagated name.
// Shape: STLport 4.x `__partial_sort`: make_heap inlined, but VC7.1 keeps `__pop_heap` (by-value 28-byte T) out of line,
//   so the loop body is a 28-byte argument copy plus `call 0x4D0B50`.
enum EConversationProtagonist { PROTAGONIST_NONE = 0 };
enum EConversationCameraPosition { CAMERA_POSITION_NONE = 0 };
enum EConversationCameraFocus { CAMERA_FOCUS_NONE = 0 };
enum EConversationCameraZoomType { CAMERA_ZOOM_NONE = 0 };
enum EConversationCameraView { CAMERA_VIEW_NONE = 0 };

class CSubtitledMessage {
public:
    class CCameraTag {                                  // Ego_r.pdb, 24 bytes of POD
    public:
        EConversationProtagonist    CameraPositionPerson;   // +0x00
        EConversationCameraPosition CameraPosition;         // +0x04
        EConversationProtagonist    CameraFocusPerson;      // +0x08
        EConversationCameraFocus    CameraFocusPosition;    // +0x0c
        EConversationCameraZoomType CameraZoomType;         // +0x10
        EConversationCameraView     CameraView;             // +0x14
    };
};

namespace std {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<float, CSubtitledMessage::CCameraTag> TCameraPair;      // 28 bytes
typedef CKeyPairCompareLess<float, CSubtitledMessage::CCameraTag> TCameraLess;

namespace std {
template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*);   // 0x004D0C90
template <class _RandomAccessIter, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __result,
                           _Tp __val, _Compare __comp, _Distance*);   // 0x004D0B50 (landed)
template <class _RandomAccessIter, class _Compare>
void __fastcall sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x004D16A0 (landed)

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIter, class _Compare>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort<TCameraPair*, TCameraPair, TCameraLess>(TCameraPair*, TCameraPair*, TCameraPair*, TCameraPair*, TCameraLess);
}