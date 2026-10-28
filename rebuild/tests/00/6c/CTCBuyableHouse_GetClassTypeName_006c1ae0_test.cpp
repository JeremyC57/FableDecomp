#include <cstdio>

class CCharString { public:
    int dummy;
    CCharString(const char* lit, long len);
};

static const char* g_lit = 0;
static int g_len = 0;
static CCharString* g_self = 0;

CCharString::CCharString(const char* lit, long len)
{
    g_self = this; g_lit = lit; g_len = len;
    this->dummy = 0x1234;
}

class CTCBuyableHouse { public:
    CCharString GetClassTypeName();
};

CCharString CTCBuyableHouse::GetClassTypeName()
{
    return CCharString((const char*)0x123a5ec, -1);
}

int main()
{
    CTCBuyableHouse act;
    CCharString r = act.GetClassTypeName();
    if (g_lit == (const char*)0x123a5ec && g_len == -1 && r.dummy == 0x1234)
        printf("006c1ae0_TEST PASS\n");
    else
        printf("FAIL lit=%p len=%d dummy=%x\n", g_lit, g_len, r.dummy);
    return 0;
}