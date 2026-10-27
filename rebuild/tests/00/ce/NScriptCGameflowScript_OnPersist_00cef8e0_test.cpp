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
struct Master { int pad; EGameflowPosition PostSavePosition; };
struct Script {
    Master* MasterScriptData; unsigned int CoreQuestWaiting; Names SavedScriptNames; Names SavedCardDefNames;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("PostSavePosition", MasterScriptData->PostSavePosition, (EGameflowPosition)0);
        context.Transfer("CoreQuestWaiting", CoreQuestWaiting, 0U);
        context.TransferVector("SavedScriptNames", SavedScriptNames);
        context.TransferVector("SavedCardDefNames", SavedCardDefNames);
    }
};
int main()
{
    int ok = 1;
    Master m; m.PostSavePosition = (EGameflowPosition)3;
    Script s; s.MasterScriptData = &m; s.CoreQuestWaiting = 5; s.SavedScriptNames.Count = 2; s.SavedCardDefNames.Count = 4;
    CPersistContext c; g_log[0] = 0;
    s.OnPersist(c);
    ok = std::strcmp(g_log, "PostSavePosition=3/0;CoreQuestWaiting=5/0;SavedScriptNames#2;SavedCardDefNames#4;") == 0;
    if (!ok) { std::printf("00cef8e0_TEST FAIL\n"); return 1; }
    std::printf("00cef8e0_TEST PASS\n");
    return 0;
}