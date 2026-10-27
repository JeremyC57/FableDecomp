// Test for 0x00A44690 CSuspendableProcessMicrothread::InternalSuspend: yields the running microthread exactly once.
#include <stdio.h>
static int g_sleeps = 0;
namespace MicroThread { void MicroThreadSleep() { ++g_sleeps; } }
class CSuspendableProcessMicrothread {
public:
    static void Suspend() { InternalSuspend(); }
protected:
    static void InternalSuspend() { MicroThread::MicroThreadSleep(); }
};
int main()
{
    CSuspendableProcessMicrothread::Suspend();
    bool ok = g_sleeps == 1;
    CSuspendableProcessMicrothread::Suspend();
    if (g_sleeps != 2) ok = false;
    puts(ok ? "00a44690_TEST PASS" : "00a44690_TEST FAIL");
    return ok ? 0 : 1;
}