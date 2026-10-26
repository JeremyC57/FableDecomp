// Behaviour test for 0x00AF2BA0 std::__uninitialized_copy<pair<CCharString, CRegisteredPSCInfo>*, ...>: copy-construct
// [first, last) of the registered-PSC vector into raw storage at result and return the end (STLport 4.6 shape).
#include <cstdio>
#include <cstdlib>
inline void* operator new(unsigned int, void* p) { return p; }

static int g_copies = 0;
class CPSCBase;

class CCharString {
public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }   // 0x0099EC30 stand-in
};

enum EPSCType { PSC_TYPE_NONE = 0 };

namespace NParticleEngine {
class CRegisteredPSCInfo { public:
    CPSCBase* (__cdecl* AllocFunc)();
    EPSCType ComponentType;
    CCharString GUIName;
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                             _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

typedef std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCPair;
static CPSCBase* __cdecl AllocA() { return 0; }

int main()
{
    static const char* names[3] = { "Light", "Sound", "Decal" };
    static const char* gui[3] = { "GUI Light", "GUI Sound", "GUI Decal" };
    CPSCPair* src = (CPSCPair*)std::malloc(sizeof(CPSCPair) * 3);
    for (int i = 0; i < 3; ++i) {
        new (&src[i].first) CCharString(names[i]);
        src[i].second.AllocFunc = AllocA;
        src[i].second.ComponentType = (EPSCType)(i + 1);
        new (&src[i].second.GUIName) CCharString(gui[i]);
    }
    CPSCPair* dst = (CPSCPair*)std::malloc(sizeof(CPSCPair) * 4);
    std::__false_type tag;
    CPSCPair* r = std::__uninitialized_copy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && g_copies == 6;
    for (int i = 0; i < 3; ++i)
        if (dst[i].first.PStringData != names[i] || dst[i].second.AllocFunc != AllocA
            || dst[i].second.ComponentType != (EPSCType)(i + 1) || dst[i].second.GUIName.PStringData != gui[i]) ok = false;
    if (std::__uninitialized_copy(src, src, dst + 1, tag) != dst + 1 || g_copies != 6) ok = false;
    std::free(src); std::free(dst);
    if (!ok) { std::printf("00af2ba0_TEST FAIL\n"); return 1; }
    std::printf("00af2ba0_TEST PASS\n");
    return 0;
}
