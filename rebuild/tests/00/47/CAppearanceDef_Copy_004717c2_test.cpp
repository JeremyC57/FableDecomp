// behaviour test for 0x004717C2: CAppearanceDef::Copy forwards to CAppearanceDef::operator= with this and the source object
// NF337 (NF324 Q2 form): the row now defines the PDB member CAppearanceDef::Copy (?Copy@CAppearanceDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee operator= (??4CAppearanceDef@@QAEAAV0@ABV0@@Z, out of line in the row) is stubbed as a member here.
#include <cstdio>

class CDefClassBase;
class CAppearanceDef { public:
    virtual void Copy(const CDefClassBase* other);
    CAppearanceDef& operator=(const CAppearanceDef& other);
};

static int g_calls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
CAppearanceDef& CAppearanceDef::operator=(const CAppearanceDef& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}

static unsigned long g_dst[4];
static unsigned long g_src[4];

int main()
{
    ((CAppearanceDef*)(g_dst))->CAppearanceDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src) {
        std::printf("OK_004717c2\n");
        return 0;
    }
    std::printf("FAIL_004717c2 calls=%d\n", g_calls);
    return 1;
}
