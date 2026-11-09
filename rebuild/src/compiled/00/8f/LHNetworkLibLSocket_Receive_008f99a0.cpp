// LHNetworkLib::LSocket::Receive(vector<char>&) -- receive into the buffer, discarding the sender/receiver addresses (two empty LAddress locals)
// Leaf: ?Receive@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@@Z
// Retail 0x008F99A0, 0xCF bytes, thiscall `ret 4`, bool. Ego_r.pdb `?Receive@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@@Z`,
//   the one-argument overload of Receive(vector<char>&, LAddress&, const LAddress&) (0x008F9320, same TU, defined below).
// Callees: malloc @ 0x00BFEA0E, basic_string::assign(const char*, const char*) @ 0x005693B0, LSocket::Receive (3 args)
//   @ 0x008F9320, free @ 0x00BFEA14.
// Shape: each LAddress is default-constructed inline (engine/LSocket.h FABLE_RETAIL_LSOCKET_LADDRESS): the STLport empty
//   string (malloc(8), terminator), then ClearData -- `ServerName = ""` and the four scalar zeroes from the shared zero
//   register. The bool result is parked in the dead argument slot across the two destructors (null-tested free of each
//   name block, second local first).
// Same TU (chunk 892): retail's LSocket.cpp defines the members the 3-argument Receive passes the two addresses to
//   (ResolveHost 0x008F86A0 / 0x008F86F0, CreateSocket 0x008F87A0, ConvertSockaddr2LAddress 0x008F8BB0, Receive 0x008F9320)
//   before this row, and the STLport string members are COMDATs of the TU (stlport/fable_string_bodies.h). With those
//   bodies VC7.1 knows neither LAddress is captured: _String_base's zero mem-initialisers die under the malloc, the literal
//   pushes go first and the string's start/finish stores sink to the assign call (retail 0x008F99B4..0x008F99D0).
//   Declared only, every call is assumed to capture them (231 v 207 bytes, six zero stores kept).
#include "rebuild_abi.h"
#define FABLE_RETAIL_LSOCKET_LADDRESS
#define FABLE_RETAIL_LSOCKET_RECEIVE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/LSocket.h"
#include "stlport/fable_string_bodies.h"
extern "C" char* FABLE_CDECL _itoa(int value, char* buffer, int radix);                // 0x00BFED34
struct sockaddr { unsigned short sa_family; char sa_data[14]; };
struct hostent { char* h_name; char** h_aliases; short h_addrtype; short h_length; char** h_addr_list; };
struct timeval { long tv_sec; long tv_usec; };
struct fd_set { unsigned int fd_count; unsigned int fd_array[64]; };
#ifdef _MSC_VER
#define FABLE_WSAAPI __declspec(dllimport) __stdcall
#else
#define FABLE_WSAAPI
#endif
extern "C" int FABLE_WSAAPI WSAGetLastError();                                                         // WSOCK32 #111
extern "C" unsigned long FABLE_WSAAPI inet_addr(const char* cp);                                       // WSOCK32 #10
extern "C" char* FABLE_WSAAPI inet_ntoa(in_addr in);                                                   // WSOCK32 #11
extern "C" hostent* FABLE_WSAAPI gethostbyname(const char* name);                                      // WSOCK32 #52
extern "C" unsigned short FABLE_WSAAPI htons(unsigned short value);                                    // WSOCK32 #9
extern "C" unsigned short FABLE_WSAAPI ntohs(unsigned short value);                                    // WSOCK32 #15
extern "C" unsigned int FABLE_WSAAPI socket(int af, int type, int protocol);                           // WSOCK32 #23
extern "C" int FABLE_WSAAPI bind(unsigned int s, const sockaddr* name, int length);                    // WSOCK32 #2
extern "C" int FABLE_WSAAPI select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const timeval* timeout);   // WSOCK32 #18
extern "C" int FABLE_WSAAPI recvfrom(unsigned int s, char* buffer, int length, int flags, sockaddr_in* from, int* fromLength); // WSOCK32 #17
extern "C" int FABLE_WSAAPI recv(unsigned int s, char* buffer, int length, int flags);                // WSOCK32 #16
extern "C" int FABLE_WSAAPI closesocket(unsigned int s);                                               // WSOCK32 #3
extern "C" int FABLE_STDCALL __WSAFDIsSet(unsigned int fd, fd_set* set);                              // 0x00C003EC (wsock32.lib stub)

namespace LHNetworkLib {

#pragma auto_inline(off)
unsigned long FABLE_FASTCALL LSocket::ResolveHost(const std::string& host)          // 0x008F86A0 (landed body)
{
    char* scratch = (char*)malloc(1);
    scratch[0] = 0;
    unsigned long ip = inet_addr(host.c_str());
    if (ip == (unsigned long)-1) {
        hostent* entry = gethostbyname(host.c_str());
        if (entry)
            ip = *(unsigned long*)entry->h_addr_list[0];
    }
    free(scratch);
    return ip;
}

bool FABLE_FASTCALL LSocket::ResolveHost(LAddress& address, unsigned long& ip)      // 0x008F86F0 (landed body)
{
    unsigned long serverIP = address.ServerIP;
    if (serverIP == 0) {
        if (address.ServerName == std::string("ANY")) {
            ip = 0;
            return true;
        }
        serverIP = ResolveHost(address.ServerName);
        address.ServerIP = serverIP;
        if (ip == (unsigned long)-1)
            return false;
    }
    ip = serverIP;
    return true;
}

unsigned int FABLE_FASTCALL LSocket::CreateSocket(const LAddress& address, LSOCKET_PROTOCOL protocol)   // 0x008F87A0 (landed body)
{
    sockaddr_in sa;
    unsigned int s;
    switch (protocol) {
    case LSP_TCP:
        s = socket(2, 1, 0);                    // AF_INET, SOCK_STREAM
        break;
    case LSP_UDP:
        s = socket(2, 2, 0);                    // AF_INET, SOCK_DGRAM
        break;
    default:
        return (unsigned int)-1;
    }
    if (s == (unsigned int)-1)
        return (unsigned int)-1;
    ResolveHost(const_cast<LAddress&>(address), sa.sin_addr.S_un.S_addr);
    sa.sin_port = htons(address.ServerPort);
    sa.sin_family = 2;                          // AF_INET
    if (bind(s, (sockaddr*)&sa, sizeof(sa)) != 0)
        return (unsigned int)-1;
    return s;
}

bool FABLE_FASTCALL LSocket::ConvertSockaddr2LAddress(const sockaddr_in& sa, LAddress& address)   // 0x008F8BB0 (landed body)
{
    address.ServerName = inet_ntoa(sa.sin_addr);
    address.ServerIP = sa.sin_addr.S_un.S_addr;
    address.ServerPort = ntohs(sa.sin_port);
    return true;
}

bool LSocket::Receive(std::vector<char>& data, LAddress& from, const LAddress& local)   // 0x008F9320 (landed body)
{
    int received = 0;
    NrLastDataReceived = 0;
    if (ConnectionMode == LSP_TCP_PACKET)
        return false;
    if (local.ServerName.c_str()[0] != 0 && local.TransportProtocol == LSP_UDP) {   // a named UDP address
        sockaddr_in fromAddress;
        int fromLength = sizeof(fromAddress);
        if (!NoListeningBinding) {
            Socket = CreateSocket(local, LSP_UDP);
            SetNonBlocking(true);
        }
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 50;
        fd_set readSet;
        readSet.fd_count = 0;
        readSet.fd_array[readSet.fd_count++] = Socket;
        int result = select(0, &readSet, 0, 0, &timeout);
        if (result != -1 && result == 1 && __WSAFDIsSet(Socket, &readSet)) {
            received = recvfrom(Socket, data.begin(), data.size(), 0, &fromAddress, &fromLength);
            if (received == -1) {
                std::string error("Winsock Error: ");
                char number[50];
                _itoa(WSAGetLastError(), number, 10);
                return false;
            }
        }
        data.resize(received, char());
        ConvertSockaddr2LAddress(fromAddress, from);
        if (!NoListeningBinding)
            closesocket(Socket);
    }
    else {
        if (Socket == (unsigned int)-1)
            return false;
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 50;
        fd_set readSet;
        readSet.fd_count = 0;
        readSet.fd_array[readSet.fd_count++] = Socket;
        int result = select(0, &readSet, 0, 0, &timeout);
        if (result == -1 || result != 1 || !__WSAFDIsSet(Socket, &readSet))
            return false;
        unsigned long size = data.size();
        if (size == 0)
            size = CheckReadSize();
        if (data.size() == 0)
            data.resize(size);
        char* buffer = data.begin();
        received = recv(Socket, buffer, size, 0);
        if (received == -1) {
            switch (WSAGetLastError()) {
            case 10053:                            // WSAECONNABORTED
            case 10054:                            // WSAECONNRESET
                received = 0;
                break;
            default: {
                std::string report("Winsock Error: ");
                char number[50];
                _itoa(WSAGetLastError(), number, 10);
                return false;
            }
            }
        }
        data.resize(received, char());
        if (received == 0)
            return false;
    }
    NrTotalDataReceived += received;
    NrLastDataReceived = received;
    return true;
}
#pragma auto_inline(on)

bool LSocket::Receive(std::vector<char>& data)
{
    LAddress from;
    LAddress to;
    return Receive(data, from, to);
}

}