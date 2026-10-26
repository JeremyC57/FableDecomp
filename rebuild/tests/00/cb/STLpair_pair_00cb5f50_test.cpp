// Behaviour test for pair<CCharString, CScriptInfo>::pair(const pair&) (0x00CB5F50).
#include <cstdio>
#include <cstring>

static int g_copies = 0;
class CCharString {
public:
    char Text[16];
    CCharString(const char* s) { std::strcpy(Text, s); }
    CCharString(const CCharString& other) { std::strcpy(Text, other.Text); ++g_copies; }
};

class CScriptBase;
class CScriptDataBase;
static CScriptDataBase* MasterAlloc() { return 0; }
static CScriptBase* Alloc(CScriptDataBase*, void*) { return 0; }

class CScriptInfo { public:
    CScriptBase*     (*AllocFunc)(CScriptDataBase*, void*);
    CScriptDataBase* (*MasterScriptDataAlloc)();
    CCharString      Name;
    long             ID;
    bool             MasterScript;
    CScriptInfo(const char* name) : AllocFunc(Alloc), MasterScriptDataAlloc(MasterAlloc), Name(name), ID(42), MasterScript(true) {}
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};

int main()
{
    pair<CCharString, CScriptInfo> a(CCharString("Q_Key"), CScriptInfo("QuestScript"));
    g_copies = 0;
    pair<CCharString, CScriptInfo> b(a);
    if (g_copies != 2 || std::strcmp(b.first.Text, "Q_Key") != 0 || std::strcmp(b.second.Name.Text, "QuestScript") != 0 ||
        b.second.AllocFunc != Alloc || b.second.MasterScriptDataAlloc != MasterAlloc || b.second.ID != 42 || !b.second.MasterScript) {
        std::printf("00cb5f50_TEST FAIL\n"); return 1;
    }
    std::printf("00cb5f50_TEST PASS\n");
    return 0;
}