// Behaviour test for 0x00CB6230 std::swap<pair<CCharString,CScriptInfo>> (copy-construct a temporary, assign both ways).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00cb6230_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CCharString {                                     // test model: a short owned text
public:
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CCharString(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CCharString() { ++g_dtors; }
    const CCharString& operator=(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
    bool Is(const char* s) const { return std::strcmp(Text, s) == 0; }
};

static int AllocA() { return 1; }
static int AllocB() { return 2; }
class CScriptInfo {
public:
    int (*AllocFunc)();
    int (*MasterScriptDataAlloc)();
    CCharString Name;
    long ID;
    bool MasterScript;
    CScriptInfo(int (*f)(), int (*g)(), const char* n, long id, bool m) : AllocFunc(f), MasterScriptDataAlloc(g), Name(n), ID(id), MasterScript(m) { }
};

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
    pair<CCharString, CScriptInfo> a(CCharString("alpha"), CScriptInfo(AllocA, 0, "ScriptA", 7, true));
    pair<CCharString, CScriptInfo> b(CCharString("beta"), CScriptInfo(AllocB, AllocA, "ScriptB", 9, false));
    g_copies = g_assigns = 0;
    swap_model(a, b);
    CHECK(a.first.Is("beta") && a.second.Name.Is("ScriptB") && a.second.ID == 9 && !a.second.MasterScript);
    CHECK(a.second.AllocFunc == AllocB && a.second.MasterScriptDataAlloc == AllocA);
    CHECK(b.first.Is("alpha") && b.second.Name.Is("ScriptA") && b.second.ID == 7 && b.second.MasterScript);
    CHECK(b.second.AllocFunc == AllocA && b.second.MasterScriptDataAlloc == 0);
    CHECK(g_copies == 2 && g_assigns == 4);
    std::printf("00cb6230_TEST PASS\n");
    return 0;
}