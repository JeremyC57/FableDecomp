// Behaviour test for 0x008FA120 LHNetworkLib::LSocket::ClearData: every field the reconstruction writes is reset,
// both byte queues end empty (resize(0) then clear and clear then resize(0)), and the socket is INVALID_SOCKET.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct SockAddrIn { short family; unsigned short port; unsigned int addr; char zero[8]; };
struct LAddress {
    std::string ServerName; unsigned int ServerIP; unsigned short ServerPort; int TransportProtocol; int TransportFlags;
    void ClearData() { ServerName = ""; ServerPort = 0; ServerIP = 0; TransportProtocol = 0; TransportFlags = 0; }
};
struct LSocket {
    LAddress RemoteAddress, LocalAddress;
    unsigned int Socket;
    SockAddrIn SocketInformation, SocketInterfaceInformation;
    bool ListeningSocket;
    int ConnectionMode;
    std::vector<char> SendOutputQueue, InternalPacketBuffer;
    bool NoListeningBinding;
    void* ServerObject;
    bool ActivityPending;
    unsigned int NrTotalDataSent, NrTotalDataReceived, NrLastDataSent, NrLastDataReceived;
    void ClearData()
    {
        ConnectionMode = 0;
        ActivityPending = false;
        RemoteAddress.ClearData();
        LocalAddress.ClearData();
        NrLastDataReceived = 0; NrTotalDataReceived = 0; NrTotalDataSent = 0; NrLastDataSent = 0;
        std::memset(&SocketInformation, 0, sizeof(SocketInformation));
        Socket = (unsigned int)-1;
        InternalPacketBuffer.resize(0);
        InternalPacketBuffer.clear();
        SendOutputQueue.clear();
        SendOutputQueue.resize(0);
        ListeningSocket = false;
    }
};

int main()
{
    static LSocket s;
    s.RemoteAddress.ServerName = "remote.host"; s.RemoteAddress.ServerIP = 0x0100007F; s.RemoteAddress.ServerPort = 80;
    s.RemoteAddress.TransportProtocol = 2; s.RemoteAddress.TransportFlags = 3;
    s.LocalAddress.ServerName = "local"; s.LocalAddress.ServerIP = 5; s.LocalAddress.ServerPort = 81;
    s.LocalAddress.TransportProtocol = 1; s.LocalAddress.TransportFlags = 1;
    s.Socket = 42; std::memset(&s.SocketInformation, 0xAB, sizeof(s.SocketInformation));
    std::memset(&s.SocketInterfaceInformation, 0xCD, sizeof(s.SocketInterfaceInformation));
    s.ListeningSocket = true; s.ConnectionMode = 2; s.ActivityPending = true; s.NoListeningBinding = true;
    s.SendOutputQueue.assign(10, 'x'); s.InternalPacketBuffer.assign(7, 'y');
    s.NrTotalDataSent = 1; s.NrTotalDataReceived = 2; s.NrLastDataSent = 3; s.NrLastDataReceived = 4;
    s.ClearData();
    int ok = 1;
    ok &= s.ConnectionMode == 0 && !s.ActivityPending && !s.ListeningSocket;
    ok &= s.RemoteAddress.ServerName.empty() && s.RemoteAddress.ServerIP == 0 && s.RemoteAddress.ServerPort == 0;
    ok &= s.RemoteAddress.TransportProtocol == 0 && s.RemoteAddress.TransportFlags == 0;
    ok &= s.LocalAddress.ServerName.empty() && s.LocalAddress.ServerIP == 0 && s.LocalAddress.ServerPort == 0;
    ok &= s.NrTotalDataSent == 0 && s.NrTotalDataReceived == 0 && s.NrLastDataSent == 0 && s.NrLastDataReceived == 0;
    ok &= s.Socket == 0xFFFFFFFFu;
    ok &= s.SocketInformation.family == 0 && s.SocketInformation.port == 0 && s.SocketInformation.addr == 0;
    ok &= s.SocketInterfaceInformation.addr == 0xCDCDCDCDu;     // untouched
    ok &= s.NoListeningBinding;                                  // untouched
    ok &= s.SendOutputQueue.empty() && s.InternalPacketBuffer.empty();
    std::printf("008fa120_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}