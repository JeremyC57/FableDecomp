#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

static const char* g_captured_s;
static int g_captured_n;

class CCharString { public:
    char* buf;
    CCharString(const char* s, long n) {
        g_captured_s = s;
        g_captured_n = n;
        buf = (char*)s;
    }
    CCharString(const CCharString& o) { buf = o.buf; }
};

struct CActionDoCreatureAction {
    int _pad;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("CActionDoCreatureAction", -1);
}

int main()
{
    CActionDoCreatureAction obj; obj._pad = 0;
    CCharString r = obj.GetActionName();
    bool ok = (RetailTextEquals(g_captured_s, "CActionDoCreatureAction"))
           && (g_captured_n == -1)
           && (RetailTextEquals(r.buf, "CActionDoCreatureAction"));
    if (ok) printf("00718db0_TEST PASS\n");
    else printf("FAIL s=%p n=%d buf=%p\n", g_captured_s, g_captured_n, r.buf);
    return 0;
}