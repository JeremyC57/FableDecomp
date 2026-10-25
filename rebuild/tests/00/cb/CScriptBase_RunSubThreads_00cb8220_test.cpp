// Behaviour test for 00cb8220: CScriptBase::RunSubThreads -- spawned functions first, then entity threads, on the same object
#include <cstdio>
#include <cstring>
static char g_seq[8] = ""; static const void* g_this[2] = { 0, 0 };
class CScriptBase {
public:
    void RunSpawnedFunctions() { std::strcat(g_seq, "S"); g_this[0] = this; }
    void RunEntityThreads() { std::strcat(g_seq, "E"); g_this[1] = this; }
    void RunSubThreads() { RunSpawnedFunctions(); RunEntityThreads(); }
};
int main()
{
    CScriptBase s; s.RunSubThreads();
    int ok = !std::strcmp(g_seq, "SE") && g_this[0] == &s && g_this[1] == &s;
    std::printf(ok ? "00cb8220_TEST PASS\n" : "00cb8220_TEST FAIL\n");
    return ok ? 0 : 1;
}
