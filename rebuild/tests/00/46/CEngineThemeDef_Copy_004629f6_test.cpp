// behaviour test for 0x004629F6: CEngineThemeDef::Copy forwards to CEngineThemeDef::operator= with this and the source object
// NF337 (NF324 Q2 form): the row now defines the PDB member CEngineThemeDef::Copy (?Copy@CEngineThemeDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee operator= (??4CEngineThemeDef@@QAEAAV0@ABV0@@Z, out of line in the row) is stubbed as a member here.
#include <cstdio>

class CDefClassBase;
class CEngineThemeDef { public:
    virtual void Copy(const CDefClassBase* other);
    CEngineThemeDef& operator=(const CEngineThemeDef& other);
};

static int g_calls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
CEngineThemeDef& CEngineThemeDef::operator=(const CEngineThemeDef& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}

static unsigned long g_dst[4];
static unsigned long g_src[4];

int main()
{
    ((CEngineThemeDef*)(g_dst))->CEngineThemeDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src) {
        std::printf("OK_004629f6\n");
        return 0;
    }
    std::printf("FAIL_004629f6 calls=%d\n", g_calls);
    return 1;
}
