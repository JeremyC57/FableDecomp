#ifndef FABLE_RETAIL_LSOCKET_HEADER
#define FABLE_RETAIL_LSOCKET_HEADER
#include "stlport/fable_string.h"
#include "LHNetworkTypes.h"
#ifdef FABLE_RETAIL_LSOCKET_RECEIVE
#include "rebuild_abi.h"
#endif
#include <stdio.h>
// SDK Winsock structures: original CodeView fields and native 16-byte copies.
struct in_addr {
    union {
        struct { unsigned char s_b1,s_b2,s_b3,s_b4; } S_un_b;
        struct { unsigned short s_w1,s_w2; } S_un_w;
        unsigned long S_addr;
    } S_un;
};
struct sockaddr_in { short sin_family;unsigned short sin_port;in_addr sin_addr;char sin_zero[8]; };
namespace LHNetworkLib {
struct LAddress {
    std::string ServerName;
    unsigned long ServerIP;
    unsigned short ServerPort;
    LSOCKET_PROTOCOL TransportProtocol;
    LSOCKET_FLAGS TransportFlags;
#ifdef FABLE_RETAIL_LSOCKET_LADDRESS
    // Retail opt-in (chunk 892, row 0x008F99A0 LSocket::Receive(vector<char>&)): Ego_r.pdb / FableWin.pdb ??0LAddress@LHNetworkLib@@QAE@XZ
    // and ?ClearData@LAddress@LHNetworkLib@@QAEXXZ (public), inline in retail: each default LAddress builds the STLport empty string
    // (malloc(8), terminator) and ClearData assigns "" (assign(s, s + 0): the literal pushed twice, 0x008F99B4 / 0x008F99BF), then
    // zeroes the scalars from one zero register (ServerPort's word store first, 0x008F99D9).
    LAddress() { ClearData(); }
    void ClearData()
    {
        ServerName = "";
        ServerPort = 0;
        ServerIP = 0;
        TransportProtocol = LSP_NONE;
        TransportFlags = LSF_NONE;
    }
#endif
};
class LSocketServer;
class LSocket {
    LAddress RemoteAddress;
    LAddress LocalAddress;
    unsigned int Socket;
    sockaddr_in SocketInformation;
    sockaddr_in SocketInterfaceInformation;
    bool ListeningSocket;
    LSOCKET_PROTOCOL ConnectionMode;
    std::vector<char> SendOutputQueue;
    std::vector<char> InternalPacketBuffer;
    bool NoListeningBinding;
    LSocketServer* ServerObject;
public:
    bool ActivityPending;
    unsigned long NrTotalDataSent;
    unsigned long NrTotalDataReceived;
    unsigned long NrLastDataSent;
    unsigned long NrLastDataReceived;
    FILE* _DEBUGFP;
    bool IsReadable(long);
    bool Receive(std::vector<char>&);
#ifdef FABLE_RETAIL_LSOCKET_RECEIVE
    // Retail opt-in (chunk 892, row 0x008F99A0): the LSocket.cpp members the one-argument Receive reaches, with Ego_r.pdb's access
    // (SI public static __fastcall, CI private static __fastcall, AAE private, QAE public). Declarations; the row defines the bodies
    // retail compiles into the same TU (0x008F86A0 .. 0x008F9320), so VC7.1 sees that none of them captures an LAddress.
    static unsigned long FABLE_FASTCALL ResolveHost(const std::string& host);                     // 0x008F86A0
    static bool FABLE_FASTCALL ResolveHost(LAddress& address, unsigned long& ip);                 // 0x008F86F0
    unsigned long CheckReadSize();                                                            // 0x008F8370
    bool Receive(std::vector<char>& data, LAddress& from, const LAddress& local);             // 0x008F9320
private:
    static unsigned int FABLE_FASTCALL CreateSocket(const LAddress& address, LSOCKET_PROTOCOL protocol);   // 0x008F87A0
    static bool FABLE_FASTCALL ConvertSockaddr2LAddress(const sockaddr_in& from, LAddress& address);       // 0x008F8BB0
    bool SetNonBlocking(bool nonBlocking);                                                    // 0x008F8870
public:
#endif
};
}
#ifdef _MSC_VER
typedef char LSocketSizeCheck[sizeof(LHNetworkLib::LSocket)==0x9c?1:-1];
#endif
#endif
