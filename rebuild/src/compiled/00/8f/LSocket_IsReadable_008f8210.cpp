// LSocket::IsReadable(long timeoutMs) -- select()s the socket for reading (50 us when 0, ms*1000 us otherwise, blocking when -1); true when data can be read
// Leaf: IsReadable
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F8210, 197 bytes (+ int3 padding), `ret 4`. Ego_r.pdb ?IsReadable@LSocket@LHNetworkLib@@QAE_NJ@Z.
// Callees: select (WSOCK32 ordinal 18, IAT 0x0144055C), __WSAFDIsSet @ 0x00C003EC (the static wsock32.lib helper).
// Called by LHHttp2::IsDataAvailable (landed 0x0083EA50) with 0.
// Layout: retail LSocket, Socket +0x38 (as the landed LSocket::Attach 0x008F8C00). Frame: timeval +4, fd_set +0x0C (0x10C).
// Shape: the select + result test is the PDB's private SelectStatus(LSOCKET_SELECT, timeval*) inlined twice -- once with the
// timeval, once with NULL for the blocking -1 case -- so each copy builds its own FD_SET stores and the two FD_ISSET calls
// cross-jump into one; the timeout*1000 store precedes the -1 test.
struct timeval {                                   // winsock timeval
    long tv_sec;
    long tv_usec;
};

struct fd_set {                                    // winsock 1.1 fd_set, FD_SETSIZE 64
    unsigned int fd_count;
    unsigned int fd_array[64];
};

extern "C" __declspec(dllimport) int __stdcall select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
                                                      const timeval* timeout);   // WSOCK32 #18
extern "C" int __stdcall __WSAFDIsSet(unsigned int fd, fd_set* set);              // 0x00C003EC (wsock32.lib stub)

#define FD_ZERO(set) (((fd_set*)(set))->fd_count = 0)
#define FD_SET(fd, set) do { \
    if (((fd_set*)(set))->fd_count < 64) \
        ((fd_set*)(set))->fd_array[((fd_set*)(set))->fd_count++] = (fd); \
} while (0)
#define FD_ISSET(fd, set) __WSAFDIsSet((unsigned int)(fd), (fd_set*)(set))
struct __false_type {                              // STLport empty tag; a user constructor keeps the temporary store-free
    __false_type() { }
};

struct CRetailString {                             // STLport basic_string<char>, 12 bytes in retail (global, the landed 0x00841E90 name)
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
    __forceinline bool SelectRead(timeval* timeout)
    {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(Socket, &readSet);
        int result = select(0, &readSet, 0, 0, timeout);
        if (result == -1)
            return false;
        if (result == 1 && FD_ISSET(Socket, &readSet))
            return true;
        return false;
    }
    bool IsReadable(long timeoutMs);
};

bool LSocket::IsReadable(long timeoutMs)
{
    timeval timeout;
    timeout.tv_sec = 0;
    if (timeoutMs == 0)
        timeout.tv_usec = 50;
    else
        timeout.tv_usec = timeoutMs * 1000;
    if (timeoutMs == -1)
        return SelectRead(0);
    return SelectRead(&timeout);
}