#include <stdio.h>
#include <stddef.h>
// Behaviour test for 0x0099AF10 CWideString::GetAsCharArray: links against the source object.
int main();
struct CWideStringData {
    wchar_t*      text;
    unsigned long unknown04;
    unsigned long unknown08;
    long          owners;
};
wchar_t g_FableEmptyWideString_0129A8E0[1] = {0};
class CWideString {
public:
    const wchar_t* GetAsCharArray() const;
private:
    friend int ::main();
    CWideStringData* storage_;
};
int main()
{
    CWideString value;
    value.storage_ = 0;
    if (value.GetAsCharArray() != g_FableEmptyWideString_0129A8E0) { puts("0099af10_TEST FAIL empty"); return 1; }
    wchar_t text[] = L"names.bin";
    CWideStringData storage = { text, 0, 0, 1 };
    value.storage_ = &storage;
    if (value.GetAsCharArray() != text) { puts("0099af10_TEST FAIL text"); return 2; }
    puts("0099af10_TEST PASS");
    return 0;
}
