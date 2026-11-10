// Behaviour test for LHNetworkLib::LSocket::CheckForActivity (0x008F8540): clears the global activity flag; an empty list is
// false; otherwise every socket goes into one set passed as the read, write or exception set by mode (timeout 50us when 0,
// else ms*1000; none when -1), each socket's pending flag is cleared, then set for sockets select reports. -1 from select is false.
#include <stdio.h>

enum LSOCKET_SELECT { LSS_READ, LSS_WRITE, LSS_EXCEPTION };
struct LSocket { int Socket; bool ActivityPending; };
struct SocketList { LSocket* items[8]; int n; };   // stands in for the std::list<LSocket*> argument (no <list>: C4530 under /W3 without /EHsc)
struct Set { int fds[16]; int n; };
static bool g_LSocketActivityDetected = true;
static int g_selResult; static int g_ready; static int g_whichSet; static long g_usec; static bool g_noTimeout;
static int FakeSelect(Set* r, Set* w, Set* e, long* usec)
{
    g_whichSet = r ? 0 : w ? 1 : e ? 2 : -1;
    g_noTimeout = usec == 0; g_usec = usec ? *usec : -1;
    Set* s = r ? r : w ? w : e;
    if (g_selResult > 0) { int keep = 0; for (int i = 0; i < s->n; ++i) if (s->fds[i] == g_ready) s->fds[keep++] = g_ready; s->n = keep; }
    return g_selResult;
}
static bool IsSet(const Set& s, int fd) { for (int i = 0; i < s.n; ++i) if (s.fds[i] == fd) return true; return false; }
static bool CheckForActivity(SocketList& sockets, LSOCKET_SELECT mode, unsigned long timeoutMs)
{
    g_LSocketActivityDetected = false;
    if (sockets.n == 0) return false;
    long usec = timeoutMs == 0 ? 50 : (long)(timeoutMs * 1000);
    Set set; set.n = 0;
    Set* readSet; Set* writeSet; Set* exceptSet;
    exceptSet = readSet = writeSet = 0;
    switch (mode) { case LSS_READ: readSet = &set; break; case LSS_WRITE: writeSet = &set; break; case LSS_EXCEPTION: exceptSet = &set; break; }
    for (int i = 0; i < sockets.n; ++i) { set.fds[set.n++] = sockets.items[i]->Socket; sockets.items[i]->ActivityPending = false; }
    int result = timeoutMs == (unsigned long)-1 ? FakeSelect(readSet, writeSet, exceptSet, 0) : FakeSelect(readSet, writeSet, exceptSet, &usec);
    if (result == -1) return false;
    if (result > 0) {
        for (int i = 0; i < sockets.n; ++i)
            if (IsSet(set, sockets.items[i]->Socket)) sockets.items[i]->ActivityPending = true;
        g_LSocketActivityDetected = true;
    }
    return true;
}
int main()
{
    int fails = 0;
    SocketList none; none.n = 0;
    if (CheckForActivity(none, LSS_READ, 0) || g_LSocketActivityDetected) { printf("empty FAIL\n"); ++fails; }
    LSocket a = { 3, true }, b = { 5, true };
    SocketList l; l.items[0] = &a; l.items[1] = &b; l.n = 2;
    g_selResult = 1; g_ready = 5;
    if (!CheckForActivity(l, LSS_WRITE, 0) || g_whichSet != 1 || g_usec != 50 || a.ActivityPending || !b.ActivityPending || !g_LSocketActivityDetected) { printf("write FAIL\n"); ++fails; }
    g_selResult = 0;
    if (!CheckForActivity(l, LSS_EXCEPTION, 2) || g_whichSet != 2 || g_usec != 2000 || a.ActivityPending || b.ActivityPending || g_LSocketActivityDetected) { printf("timeout FAIL\n"); ++fails; }
    g_selResult = -1;
    if (CheckForActivity(l, LSS_READ, (unsigned long)-1) || g_whichSet != 0 || !g_noTimeout) { printf("error FAIL\n"); ++fails; }
    if (fails) { printf("008f8540_TEST FAIL\n"); return 1; }
    printf("008f8540_TEST PASS\n");
    return 0;
}