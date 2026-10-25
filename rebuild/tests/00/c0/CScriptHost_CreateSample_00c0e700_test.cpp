#include <cstdio>
struct CAudioFormat { int x; };
class CScriptHost;
static CScriptHost* g_owner = 0; static unsigned long g_offset = 0; static const CAudioFormat* g_format = 0;
struct CSample { unsigned char _body[0x24]; CSample(CScriptHost* owner, unsigned long dataOffset, const CAudioFormat* format) { g_owner = owner; g_offset = dataOffset; g_format = format; } };
class CScriptHost { public: CSample* CreateSample(const CAudioFormat* format, unsigned long dataOffset) { return new CSample(this, dataOffset, format); } };
int main()
{
    CScriptHost host; CAudioFormat f;
    CSample* s = host.CreateSample(&f, 0x1234);
    bool ok = s != 0 && g_owner == &host && g_offset == 0x1234 && g_format == &f;
    delete s;
    if (!ok) { std::printf("00c0e700_TEST FAIL\n"); return 1; }
    std::printf("00c0e700_TEST PASS\n");
    return 0;
}
