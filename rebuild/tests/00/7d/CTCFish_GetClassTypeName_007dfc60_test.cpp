#include <cstdio>

// Standalone behaviour test. We provide a local CCharString whose ctor
// records the string pointer and length it was called with, then verify
// GetActionName forwards the fixed literal + (-1).

static const char* g_lastStr;
static int g_lastLen;

class CCharString {
public:
    CCharString(const char* s, int n) { g_lastStr = s; g_lastLen = n; p = (void*)s; }
    void* p;
};

class CTCFish {
public:
    CCharString GetClassTypeName();
};

CCharString CTCFish::GetClassTypeName()
{
    return CCharString((const char*)0x0123d7b0, -1);
}

int main()
{
    CTCFish a;
    CCharString r = a.GetClassTypeName();
    if (g_lastStr == (const char*)0x0123d7b0 && g_lastLen == -1 && r.p == (void*)0x0123d7b0)
        printf("OK str=%p len=%d\n", g_lastStr, g_lastLen);
    else
        printf("FAIL\n");
    return 0;
}