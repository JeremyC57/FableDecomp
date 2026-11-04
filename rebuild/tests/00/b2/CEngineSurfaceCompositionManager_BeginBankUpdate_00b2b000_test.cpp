// Behaviour test for CEngineSurfaceCompositionManager::BeginBankUpdate (0x00B2B000): free the shadow buffer, open the
// intermediate bank read/write (create = 1 only when no morphs were generated), then begin the entry update.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[96];
struct Bank { unsigned long create; void Open(unsigned long c) { create = c; strcat(g_log, "open,"); } void Begin() { strcat(g_log, "begin"); } };
static void Dealloc() { strcat(g_log, "shadow,"); }
static void BeginBankUpdate(Bank& b, unsigned long morphs) { Dealloc(); unsigned long create = 0; if (morphs == 0) create = 1; b.Open(create); b.Begin(); }
int main()
{
    Bank a, b;
    BeginBankUpdate(a, 0);
    bool first = strcmp(g_log, "shadow,open,begin") == 0;
    BeginBankUpdate(b, 3);
    bool ok = first && a.create == 1 && b.create == 0;
    printf(ok ? "00b2b000_TEST PASS\n" : "00b2b000_TEST FAIL\n");
    return ok ? 0 : 1;
}