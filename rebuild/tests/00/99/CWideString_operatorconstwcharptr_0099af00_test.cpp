#include <stdio.h>
// Behaviour test for 0x0099AF00 CWideString::operator const wchar_t*: links against the source object.
#include "fable_string.h"

wchar_t g_FableEmptyWideString_0129A8E0[1] = {0};

int main()
{
    fable_u8 objectBytes[sizeof(CWideString)];
    CWideString* value = reinterpret_cast<CWideString*>(objectBytes);

    *reinterpret_cast<CWideStringData**>(value) = 0;
    if (static_cast<const wchar_t*>(*value) != g_FableEmptyWideString_0129A8E0) { puts("CCHARSTRING_OPERATOR_CHARPTR_0099AF00_TEST FAIL empty"); return 1; }

    wchar_t text[] = L"names.bin";
    CWideStringData storage;
    storage.text = text;
    storage.unknown04 = 0;
    storage.unknown08 = 0;
    storage.owners = 1;
    *reinterpret_cast<CWideStringData**>(value) = &storage;
    if (static_cast<const wchar_t*>(*value) != text) { puts("CCHARSTRING_OPERATOR_CHARPTR_0099AF00_TEST FAIL text"); return 2; }

    puts("CCHARSTRING_OPERATOR_CHARPTR_0099AF00_TEST PASS");
    return 0;
}
