// behaviour test for 0x0045390C: CHeroMarriageDef::Copy forwards to CHeroMarriageDef::operator= with this and the source object
// NF386 (NF324 Q2 form, NF337 gen.py): the row now defines the PDB member CHeroMarriageDef::Copy (?Copy@CHeroMarriageDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee operator= (??4CHeroMarriageDef@@QAEAAV0@ABV0@@Z, out of line in the row) is stubbed as a member here.
#include <cstdio>

class CDefClassBase;
class CHeroMarriageDef { public:
    virtual void Copy(const CDefClassBase* other);
    CHeroMarriageDef& operator=(const CHeroMarriageDef& other);
};

static int g_calls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
CHeroMarriageDef& CHeroMarriageDef::operator=(const CHeroMarriageDef& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}

static unsigned long g_dst[4];
static unsigned long g_src[4];

int main()
{
    ((CHeroMarriageDef*)(g_dst))->CHeroMarriageDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src) {
        std::printf("CHeroMarriageDef_0045390c_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL_0045390c calls=%d\n", g_calls);
    return 1;
}
