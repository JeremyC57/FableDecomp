#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
static char g_log[512];
struct Names { int Count; };
enum EGameflowPosition { };
class CPersistContext { public:
    void Transfer(const char* name, EGameflowPosition& v, const EGameflowPosition& d) { char b[64]; std::sprintf(b, "%s=%d/%d;", name, (int)v, (int)d); std::strcat(g_log, b); }
    void Transfer(const char* name, unsigned int& v, const unsigned int& d) { char b[64]; std::sprintf(b, "%s=%u/%u;", name, v, d); std::strcat(g_log, b); }
    void Transfer(const char* name, bool& v, const bool& d) { char b[64]; std::sprintf(b, "%s=%d/%d;", name, (int)v, (int)d); std::strcat(g_log, b); }
    void TransferVector(const char* name, Names& v) { char b[64]; std::sprintf(b, "%s#%d;", name, v.Count); std::strcat(g_log, b); }
};
struct Script {
    bool QuestStartScreenShown;
    void OnPersist(CPersistContext& context) { context.Transfer("QuestStartScreenShown", QuestStartScreenShown, false); }
};
int main()
{
    int ok = 1;
    Script s; s.QuestStartScreenShown = true; CPersistContext c; g_log[0] = 0;
    s.OnPersist(c);
    ok = std::strcmp(g_log, "QuestStartScreenShown=1/0;") == 0;
    if (!ok) { std::printf("00d2aa10_TEST FAIL\n"); return 1; }
    std::printf("00d2aa10_TEST PASS\n");
    return 0;
}