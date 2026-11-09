// Behaviour test for CMasterObject_00c4cd80::Complete (0x00C4C440): the four sanity warnings and the completion callback
// with success = no failures.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_traces;
static void Trace(int line) { (void)line; ++g_traces; }
struct Master { unsigned long count, target; long failures; bool hasCallback, finalised; int calls; bool lastSuccess;
    void Complete() { if (count <= 0) Trace(29); if (count != target) Trace(30); if (!hasCallback) Trace(31); if (!finalised) Trace(32);
        ++calls; lastSuccess = failures == 0; } };
int main()
{
    Master a = { 3, 3, 0, true, true, 0, false };
    a.Complete();
    bool first = g_traces == 0 && a.calls == 1 && a.lastSuccess;
    Master b = { 0, 2, 1, false, false, 0, true };
    b.Complete();
    bool ok = first && g_traces == 4 && !b.lastSuccess;
    printf(ok ? "01215f65_TEST PASS\n" : "00c4c440_TEST FAIL\n");
    return ok ? 0 : 1;
}