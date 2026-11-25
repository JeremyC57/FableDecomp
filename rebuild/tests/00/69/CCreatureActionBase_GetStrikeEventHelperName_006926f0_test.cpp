#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

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
    return CCharString("", -1);
}

int main()
{
    CCreatureActionBase act;
    CCharString r = act.GetStrikeEventHelperName();
    if (RetailTextEquals(g_lit, "") && g_len == -1 && r.dummy == 0x1234)
        printf("006926f0_TEST PASS\n");
    else
        printf("FAIL lit=%p len=%d dummy=%x\n", g_lit, g_len, r.dummy);
    return 0;
}