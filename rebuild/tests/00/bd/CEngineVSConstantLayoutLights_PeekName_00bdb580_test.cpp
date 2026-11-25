#include <cstdio>
#include <cstring>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// Independent behaviour test. We provide a local CCharString ctor that copies
// the pointer/length so we can observe PeekName wiring the literal + (-1).

class CCharString { public:
    char* m_data;
    int   m_len;
    CCharString(const char* s, long len) { m_data = (char*)s; m_len = len; }
};

class CEngineVSConstantLayoutLights {
public:
    CCharString PeekName();
};

CCharString CEngineVSConstantLayoutLights::PeekName()
{
    return CCharString("LayoutLights", -1);
}

int main()
{
    CEngineVSConstantLayoutLights obj;
    CCharString s = obj.PeekName();
    if (RetailTextEquals(s.m_data, "LayoutLights") && s.m_len == -1)
        printf("PEEKNAME_OK\n");
    else
        printf("PEEKNAME_FAIL %p %d\n", (void*)s.m_data, s.m_len);
    return (RetailTextEquals(s.m_data, "LayoutLights") && s.m_len == -1) ? 0 : 1;
}