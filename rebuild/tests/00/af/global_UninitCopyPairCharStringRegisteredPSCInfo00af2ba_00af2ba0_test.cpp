#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_copies; }
};
struct CPSCBase;
static CPSCBase* AllocA() { return 0; }
enum EPSCType { PSC_TYPE_NONE = 0, PSC_TYPE_LIGHT = 3 };
struct CRegisteredPSCInfo { CPSCBase* (*AllocFunc)(); EPSCType ComponentType; CCharString GUIName; };
struct SElem { CCharString first; CRegisteredPSCInfo second; };
struct __false_type { __false_type() { } };

static SElem* UninitCopy(const SElem* first, const SElem* last, SElem* result, const __false_type&)
{
    SElem* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SElem(*first);
    return cur;
}

int main()
{
    static const char* names[3] = { "Light", "Sound", "Decal" };
    static const char* gui[3] = { "GUI Light", "GUI Sound", "GUI Decal" };
    SElem* src = (SElem*)std::malloc(sizeof(SElem) * 3);
    for (int i = 0; i < 3; ++i) {
        new (&src[i].first) CCharString(names[i]);
        src[i].second.AllocFunc = AllocA; src[i].second.ComponentType = (EPSCType)(i + 1);
        new (&src[i].second.GUIName) CCharString(gui[i]);
    }
    SElem* dst = (SElem*)std::malloc(sizeof(SElem) * 4);
    __false_type tag;
    SElem* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && g_copies == 6;
    for (int i = 0; i < 3; ++i)
        if (dst[i].first.m_pData != names[i] || dst[i].second.AllocFunc != AllocA || dst[i].second.ComponentType != (EPSCType)(i + 1) || dst[i].second.GUIName.m_pData != gui[i]) ok = false;
    if (UninitCopy(src, src, dst + 1, tag) != dst + 1) ok = false;
    std::free(src); std::free(dst);
    if (!ok) { std::printf("00af2ba0_TEST FAIL\n"); return 1; }
    std::printf("00af2ba0_TEST PASS\n");
    return 0;
}