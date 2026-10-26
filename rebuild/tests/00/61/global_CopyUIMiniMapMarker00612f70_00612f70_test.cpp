#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

struct random_access_iterator_tag { };
static int g_assigns = 0;
class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; ++g_assigns; return *this; }
};
static const char* g_text[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
class C2DVector { public: float X, Y; };
struct CUIMiniMapMarker {
    long        Region;
    CCharString UIDefName;
    C2DVector   Position;
    long        DefNameLength;
    long        GraphicIndex;
    float       Angle;
};

typedef CUIMiniMapMarker T_;
enum { N = 3 };

static T_* Copy(const T_* first, const T_* last, T_* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    bool ok = true;
    static T_ src[N];
    static T_ dst[N];
    for (int i = 0; i < N; ++i) {
        T_* s = &src[i];
        T_* d = &dst[i];
        s->Region = i; s->UIDefName.m_pData = g_text[i]; s->Position.X = 1.0f * i; s->Position.Y = -2.0f; s->DefNameLength = 5 + i;
        s->GraphicIndex = 20 + i; s->Angle = 0.125f * i;
    }
    random_access_iterator_tag tag;
    T_* r = Copy(src, src + N, dst, tag, (int*)0);
    if (r != dst + N) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->Region != i || d->UIDefName.m_pData != g_text[i] || d->Position.X != 1.0f * i || d->Position.Y != -2.0f) ok = false;
        if (d->DefNameLength != 5 + i || d->GraphicIndex != 20 + i || d->Angle != 0.125f * i) ok = false;
    }
    if (g_assigns != N) ok = false;
    if (Copy(src, src, dst + 1, tag, (int*)0) != dst + 1) ok = false;
    if (!ok) { std::printf("00612f70_TEST FAIL\n"); return 1; }
    std::printf("00612f70_TEST PASS\n");
    return 0;
}