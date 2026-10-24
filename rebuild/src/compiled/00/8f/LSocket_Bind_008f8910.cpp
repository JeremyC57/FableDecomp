// LSocket::Bind(LAddress& address) -- creates the socket for the address's transport protocol and records the protocol; true when a valid handle was created
// Leaf: Bind
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F8910, 38 bytes, `ret 4`. Ego_r.pdb ?Bind@LSocket@LHNetworkLib@@QAE_NAAULAddress@2@@Z.
// Callee: LSocket::CreateSocket(const LAddress&, LSOCKET_PROTOCOL) @ 0x008F87A0 (private static `__fastcall`: address in ecx,
// protocol in edx; PDB ?CreateSocket@LSocket@LHNetworkLib@@CIIABULAddress@2@W4LSOCKET_PROTOCOL@2@@Z).
// Layout: retail LSocket (0x9C bytes; Socket +0x38, ConnectionMode +0x60) and LAddress (0x1C; TransportProtocol +0x14),
// as the landed LSocket::Attach 0x008F8C00. Called by LSocketServer::RequestListener 0x0090B0C6.
// Shape: the protocol is re-read after the call for the ConnectionMode store; the result compares the returned handle,
// written `? true : false` (a plain bool compare adds `xor edx,edx ; setne dl ; mov al,dl`).
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
    static unsigned int __fastcall CreateSocket(const LAddress& address, long protocol);   // 0x008F87A0
    bool Bind(LAddress& address);
};

bool LSocket::Bind(LAddress& address)
{
    Socket = CreateSocket(address, address.TransportProtocol);
    ConnectionMode = address.TransportProtocol;
    return Socket != (unsigned int)-1 ? true : false;
}