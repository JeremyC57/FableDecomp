// _global::UninitCopy_CCameraTagPair_004cfac0(const CCameraTagPair* first, const CCameraTagPair* last, CCameraTagPair* result, const __false_type&) -- STLport __uninitialized_copy over CSubtitledMessage's 28-byte pair<float, CCameraTag> camera-tag entries: placement-copy each entry, return the advanced result
// Leaf: UninitCopy_CCameraTagPair_004cfac0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CFAC0, length 83 bytes, `ret 8`.
// Callers: the vector<pair<float, CCameraTag>> copy constructor @ 0x004D08A0 (call @ 0x004D08F8) and -- folded by the
//   retail linker's ICF -- the landed vector<CActionInputControl>::operator= @ 0x00719540 (calls @ 0x007195B3 /
//   0x0071966C) plus 0x00718FB0 (call @ 0x00719089). Byte-identical to 0x004CFA10: the const_iterator
//   (`__uninitialized_copy<const T*, T*>`) instance of the same template.
// Callees: none.
// Layout (Ego_r.pdb, as in the landed __uninitialized_fill_n twin @ 0x004CFA70): pair<float first +0x0,
//   CSubtitledMessage::CCameraTag second +0x4> = 28 bytes; CCameraTag is six enums (24 bytes).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). STLport's explicit pair copy constructor ("VC++ needs
//   this": first(o.first), second(o.second)) copies the float key word, then the trivially copyable CCameraTag as a
//   block through a second strength-reduced source pointer (`lea esi,[ecx+4]` before the loop, `mov ebx,esi`) and
//   `lea edi,[eax+4]` rebuilt in each iteration; the six words go through ebp. The body must be STLport's own spelling:
//   a `cur` copy of result and the element built through `_Construct(&*cur, *first)` -- the direct
//   `new (result) T(*first)` loop strength-reduces dest+4 as well, runs out of registers and spills `last` (96 v 83).
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, unsigned int*, ..>` is propagated.
inline void* operator new(unsigned int, void* where) { return where; }

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

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

CCameraTagPair* __fastcall UninitCopy_CCameraTagPair_004cfac0(const CCameraTagPair* first, const CCameraTagPair* last, CCameraTagPair* result, const __false_type&)
{
    CCameraTagPair* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}