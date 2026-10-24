// std::iter_swap<pair<float, CSubtitledMessage::CCameraTag>*, pair<float, CSubtitledMessage::CCameraTag>*>(a, b) -- STLport: exchange the two 28-byte camera-tag entries the pointers name, through a temporary
// Leaf: ??$iter_swap@PAU?$pair@MVCCameraTag@CSubtitledMessage@@@std@@PAU12@@std@@YIXPAU?$pair@MVCCameraTag@CSubtitledMessage@@@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CFC50, length 96 bytes (85 + int3 padding), __fastcall (/Gr TU: ecx = a,
// edx = b), plain `ret`.
// Identity: byte-identical to swap<pair<float, CCameraTag> > 0x004CF450 (chunk 372) -- the same 28-byte CameraTags element
// of CSubtitledMessage, and a second, distinct COMDAT, so a second template with the same body. STLport's
// `iter_swap(i1, i2) { __iter_swap(i1, i2, value_type(i1)); }` (used by the sort partition of the camera-tag CVectorMap)
// is that body; the iter_swap name is inferred (no callers survive), the bytes do not distinguish it from another
// same-body instance. The catalog "swap<CDebugSphere>" is propagated (CDebugSphere is 76 bytes).
// Shape: `_Tp __tmp = *a; *a = *b; *b = __tmp;` -- the STLport pair's user copy ctor builds the temporary (one dword,
// then the six CCameraTag dwords through a pointer), the two assignments are `rep movsd` (7 dwords).
enum EConversationProtagonist { ECP_0 };
enum EConversationCameraPosition { ECCP_0 };
enum EConversationCameraFocus { ECCF_0 };
enum EConversationCameraZoomType { ECCZ_0 };
enum EConversationCameraView { ECCV_0 };

class CSubtitledMessage {
public:
    class CCameraTag {                                       // Ego_r.pdb, 24 bytes
    public:
        EConversationProtagonist    CameraPositionPerson;    // +0x00
        EConversationCameraPosition CameraPosition;          // +0x04
        EConversationProtagonist    CameraFocusPerson;       // +0x08
        EConversationCameraFocus    CameraFocusPosition;     // +0x0c
        EConversationCameraZoomType CameraZoomType;          // +0x10
        EConversationCameraView     CameraView;              // +0x14
    };
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) {}
};

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
__forceinline void __iter_swap(_ForwardIter1 a, _ForwardIter2 b, _Tp*)
{
    _Tp tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class _ForwardIter1, class _ForwardIter2> void __fastcall iter_swap(_ForwardIter1 i1, _ForwardIter2 i2)
{
    __iter_swap(i1, i2, (pair<float, CSubtitledMessage::CCameraTag>*)0);
}

typedef pair<float, CSubtitledMessage::CCameraTag> _CameraTagEntry;
template void __fastcall iter_swap<_CameraTagEntry*, _CameraTagEntry*>(_CameraTagEntry*, _CameraTagEntry*);
}