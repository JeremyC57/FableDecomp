#include <cstdio>

class CDefClassBase;

class CThingSwitchDefLayout { public:
    char pad[0x70];
    int f70;
};

// NF335 (NF324 Q2): the row now defines the PDB member CThingSwitchDef::Copy
// (?Copy@CThingSwitchDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol. The row's base-class copy
// callee base_clone stays a member of the class (the row binds ?base_clone@CThingSwitchDef@@QAEXPBVCDefClassBase@@@Z), so
// the model declares it too and the stub below defines it.
class CThingSwitchDef { public:
    void base_clone(const CDefClassBase* src);
    virtual void Copy(const CDefClassBase*);
};

static void* g_self;
static const void* g_src;

void CThingSwitchDef::base_clone(const CDefClassBase* src)
{
    g_self = this;
    g_src = src;
}

int main()
{
    CThingSwitchDefLayout dst; dst.f70 = 0;
    CThingSwitchDefLayout srcObj; srcObj.f70 = 0x12345678;
    ((CThingSwitchDef*)(&dst))->CThingSwitchDef::Copy((const CDefClassBase*)&srcObj);
    if (dst.f70 == 0x12345678 && g_self == &dst && g_src == (const void*)&srcObj)
        printf("004644fa_TEST PASS\n");
    else
        printf("FAIL %x\n", dst.f70);
    return 0;
}