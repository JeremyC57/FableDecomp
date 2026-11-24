// behaviour test for 0x0046F4EB: CSkyDef::Copy forwards to CSkyDef::operator= with this and the source object
// NF337 (NF324 Q2 form): the row now defines the PDB member CSkyDef::Copy (?Copy@CSkyDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee operator= (??4CSkyDef@@QAEAAV0@ABV0@@Z, out of line in the row) is stubbed as a member here.
#include <cstdio>

class CDefClassBase;
class CSkyDef { public:
    virtual void Copy(const CDefClassBase* other);
    CSkyDef& operator=(const CSkyDef& other);
};

static int g_calls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
CSkyDef& CSkyDef::operator=(const CSkyDef& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}

static unsigned long g_dst[4];
static unsigned long g_src[4];

int main()
{
    ((CSkyDef*)(g_dst))->CSkyDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src) {
        std::printf("PASS_0046f4eb\n");
        return 0;
    }
    std::printf("FAIL_0046f4eb calls=%d\n", g_calls);
    return 1;
}
