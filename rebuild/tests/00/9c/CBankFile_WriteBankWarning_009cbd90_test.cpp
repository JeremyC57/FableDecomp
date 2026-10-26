#include <cstdio>

class CCharString { public: int x; };
static const CCharString* g_msg = 0;
struct CB { virtual void v00(){} virtual void v04(){} virtual void v08(){} virtual void Warning(const CCharString& m) { g_msg = &m; } };
struct B { unsigned char pad[0xec]; CB* ProgressCallback; void* h; bool WarningFlag;
    void WriteBankWarning(const CCharString& m) { CB* c = ProgressCallback; if (c) c->Warning(m); WarningFlag = true; } };
int main()
{
    CCharString s; B b; b.WarningFlag = false; b.ProgressCallback = 0; b.WriteBankWarning(s);
    if (!b.WarningFlag || g_msg) { std::printf("009cbd90_TEST FAIL\n"); return 1; }
    CB cb; b.ProgressCallback = &cb; b.WriteBankWarning(s); if (g_msg != &s) { std::printf("009cbd90_TEST FAIL\n"); return 1; }
    std::printf("009cbd90_TEST PASS\n");
    return 0;
}