// Test for 0x009D47E0 CBankCreatorException's copy constructor: copies both strings (entry symbol, error text) member-wise,
// as used when the exception object is thrown by value.
#include <stdio.h>
#include <string.h>
static int g_copies = 0;
class CCharString {
public:
    char Text[16];
    CCharString(const char* s) { strcpy(Text, s); }
    CCharString(const CCharString& o) { strcpy(Text, o.Text); ++g_copies; }
};
class CBankCreatorException { public: CCharString EntrySymbol; CCharString ErrorText;
    CBankCreatorException(const char* a, const char* b) : EntrySymbol(a), ErrorText(b) { } };
int main()
{
    CBankCreatorException e("ENTRY_X", "bad crc");
    bool ok = false;
    try { throw e; }
    catch (const CBankCreatorException& c) { ok = strcmp(c.EntrySymbol.Text, "ENTRY_X") == 0 && strcmp(c.ErrorText.Text, "bad crc") == 0; }
    if (g_copies < 2) ok = false;
    puts(ok ? "009d47e0_TEST PASS" : "009d47e0_TEST FAIL");
    return ok ? 0 : 1;
}