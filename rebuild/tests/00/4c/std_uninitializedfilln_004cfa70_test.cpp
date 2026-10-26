// behaviour test for 0x004cfa70 (std::__uninitialized_fill_n over pair<float, CCameraTag>)
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
class CSubtitledMessage { public:
    class CCameraTag { public:                         // 24 bytes
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

int main()
{
    static double raw[64];
    CCameraTagPair* p = (CCameraTagPair*)raw;
    static double xs[16];
    CCameraTagPair& x = *(CCameraTagPair*)xs;
    x.first = 2.5f; x.second.CameraPositionPerson = (EConversationProtagonist)1; x.second.CameraPosition = (EConversationCameraPosition)2;
    x.second.CameraFocusPerson = (EConversationProtagonist)3; x.second.CameraFocusPosition = (EConversationCameraFocus)4;
    x.second.CameraZoomType = (EConversationCameraZoomType)5; x.second.CameraView = (EConversationCameraView)6;
    p[2].first = -1.0f;
    __false_type tag;
    CCameraTagPair* e = std::__uninitialized_fill_n(p, 2u, x, tag);
    int ok = e == p + 2 && p[0].first == 2.5f && p[1].second.CameraView == 6 && p[0].second.CameraFocusPosition == 4 && p[2].first == -1.0f;
    if (!ok) { printf("004cfa70_TEST FAIL\n"); return 1; }
    printf("004cfa70_TEST PASS\n");
    return 0;
}