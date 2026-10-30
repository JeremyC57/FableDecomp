// LSocket::Receive(vector<char>& data, LAddress& from, const LAddress& local) -- reads waiting data: for a named UDP local address (re)creates the bound datagram socket unless NoListeningBinding, selects for 50 us and recvfrom()s into the buffer, records the sender and closes the socket again; otherwise selects the connected socket and recv()s into the buffer (sized by FIONREAD when empty), treating WSAEWOULDBLOCK / WSAEINPROGRESS as no data; updates the receive counters; false on an error or no data
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): a named `char* buffer = data._M_start;` before recv() gives retail's
//   push-buffer-then-load-socket order (eax reuse); the parked form passed the member inline and hoisted Socket into ecx.
// Leaf: ?Receive@LSocket@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F9320, 674 bytes (padded to 688), `ret 0xc`, /GS (cookie 0x0139C8A8,
//   __security_check_cookie 0x00BFE9F9). Ego_r.pdb
//   ?Receive@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@AAULAddress@2@ABU52@@Z.
// Callees: LSocket::CreateSocket @ 0x008F87A0 (static __fastcall, landed), LSocket::SetNonBlocking @ 0x008F8870 (landed),
//   select (WSOCK32 #18), __WSAFDIsSet @ 0x00C003EC, recvfrom (WSOCK32 #17), recv (WSOCK32 #16), basic_string<char>(const
//   char*, const allocator&) @ 0x00568440 (the dropped "Winsock Error: " report), WSAGetLastError (WSOCK32 #111), _itoa
//   0x00BFED34, free 0x00BFEA14, vector<char>::resize(size_type, const char&) @ 0x004122D0, vector<char>::resize(size_type)
//   @ 0x00412880 (manifest "_Umove<EWeaponClass*>" is propagated: the body is `resize(n, char())`), LSocket::
//   ConvertSockaddr2LAddress @ 0x008F8BB0 (static __fastcall, landed), closesocket (WSOCK32 #3), LSocket::CheckReadSize
//   @ 0x008F8370 (landed).
// Layout: retail LSocket (0x9C bytes) as the landed LSocket rows: Socket +0x38, ConnectionMode +0x60, NoListeningBinding
//   +0x7c, NrTotalDataReceived +0x8c, NrLastDataReceived +0x94.
// Shape: the select + result test is the landed IsReadable's inline SelectRead; STLport's one-argument resize(n) is
//   inlined (`resize(n, char())`) at the two result sites and called out of line for the empty-buffer sizing (VC7.1's
//   inline budget; modelled with the two-argument form written out); the WSAEWOULDBLOCK / WSAEINPROGRESS range test is
//   an `||`, so its report block is laid out cold after the epilogue.
extern "C" void __cdecl free(void* block);                                         // 0x00BFEA14
extern "C" char* __cdecl _itoa(int value, char* buffer, int radix);                // 0x00BFED34
extern "C" __declspec(dllimport) int __stdcall WSAGetLastError();                  // WSOCK32 #111

namespace std {
template <class T> class allocator {
public:
    allocator() { }
};
}

struct CNetErrorString {                           // STLport basic_string<char> (12 bytes), built by the out-of-line ctor
    char* m_start;
    char* m_finish;
    char* m_endOfStorage;
    CNetErrorString(const char* text, const std::allocator<char>& a = std::allocator<char>());   // 0x00568440
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
    const char* c_str() const { return _M_start; }
};

struct LAddress {                                  // Ego_r.pdb names; retail 0x1C bytes (12-byte STLport string)
    CRetailString  ServerName;                     // +0x00
    unsigned long  ServerIP;                       // +0x0C
    unsigned short ServerPort;                     // +0x10
    long           TransportProtocol;              // +0x14 LSOCKET_PROTOCOL
    long           TransportFlags;                 // +0x18 LSOCKET_FLAGS
};

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
    unsigned int size() const { return _M_finish - _M_start; }
    void resize(unsigned int newSize, const char& fill);   // 0x004122D0
    void resize(unsigned int newSize);                     // 0x00412880
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
    static unsigned int __fastcall CreateSocket(const LAddress& address, long protocol);           // 0x008F87A0
    static bool __fastcall ConvertSockaddr2LAddress(const sockaddr_in& from, LAddress& address);  // 0x008F8BB0
    bool SetNonBlocking(bool nonBlocking);         // 0x008F8870
    unsigned long CheckReadSize();                 // 0x008F8370
    __forceinline bool SelectRead(long microseconds)
    {
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = microseconds;
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(Socket, &readSet);
        int result = select(0, &readSet, 0, 0, &timeout);
        if (result == -1)
            return false;
        if (result == 1 && FD_ISSET(Socket, &readSet))
            return true;
        return false;
    }
    bool Receive(CCharVector& data, LAddress& from, const LAddress& local);
};

extern "C" __declspec(dllimport) int __stdcall recvfrom(unsigned int s, char* buffer, int length, int flags,
                                                        sockaddr_in* from, int* fromLength);   // WSOCK32 #17
extern "C" __declspec(dllimport) int __stdcall recv(unsigned int s, char* buffer, int length, int flags);   // WSOCK32 #16
extern "C" __declspec(dllimport) int __stdcall closesocket(unsigned int s);                       // WSOCK32 #3

bool LSocket::Receive(CCharVector& data, LAddress& from, const LAddress& local)
{
    int received = 0;
    NrLastDataReceived = 0;
    if (ConnectionMode == 3)
        return false;
    if (local.ServerName.c_str()[0] != 0 && local.TransportProtocol == 2) {   // a named UDP address
        sockaddr_in fromAddress;
        int fromLength = sizeof(fromAddress);
        if (!NoListeningBinding) {
            Socket = CreateSocket(local, 2);
            SetNonBlocking(true);
        }
        if (SelectRead(50)) {
            received = recvfrom(Socket, data._M_start, data.size(), 0, &fromAddress, &fromLength);
            if (received == -1) {
                CNetErrorString error("Winsock Error: ");
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
        if (!SelectRead(50))
            return false;
        unsigned long size = data.size();
        if (size == 0)
            size = CheckReadSize();
        if (data.size() == 0)
            data.resize(size);
        char* buffer = data._M_start;
        received = recv(Socket, buffer, size, 0);
        if (received == -1) {
            switch (WSAGetLastError()) {
            case 10053:                            // WSAECONNABORTED
            case 10054:                            // WSAECONNRESET
                received = 0;
                break;
            default: {
                CNetErrorString report("Winsock Error: ");
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