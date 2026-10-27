// Test for 0x0099CF70 CSystemRegistry::~CSystemRegistry: the key is closed exactly when it is open; the root is never closed.
#include <stdio.h>
static int g_closed = 0; static void* g_last = 0;
static int RegCloseKeyMock(void* k) { ++g_closed; g_last = k; return 0; }
class CSystemRegistry {
public:
    ~CSystemRegistry() { if (hKey) RegCloseKeyMock(hKey); }
    void* hRoot;
    void* hKey;
};
int main()
{
    int a, b;
    { CSystemRegistry r; r.hRoot = &a; r.hKey = &b; }
    bool ok = g_closed == 1 && g_last == &b;
    { CSystemRegistry r; r.hRoot = &a; r.hKey = 0; }
    if (g_closed != 1) ok = false;
    puts(ok ? "0099cf70_TEST PASS" : "0099cf70_TEST FAIL");
    return ok ? 0 : 1;
}