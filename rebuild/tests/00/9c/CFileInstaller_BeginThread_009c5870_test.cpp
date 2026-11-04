// Behaviour test for CFileInstaller::BeginThread (0x009C5870): raise the thread priority to above normal (1), then run the
// installer's ThreadMain on the argument.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_priority = -9;
struct Installer { int ran; void ThreadMain() { ++ran; } };
static void SetPriority(int p) { g_priority = p; }
static void BeginThread(void* p) { SetPriority(1); ((Installer*)p)->ThreadMain(); }
int main()
{
    Installer i = { 0 };
    BeginThread(&i);
    bool ok = g_priority == 1 && i.ran == 1;
    printf(ok ? "009c5870_TEST PASS\n" : "009c5870_TEST FAIL\n");
    return ok ? 0 : 1;
}