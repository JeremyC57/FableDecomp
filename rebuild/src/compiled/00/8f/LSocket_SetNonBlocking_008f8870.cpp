// LSocket::SetNonBlocking(bool nonBlocking) -- switches the socket's blocking mode (ioctlsocket FIONBIO); on failure builds the "Winsock Error: " report (debug output compiled out) and returns false
// Leaf: SetNonBlocking
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F8870, 160 bytes, `ret 4`, /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Ego_r.pdb ?SetNonBlocking@LSocket@LHNetworkLib@@AAE_N_N@Z. Called by LSocketServer::RequestListener 0x0090B0CE.
// Callees: ioctlsocket (WSOCK32 #12, FIONBIO 0x8004667E, the bool argument's own slot as the u_long), malloc 0x00BFEA0E /
// memmove 0x00BFEAE6 / free 0x00BFEA14 (the STLport string of the 15-char literal "Winsock Error: " @ 0x01288C20),
// WSAGetLastError (WSOCK32 #111), _itoa 0x00BFED34 into a char buffer (frame 0x34 bytes below the cookie).
// Shape: the report string and the converted error number are built and dropped (the release logging macro keeps only
// their side effects); every return has its own cookie-check epilogue.
extern "C" void* __cdecl malloc(unsigned int size);                                // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int size);  // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                                         // 0x00BFEA14
extern "C" char* __cdecl _itoa(int value, char* buffer, int radix);                // 0x00BFED34
extern "C" __declspec(dllimport) int __stdcall WSAGetLastError();                  // WSOCK32 #111
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)

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
    __forceinline ~CNetErrorString()
    {
        if (m_start)
            free(m_start);
    }
};
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
    bool SetNonBlocking(bool nonBlocking);
};

extern "C" __declspec(dllimport) int __stdcall ioctlsocket(unsigned int s, long cmd, unsigned long* argp);   // WSOCK32 #12

bool LSocket::SetNonBlocking(bool nonBlocking)
{
    if (Socket == (unsigned int)-1)
        return false;
    if (ioctlsocket(Socket, 0x8004667E, (unsigned long*)&nonBlocking) != 0) {
        CNetErrorString error("Winsock Error: ");
        char number[50];
        _itoa(WSAGetLastError(), number, 10);
        return false;
    }
    return true;
}