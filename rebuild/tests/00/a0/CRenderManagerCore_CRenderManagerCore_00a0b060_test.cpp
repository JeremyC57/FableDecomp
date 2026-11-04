// Behaviour test for CRenderManagerCore::~CRenderManagerCore (0x00A0B060): uninitialise the system font only while it is valid.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Font { bool valid; int uninit; bool IsValid() const { return valid; } void Uninitialise() { ++uninit; valid = false; } };
struct Core { Font* SystemFont; ~Core() { if (SystemFont->IsValid()) SystemFont->Uninitialise(); } };
int main()
{
    Font a = { true, 0 }, b = { false, 0 };
    { Core c; c.SystemFont = &a; }
    { Core d; d.SystemFont = &b; }
    bool ok = a.uninit == 1 && b.uninit == 0;
    printf(ok ? "00a0b060_TEST PASS\n" : "00a0b060_TEST FAIL\n");
    return ok ? 0 : 1;
}