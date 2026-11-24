#include <cstdio>

class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);
};

class CWifeDefLayout { public:
    char pad[0x28];
    int f28;
    void base_clone(const CDefClassBase* src);
};

static int g_base_called = 0;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other_)
{
    const CDefClassBase* src = (const CDefClassBase*)&other_; (void)src;
    g_base_called = 1;
    (void)src;
    return *this;
}

// NF335 (NF324 Q2): the row now defines the PDB member CWifeDef::Copy
// (?Copy@CWifeDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CWifeDef { public:
    virtual void Copy(const CDefClassBase*);
};

int main() {
    CWifeDefLayout dst; dst.f28 = 0x11111111;
    CWifeDefLayout srcObj; srcObj.f28 = 0x7A6B5C4D;
    ((CWifeDef*)(&dst))->CWifeDef::Copy((const CDefClassBase*)&srcObj);
    if (g_base_called == 1 && dst.f28 == 0x7A6B5C4D) {
        printf("004e01c0_TEST PASS\n");
    } else {
        printf("FAIL base=%d f28=%08x\n", g_base_called, (unsigned)dst.f28);
    }
    return 0;
}