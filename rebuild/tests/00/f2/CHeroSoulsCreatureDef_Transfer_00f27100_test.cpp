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
class CHeroSoulsCreatureDef {
public:
    CCharString CreatureType; CCharString Location; long NumCreatures;
    void Transfer(CPersistContext& context)
    {
        context.Transfer("CreatureType", CreatureType);
        context.Transfer("Location", Location);
        context.Transfer("NumCreatures", NumCreatures, 0L);
    }
};

int main()
{
    CPersistContext ctx; CHeroSoulsCreatureDef d; d.NumCreatures = 0;
    d.Transfer(ctx);
    if (std::strcmp(g_log, "CreatureType:str:'';Location:str:'';NumCreatures:long:0;") != 0) do { std::printf("00f27100_TEST FAIL\n"); return 1; } while (0);
    if (std::strcmp(d.CreatureType.Text, "set") != 0 || std::strcmp(d.Location.Text, "set") != 0 || d.NumCreatures != 1) do { std::printf("00f27100_TEST FAIL\n"); return 1; } while (0);
    std::printf("00f27100_TEST PASS\n");
    return 0;
}