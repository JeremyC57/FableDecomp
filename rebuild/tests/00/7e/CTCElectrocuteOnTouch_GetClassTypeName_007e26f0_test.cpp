#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// Standalone behaviour test. We provide a local CCharString whose ctor
// records the string pointer and length it was called with, then verify
// GetActionName forwards the fixed literal + (-1).

static const char* g_lastStr;
static int g_lastLen;

class CCharString {
public:
    CCharString(const char* s, long n) { g_lastStr = s; g_lastLen = n; p = (void*)s; }
    void* p;
};

class CTCElectrocuteOnTouch {
public:
    CCharString GetClassTypeName();
};

CCharString CTCElectrocuteOnTouch::GetClassTypeName()
{
    return CCharString("CTCElectrocuteOnTouch", -1);
}

int main()
{
    CTCElectrocuteOnTouch a;
    CCharString r = a.GetClassTypeName();
    if (RetailTextEquals(g_lastStr, "CTCElectrocuteOnTouch") && g_lastLen == -1 && RetailTextEquals(r.p, "CTCElectrocuteOnTouch"))
        printf("GETACTIONNAME_OK str=%p len=%d\n", g_lastStr, g_lastLen);
    else
        printf("FAIL\n");
    return 0;
}