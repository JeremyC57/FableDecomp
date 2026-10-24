#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int g_errStrings = 0;
static int WSAGetLastErrorStub() { return 10035; }
struct CNetErrorString {
    char* m_start;
    char* m_finish;
    char* m_endOfStorage;
    CNetErrorString(const char* text)
    {
        unsigned int n = strlen(text);
        m_start = (char*)malloc(n + 1);
        memcpy(m_start, text, n);
        m_finish = m_start + n;
        m_endOfStorage = m_start + n + 1;
        *m_finish = 0;
        if (strcmp(m_start, "Winsock Error: ") == 0) ++g_errStrings;
    }
    ~CNetErrorString()
    {
        if (m_start)
            free(m_start);
    }
};
static void itoa10(int value, char* out)
{
    sprintf(out, "%d", value);
}
static long g_cmd = 0; static unsigned long g_arg = 99; static int g_ioctlResult = 0;
static int ioctlStub(unsigned int s, long cmd, unsigned long* argp) { (void)s; g_cmd = cmd; g_arg = *(unsigned char*)argp; return g_ioctlResult; }
class LSocket {
public:
    unsigned int Socket;
    bool SetNonBlocking(bool nonBlocking);
};
bool LSocket::SetNonBlocking(bool nonBlocking)
{
    if (Socket == (unsigned int)-1)
        return false;
    unsigned long arg = nonBlocking ? 1 : 0;
    if (ioctlStub(Socket, 0x8004667E, &arg) != 0) {
        CNetErrorString error("Winsock Error: ");
        char number[50];
        itoa10(WSAGetLastErrorStub(), number);
        return false;
    }
    return true;
}
int main()
{
    int ok = 1;
    LSocket s;
    s.Socket = (unsigned int)-1;
    if (s.SetNonBlocking(true) != false || g_arg != 99) ok = 0;
    s.Socket = 2;
    if (s.SetNonBlocking(true) != true || g_cmd != 0x8004667E || g_arg != 1) ok = 0;
    g_ioctlResult = -1;
    if (s.SetNonBlocking(false) != false || g_arg != 0 || g_errStrings != 1) ok = 0;
    printf(ok ? "008f8870_TEST PASS\n" : "008f8870_TEST FAIL\n");
    return ok ? 0 : 1;
}