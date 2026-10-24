#include <stdio.h>
struct timeval_t { long tv_sec; long tv_usec; };
struct fdset_t { unsigned int fd_count; unsigned int fd_array[64]; };
static int g_selResult = 0;
static long g_lastUsec = 0;
static int g_lastWhich = 0;
static int sel(int nfds, fdset_t* r, fdset_t* w, fdset_t* e, const timeval_t* t)
{
    (void)nfds; (void)e;
    g_lastUsec = t->tv_usec;
    g_lastWhich = r ? 1 : (w ? 2 : 0);
    if (g_selResult <= 0) { if (r) r->fd_count = 0; if (w) w->fd_count = 0; }
    return g_selResult;
}
static int isset(unsigned int fd, fdset_t* set)
{
    unsigned int i = 0;
    for (i = 0; i < set->fd_count; ++i)
        if (set->fd_array[i] == fd) return 1;
    return 0;
}
#define FD_ZERO_T(set) ((set)->fd_count = 0)
#define FD_SET_T(fd, set) do { if ((set)->fd_count < 64) (set)->fd_array[(set)->fd_count++] = (fd); } while (0)
static int g_nullTimeout = 0;
class LSocket {
public:
    unsigned int Socket;
    bool SelectRead(timeval_t* timeout)
    {
        fdset_t readSet;
        FD_ZERO_T(&readSet);
        FD_SET_T(Socket, &readSet);
        timeval_t dummy;
        dummy.tv_sec = 0; dummy.tv_usec = -7;
        if (timeout == 0) ++g_nullTimeout;
        int result = sel(0, &readSet, 0, 0, timeout ? timeout : &dummy);
        if (result == -1)
            return false;
        if (result == 1 && isset(Socket, &readSet))
            return true;
        return false;
    }
    bool IsReadable(long timeoutMs);
};
bool LSocket::IsReadable(long timeoutMs)
{
    timeval_t timeout;
    timeout.tv_sec = 0;
    if (timeoutMs == 0)
        timeout.tv_usec = 50;
    else
        timeout.tv_usec = timeoutMs * 1000;
    if (timeoutMs == -1)
        return SelectRead(0);
    return SelectRead(&timeout);
}
int main()
{
    int ok = 1;
    LSocket s;
    s.Socket = 9;
    g_selResult = 1;
    if (s.IsReadable(0) != true || g_lastUsec != 50 || g_lastWhich != 1) ok = 0;
    if (s.IsReadable(2) != true || g_lastUsec != 2000) ok = 0;
    if (s.IsReadable(-1) != true || g_nullTimeout != 1 || g_lastUsec != -7) ok = 0;
    g_selResult = -1;
    if (s.IsReadable(5) != false) ok = 0;
    g_selResult = 0;
    if (s.IsReadable(5) != false) ok = 0;
    printf(ok ? "008f8210_TEST PASS\n" : "008f8210_TEST FAIL\n");
    return ok ? 0 : 1;
}