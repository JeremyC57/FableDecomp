// Behaviour test for 0x007167E0 NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags: seven camera tags are
// appended to the static DefaultCameraTags in order (two speakee-over-left-shoulder, speaker face, speakee over left
// then right shoulder, speaker face, the neutral left-position level shot), growing through the overflow path
#include <cstdio>
#include <cstdlib>

namespace model {

struct CCameraTag {
    int CameraPositionPerson, CameraPosition, CameraFocusPerson, CameraFocusPosition, CameraZoomType, CameraView;
};
CCameraTag tag(int a, int b, int c, int d, int e, int f) { CCameraTag t; t.CameraPositionPerson = a; t.CameraPosition = b; t.CameraFocusPerson = c; t.CameraFocusPosition = d; t.CameraZoomType = e; t.CameraView = f; return t; }

struct Vec {
    CCameraTag* s;
    CCameraTag* f;
    CCameraTag* e;
    int overflows;
    void insert_overflow(const CCameraTag& x)
    {
        ++overflows;
        unsigned int old = (unsigned int)(f - s);
        unsigned int len = old + (old < 1u ? 1u : old);
        CCameraTag* ns = (CCameraTag*)std::malloc(len * sizeof(CCameraTag));
        CCameraTag* nf = ns;
        for (CCameraTag* p = s; p != f; ++p) *nf++ = *p;
        *nf++ = x;
        if (s) std::free(s);
        s = ns; f = nf; e = ns + len;
    }
    void push_back(const CCameraTag& x)
    {
        if (f != e) { *f = x; ++f; }
        else insert_overflow(x);
    }
};
Vec DefaultCameraTags = { 0, 0, 0, 0 };

enum { NONE = 0, SPEAKER = 1, SPEAKEE = 2 };
enum { IN_FRONT_OF_FACE = 0, BEHIND_LEFT_SHOULDER_NEAR = 3, BEHIND_RIGHT_SHOULDER_NEAR = 5, POS_LEFT = 11 };
enum { FOCUS_FACE = 0, FOCUS_LEVEL = 6 };
enum { NO_ZOOM = 0 };
enum { VIEW_MEDIUM = 1 };

void InitialiseDefaultCameraTags()
{
    DefaultCameraTags.push_back(tag(SPEAKEE, BEHIND_LEFT_SHOULDER_NEAR, SPEAKER, FOCUS_FACE, NO_ZOOM, VIEW_MEDIUM));
    DefaultCameraTags.push_back(tag(SPEAKEE, BEHIND_LEFT_SHOULDER_NEAR, SPEAKER, FOCUS_FACE, NO_ZOOM, VIEW_MEDIUM));
    DefaultCameraTags.push_back(tag(SPEAKER, IN_FRONT_OF_FACE, SPEAKER, FOCUS_FACE, NO_ZOOM, VIEW_MEDIUM));
    DefaultCameraTags.push_back(tag(SPEAKER, BEHIND_LEFT_SHOULDER_NEAR, SPEAKEE, FOCUS_FACE, NO_ZOOM, VIEW_MEDIUM));
    DefaultCameraTags.push_back(tag(SPEAKER, BEHIND_RIGHT_SHOULDER_NEAR, SPEAKEE, FOCUS_FACE, NO_ZOOM, VIEW_MEDIUM));
    DefaultCameraTags.push_back(tag(SPEAKER, IN_FRONT_OF_FACE, SPEAKER, FOCUS_FACE, NO_ZOOM, VIEW_MEDIUM));
    DefaultCameraTags.push_back(tag(NONE, POS_LEFT, NONE, FOCUS_LEVEL, NO_ZOOM, VIEW_MEDIUM));
}

}

int main()
{
    using namespace model;
    InitialiseDefaultCameraTags();
    // the six dwords of each tag, as the retail body loads them (field order +0x00 .. +0x14)
    static const int want[7][6] = {
        { 2, 3, 1, 0, 0, 1 }, { 2, 3, 1, 0, 0, 1 }, { 1, 0, 1, 0, 0, 1 }, { 1, 3, 2, 0, 0, 1 },
        { 1, 5, 2, 0, 0, 1 }, { 1, 0, 1, 0, 0, 1 }, { 0, 11, 0, 6, 0, 1 }
    };
    int bad = 0;
    if (sizeof(CCameraTag) != 24u) bad = 1;
    if (DefaultCameraTags.f - DefaultCameraTags.s != 7) bad = 2;
    for (int i = 0; i < 7 && !bad; ++i) {
        const int* p = &DefaultCameraTags.s[i].CameraPositionPerson;
        for (int j = 0; j < 6; ++j)
            if (p[j] != want[i][j]) bad = 3;
    }
    if (DefaultCameraTags.overflows != 4 || DefaultCameraTags.e - DefaultCameraTags.s != 8) bad = 4;   // capacity 1, 2, 4, 8
    std::free(DefaultCameraTags.s);
    if (bad) { std::printf("007167e0_TEST FAIL\n"); return 1; }
    std::printf("007167e0_TEST PASS\n");
    return 0;
}