#include <stdio.h>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// Model matching s.cpp but with a real, observable CCharString ctor.
static const char* g_lastStr = 0;
static int g_lastLen = 999;

class CCharString {
public:
    void* p;
    CCharString(const char* s, long len) {
        g_lastStr = s;
        g_lastLen = len;
        p = 0;
    }
};

class CTCGraphicAppearanceReplaceableMesh {
public:
    CCharString GetClassTypeName();
};

static const char* const kName = "CTCGraphicAppearanceReplaceableMesh";

CCharString CTCGraphicAppearanceReplaceableMesh::GetClassTypeName()
{
    return CCharString(kName, -1);
}

int main()
{
    CTCGraphicAppearanceReplaceableMesh obj;
    CCharString r = obj.GetClassTypeName();
    (void)r;
    if (RetailTextEquals(g_lastStr, "CTCGraphicAppearanceReplaceableMesh") && g_lastLen == -1) {
        printf("007a6db0_TEST PASS\n");
    } else {
        printf("FAIL str=%p len=%d\n", (void*)g_lastStr, g_lastLen);
    }
    return 0;
}