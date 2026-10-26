#include <cstdio>

class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);
};

class CWifeDef { public:
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

void __fastcall CWifeDef_Copy(CWifeDef* self, int /*edx*/, const CDefClassBase* src);

int main() {
    CWifeDef dst; dst.f28 = 0x11111111;
    CWifeDef srcObj; srcObj.f28 = 0x7A6B5C4D;
    CWifeDef_Copy(&dst, 0, (const CDefClassBase*)&srcObj);
    if (g_base_called == 1 && dst.f28 == 0x7A6B5C4D) {
        printf("004e01c0_TEST PASS\n");
    } else {
        printf("FAIL base=%d f28=%08x\n", g_base_called, (unsigned)dst.f28);
    }
    return 0;
}