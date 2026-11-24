// behaviour test for 0x007ADEF0: CAIScratchpadDef::Copy forwards to CAIScratchpadDef::operator= with this and the source object
// NF386 (NF324 Q2 form, NF337 gen.py): the row now defines the PDB member CAIScratchpadDef::Copy (?Copy@CAIScratchpadDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee operator= (??4CAIScratchpadDef@@QAEAAV0@ABV0@@Z, out of line in the row) is stubbed as a member here.
#include <cstdio>

class CDefClassBase;
class CAIScratchpadDef { public:
    virtual void Copy(const CDefClassBase* other);
    CAIScratchpadDef& operator=(const CAIScratchpadDef& other);
};

static int g_calls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
CAIScratchpadDef& CAIScratchpadDef::operator=(const CAIScratchpadDef& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}

static unsigned long g_dst[4];
static unsigned long g_src[4];

int main()
{
    ((CAIScratchpadDef*)(g_dst))->CAIScratchpadDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src) {
        std::printf("PASS_007adef0\n");
        return 0;
    }
    std::printf("FAIL_007adef0 calls=%d\n", g_calls);
    return 1;
}
