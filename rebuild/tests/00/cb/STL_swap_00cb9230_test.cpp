// Behaviour test for 0x00CB9230 std::swap<pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00cb9230_TEST FAIL\n"); return 1; } } while (0)

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

#ifdef _MSC_VER
typedef unsigned __int64 TUID;
#else
typedef unsigned long long TUID;
#endif
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {
    public:
        TUID        UID;
        CCharString PersistScript;
        CPersistInfo(TUID u, const char* p) : UID(u), PersistScript(p) { }
    };
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
    typedef pair<TUID, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> TPair;
    TUID big = ((TUID)0x12345678 << 32) | 0x9abcdef0;
    TPair a(big, NScript::CRuntimeScriptedEntityPersist::CPersistInfo(big + 1, "ScriptA"));
    TPair b((TUID)42, NScript::CRuntimeScriptedEntityPersist::CPersistInfo((TUID)43, "ScriptB"));
    g_copies = g_assigns = 0;
    swap_model(a, b);
    CHECK(a.first == (TUID)42 && a.second.UID == (TUID)43 && a.second.PersistScript.Is("ScriptB"));
    CHECK(b.first == big && b.second.UID == big + 1 && b.second.PersistScript.Is("ScriptA"));
    CHECK(g_copies == 1 && g_assigns == 2);
    std::printf("00cb9230_TEST PASS\n");
    return 0;
}