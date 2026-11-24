// behaviour test for 0x004541DA: CSnowTrollDef::Copy forwards to CSnowTrollDef::operator= with this and the source object
// NF386 (NF324 Q2 form, NF337 gen.py): the row now defines the PDB member CSnowTrollDef::Copy (?Copy@CSnowTrollDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee operator= (??4CSnowTrollDef@@QAEAAV0@ABV0@@Z, out of line in the row) is stubbed as a member here.
#include <cstdio>

class CDefClassBase;
class CSnowTrollDef { public:
    virtual void Copy(const CDefClassBase* other);
    CSnowTrollDef& operator=(const CSnowTrollDef& other);
};

static int g_calls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
CSnowTrollDef& CSnowTrollDef::operator=(const CSnowTrollDef& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}

static unsigned long g_dst[4];
static unsigned long g_src[4];

int main()
{
    ((CSnowTrollDef*)(g_dst))->CSnowTrollDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src) {
        std::printf("CSnowTrollDef_004541da_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL_004541da calls=%d\n", g_calls);
    return 1;
}
