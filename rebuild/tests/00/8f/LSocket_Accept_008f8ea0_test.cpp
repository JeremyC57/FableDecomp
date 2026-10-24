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
struct sockaddr_t { short family; unsigned short port; unsigned long addr; char zero[8]; };
static unsigned int g_acceptResult = 0; static int g_lenSeen = 0;
static unsigned int acceptStub(unsigned int s, sockaddr_t* address, int* length) { (void)s; g_lenSeen = *length; address->port = 77; return g_acceptResult; }
class LSocket {
public:
    unsigned int Socket;
    sockaddr_t   SocketInterfaceInformation;
    bool         ListeningSocket;
    unsigned int attached;
    unsigned short remotePort;
    unsigned short localPort;
    bool Attach(unsigned int socket, const sockaddr_t& remote, const sockaddr_t& local)
    {
        attached = socket; remotePort = remote.port; localPort = local.port; return true;
    }
    bool Accept(LSocket& client);
};
bool LSocket::Accept(LSocket& client)
{
    if (!ListeningSocket) {
        CNetErrorString error("Winsock Error: ");
        char number[50];
        itoa10(WSAGetLastErrorStub(), number);
        return false;
    }
    sockaddr_t remote;
    int length = sizeof(remote);
    unsigned int socket = acceptStub(Socket, &remote, &length);
    if (socket == (unsigned int)-1)
        return false;
    client.Attach(socket, remote, SocketInterfaceInformation);
    return true;
}
int main()
{
    int ok = 1;
    LSocket server, client;
    client.attached = 0;
    server.Socket = 1; server.SocketInterfaceInformation.port = 8080;
    server.ListeningSocket = false;
    if (server.Accept(client) != false || g_errStrings != 1 || client.attached != 0) ok = 0;
    server.ListeningSocket = true;
    g_acceptResult = (unsigned int)-1;
    if (server.Accept(client) != false || client.attached != 0 || g_lenSeen != (int)sizeof(sockaddr_t)) ok = 0;
    g_acceptResult = 42;
    if (server.Accept(client) != true || client.attached != 42 || client.remotePort != 77 || client.localPort != 8080) ok = 0;
    printf(ok ? "008f8ea0_TEST PASS\n" : "008f8ea0_TEST FAIL\n");
    return ok ? 0 : 1;
}