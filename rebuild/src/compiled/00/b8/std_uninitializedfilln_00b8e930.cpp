// std::__uninitialized_fill_n(SCameraTagEntry* first, unsigned int n, const SCameraTagEntry& x, const __false_type&) -- copy-construct n copies of a 28-byte camera-tag entry into raw storage at first and return first + n
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x00B8E930, 70 bytes, `ret 8`: __fastcall (first ecx -> eax, n edx; x and the empty tag by reference
// on the stack), returning the end pointer in eax. Call sites 0x00B8EF4A, 0x00B8F41C, 0x00B8F5C9. Byte-identical to 0x0040EAC0.
// Element: 28 bytes, the catalog's pair<float, CSubtitledMessage::CCameraTag> (Time + CameraPositionPerson,
//   CameraPosition, CameraFocusPerson, CameraFocusPosition, CameraZoomType, CameraView from Ego_r.pdb).
// Shape: every word is copied straight from x into the new element through esi (no block copy, no second induction
// register), which is a FLAT record whose user copy constructor copies its seven scalars one by one. Probed: the nested
// pair { float; CCameraTag } with a user copy constructor on CCameraTag (with or without STLport's pair copy
// constructor) adds a second induction register `lea esi,[eax+0Ch]` (74 v 70); a trivially copyable CCameraTag under
// the pair constructor gives the `lea` block copy of 0x004CFA70. Placement-new null check kept; n counts down in edx.
#include <new>
enum EConversationProtagonist { CONVERSATION_PROTAGONIST_NONE };
enum EConversationCameraPosition { CONVERSATION_CAMERA_POSITION_NONE };
enum EConversationCameraFocus { CONVERSATION_CAMERA_FOCUS_NONE };
enum EConversationCameraZoomType { CONVERSATION_CAMERA_ZOOM_NONE };
enum EConversationCameraView { CONVERSATION_CAMERA_VIEW_NONE };
struct SCameraTagEntry {                        // pair<float, CSubtitledMessage::CCameraTag> flattened, 28 bytes
    float                       Time;                   // +0x00 (pair.first)
    EConversationProtagonist    CameraPositionPerson;   // +0x04
    EConversationCameraPosition CameraPosition;         // +0x08
    EConversationProtagonist    CameraFocusPerson;      // +0x0C
    EConversationCameraFocus    CameraFocusPosition;    // +0x10
    EConversationCameraZoomType CameraZoomType;         // +0x14
    EConversationCameraView     CameraView;             // +0x18
    SCameraTagEntry(const SCameraTagEntry& o)
        : Time(o.Time), CameraPositionPerson(o.CameraPositionPerson), CameraPosition(o.CameraPosition),
          CameraFocusPerson(o.CameraFocusPerson), CameraFocusPosition(o.CameraFocusPosition),
          CameraZoomType(o.CameraZoomType), CameraView(o.CameraView) {}
};
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
template SCameraTagEntry* __fastcall __uninitialized_fill_n(SCameraTagEntry*, unsigned int, const SCameraTagEntry&, const __false_type&);
}