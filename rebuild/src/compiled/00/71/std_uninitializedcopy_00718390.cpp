// std::__uninitialized_copy(CSubtitledMessage::CCameraTag* first, CSubtitledMessage::CCameraTag* last, CSubtitledMessage::CCameraTag* result, const __false_type&) -- copy-construct a run of 24-byte camera tags into raw storage
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00718390, length 74 bytes (+ int3 padding). __fastcall (first ecx, last edx),
//         `ret 8` (result, then the tag reference).
// Callees: none.
// Identity: the manifest label "_Move<CTVertexLandscapeBackground*,...>" is a fold-sibling guess. Called twice by
//   vector<CCameraTag>::_M_insert_overflow 0x007196D0, whose element is pushed by CActionTalkBase::
//   InitialiseDefaultCameraTags (0x007167E0: six-enum records into the static DefaultCameraTags vector @ 0x013BAD18).
//   Ego_r.pdb CSubtitledMessage::CCameraTag = six 4-byte enums (24 bytes).
// Shape: STLport `__uninitialized_copy(first, last, result, __false_type)` verbatim: the placement `_Construct` with its
//   null test and the implicit copy as six dword moves through ebx.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };

enum EConversationProtagonist { CONVERSATION_PROTAGONIST_DUMMY };
enum EConversationCameraPosition { CONVERSATION_CAMERA_POSITION_DUMMY };
enum EConversationCameraFocus { CONVERSATION_CAMERA_FOCUS_DUMMY };
enum EConversationCameraZoomType { CONVERSATION_CAMERA_ZOOM_TYPE_DUMMY };
enum EConversationCameraView { CONVERSATION_CAMERA_VIEW_DUMMY };

class CSubtitledMessage {
public:
    class CCameraTag {                                  // Ego_r.pdb, 24 bytes
    public:
        EConversationProtagonist CameraPositionPerson;  // +0x00
        EConversationCameraPosition CameraPosition;     // +0x04
        EConversationProtagonist CameraFocusPerson;     // +0x08
        EConversationCameraFocus CameraFocusPosition;   // +0x0C
        EConversationCameraZoomType CameraZoomType;     // +0x10
        EConversationCameraView CameraView;             // +0x14
    };
};

namespace std {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CSubtitledMessage::CCameraTag* __fastcall __uninitialized_copy<CSubtitledMessage::CCameraTag*, CSubtitledMessage::CCameraTag*>(
    CSubtitledMessage::CCameraTag*, CSubtitledMessage::CCameraTag*, CSubtitledMessage::CCameraTag*, const __false_type&);
}