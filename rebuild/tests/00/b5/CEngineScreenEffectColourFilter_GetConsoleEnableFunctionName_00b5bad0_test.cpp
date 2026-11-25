#include <stdio.h>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString { public:
    char* m_data;
};

static const char* g_last_str = 0;
static int g_last_len = 0;

// Local stand-in for the real ctor at 0x0099ebf0.
CCharString* __fastcall CCharString_ctor(CCharString* self, int /*edx*/, const char* str, int len)
{
    g_last_str = str;
    g_last_len = len;
    self->m_data = (char*)str;
    return self;
}

class CEngineScreenEffectColourFilter { public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineScreenEffectColourFilter::GetConsoleEnableFunctionName()
{
    CCharString ret;
    CCharString_ctor(&ret, 0, "EnableScreenEffectColourFilter", -1);
    return ret;
}

int main()
{
    CEngineScreenEffectColourFilter obj;
    CCharString s = obj.GetConsoleEnableFunctionName();

    int ok = 1;
    if (g_last_len != -1) ok = 0;
    if (!RetailTextEquals(g_last_str, "EnableScreenEffectColourFilter")) ok = 0;
    if (s.m_data != g_last_str) ok = 0;

    if (ok) printf("COLOURFILTER_CONSOLEENABLE_OK\n");
    else    printf("FAIL str=%p len=%d\n", (void*)s.m_data, g_last_len);
    return ok ? 0 : 1;
}