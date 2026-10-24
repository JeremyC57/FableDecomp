#include <cstdio>
#include <string.h>

struct _GUID {
    unsigned int   Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};

int IsEqualGUID(const _GUID& rguid1, const _GUID& rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(_GUID));
}

static void Set(_GUID& g, unsigned int d1, unsigned char last)
{
    memset(&g, 0, sizeof(g));
    g.Data1 = d1;
    g.Data2 = 0x1234;
    g.Data3 = 0xABCD;
    g.Data4[7] = last;
}

int main()
{
    _GUID a, b, c, d;
    Set(a, 0xDEADBEEF, 1);
    Set(b, 0xDEADBEEF, 1);
    Set(c, 0xDEADBEEF, 2);
    Set(d, 0xDEADBEEE, 1);
    if (!(IsEqualGUID(a, b) == 1 && IsEqualGUID(a, a) == 1 && IsEqualGUID(a, c) == 0 && IsEqualGUID(a, d) == 0)) { std::printf("00a3aef0_TEST FAIL\n"); return 1; }
    std::printf("00a3aef0_TEST PASS\n");
    return 0;
}