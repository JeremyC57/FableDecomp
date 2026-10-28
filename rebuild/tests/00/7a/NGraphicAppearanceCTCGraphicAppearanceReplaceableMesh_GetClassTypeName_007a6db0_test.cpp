#include <stdio.h>

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

static const char* const kName = (const char* const)0x0123c49c;

CCharString CTCGraphicAppearanceReplaceableMesh::GetClassTypeName()
{
    return CCharString(kName, -1);
}

int main()
{
    CTCGraphicAppearanceReplaceableMesh obj;
    CCharString r = obj.GetClassTypeName();
    (void)r;
    if (g_lastStr == (const char*)0x0123c49c && g_lastLen == -1) {
        printf("007a6db0_TEST PASS\n");
    } else {
        printf("FAIL str=%p len=%d\n", (void*)g_lastStr, g_lastLen);
    }
    return 0;
}