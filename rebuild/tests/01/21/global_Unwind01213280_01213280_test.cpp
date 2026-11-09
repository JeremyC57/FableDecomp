#include <cstdio>
// Model of the libpk stream-load handler's Process(request): under the global lock the request's pool block is unlocked; a flagged,
// active request larger than the 0x24-byte sample header copies the header out of the locked block, finds or creates the bank's
// sample (a new sample gets a stream buffer: attach, finish loading, and on failure release + delete it and drop the sample), finds
// or creates the sample's driver from the header's play parameters, and starts the sound; the request is always released and deleted.
static int g_log[32]; static int g_n = 0;
static void L(int v) { g_log[g_n++] = v; }
struct Req { int flag; bool active; unsigned size; bool haveSample, haveDriver, formatOk, finishOk, paramsOk; int handle; };
static int g_deleted = 0;
static void Process(Req* r)
{
    L(1);                                              // unlock
    if (r->flag != 0 && r->active && r->size > 0x24) {
        L(2); L(3);                                    // lock, copy header, unlock
        bool sample = r->haveSample;
        if (!sample) {
            if (r->formatOk) {
                L(4); sample = true; r->handle = -1;   // create sample + buffer
                if (!r->finishOk) { L(5); sample = false; }
            }
        }
        if (sample) {
            bool driver = r->haveDriver;
            if (!driver && r->paramsOk) { L(6); driver = true; }
            if (driver) L(7);                          // start sound
        }
    }
    L(8); ++g_deleted;
}
int main()
{
    int bad = 0;
    Req a = { 1, true, 0x100, false, false, true, true, true, 5 };
    Process(&a);
    int e1[] = { 1, 2, 3, 4, 6, 7, 8 };
    if (g_n != 7) bad = 1; else for (int i = 0; i < 7; ++i) if (g_log[i] != e1[i]) bad = 1;
    if (a.handle != -1) bad = 1;
    g_n = 0; Req b = { 1, true, 0x24, false, false, true, true, true, 5 };          // header-sized request: nothing loaded
    Process(&b);
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 8 || b.handle != 5) bad = 1;
    g_n = 0; Req c = { 1, true, 0x100, false, false, true, false, true, 5 };        // finish fails: buffer dropped, no sound
    Process(&c);
    int e3[] = { 1, 2, 3, 4, 5, 8 };
    if (g_n != 6) bad = 1; else for (int i = 0; i < 6; ++i) if (g_log[i] != e3[i]) bad = 1;
    g_n = 0; Req d = { 0, true, 0x100, true, true, true, true, true, 5 };           // unflagged: release only
    Process(&d);
    if (g_n != 2 || g_deleted != 4) bad = 1;
    std::printf(bad ? "00c267e0_TEST FAIL\n" : "01213280_TEST PASS\n");
    return bad;
}