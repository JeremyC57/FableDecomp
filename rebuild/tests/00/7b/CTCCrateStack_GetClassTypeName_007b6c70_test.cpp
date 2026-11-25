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
    char* p;
    CCharString(const char* s, long len);
};

class CTCCrateStack { public:
    CCharString GetClassTypeName();
};

// Provide the ctor body so behavior can be exercised without touching 0x123c8d0.
static const char* g_lastStr = 0;
static int g_lastLen = 0;
CCharString::CCharString(const char* s, long len) {
    g_lastStr = s;
    g_lastLen = len;
    p = (char*)s;
}

CCharString CTCCrateStack::GetClassTypeName()
{
    return CCharString("CTCCrateStack", -1);
}

int main()
{
    CTCCrateStack obj;
    CCharString r = obj.GetClassTypeName();
    bool ok = (RetailTextEquals(g_lastStr, "CTCCrateStack"))
           && (g_lastLen == -1)
           && (RetailTextEquals(r.p, "CTCCrateStack"));
    if (ok) printf("007b6c70_TEST PASS\n");
    else    printf("007b6c70_TEST FAIL str=%p len=%d p=%p\n", g_lastStr, g_lastLen, r.p);
    return 0;
}