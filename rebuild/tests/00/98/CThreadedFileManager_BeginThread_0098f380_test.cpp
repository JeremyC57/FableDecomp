// Behaviour test for CThreadedFileManager::BeginThread (0x0098F380): raise the thread priority to above normal (1), then run the
// manager's ThreadMain on the argument.
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
    printf(ok ? "0098f380_TEST PASS\n" : "0098f380_TEST FAIL\n");
    return ok ? 0 : 1;
}