// Behaviour test for CFontDataBank::~CFontDataBank (0x00A63680): CloseFonts runs before the members and the bank base.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[64];
struct Bank { ~Bank() { strcat(g_log, "bank"); } };
struct Name { ~Name() { strcat(g_log, "name,"); } };
struct FontBank : Bank { Name Filename; void CloseFonts() { strcat(g_log, "close,"); } ~FontBank() { CloseFonts(); } };
int main()
{
    { FontBank f; }
    bool ok = strcmp(g_log, "close,name,bank") == 0;
    printf(ok ? "00a63680_TEST PASS\n" : "00a63680_TEST FAIL\n");
    return ok ? 0 : 1;
}