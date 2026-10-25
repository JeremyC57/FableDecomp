// LSocket::Open(LAddress& remote, LAddress& local, LSOCKET_PROTOCOL protocol) -- opens a client connection: resolves the remote address into SocketInformation, creates the stream (TCP / protocol 3) or datagram socket, resolves the local address, binds and connects (on failure builds the dropped "Winsock Error: " report), switches to non-blocking and records both addresses and the local socket information; false on any failure
// Leaf: ?Open@LSocket@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F9820, 376 bytes (padded to 384), `ret 0xc`, /GS (cookie 0x0139C8A8,
//   __security_check_cookie 0x00BFE9F9). Ego_r.pdb ?Open@LSocket@LHNetworkLib@@QAE_NAAULAddress@2@0W4LSOCKET_PROTOCOL@2@@Z.
// Callees: htons (WSOCK32 #9), LSocket::ResolveHost(LAddress&, unsigned long&) @ 0x008F86F0 (static __fastcall, landed),
//   socket (WSOCK32 #23), bind (WSOCK32 #2), connect (WSOCK32 #4), basic_string<char>(const char*, const allocator&)
//   @ 0x00568440 (landed; out of line in this TU, unlike the inline copy in SetNonBlocking / Accept), WSAGetLastError
//   (WSOCK32 #111), _itoa 0x00BFED34, free 0x00BFEA14, LSocket::SetNonBlocking(bool) @ 0x008F8870 (landed),
//   LAddress::operator= @ 0x00841F60 (landed; called out of line here).
// Layout: retail LSocket (0x9C bytes) as the landed LSocket rows: RemoteAddress +0, LocalAddress +0x1c, Socket +0x38,
//   SocketInformation +0x3c, SocketInterfaceInformation +0x4c, ConnectionMode +0x60.
// Shape: retail tests SocketInformation's address against INADDR_NONE after resolving each address (the local check
//   re-reads the remote one); protocols 1 and 3 share the SOCK_STREAM call; the bind and connect failures are two
//   identical report blocks that VC7.1 cross-jumps into one (written as `bind != 0 || connect != 0` the shared block is
//   laid out cold after the success path); each return keeps its own cookie-check epilogue. 3 grades.
extern "C" void __cdecl free(void* block);                                         // 0x00BFEA14
extern "C" char* __cdecl _itoa(int value, char* buffer, int radix);                // 0x00BFED34
extern "C" __declspec(dllimport) int __stdcall WSAGetLastError();                  // WSOCK32 #111

namespace _STL {
template <class T> class allocator {
public:
    allocator() { }
};
}

struct CNetErrorString {                           // STLport basic_string<char> (12 bytes), built by the out-of-line ctor
    char* m_start;
    char* m_finish;
    char* m_endOfStorage;
    CNetErrorString(const char* text, const _STL::allocator<char>& a = _STL::allocator<char>());   // 0x00568440
    ~CNetErrorString()
    {
        if (m_start)
            free(m_start);
    }
};

struct CRetailString {                             // STLport basic_string<char>, 12 bytes in retail
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
};

struct LAddress {                                  // Ego_r.pdb names; retail 0x1C bytes (12-byte STLport string)
    CRetailString  ServerName;                     // +0x00
    unsigned long  ServerIP;                       // +0x0C
    unsigned short ServerPort;                     // +0x10
    long           TransportProtocol;              // +0x14 LSOCKET_PROTOCOL
    long           TransportFlags;                 // +0x18 LSOCKET_FLAGS
    LAddress& operator=(const LAddress& other);    // 0x00841F60
};

struct sockaddr_in {                               // winsock sockaddr_in, 16 bytes
    short          sin_family;
    unsigned short sin_port;
    unsigned long  sin_addr;
    char           sin_zero[8];
};

struct CCharVector {                               // STLport vector<char>, 12 bytes in retail
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
};

class LSocketServer;

class LSocket {                                    // retail 0x9C bytes (Ego_r.pdb 196 with its larger LAddress/vector)
public:
    LAddress       RemoteAddress;                  // +0x00
    LAddress       LocalAddress;                   // +0x1C
    unsigned int   Socket;                         // +0x38
    sockaddr_in    SocketInformation;              // +0x3C
    sockaddr_in    SocketInterfaceInformation;     // +0x4C
    bool           ListeningSocket;                // +0x5C
    long           ConnectionMode;                 // +0x60 LSOCKET_PROTOCOL
    CCharVector    SendOutputQueue;                // +0x64
    CCharVector    InternalPacketBuffer;           // +0x70
    bool           NoListeningBinding;             // +0x7C
    LSocketServer* ServerObject;                   // +0x80
    bool           ActivityPending;                // +0x84
    unsigned long  NrTotalDataSent;                // +0x88
    unsigned long  NrTotalDataReceived;            // +0x8C
    unsigned long  NrLastDataSent;                 // +0x90
    unsigned long  NrLastDataReceived;             // +0x94
    void*          _DEBUGFP;                       // +0x98
    static bool __fastcall ResolveHost(LAddress& address, unsigned long& ip);   // 0x008F86F0
    bool SetNonBlocking(bool nonBlocking);         // 0x008F8870
    bool Open(LAddress& remote, LAddress& local, long protocol);
};

extern "C" __declspec(dllimport) unsigned short __stdcall htons(unsigned short value);                       // WSOCK32 #9
extern "C" __declspec(dllimport) unsigned int __stdcall socket(int af, int type, int protocol);              // WSOCK32 #23
extern "C" __declspec(dllimport) int __stdcall bind(unsigned int s, const sockaddr_in* name, int length);    // WSOCK32 #2
extern "C" __declspec(dllimport) int __stdcall connect(unsigned int s, const sockaddr_in* name, int length); // WSOCK32 #4

bool LSocket::Open(LAddress& remote, LAddress& local, long protocol)
{
    SocketInformation.sin_port = htons(remote.ServerPort);
    SocketInformation.sin_family = 2;              // AF_INET
    ResolveHost(remote, SocketInformation.sin_addr);
    if (SocketInformation.sin_addr == 0xffffffff)  // INADDR_NONE
        return false;
    ConnectionMode = protocol;
    switch (protocol) {
    case 1:
    case 3:
        Socket = socket(2, 1, 0);                  // AF_INET, SOCK_STREAM
        break;
    case 2:
        Socket = socket(2, 2, 0);                  // AF_INET, SOCK_DGRAM
        break;
    default:
        return false;
    }
    sockaddr_in localInformation;
    localInformation.sin_port = htons(local.ServerPort);
    localInformation.sin_family = 2;
    ResolveHost(local, localInformation.sin_addr);
    if (SocketInformation.sin_addr == 0xffffffff)
        return false;
    if (bind(Socket, &localInformation, sizeof(localInformation)) != 0) {
        CNetErrorString error("Winsock Error: ");
        char number[50];
        _itoa(WSAGetLastError(), number, 10);
        return false;
    }
    if (connect(Socket, &SocketInformation, sizeof(SocketInformation)) != 0) {
        CNetErrorString error("Winsock Error: ");
        char number[50];
        _itoa(WSAGetLastError(), number, 10);
        return false;
    }
    if (!SetNonBlocking(true))
        return false;
    RemoteAddress = remote;
    LocalAddress = local;
    SocketInterfaceInformation = localInformation;
    return true;
}