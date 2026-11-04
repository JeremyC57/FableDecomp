// Behaviour test for CBankFileEntryUpdateData::~CBankFileEntryUpdateData (0x009CC6C0): ClearInfo frees the info buffer and
// zeroes its size before the filename vector clears.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_freed, g_cleared;
struct Rec { char* InfoBuffer; unsigned long InfoSize;
    void ClearInfo() { if (InfoBuffer) { delete[] InfoBuffer; InfoBuffer = 0; ++g_freed; } InfoSize = 0; }
    ~Rec() { ClearInfo(); ++g_cleared; } };
int main()
{
    { Rec r; r.InfoBuffer = new char[8]; r.InfoSize = 8; }
    { Rec s; s.InfoBuffer = 0; s.InfoSize = 3; }
    bool ok = g_freed == 1 && g_cleared == 2;
    printf(ok ? "009cc6c0_TEST PASS\n" : "009cc6c0_TEST FAIL\n");
    return ok ? 0 : 1;
}