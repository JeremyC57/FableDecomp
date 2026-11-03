#ifndef FABLE_RETAIL_LSOCKET_HEADER
#define FABLE_RETAIL_LSOCKET_HEADER
#include "stlport/fable_string.h"
#include "LHNetworkTypes.h"
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
};
}
#ifdef _MSC_VER
typedef char LSocketSizeCheck[sizeof(LHNetworkLib::LSocket)==0x9c?1:-1];
#endif
#endif
