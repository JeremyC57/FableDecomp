// std::__make_heap(pair<float, CSubtitledMessage::CCameraTag>* first, pair<float, CSubtitledMessage::CCameraTag>* last, CKeyPairCompareLess comp, pair<float, CSubtitledMessage::CCameraTag>*, int*) -- STLport heapify of a subtitled message's camera tags by time: sift every parent down, from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D0C90, 132 bytes, `ret 0Ch` (/Gr algorithm TU, explicit __fastcall here: first in
//   ecx, last in edx, then the comparator slot and the two tag pointers).
// Callers: 0x004D10C9 (make_heap front end), the __partial_sort 0x004D1C80 (this chunk), and an absolute reference at
//   0x004CDF5C.
// Callee: __adjust_heap(first, parent, len, T(*(first + parent)), comp) 0x004D0660 (landed).
// Identity: CSubtitledMessage::CameraTags, CVectorMap<float, CSubtitledMessage::CCameraTag> entries (28 bytes); the catalog's
//   "std::make_heap_CDebugBox" is a mis-propagated name.
// Shape: STLport 4.x `__make_heap` verbatim -- len is the 28-byte stride division (magic multiply), each pass copies the
//   parent into a 28-byte argument block through the pair's copy constructor.
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
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x004D0660 (landed)

template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*)
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
template void __fastcall __make_heap<TCameraPair*, TCameraLess, TCameraPair, int>(TCameraPair*, TCameraPair*, TCameraLess, TCameraPair*, int*);
}