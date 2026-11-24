// behaviour test for 0x004601E5: CSpecialAbilitiesTurncoatSpellDef::Copy forwards to CSpecialAbilitiesTurncoatSpellDef::operator= with this and the source object
// NF337 (NF324 Q2 form): the row now defines the PDB member CSpecialAbilitiesTurncoatSpellDef::Copy (?Copy@CSpecialAbilitiesTurncoatSpellDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee operator= (??4CSpecialAbilitiesTurncoatSpellDef@@QAEAAV0@ABV0@@Z, out of line in the row) is stubbed as a member here.
#include <cstdio>

class CDefClassBase;
class CSpecialAbilitiesTurncoatSpellDef { public:
    virtual void Copy(const CDefClassBase* other);
    CSpecialAbilitiesTurncoatSpellDef& operator=(const CSpecialAbilitiesTurncoatSpellDef& other);
};

static int g_calls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
CSpecialAbilitiesTurncoatSpellDef& CSpecialAbilitiesTurncoatSpellDef::operator=(const CSpecialAbilitiesTurncoatSpellDef& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}

static unsigned long g_dst[4];
static unsigned long g_src[4];

int main()
{
    ((CSpecialAbilitiesTurncoatSpellDef*)(g_dst))->CSpecialAbilitiesTurncoatSpellDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src) {
        std::printf("OK_004601e5\n");
        return 0;
    }
    std::printf("FAIL_004601e5 calls=%d\n", g_calls);
    return 1;
}
