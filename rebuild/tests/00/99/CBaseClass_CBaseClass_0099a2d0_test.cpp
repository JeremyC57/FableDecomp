#include <stdio.h>
#include <string.h>
#include <new>
static int g_dtor;
class CBaseClass {
public:
    CBaseClass();
    virtual ~CBaseClass() { ++g_dtor; }
};

CBaseClass::CBaseClass()
{
}

int main()
{
    CBaseClass ref;
    void* vt; memcpy(&vt, &ref, 4);
    unsigned char raw[sizeof(CBaseClass)]; memset(raw, 0xcd, sizeof raw);
    CBaseClass* p = new (raw) CBaseClass;
    void* got; memcpy(&got, raw, 4);
    bool ok = p == (CBaseClass*)raw && got == vt && sizeof(CBaseClass) == 4;
    CBaseClass* q = p; q->~CBaseClass();
    ok = ok && g_dtor == 1;
    printf(ok ? "0099a2d0_TEST PASS\n" : "0099a2d0_TEST FAIL\n");
    return ok ? 0 : 1;
}