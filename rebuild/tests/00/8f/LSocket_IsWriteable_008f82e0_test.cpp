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
class LSocket {
public:
    unsigned int Socket;
    bool IsWriteable(long timeoutMs);
};
bool LSocket::IsWriteable(long timeoutMs)
{
    timeval_t timeout;
    timeout.tv_sec = 0;
    if (timeoutMs == 0)
        timeout.tv_usec = 50;
    else
        timeout.tv_usec = timeoutMs * 1000;
    fdset_t writeSet;
    FD_ZERO_T(&writeSet);
    FD_SET_T(Socket, &writeSet);
    int result = sel(0, 0, &writeSet, 0, &timeout);
    if (result == -1)
        return false;
    if (result == 1 && isset(Socket, &writeSet))
        return true;
    return false;
}
int main()
{
    int ok = 1;
    LSocket s;
    s.Socket = 4;
    g_selResult = 1;
    if (s.IsWriteable(0) != true || g_lastUsec != 50 || g_lastWhich != 2) ok = 0;
    if (s.IsWriteable(3) != true || g_lastUsec != 3000) ok = 0;
    g_selResult = -1;
    if (s.IsWriteable(1) != false) ok = 0;
    g_selResult = 0;
    if (s.IsWriteable(1) != false) ok = 0;
    printf(ok ? "008f82e0_TEST PASS\n" : "008f82e0_TEST FAIL\n");
    return ok ? 0 : 1;
}