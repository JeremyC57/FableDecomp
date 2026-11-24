// behaviour test for 0x004E01A7: CBuyHouseDef::Copy calls CDefClassBase::operator= with this and the source object, then copies Main (+0x25)
// NF337 (NF324 Q2 form): the row now defines the PDB member CBuyHouseDef::Copy (?Copy@CBuyHouseDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee CDefClassBase::operator= (??4CDefClassBase@@QAEAAV0@ABV0@@Z) is stubbed as a member here.
#include <cstdio>

class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);
};
class CBuyHouseDef { public:
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
    *(unsigned char*)((char*)g_src + 0x25) = 0x5A;
    ((CBuyHouseDef*)(g_dst))->CBuyHouseDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src && *(unsigned char*)((char*)g_dst + 0x25) == 0x5A) {
        std::printf("004e01a7_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL_004e01a7 calls=%d\n", g_calls);
    return 1;
}
