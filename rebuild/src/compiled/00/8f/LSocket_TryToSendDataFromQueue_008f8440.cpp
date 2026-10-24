// LSocket::TryToSendDataFromQueue() -- when output is queued on an open socket and it is writable, send()s the queue and erases at the sent position; false only on SOCKET_ERROR
// Leaf: TryToSendDataFromQueue
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F8440, 92 bytes (+ int3 padding). Ego_r.pdb ?TryToSendDataFromQueue@LSocket@LHNetworkLib@@AAE_NXZ.
// Callees: LSocket::IsWriteable(long) @ 0x008F82E0, send (WSOCK32 ordinal 19, IAT 0x01440554), STLport
// vector<char>::erase(first, last) @ 0x0040F390 (out of line; manifest "Buffer_RellocateAndUpdateWritePointer").
// Layout: retail LSocket (0x9C bytes): Socket +0x38, SendOutputQueue +0x64 (12-byte STLport vector<char>).
// Shape: retail passes the SAME iterator, begin() + sent, as both erase bounds (one register pushed twice) -- reproduced
// as written; `sent` starts at 0 in the zero register, so the unwritable path erases at begin().
extern "C" __declspec(dllimport) int __stdcall send(unsigned int s, const char* buf, int len, int flags);   // WSOCK32 #19
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
    char* begin() { return _M_start; }
    char* end() { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    char* erase(char* first, char* last);         // 0x0040F390
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
    bool IsWriteable(long timeoutMs);             // 0x008F82E0
    bool TryToSendDataFromQueue();
};

bool LSocket::TryToSendDataFromQueue()
{
    int sent = 0;
    if (SendOutputQueue.size() != 0 && Socket != (unsigned int)-1) {
        if (IsWriteable(0)) {
            sent = send(Socket, SendOutputQueue.begin(), SendOutputQueue.size(), 0);
            if (sent == -1)
                return false;
        }
        SendOutputQueue.erase(SendOutputQueue.begin() + sent, SendOutputQueue.begin() + sent);
    }
    return true;
}