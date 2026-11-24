// behaviour test for 0x004608C1: CSpecialAbilitiesUnholyPowerDef::Copy forwards to CSpecialAbilitiesUnholyPowerDef::operator= with this and the source object
// NF386 (NF324 Q2 form, NF337 gen.py): the row now defines the PDB member CSpecialAbilitiesUnholyPowerDef::Copy (?Copy@CSpecialAbilitiesUnholyPowerDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee operator= (??4CSpecialAbilitiesUnholyPowerDef@@QAEAAV0@ABV0@@Z, out of line in the row) is stubbed as a member here.
#include <cstdio>

class CDefClassBase;
class CSpecialAbilitiesUnholyPowerDef { public:
    virtual void Copy(const CDefClassBase* other);
    CSpecialAbilitiesUnholyPowerDef& operator=(const CSpecialAbilitiesUnholyPowerDef& other);
};

static int g_calls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
CSpecialAbilitiesUnholyPowerDef& CSpecialAbilitiesUnholyPowerDef::operator=(const CSpecialAbilitiesUnholyPowerDef& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}

static unsigned long g_dst[4];
static unsigned long g_src[4];

int main()
{
    ((CSpecialAbilitiesUnholyPowerDef*)(g_dst))->CSpecialAbilitiesUnholyPowerDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src) {
        std::printf("OK_004608c1\n");
        return 0;
    }
    std::printf("FAIL_004608c1 calls=%d\n", g_calls);
    return 1;
}
