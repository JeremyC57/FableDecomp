// LHNetworkLib::LSocket::CreateSocket(const LAddress&, LSOCKET_PROTOCOL) -- TCP/UDP socket bound to the address's IP and port; INVALID_SOCKET on any failure
// Leaf: ?CreateSocket@LSocket@LHNetworkLib@@
// Flags: /GS
// Retail 0x008F87A0, 0x8A bytes, static __fastcall (address in ecx, protocol in edx), plain `ret`, SOCKET. PDB name set:
// `?CreateSocket@LSocket@LHNetworkLib@@CIIABULAddress@2@W4LSOCKET_PROTOCOL@2@@Z`; the manifest name agrees.
// Callees: socket (WSOCK32 #23, IAT 0x01440540), LSocket::ResolveHost(LAddress&, unsigned long&) @ 0x008F86F0, htons
// (WSOCK32 #9), bind (WSOCK32 #2, IAT 0x01440548), __security_check_cookie @ 0x00BFE9F9 (cookie 0x0139C8A8).
// Shape: built with /GS (the sockaddr_in local holds a char array): cookie copy at entry, check on both exits. The
// protocol switch (1 = TCP -> SOCK_STREAM, 2 = UDP -> SOCK_DGRAM) is `dec eax ; je` / `dec eax ; jne`, and VC
// tail-merges the two socket() calls after their differing `push` pairs. The bind failure does not close the socket.
// The sockaddr's port and family are stored after htons (the family store scheduled last); sin_zero stays as is.
extern "C" __declspec(dllimport) unsigned int __stdcall socket(int af, int type, int protocol);              // WSOCK32 #23
extern "C" __declspec(dllimport) unsigned short __stdcall htons(unsigned short hostshort);                   // WSOCK32 #9

struct sockaddr {
    unsigned short sa_family;
    char           sa_data[14];
};
extern "C" __declspec(dllimport) int __stdcall bind(unsigned int s, const sockaddr* name, int namelen);     // WSOCK32 #2

struct in_addr {
    unsigned long S_addr;
};

struct sockaddr_in {                            // winsock sockaddr_in, 16 bytes
    short          sin_family;                  // +0x00
    unsigned short sin_port;                    // +0x02
    in_addr        sin_addr;                    // +0x04
    char           sin_zero[8];                 // +0x08
};

namespace std {
template <class _CharT> class char_traits { };
template <class _Tp> class allocator { };
template <class _CharT, class _Traits, class _Alloc>
class basic_string {                            // STLport basic_string, 12 bytes
public:
    _CharT* _M_start;
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;
}

namespace LHNetworkLib {

enum LSOCKET_PROTOCOL {
    LSOCKET_PROTOCOL_NONE = 0,
    LSOCKET_PROTOCOL_TCP  = 1,
    LSOCKET_PROTOCOL_UDP  = 2
};

struct LAddress {                               // Ego_r.pdb names; retail 0x1C bytes
    std::string   ServerName;                  // +0x00
    unsigned long  ServerIP;                    // +0x0C
    unsigned short ServerPort;                  // +0x10
    long           TransportProtocol;           // +0x14
    long           TransportFlags;              // +0x18
};

class LSocket {
public:
    static bool __fastcall ResolveHost(LAddress& address, unsigned long& ip);          // 0x008F86F0
    static unsigned int __fastcall CreateSocket(const LAddress& address, LSOCKET_PROTOCOL protocol);
};

unsigned int __fastcall LSocket::CreateSocket(const LAddress& address, LSOCKET_PROTOCOL protocol)
{
    sockaddr_in sa;
    unsigned int s;
    switch (protocol) {
    case LSOCKET_PROTOCOL_TCP:
        s = socket(2, 1, 0);                    // AF_INET, SOCK_STREAM
        break;
    case LSOCKET_PROTOCOL_UDP:
        s = socket(2, 2, 0);                    // AF_INET, SOCK_DGRAM
        break;
    default:
        return (unsigned int)-1;
    }
    if (s == (unsigned int)-1)
        return (unsigned int)-1;
    ResolveHost(const_cast<LAddress&>(address), sa.sin_addr.S_addr);
    sa.sin_port = htons(address.ServerPort);
    sa.sin_family = 2;                          // AF_INET
    if (bind(s, (sockaddr*)&sa, sizeof(sa)) != 0)
        return (unsigned int)-1;
    return s;
}

}