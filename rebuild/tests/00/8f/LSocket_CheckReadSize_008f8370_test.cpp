// Behaviour test for 0x008F8370 LSocket::CheckReadSize
#include <stdio.h>
static long g_cmd = 0;
static int ioctlsocket(unsigned int s, long cmd, unsigned long* argp) { g_cmd = cmd; *argp = s * 3; return 0; }
class LSocket {
public:
    unsigned int Socket;
    unsigned long CheckReadSize();
};
unsigned long LSocket::CheckReadSize()
{
    unsigned long size = 0;
    ioctlsocket(Socket, 0x4004667F, &size);
    return size;
}
int main()
{
    int ok = 1;
    LSocket s;
    s.Socket = 7;
    if (s.CheckReadSize() != 21 || g_cmd != 0x4004667F) ok = 0;
    printf(ok ? "008f8370_TEST PASS\n" : "008f8370_TEST FAIL\n");
    return ok ? 0 : 1;
}