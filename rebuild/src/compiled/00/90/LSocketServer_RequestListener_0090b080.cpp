// LSocketServer::RequestListener(LAddress& address) -- creates the listener socket; TCP (1): Listen on the address, deleting the socket on failure; UDP (2): keep the address, Bind it, go non-blocking and mark it unbound-listening; true on success
// Leaf: RequestListener
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090B080, 155 bytes, `ret 4`. Ego_r.pdb ?RequestListener@LSocketServer@LHNetworkLib@@QAE_NAAULAddress@2@@Z.
// Callees: operator new @ 0x00BFEA1A (0x9C bytes = retail sizeof(LSocket)), LSocket::LSocket @ 0x008F8A80 (manifest
// "Socket_Constructor"), LSocket::Listen(LAddress&) @ 0x008F8CC0, LAddress::operator= @ 0x00841F60, LSocket::Bind @ 0x008F8910,
// LSocket::SetNonBlocking(bool) @ 0x008F8870, LSocket::~LSocket @ 0x008F9780 (landed), operator delete @ 0x00BFE9BC.
// Layout: retail LSocketServer: ListenerSocket +0, the three 4-byte STLport lists +0x0C/+0x10/+0x14, UDPListenerAddress +0x18;
// LSocket::NoListeningBinding +0x7C; LAddress::TransportProtocol +0x14. Global namespace, as the landed LSocket rows.
// Shape: a two-case switch on the protocol (`dec ; je ; dec ; jne`); a `bool result` (false, set true in the UDP case BEFORE
// the Bind) lives in ebx and is passed to SetNonBlocking, stored in the flag and returned -- literal `true` arguments give
// immediates and a separate return; the successful Listen returns true directly. The failed-Listen path is `delete`
// (null test + dtor + operator delete) and a clear.
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
    LAddress& operator=(const LAddress& other);   // 0x00841F60
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
    LSocket();                                     // 0x008F8A80
    ~LSocket();                                    // 0x008F9780
    bool Listen(LAddress& address);                // 0x008F8CC0
    bool Bind(LAddress& address);                  // 0x008F8910
    bool SetNonBlocking(bool nonBlocking);         // 0x008F8870
};

void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CSocketListNode;
struct CSocketList {                               // STLport list<LSocket*>, 4 bytes in retail (the sentinel pointer)
    CSocketListNode* _M_node;
};

class LSocketServer {                              // retail 0x34 bytes (Ego_r.pdb 92 with 12-byte lists and a 44-byte LAddress)
public:
    LSocket*      ListenerSocket;                  // +0x00
    unsigned long MaxConnections;                  // +0x04
    bool          Paused;                          // +0x08
    CSocketList   ListOfAcceptedConnections;       // +0x0C
    CSocketList   ListOfConnectedSockets;          // +0x10
    CSocketList   ListOfUDPDatagramms;             // +0x14
    LAddress      UDPListenerAddress;              // +0x18
    bool RequestListener(LAddress& address);
};

bool LSocketServer::RequestListener(LAddress& address)
{
    ListenerSocket = new LSocket;
    bool result = false;
    switch (address.TransportProtocol) {
    case 1:
        if (ListenerSocket->Listen(address))
            return true;
        delete ListenerSocket;
        ListenerSocket = 0;
        break;
    case 2:
        UDPListenerAddress = address;
        result = true;
        ListenerSocket->Bind(UDPListenerAddress);
        ListenerSocket->SetNonBlocking(result);
        ListenerSocket->NoListeningBinding = result;
        break;
    }
    return result;
}