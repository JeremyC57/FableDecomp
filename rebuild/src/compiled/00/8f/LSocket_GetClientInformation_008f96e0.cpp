// LSocket::GetClientInformation(LAddress& address) -- copies the remote address of an open socket to the caller; false for INVALID_SOCKET
// Leaf: GetClientInformation
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F96E0, 76 bytes, `ret 4`. Ego_r.pdb ?GetClientInformation@LSocket@LHNetworkLib@@QAE_NAAULAddress@2@@Z.
// Callee: STLport basic_string<char>::_M_assign_dispatch @ 0x00569140 (inside the inlined LAddress::operator=, the
// out-of-line copy of which is 0x00841F60).
// Layout: retail LSocket (0x9C bytes; PDB 196) with RemoteAddress +0 and Socket +0x38, as the landed LSocket::Attach
// 0x008F8C00 / ~LSocket 0x008F9780 (global-namespace models). PDB member names.
// Shape: the INVALID_SOCKET early return comes first; `address = RemoteAddress` is the implicit memberwise assignment
// inlined (string self-check compares `this` -- RemoteAddress is at +0 -- with the destination).
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
    bool GetClientInformation(LAddress& address);
};

bool LSocket::GetClientInformation(LAddress& address)
{
    if (Socket == (unsigned int)-1)
        return false;
    address = RemoteAddress;
    return true;
}