// std::__uninitialized_fill_n(pair<float, CSubtitledMessage::CCameraTag>* first, unsigned int n, const pair<..>& x, const __false_type&) -- copy-construct n copies of a timed camera tag into raw storage at first and return first + n
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x004CFA70, 80 bytes, `ret 8`: __fastcall (first ecx -> eax, n edx; x and the empty tag by
// reference on the stack), returning the end pointer in eax. Only call site 0x004D1410.
// Types (Ego_r.pdb): CSubtitledMessage::CCameraTag (24 bytes) = CameraPositionPerson +0, CameraPosition +4,
//   CameraFocusPerson +8, CameraFocusPosition +0xC, CameraZoomType +0x10, CameraView +0x14 (all enums); the pair is 28
//   bytes (the CameraTags CVectorMap<float, CCameraTag> of CSubtitledMessage).
// Shape: STLport's pair copy constructor ("VC++ needs this": first(o.first), second(o.second)) is memberwise: the float
// key word directly, then the trivially copyable CCameraTag as a 24-byte block through `lea esi,[ecx+4]` /
// `lea edi,[eax+4]` (six words through ebx). Placement-new null check kept; n counts down in edx.
#include <new>
enum EConversationProtagonist { CONVERSATION_PROTAGONIST_NONE };
enum EConversationCameraPosition { CONVERSATION_CAMERA_POSITION_NONE };
enum EConversationCameraFocus { CONVERSATION_CAMERA_FOCUS_NONE };
enum EConversationCameraZoomType { CONVERSATION_CAMERA_ZOOM_NONE };
enum EConversationCameraView { CONVERSATION_CAMERA_VIEW_NONE };
struct CSubtitledMessage {
    struct CCameraTag {                         // 24 bytes
        EConversationProtagonist    CameraPositionPerson;   // +0x00
        EConversationCameraPosition CameraPosition;         // +0x04
        EConversationProtagonist    CameraFocusPerson;      // +0x08
        EConversationCameraFocus    CameraFocusPosition;    // +0x0C
        EConversationCameraZoomType CameraZoomType;         // +0x10
        EConversationCameraView     CameraView;             // +0x14
    };
};
namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}
typedef std::pair<float, CSubtitledMessage::CCameraTag> CCameraTagPair;
struct __false_type { __false_type() {} };
namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template CCameraTagPair* __fastcall __uninitialized_fill_n(CCameraTagPair*, unsigned int, const CCameraTagPair&, const __false_type&);
}