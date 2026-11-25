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

CCharString::CCharString(const char* lit, long len)
{
    g_lit = lit; g_len = len;
    this->dummy = 0x1234;
}

class CTCDayOrNightOnlySupport { public:
    CCharString GetClassTypeName();
};

CCharString CTCDayOrNightOnlySupport::GetClassTypeName()
{
    return CCharString("CTCDayOrNightOnlySupport", -1);
}

int main()
{
    CTCDayOrNightOnlySupport act;
    CCharString r = act.GetClassTypeName();
    if (RetailTextEquals(g_lit, "CTCDayOrNightOnlySupport") && g_len == -1 && r.dummy == 0x1234)
        printf("00702bd0_TEST PASS\n");
    else
        printf("FAIL lit=%p len=%d dummy=%x\n", g_lit, g_len, r.dummy);
    return 0;
}