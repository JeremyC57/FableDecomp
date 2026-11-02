// Behaviour test for 0x00cb6e60 std::vector<pair<CCharString, CScriptInfo> >::_M_clear(): every pair destroyed in order
// (CScriptInfo::Name first, then the key), then the block freed (null block: nothing freed).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static char g_log[64];
static int g_n = 0;
static int g_freed = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CCharString {
public:
    CCharString() : Id('?') { }
    ~CCharString() { Log(Id); }
    char Id;
};
class CScriptInfo {
public:
    void*       AllocFunc;
    void*       MasterScriptDataAlloc;
    CCharString Name;
    long        ID;
    bool        MasterScript;
};
struct ScriptPair { CCharString first; CScriptInfo second; };

struct ScriptVector {                                       // STLport _Vector_base layout
    ScriptPair* _M_start;
    ScriptPair* _M_finish;
    ScriptPair* _M_end_of_storage;
    void _M_clear()
    {
        for (ScriptPair* p = _M_start; p != _M_finish; ++p)
            p->~ScriptPair();
        if (_M_start != 0) { std::free(_M_start); ++g_freed; }
    }
};

int main()
{
    ScriptVector v;
    v._M_start = (ScriptPair*)std::malloc(4 * sizeof(ScriptPair));
    for (int i = 0; i < 3; ++i) {
        new (&v._M_start[i]) ScriptPair();
        v._M_start[i].first.Id = (char)('a' + i);
        v._M_start[i].second.Name.Id = (char)('A' + i);
    }
    v._M_finish = v._M_start + 3;
    v._M_end_of_storage = v._M_start + 4;
    v._M_clear();
    if (std::strcmp(g_log, "AaBbCc") != 0 || g_freed != 1) {
        std::printf("00cb6e60_TEST FAIL (%s %d)\n", g_log, g_freed);
        return 1;
    }
    ScriptVector empty = { 0, 0, 0 };
    empty._M_clear();
    if (g_freed != 1 || g_n != 6) {
        std::printf("00cb6e60_TEST FAIL (empty)\n");
        return 1;
    }
    std::printf("00cb6e60_TEST PASS\n");
    return 0;
}