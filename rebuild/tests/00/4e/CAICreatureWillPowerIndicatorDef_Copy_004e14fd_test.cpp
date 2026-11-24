// behaviour test for 0x004E14FD: CAICreatureWillPowerIndicatorDef::Copy calls CDefClassBase::operator= with this and the source object, then copies RechargeTime (+0x28)
// NF337 (NF324 Q2 form): the row now defines the PDB member CAICreatureWillPowerIndicatorDef::Copy (?Copy@CAICreatureWillPowerIndicatorDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee CDefClassBase::operator= (??4CDefClassBase@@QAEAAV0@ABV0@@Z) is stubbed as a member here.
#include <cstdio>

class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);
};
class CAICreatureWillPowerIndicatorDef { public:
    virtual void Copy(const CDefClassBase* other);
};

static int g_calls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}

static unsigned long g_dst[12];
static unsigned long g_src[12];

int main()
{
    *(unsigned long*)((char*)g_src + 0x28) = 0x7A6B5C4DUL;
    ((CAICreatureWillPowerIndicatorDef*)(g_dst))->CAICreatureWillPowerIndicatorDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src && *(unsigned long*)((char*)g_dst + 0x28) == 0x7A6B5C4DUL) {
        std::printf("004e14fd_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL_004e14fd calls=%d\n", g_calls);
    return 1;
}
