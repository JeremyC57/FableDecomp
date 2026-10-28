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

class CCreatureActionBase { public:
    CCharString GetStrikeEventHelperName();
};

CCharString CCreatureActionBase::GetStrikeEventHelperName()
{
    return CCharString((const char*)0x122d70e, -1);
}

int main()
{
    CCreatureActionBase act;
    CCharString r = act.GetStrikeEventHelperName();
    if (g_lit == (const char*)0x122d70e && g_len == -1 && r.dummy == 0x1234)
        printf("006926f0_TEST PASS\n");
    else
        printf("FAIL lit=%p len=%d dummy=%x\n", g_lit, g_len, r.dummy);
    return 0;
}