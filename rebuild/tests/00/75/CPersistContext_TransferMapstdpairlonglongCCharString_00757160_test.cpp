#include <cstdio>
#include <cstring>
// CPersistContext::TransferMap<std::pair<long, long>, CCharString>(name, map&) (0x00757160): forwards to the 3-argument
// overload with a value-initialised (zeroed) pair<long, long> default
class CCharString { public: void* PStringData; };
template <class T1, class T2> struct pair { T1 first; T2 second; pair() : first(T1()), second(T2()) { } };
struct CrimeMap { long dummy; };                       // stands for std::map<CCharString, std::pair<long, long> >
static const char* g_name; static CrimeMap* g_map; static long g_a = 9, g_b = 9; static long g_calls;
struct CPersistContext {
    long pad[11];
    void TransferMap(const char* name, CrimeMap& map, const pair<long, long>& def) { g_name = name; g_map = &map; g_a = def.first; g_b = def.second; ++g_calls; }
    void TransferMap(const char* name, CrimeMap& map) { TransferMap(name, map, pair<long, long>()); }
};
int main()
{
    CPersistContext c; CrimeMap m;
    c.TransferMap("RegionNamesToCrimeDurationAndFine", m);
    if (g_calls != 1 || std::strcmp(g_name, "RegionNamesToCrimeDurationAndFine") != 0 || g_map != &m || g_a != 0 || g_b != 0) { std::printf("00757160_TEST FAIL\n"); return 1; }
    std::printf("00757160_TEST PASS\n");
    return 0;
}
