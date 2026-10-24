#include <cstdio>
#include <string.h>

struct _GUID {
    unsigned int   Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};

inline int IsEqualGUID(const _GUID& rguid1, const _GUID& rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(_GUID));
}

inline int operator==(const _GUID& guidOne, const _GUID& guidOther)
{
    return IsEqualGUID(guidOne, guidOther);
}

int operator!=(const _GUID& guidOne, const _GUID& guidOther)
{
    return !(guidOne == guidOther);
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
    if (!((a != b) == 0 && (a != c) == 1 && (a != d) == 1)) { std::printf("00a3af30_TEST FAIL\n"); return 1; }
    std::printf("00a3af30_TEST PASS\n");
    return 0;
}