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
    void* p;
    CCharString(const char* s, long n);
};
class CTCScriptedControl { public:
    CCharString GetClassTypeName();
};

static const char* g_s; static int g_n; static int g_calls;
CCharString::CCharString(const char* s, long n){ g_s=s; g_n=n; g_calls++; p=(void*)s; }

int main(){
    CTCScriptedControl obj;
    CCharString r = obj.GetClassTypeName();
    if (g_calls==1 && RetailTextEquals(g_s, "CTCScriptedControl") && g_n==-1 && RetailTextEquals(r.p, "CTCScriptedControl"))
        printf("00719a50_TEST PASS\n");
    else
        printf("FAIL calls=%d s=%p n=%d\n", g_calls, g_s, g_n);
    return 0;
}