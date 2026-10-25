// Behaviour test for 0x00AF2CD0 _STL::swap<pair<CCharString,NParticleEngine::CRegisteredPSCInfo>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00af2cd0_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CCharString {                                     // test model: a short owned text
public:
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CCharString(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CCharString() { ++g_dtors; }
    CCharString& operator=(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
    bool Is(const char* s) const { return std::strcmp(Text, s) == 0; }
};

namespace NParticleEngine {
static int AllocX() { return 3; }
enum EPSCType { PSC_TYPE_NONE = 0, PSC_TYPE_EMITTER = 4, PSC_TYPE_FORCE = 9 };
class CRegisteredPSCInfo {
public:
    int (*AllocFunc)();
    EPSCType    ComponentType;
    CCharString GUIName;
    CRegisteredPSCInfo(int (*f)(), EPSCType t, const char* n) : AllocFunc(f), ComponentType(t), GUIName(n) { }
};
}

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp> inline void swap_model(_Tp& __a, _Tp& __b)    // STLport swap
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

int main()
{
    pair<CCharString, NParticleEngine::CRegisteredPSCInfo> a(CCharString("Emitter"), NParticleEngine::CRegisteredPSCInfo(NParticleEngine::AllocX, NParticleEngine::PSC_TYPE_EMITTER, "Emit"));
    pair<CCharString, NParticleEngine::CRegisteredPSCInfo> b(CCharString("Force"), NParticleEngine::CRegisteredPSCInfo(0, NParticleEngine::PSC_TYPE_FORCE, "Push"));
    g_copies = g_assigns = 0;
    swap_model(a, b);
    CHECK(a.first.Is("Force") && a.second.AllocFunc == 0 && a.second.ComponentType == NParticleEngine::PSC_TYPE_FORCE && a.second.GUIName.Is("Push"));
    CHECK(b.first.Is("Emitter") && b.second.AllocFunc == NParticleEngine::AllocX && b.second.ComponentType == NParticleEngine::PSC_TYPE_EMITTER && b.second.GUIName.Is("Emit"));
    CHECK(g_copies == 2 && g_assigns == 4);
    std::printf("00af2cd0_TEST PASS\n");
    return 0;
}