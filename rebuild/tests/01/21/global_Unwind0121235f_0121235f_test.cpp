// Behaviour test for PKCDefaultFileManager::AsyncReadThread (0x00C1A9C0): each request is read and completed (failed on a
// short read), its file is closed, and a key -1 request ends the loop and releases the done semaphore.
#include <stdio.h>
struct Request { unsigned long key, size, got; };
static Request queue[3] = { { 1, 8, 8 }, { 2, 8, 4 }, { (unsigned long)-1, 0, 0 } };
static int next, completedOk, completedBad, closed, released;
static void Run()
{
    bool quit = false;
    do {
        Request r = queue[next++];
        if (r.key == (unsigned long)-1)
            quit = true;
        else {
            bool success = r.got == r.size;
            if (success) ++completedOk; else ++completedBad;
            ++closed;
        }
    } while (!quit);
    ++released;
}
int main()
{
    Run();
    bool ok = completedOk == 1 && completedBad == 1 && closed == 2 && released == 1 && next == 3;
    printf(ok ? "0121235f_TEST PASS\n" : "00c1a9c0_TEST FAIL\n");
    return ok ? 0 : 1;
}