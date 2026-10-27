#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
class CCharString { public: const char* p; CCharString() : p("") { } };
class CPersistContext { public:
    void Transfer(const char* name, CCharString& v, const CCharString& d) { char b[96]; std::sprintf(b, "%s=%s(def '%s')", name, v.p, d.p); Log(b); }
    template <class T> void Transfer(const char* name, T& v, const T& d) { char b[96]; std::sprintf(b, "%s=%d(def %d)", name, (int)v, (int)d); Log(b); }
    template <class T> void Transfer(const char* name, T& v) { Transfer(name, v, T()); }
};
enum AA_LocationIdentifier { LOC0, LOC1, LOC2 };
struct E { CCharString regionName; AA_LocationIdentifier regionId;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("regionName", regionName);
        context.Transfer("regionId", regionId);
    }
};
int main()
{
    g_log[0] = 0;
    bool ok = true;
    E e; e.regionName.p = "Oakvale"; e.regionId = LOC2; CPersistContext c; e.OnPersist(c);
    ok = ok && std::strcmp(g_log, "regionName=Oakvale(def '');regionId=2(def 0);") == 0 && (true);
    if (!ok) std::printf("log: %s\n", g_log);
    if (!ok) { std::printf("00e38d80_TEST FAIL\n"); return 1; }
    std::printf("00e38d80_TEST PASS\n");
    return 0;
}