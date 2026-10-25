#include <cstdio>
struct CSample { int x; };
struct CSoundPlayParameters { unsigned data[5]; };
static CSample* g_sample = 0; static const CSoundPlayParameters* g_params = 0;
struct CDriver { unsigned char _body[0x1c]; CDriver(CSample* sample, const CSoundPlayParameters& params) { g_sample = sample; g_params = &params; } };
class CScriptHost { public: CDriver* CreateDriver(CSample* sample, const CSoundPlayParameters& params) { return new CDriver(sample, params); } };
int main()
{
    CScriptHost host; CSample s; CSoundPlayParameters p;
    CDriver* d = host.CreateDriver(&s, p);
    bool ok = d != 0 && g_sample == &s && g_params == &p;
    delete d;
    if (!ok) { std::printf("00c0e870_TEST FAIL\n"); return 1; }
    std::printf("00c0e870_TEST PASS\n");
    return 0;
}
