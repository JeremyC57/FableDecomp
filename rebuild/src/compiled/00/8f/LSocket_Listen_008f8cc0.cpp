// LSocket::Listen(LAddress& address) -- opens the listening socket: a SO_REUSEADDR stream socket, the interface address resolved from `address` (its port only when it resolved), the local address recorded, then bind and listen; each failure builds the dropped "Winsock Error: " report and returns false; on success the socket is marked listening (TCP)
// Leaf: ?Listen@LSocket@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F8CC0, 468 bytes (padded to 480), `ret 4`, /GS (cookie 0x0139C8A8,
//   __security_check_cookie 0x00BFE9F9). Ego_r.pdb ?Listen@LSocket@LHNetworkLib@@QAE_NAAULAddress@2@@Z.
// Callees: socket (WSOCK32 #23), setsockopt (WSOCK32 #21; SOL_SOCKET 0xFFFF, SO_REUSEADDR 4, a one-byte bool option),
//   LSocket::ResolveHost(LAddress&, unsigned long&) @ 0x008F86F0 (static __fastcall, landed), htons (WSOCK32 #9),
//   basic_string<char>::_M_assign_dispatch @ 0x00569140 (the inlined LAddress::operator=), bind (WSOCK32 #2), listen
//   (WSOCK32 #13), malloc 0x00BFEA0E / memmove 0x00BFEAE6 / free 0x00BFEA14 (the inline "Winsock Error: " string),
//   basic_string<char>(const char*, const allocator&) @ 0x00568440 (the third report's string, out of line),
//   WSAGetLastError (WSOCK32 #111), _itoa 0x00BFED34.
// Layout: retail LSocket (0x9C bytes) as the landed LSocket rows: LocalAddress +0x1c, Socket +0x38,
//   SocketInterfaceInformation +0x4c, ListeningSocket +0x5c, ConnectionMode +0x60.
// Shape: the report string is inlined at the first two sites and called out of line at the third (VC7.1's inline
//   budget: modelled as a __forceinline constructor plus the out-of-line allocator overload); the first two reports
//   tail-merge; each return keeps its own cookie-check epilogue.
extern "C" void* __cdecl malloc(unsigned int size);                                // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int size);  // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                                         // 0x00BFEA14
extern "C" char* __cdecl _itoa(int value, char* buffer, int radix);                // 0x00BFED34
extern "C" __declspec(dllimport) int __stdcall WSAGetLastError();                  // WSOCK32 #111
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)

namespace _STL {
template <class T> class allocator {
public:
    allocator() { }
};
}

struct CNetErrorString {                           // STLport basic_string<char> (12 bytes), built from a literal
    char* m_start;
    char* m_finish;
    char* m_endOfStorage;
    static char* CopyTrivial(const char* first, const char* last, char* result)
    {
        return last == first ? result : (char*)memmove(result, first, last - first) + (last - first);
    }
    __forceinline CNetErrorString(const char* text)
    {
        unsigned int n = strlen(text);
        m_start = (char*)malloc(n + 1);
        m_finish = m_start;
        m_endOfStorage = m_start + n + 1;
        m_finish = CopyTrivial(text, text + n, m_start);
        *m_finish = 0;
    }
    CNetErrorString(const char* text, const _STL::allocator<char>& a);   // 0x00568440
    __forceinline ~CNetErrorString()
    {
        if (m_start)
            free(m_start);
    }
};

struct __false_type {                              // STLport empty tag; a user constructor keeps the temporary store-free
    __false_type() { }
};

struct CRetailString {                             // STLport basic_string<char>, 12 bytes in retail
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CRetailString& _M_assign_dispatch(const char* first, const char* last, const __false_type&);   // 0x00569140
    CRetailString& operator=(const CRetailString& other)
    {
        if (&other != this)
            _M_assign_dispatch(other._M_start, other._M_finish, __false_type());
        return *this;
    }
};

struct LAddress {                                  // Ego_r.pdb names; retail 0x1C bytes (12-byte STLport string)
    CRetailString  ServerName;                     // +0x00
    unsigned long  ServerIP;                       // +0x0C
    unsigned short ServerPort;                     // +0x10
    long           TransportProtocol;              // +0x14 LSOCKET_PROTOCOL
    long           TransportFlags;                 // +0x18 LSOCKET_FLAGS
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
    bool Listen(LAddress& address);
};

extern "C" __declspec(dllimport) unsigned short __stdcall htons(unsigned short value);                       // WSOCK32 #9
extern "C" __declspec(dllimport) unsigned int __stdcall socket(int af, int type, int protocol);              // WSOCK32 #23
extern "C" __declspec(dllimport) int __stdcall setsockopt(unsigned int s, int level, int name, const char* value, int length);   // WSOCK32 #21
extern "C" __declspec(dllimport) int __stdcall bind(unsigned int s, const sockaddr_in* name, int length);    // WSOCK32 #2
extern "C" __declspec(dllimport) int __stdcall listen(unsigned int s, int backlog);                         // WSOCK32 #13

bool LSocket::Listen(LAddress& address)
{
    bool reuse = true;
    Socket = socket(2, 1, 0);                      // AF_INET, SOCK_STREAM
    if (setsockopt(Socket, 0xffff, 4, (const char*)&reuse, sizeof(reuse)) != 0) {   // SOL_SOCKET, SO_REUSEADDR
        CNetErrorString error("Winsock Error: ");
        char number[50];
        _itoa(WSAGetLastError(), number, 10);
        return false;
    }
    sockaddr_in information;
    information.sin_port = 0;
    information.sin_family = 2;
    ResolveHost(address, information.sin_addr);
    if (information.sin_addr != 0xffffffff)
        information.sin_port = htons(address.ServerPort);
    SocketInterfaceInformation = information;
    LocalAddress = address;
    if (bind(Socket, &information, sizeof(information)) != 0) {
        CNetErrorString error("Winsock Error: ");
        char number[50];
        _itoa(WSAGetLastError(), number, 10);
        return false;
    }
    if (listen(Socket, 0x7fffffff) != 0) {
        CNetErrorString error("Winsock Error: ", _STL::allocator<char>());
        char number[50];
        _itoa(WSAGetLastError(), number, 10);
        return false;
    }
    ConnectionMode = 1;
    ListeningSocket = true;
    return true;
}