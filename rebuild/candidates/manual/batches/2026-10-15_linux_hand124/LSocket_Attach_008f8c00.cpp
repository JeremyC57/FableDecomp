// LSocket::Attach(unsigned int socket, const sockaddr_in& remote, const sockaddr_in& local) -- adopt an accepted socket handle and its two endpoint records, then decode both into the LAddress members
// Leaf: Attach
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F8C00, length 110 bytes (ret 0Ch).
// Callee: ConvertSockaddr2LAddress(sockaddr_in*, LAddress*) @ 0x008F8BB0 (__fastcall: inet_ntoa into the name
// string, the IP word at +0x0c, ntohs(port) at +0x10, returns true).
// Label correction: the manifest's "CCamera::SetView" is propagated. The only caller is LSocket::Accept
// (0x008F8EA0, call at 0x008F8F39), which passes the handle returned by accept(), the peer sockaddr_in from its frame
// and the listening socket's own SocketInterfaceInformation, with ecx = the new LSocket.
// Layout: the PDB LSocket (196 bytes) has a 44-byte LAddress; the retail one is 0x1c bytes (a 12-byte string, then
// ServerIP +0x0c, ServerPort +0x10, TransportProtocol +0x14, TransportFlags +0x18), so RemoteAddress +0x00,
// LocalAddress +0x1c, Socket +0x38, SocketInformation +0x3c, SocketInterfaceInformation +0x4c (ConvertSockaddr2LAddress
// writes +0x0c/+0x10 of its LAddress argument, matching that size). A local model is used.
// Shape: INVALID_SOCKET (-1) returns false before anything is stored; the two 16-byte sockaddr_in assignments are
// plain POD copies (four dword moves each through a scratch register) and their member addresses stay in ecx/edi for
// the two __fastcall decodes (record in ecx, LAddress in edx).
struct sockaddr_in {                          // winsock sockaddr_in, 16 bytes
    short          sin_family;
    unsigned short sin_port;
    unsigned long  sin_addr;
    char           sin_zero[8];
};
struct LAddress {                             // retail 0x1c bytes
    unsigned char  ServerName[0x0c];          // +0x00 STLport string
    unsigned long  ServerIP;                  // +0x0c
    unsigned short ServerPort;                // +0x10
    long           TransportProtocol;         // +0x14
    long           TransportFlags;            // +0x18
};
bool __fastcall ConvertSockaddr2LAddress(sockaddr_in* record, LAddress* address);   // 0x008F8BB0

class LSocket {
public:
    LAddress    RemoteAddress;                // +0x00
    LAddress    LocalAddress;                 // +0x1c
    unsigned int Socket;                      // +0x38
    sockaddr_in SocketInformation;            // +0x3c
    sockaddr_in SocketInterfaceInformation;   // +0x4c
    bool Attach(unsigned int socket, const sockaddr_in& remote, const sockaddr_in& local);
};

bool LSocket::Attach(unsigned int socket, const sockaddr_in& remote, const sockaddr_in& local)
{
    if (socket == (unsigned int)-1)
        return false;
    Socket = socket;
    SocketInformation = remote;
    SocketInterfaceInformation = local;
    ConvertSockaddr2LAddress(&SocketInformation, &RemoteAddress);
    ConvertSockaddr2LAddress(&SocketInterfaceInformation, &LocalAddress);
    return true;
}
