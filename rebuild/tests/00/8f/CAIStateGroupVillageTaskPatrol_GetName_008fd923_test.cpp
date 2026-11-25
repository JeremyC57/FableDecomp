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
    CCharString* ctor_99ebf0(const char* lit, int len);
};

static const char* g_lit = 0;
static int g_len = 0;
static CCharString* g_self = 0;

CCharString* CCharString::ctor_99ebf0(const char* lit, int len)
{
    g_self = this; g_lit = lit; g_len = len;
    this->dummy = 0x1234;
    return this;
}

CCharString* __fastcall GetActionName(CCharString* thisret)
{
    thisret->ctor_99ebf0("SG_VILLAGE_TASK_PATROL", -1);
    return thisret;
}

int main()
{
    CCharString obj;
    CCharString* r = GetActionName(&obj);
    if (r == &obj && g_self == &obj && RetailTextEquals(g_lit, "SG_VILLAGE_TASK_PATROL") && g_len == -1)
        printf("008fd923_TEST PASS\n");
    else
        printf("FAIL self=%p lit=%p len=%d\n", g_self, g_lit, g_len);
    return 0;
}