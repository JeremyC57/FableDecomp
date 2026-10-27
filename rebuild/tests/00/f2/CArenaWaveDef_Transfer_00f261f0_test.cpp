#include <cstdio>
#include <cstring>
#include <cmath>
static char g_log[512];
static char g_b[128];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public: const char* Text; CCharString() : Text("") { } };
class CPersistContext {
public:
    void Transfer(const char* n, long& v, const long& d) { std::sprintf(g_b, "%s:long:%ld", n, d); Log(g_b); v = d + 1; }
    void Transfer(const char* n, bool& v, const bool& d) { std::sprintf(g_b, "%s:bool:%d", n, (int)d); Log(g_b); v = !d; }
    void Transfer(const char* n, CCharString& v, const CCharString& d) { std::sprintf(g_b, "%s:str:'%s'", n, d.Text); Log(g_b); v.Text = "set"; }
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
    template <class T> void TransferVectorOfSubComponents(const char* n, T&) { std::sprintf(g_b, "%s:vector", n); Log(g_b); }
protected:
    template <class T> static T GetDefaultVal() { return T(); }
};
class CArenaCreatureDef { public: int X; };
struct CVec { CArenaCreatureDef* a; CArenaCreatureDef* b; CArenaCreatureDef* c; };
class CArenaWaveDef {
public:
    long NumWaveCreatures; CVec Creatures; bool ShortWave;
    void Transfer(CPersistContext& context)
    {
        context.Transfer("NumWaveCreatures", NumWaveCreatures, 0L);
        context.TransferVectorOfSubComponents("Creatures", Creatures);
        context.Transfer("ShortWave", ShortWave, false);
    }
};

int main()
{
    CPersistContext ctx; CArenaWaveDef d; d.NumWaveCreatures = 0; d.ShortWave = false;
    d.Transfer(ctx);
    if (std::strcmp(g_log, "NumWaveCreatures:long:0;Creatures:vector;ShortWave:bool:0;") != 0) do { std::printf("00f261f0_TEST FAIL\n"); return 1; } while (0);
    if (d.NumWaveCreatures != 1 || !d.ShortWave) do { std::printf("00f261f0_TEST FAIL\n"); return 1; } while (0);
    std::printf("00f261f0_TEST PASS\n");
    return 0;
}