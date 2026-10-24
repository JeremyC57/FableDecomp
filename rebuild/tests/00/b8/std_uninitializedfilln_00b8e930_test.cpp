// behaviour test for 0x00b8e930 (std::__uninitialized_fill_n over 28-byte camera-tag entries)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
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

int main()
{
    static double raw[64];
    SCameraTagEntry* p = (SCameraTagEntry*)raw;
    p[2].CameraView = (EConversationCameraView)-1;
    static double xs[16];
    SCameraTagEntry& x = *(SCameraTagEntry*)xs;
    x.Time = 1.5f; x.CameraPositionPerson = (EConversationProtagonist)1; x.CameraPosition = (EConversationCameraPosition)2;
    x.CameraFocusPerson = (EConversationProtagonist)3; x.CameraFocusPosition = (EConversationCameraFocus)4;
    x.CameraZoomType = (EConversationCameraZoomType)5; x.CameraView = (EConversationCameraView)6;
    __false_type tag;
    SCameraTagEntry* e = std::__uninitialized_fill_n(p, 2u, x, tag);
    int ok = e == p + 2 && p[0].Time == 1.5f && p[0].CameraView == 6 && p[1].CameraFocusPosition == 4 && p[1].CameraPositionPerson == 1
          && p[2].CameraView == -1 && std::__uninitialized_fill_n(p, 0u, x, tag) == p;
    if (!ok) { printf("00b8e930_TEST FAIL\n"); return 1; }
    printf("00b8e930_TEST PASS\n");
    return 0;
}