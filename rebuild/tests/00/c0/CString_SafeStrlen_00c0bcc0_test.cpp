#include <stdio.h>
static int g_calls; static const char* g_arg;
static int __stdcall StubLstrlenA(const char* t) { ++g_calls; g_arg = t; int n = 0; while (t[n]) ++n; return n; }
#define lstrlenA StubLstrlenA
class CString {
public:
    char* m_pchData;
    static int __stdcall SafeStrlen(const char* lpsz);
};

int __stdcall CString::SafeStrlen(const char* lpsz)
{
    return (lpsz == 0) ? 0 : lstrlenA(lpsz);
}

int main()
{
    const char* s = "hello";
    bool ok = CString::SafeStrlen(s) == 5 && g_calls == 1 && g_arg == s;
    ok = ok && CString::SafeStrlen(0) == 0 && g_calls == 1;
    ok = ok && CString::SafeStrlen("") == 0 && g_calls == 2;
    printf(ok ? "00c0bcc0_TEST PASS\n" : "00c0bcc0_TEST FAIL\n");
    return ok ? 0 : 1;
}