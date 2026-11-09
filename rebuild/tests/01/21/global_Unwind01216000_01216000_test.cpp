// Behaviour test for CMasterObject_00c4c8b0::LoadSlave (0x00C4CAB0): warn when finalised, load count*stride bytes; a
// failed load deletes the slave and resets, a good one commits.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_traces;
static void Trace(int line) { (void)line; ++g_traces; }
struct Master { bool finalised; int resets, commits;
    void LoadSlave(bool loadOk, unsigned long count, unsigned long stride, unsigned long* size) {
        if (finalised) Trace(124); *size = count * stride; if (!loadOk) ++resets; else ++commits; } };
int main()
{
    Master m = { false, 0, 0 }; unsigned long size = 0;
    m.LoadSlave(true, 4, 8, &size);
    bool a = size == 32 && m.commits == 1 && g_traces == 0;
    m.finalised = true; m.LoadSlave(false, 1, 2, &size);
    bool ok = a && m.resets == 1 && g_traces == 1;
    printf(ok ? "01216000_TEST PASS\n" : "00c4cab0_TEST FAIL\n");
    return ok ? 0 : 1;
}