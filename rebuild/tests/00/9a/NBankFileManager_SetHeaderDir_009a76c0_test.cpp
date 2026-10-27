// Test for 0x009A76C0 NBankFileManager::SetHeaderDir: the bank header directory global takes a copy of the argument.
#include <stdio.h>
#include <string.h>
class CWideString {
public:
    wchar_t Data[64];
    CWideString() { Data[0] = 0; }
    CWideString& operator=(const CWideString& o) { memcpy(Data, o.Data, sizeof(Data)); return *this; }
};
static CWideString g_HeaderDir;
namespace NBankFileManager { void SetHeaderDir(const CWideString& dir) { g_HeaderDir = dir; } }
int main()
{
    CWideString d; d.Data[0] = L'B'; d.Data[1] = L'k'; d.Data[2] = 0;
    NBankFileManager::SetHeaderDir(d);
    bool ok = g_HeaderDir.Data[0] == L'B' && g_HeaderDir.Data[1] == L'k' && g_HeaderDir.Data[2] == 0;
    d.Data[0] = L'X';
    if (g_HeaderDir.Data[0] != L'B') ok = false;
    puts(ok ? "009a76c0_TEST PASS" : "009a76c0_TEST FAIL");
    return ok ? 0 : 1;
}