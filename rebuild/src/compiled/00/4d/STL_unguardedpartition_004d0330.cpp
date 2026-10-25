// _STL::__unguarded_partition(pair<float, CSubtitledMessage::CCameraTag>* first, pair<float, CSubtitledMessage::CCameraTag>* last, pair<float, CSubtitledMessage::CCameraTag> pivot, CKeyPairCompareLess comp) -- STLport quicksort partition of a subtitled message's camera tags (time -> camera set-up) around a by-value pivot; returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D0330, 136 bytes, `ret 20h` (/Gr algorithm TU, explicit __fastcall here: first in
//   ecx, last in edx, the 28-byte pivot and the empty comparator slot on the stack). No callees.
// Caller: the matching __introsort_loop (call at 0x004D2409; not in this chunk).
// Identity: CSubtitledMessage::CameraTags, CVectorMap<float, CSubtitledMessage::CCameraTag, CKeyPairCompareLess<...> > (Ego_r.pdb:
//   CCameraTag is six 4-byte enums). The catalog's "std::introsort_Partition_7element" counts the seven dwords.
// Shape: STLport 4.x `__unguarded_partition` verbatim; in the inlined __iter_swap the temporary `_Tp __tmp = *__a` is the
//   pair copy constructor (seven unrolled dword moves into the 28-byte frame) while both assignments are the implicit
//   operator= (`mov ecx,7 ; rep movsd`). Both scans are float less-thans on the key.
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

namespace _STL {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<float, CSubtitledMessage::CCameraTag> TCameraPair;      // 28 bytes
typedef CKeyPairCompareLess<float, CSubtitledMessage::CCameraTag> TCameraLess;

namespace _STL {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
template TCameraPair* __fastcall __unguarded_partition<TCameraPair*, TCameraPair, TCameraLess>(TCameraPair*, TCameraPair*, TCameraPair, TCameraLess);
}